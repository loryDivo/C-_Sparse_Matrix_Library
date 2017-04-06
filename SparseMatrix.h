#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H
#include <iostream>
#include <stdexcept> // std::runtime_error
#include <algorithm> // std::swap
#include <iterator> // std::forward_iterator_tag
#include <cstddef>	// std::ptrdiff_t

/**
	Eccezione per elemento con indici che superano la dimensione della matrice sparsa
*/
class invalid_position : public std::runtime_error{
public:
		invalid_position() : std::runtime_error("elemento nella posizione corrente non valido"){};
};
/**
	Eccezione per elemento con indici minori di 0
*/
class invalid_add : public std::runtime_error{
	public:
		invalid_add ()	: std::runtime_error("impossibile aggiungere l'elemento alla matrice sparsa : indice riga o colonna minori di 0"){};
};
/**
	Eccezione per matrice sparsa vuota
*/
class empty_sparse_matrix : public std::runtime_error{
	public:
		empty_sparse_matrix() : std::runtime_error("La matrice sparsa e' vuota") {};
};
/**
	Classe che implementa una matrice sparsa di dati generici T.
	L'inserimento dei dati è effettuato utilizzando una funzione di comparazione.
	la funzione adotta l'inserimento  dei dati in base alla loro posizione logica nella matrice (da sinistra verso destra e dall'alto verso il basso (cordinate crescenti)).
	
	@brief Matrice Sparsa

	@param T tipo di dato
*/

template <typename T>
class SparseMatrix {
	
	/**
		Struttura interna che implementa un nodo della lista (una per ogni riga / colonna della matrice sparsa).
		
		@brief Nodo di una lista
	*/
	struct node{
		/**
			Sottostruttura che contiene un elemento di una matrice sparsa con relativa riga e colonna di collocazione e relativo valore
			
			@brief Elemento della lista 
		*/
		struct element{
			T value;		///< dato inserito nella lista
		    const unsigned int index_row;		///< riga dato
			const unsigned int index_colum;	///< colonna dato
			/**
				Costruttore element 
				@param v valore del dato inserito 
				@param row riga di inserimento dato
				@param colum colonna di inserimento dato
			*/
			element(const T &v, const unsigned int row , const unsigned int colum) : value(v), index_row(row), index_colum(colum) {}
			
			//gli altri costruttori fondamentali di element coincidono con quelli di default
			
		};
		element *_element;	///< puntatore a element contente numero riga (index_row), numero colonna (index_colum), valore (value)
		node *next;		///< puntatore al nodo successivo della lista
		SparseMatrix<T> *sp_matrix;	///< puntatore a matrice sparsa a cui si riferisce il nodo
		/**
			Costruttore di default
		*/
		node() : next(0), _element(0), sp_matrix(0){
		}
		/**
			Costruttore secondario di inizializzazione nodo
			@param v valore del dato inserito
			@param row riga di inserimento dato
			@param colum colonna di inserimento dato
			@param n puntatore al nodo successivo
			@param sp_matrix matrice sparsa a cui il nodo appartiene
			@throw eccezione di allocazione memoria
		*/
		
		
		node(const T &v, const int unsigned row, const unsigned int colum, SparseMatrix<T> *sp, node *n=0) : next(n), sp_matrix(sp){
			try{
				_element = new element(v, row, colum);
			}
			catch(std::bad_alloc & exc){
				sp_matrix->clear_sparse_matrix();
				throw;
			}
		}
		/**
			Copy costructor di node che costruisce un nodo e i suoi successori
			@param other altro nodo
			@throw eccezione allocazione memoria
		*/
		node(const node &other) {
			this->sp_matrix = other->sp_matrix;
			try{
				if(other->element!=0){
					this->_element = new element(other->_element->value, other->_element->index_row, other->_element->index_colum);
				}
				else{
					this->_element=0;
				}
				if(other->next != 0){
					this->next = new node(*(other->next));
				}
				else{
					this->next=0;
				}
			}
			catch(std::bad_alloc & exc){
				sp_matrix->clear_sparse_matrix();
				throw;
			}
		}
		/**
			Copy assignment di node 
			@param other altro nodo
			@throw eccezione allocazione memoria (da copy costructor di node)
		*/
		node &operator = (const node &other){
			if(this!=&other){
				node tmp(other);
				std::swap(*(this->_element),tmp->_element);
				std::swap(*(this->next), tmp->next);
			}
			return *this;
		}
		/**
			Distruttore di node
		*/
		~node(){
			delete _element;
			_element = 0;
		}
	};
	
	node *head; 	///< puntatore alla testa della lista
	T default_value;	///< valore di default inserito nella lista
	unsigned int tot_insert_element;	///< totale elementi inseriti nella matrice sparsa
	unsigned int nrow;	///< numero di righe della matrice sparsa
	unsigned int ncolum; ///< numero di colonne della matrice sparsa
	
	/**
		Metodo che rimuove la matrice sparsa ricorsivamente
		@param n nodo della matrice sparsa da cancellare 
	*/
	void clear_helper(node *n) {
		if(n==0) return;
		node *tmp=n->next;
		delete n;
		n=0;
		clear_helper(tmp);
	}
public:
	
	typedef T value_type; ///< Tipo dati della matrice sparsa

	/** 
		Costruttore che inizializza la matrice sparsa
		@param def_value valore di default scelto dall'utente
	*/
	SparseMatrix(const T &def_value) : head(0), default_value(def_value), tot_insert_element(1), nrow(0), ncolum(0) {}
	
	/**
		Distruttore
	*/
	~SparseMatrix(){
		clear_sparse_matrix();
	}
	
	/**
		Costruttore di copia
		
		@param other lista da copiare
		@throw eccezione di allocazione memoria (add)
	*/
	SparseMatrix(const SparseMatrix &other) : head(0), default_value(other.default_value), tot_insert_element(1), nrow(0), ncolum(0){
		const node *tmp = other.head;
		while(tmp!=0) {
			add(tmp->_element->value, tmp->_element->index_row, tmp->_element->index_colum);
			tmp = tmp->next;
		}
	}
	/**
		Costruttore che costruisce una matrice sparsa partendo da una matrice sparsa definita su tipo Q
		@param other matrice sparsa definita su tipo Q
		@throw eccezione allocazione memoria (add)
	*/
	template <typename Q>
	SparseMatrix(const SparseMatrix<Q> &other): head(0), default_value(static_cast<T>(other.get_default_value())), tot_insert_element(1), nrow(0), ncolum(0) {
		typename SparseMatrix<Q>::const_iterator i, ie;
		for(i = other.begin(), ie = other.end(); i!= ie; ++i){
			add(static_cast<T>((*i).value), (*i).index_row, (*i).index_colum);
		}
	}
	/**
		Operatore di assegnamento
		
		@param other lista da copiare
		@return reference a this
		@throw eccezione di allocazione di memoria
	*/
	SparseMatrix&operator=(const SparseMatrix &other){
		if(this != &other) {
			SparseMatrix tmp(other);
			std::swap(nrow, tmp.nrow);
			std::swap(ncolum, tmp.ncolum);
			std::swap(tot_insert_element, tmp.tot_insert_element);
			std::swap(head, tmp.head);
			std::swap(default_value, tmp.default_value);
		}
		return *this;
	}
	/**
		Ritorna valore di default della matrice sparsa
	*/
	const T &get_default_value() const{
		return default_value;
	}
	/**
		Ritorna numero di righe della matrice sparsa
	*/
	unsigned int get_row_sparse_matrix() const{
		return nrow;
	}
	/**
		Ritorna numero di colonne della matrice sparsa
	*/
	unsigned int get_colum_sparse_matrix() const{
		return ncolum;
	}
	/** Ritorna numero totale di righe e colonne della matrice sparsa
	*/
	unsigned int get_tot_value_sparse_matrix() const{
		return nrow * ncolum;
	}
	/**
		Ritorna numero di elementi inseriti nella matrice sparsa
	*/
	unsigned int get_tot_insert_element() const{
		return tot_insert_element;
	}
	/**
		Inserisce un elemento nella matrice sparsa secondo l'indice
		di riga e colonna definito dalla funzione di confronto (compare base)
		
		@param value valore da inserire
		@param row riga del valore
		@param colum colonna del valore
		@throw eccezione allocazione memoria
	*/
	void add(const value_type &value, const int row, const int colum) {
		if(row<0 || colum<0){	//elemento che è in posizione negativa e non esistente
			throw invalid_add();
		}
		node *current=0;
		try{
			current = new node(value, row, colum, this);
		}
		catch (std::bad_alloc & exc) {
			clear_sparse_matrix();
			throw;
		}
		if (head==0) {
			head = current;
			return;
		}
		node *n = head, *prev=head;
		while(n!=0 && compare_base(n->_element->index_row, row) ==-1) {
			prev = n;
			n = n->next;
		}
		while(n!=0 && compare_base(n->_element->index_colum, colum) ==-1 && compare_base(n->_element->index_row, row) == 0){
			prev=n;
			n = n->next;
		}
		if(current->_element->index_row>nrow){
			nrow=current->_element->index_row;
		}
		if(current->_element->index_colum>ncolum){
			ncolum=current->_element->index_colum;
		}
		if(n!=0 &&compare_base(n->_element->index_colum, colum) == 0 && compare_base(n->_element->index_row, row)==0){
			n->_element->value = current->_element->value;
			delete current;
			return;
		}
		if(n==head) {
			current->next = head;
			head = current;
			return;
		}
		//in ordine: prev - current - prev->next
		current->next = prev->next;
		prev->next = current;
		tot_insert_element++;
	}
	/**
		Legge un elemento dalla matrice sparsa 
		@param row riga elemento
		@param colum colonna elemento
		@return elemento ricercato
		@throw eccezione per elemento fuori dal range della matrice sparsa
	*/
	T operator()(const int row, const int colum) const{
		if(row>nrow || colum>ncolum || row<0 || colum<0){	//eccezione elemento che supera la grandezza della matrice
			throw invalid_position();
		}
		node *n = head;
		while(n!=0 &&compare_base(n->_element->index_row, row) == -1) {
			n = n->next;
		}
		while(n!=0 &&compare_base(n->_element->index_colum, colum) == -1 && compare_base(n->_element->index_row, row)==0) {
			n = n->next;
		}
		if(n!= 0 && compare_base(n->_element->index_colum, colum) == 0 && compare_base(n->_element->index_row, row) == 0){
			return n->_element->value;
		}
		else{
			return default_value;
		}
	}
	/**
		Metodo print che stampa la matrice sparsa
		@throw eccezione matrice sparsa vuota 
		
	*/
	void print() const {
		if(head==0){
			throw empty_sparse_matrix();
		}
		node *n = head;
		while(n!=0) {
			std::cout<<"Elemento riga " << n->_element->index_row<<" Colonna " << n->_element->index_colum<<" Valore " << n->_element->value<<std::endl;
			n=n->next;
		}
		std::cout<<std::endl;
	}
	/**
		Metodo rimozione matrice sparsa 
		
	*/
	void clear_sparse_matrix(){
		clear_helper(head);
		head=0;
		nrow=0;
		ncolum=0;
		tot_insert_element=0;
	}
	
	/**
	Funzione di comparazione di righe e colonne della matrice.
	Confronta le righe e le colonne e ritorna:
	-1 se a<b (riga/colonna "a" minore di riga/colonna "b");
	0 se a==b (riga/colonna "a" uguale di riga/colonna "b");
	+1 se a>b (riga/colonna "a" maggiore di riga/colonna "b").
	
*/

	const int compare_base(const int &a, const int &b) const {
		if (a<b) return -1;
		else if (a==b) return 0;
		else return 1;
	}

	/**
		Iteratore della matrice sparsa

		@brief Iteratore della matrice sparsa
	*/
	class const_iterator; // forward declaration

	class iterator {
		node *n;	
	public:
		typedef std::forward_iterator_tag iterator_category;
		typedef typename node::element    	value_type;
		typedef ptrdiff_t                 	difference_type;
		typedef typename node::element*   	pointer;
		typedef typename node::element&  	reference;

	
		iterator() : n(0){}
		
		iterator(const iterator &other) : n(other.n){
		}

		iterator& operator=(const iterator &other) {
			n = other.n;
			return *this;
		}

		~iterator() {
		}

		// Ritorna valore di element riferito dall'iteratore (dereferenziamento)
		reference operator*() const {
			return *(n->_element);
		}

		// Ritorna il puntatore element alla struttura riferita dall'iteratore
		pointer operator->() const {
			return n->_element;
		}

		// Operatore di iterazione post-incremento
		iterator operator++(int) {
			iterator tmp(*this);
			n = n->next;
			return tmp;
		}

		// Operatore di iterazione pre-incremento
		iterator& operator++() {
			n = n->next;
			return *this;
		}

		// Uguaglianza
		bool operator==(const iterator &other) const {
			return (n == other.n);
		}

		// Diversita'
		bool operator!=(const iterator &other) const {
			return (n != other.n);
		}
		
		// const_iterator & iterator 
		friend class const_iterator;

		// Uguaglianza
		bool operator==(const const_iterator &other) const {
			return (n == other.n);
		}

		// Diversita'
		bool operator!=(const const_iterator &other) const {
			return (n != other.n);
		}

	private:
		//Dati membro

		// La classe container deve essere messa friend dell'iteratore per poter
		// usare il costruttore di inizializzazione.
		friend class SparseMatrix;

		// Costruttore privato di inizializzazione usato dalla classe container
		// tipicamente nei metodi begin e end
		iterator(node *n1) : n(n1) { 
		}
		
		
	}; // classe iterator
	
	/**
		Ritorna l'iteratore all'inizio della sequenza dati
	
		@return iteratore all'inizio della sequenza
		@throw matrice sparsa vuota
	*/
	// Ritorna l'iteratore all'inizio della sequenza dati
	iterator begin() {
		if(head==0){
			throw empty_sparse_matrix();
		}
		return iterator(head);
	}
	
	/**
		Ritorna l'iteratore alla fine della sequenza dati
	
		@return iteratore alla fine della sequenza
		@throw matrice sparsa vuota
	*/
	// Ritorna l'iteratore alla fine della sequenza dati
	iterator end() {
		if(head==0){
			throw empty_sparse_matrix();
		}
		return iterator(0);
	}
	
	/**
		Iteratore costande della lista

		@brief Iteratore costante della lista
	*/
	
	class const_iterator {
		node *n;	
	public:
		typedef std::forward_iterator_tag iterator_category;
		typedef typename node::element    		value_type;
		typedef ptrdiff_t                 	    difference_type;
		typedef const typename node::element*  	pointer;
		typedef const typename node::element&   reference;

	
		const_iterator() : n(0){
		}
		
		const_iterator(const const_iterator &other) : n(other.n){
		}

		const_iterator(const iterator &other) : n(other.n) {
		}
		
		const_iterator& operator=(const const_iterator &other) {
			n = other.n;
			return *this;
		}

		~const_iterator() {
		}

		// Ritorna la valore di element riferito dall'iteratore (dereferenziamento)
		reference operator*() const {
			return *(n->_element);
		}

		// Ritorna il puntatore alla struttura element riferito dall'iteratore
		pointer operator->() const {
			return n->_element;
		}
		
		// Operatore di iterazione post-incremento
		const_iterator operator++(int) {
			const_iterator tmp(*this);
			n = n->next;
			return tmp;
		}

		// Operatore di iterazione pre-incremento
		const_iterator& operator++() {
			n = n->next;
			return *this;
		}

		// Uguaglianza
		bool operator==(const const_iterator &other) const {
			return (n == other.n);
		}
		
		// Diversita'
		bool operator!=(const const_iterator &other) const {
			return (n != other.n);
		}

		//const_iterator & iterator
		
		friend class iterator;

		// Uguaglianza
		bool operator==(const iterator &other) const {
			return (n == other.n);
		}

		// Diversita'
		bool operator!=(const iterator &other) const {
			return (n != other.n);
		}


	private:
		//Dati membro

		// La classe container deve essere messa friend dell'iteratore per poter
		// usare il costruttore di inizializzazione.
		friend class SparseMatrix; 

		// Costruttore privato di inizializzazione usato dalla classe container
		// tipicamente nei metodi begin e end
		const_iterator(node *n1) : n(n1) { 
		}
		
	}; // classe const_iterator
	/**
		Ritorna l'iteratore costante all'inizio della sequenza dati
	
		@return const iterator all'inizio della sequenza
		@throw matrice sparsa vuota
	*/
	// Ritorna l'iteratore all'inizio della sequenza dati
	const_iterator begin() const {
		if(head==0){
			throw empty_sparse_matrix();
		}
		return const_iterator(head);
	}
	/**
		Ritorna l'iteratore costante alla fine della sequenza dati
	
		@return const iteratore alla fine della sequenza
		@throw matrice sparsa vuota
	*/
	// Ritorna l'iteratore alla fine della sequenza dati
	const_iterator end() const {
		if(head==0){
			throw empty_sparse_matrix();
		}
		return const_iterator(0);
	}
	
};
/**
	Funzione evaluate che data una matrice sparsa M e un predicato P, ritorna quanti valori in M (compresi i default)
	soddisfano P.
	
	@brief funzione di valutazione matrice sparsa dato un predicato
	@param tipo generico T
	@param predicato generico P
	@return numero di valori che soddisfano il predicato generico P
*/
template <typename T, typename Predicate>
	int evaluate (const SparseMatrix<T>& M, Predicate P) {
		typename SparseMatrix<T>::const_iterator i, ie;
		int true_count = 0;		//numero valori che soddisfano il predicato
		int no_default_value = 0;	//numero valori non di default
		for(i = M.begin(), ie = M.end(); i!=ie;++i){
			no_default_value ++;
			if(P((*i).value)){
				true_count ++;
			}
		}
		if(P(M.get_default_value())){
			int number_of_default_value = 0;	//numero valori di default
			number_of_default_value = M.get_tot_value_sparse_matrix() - no_default_value;
			true_count = number_of_default_value + true_count;
			return true_count;
		}
	return true_count;
	}



#endif