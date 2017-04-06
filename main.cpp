#include <iostream>
#include <string>
#include "SparseMatrix.h"
bool is_equal_two(const int &s1) {
	return s1 ==2;
}
/**#in
	Struct point che implementa un punto 2D.
*/
struct point {
	int x; ///< coordinata x del punto
	int y; ///< coordinata y del punto

	point(int xx, int yy) : x(xx), y(yy) {}
};
/**#in
	Class point che implementa un punto 2D in double.
*/
class pointdouble{
	double x;
	double y;
	public:
	pointdouble(double xx, double yy) : x(xx), y(yy){}
	double getX() const {
		return x;
	}
	double getY() const{
		return y;
	}
};
/**#in
	Class point che implementa un punto 2D di stringhe.
*/
class pointstring{
	std::string x;
	std::string y;
	public:
	pointstring(std::string xx, std::string yy) : x(xx), y(yy){}
	std::string getX() const{
		return x;
	}
	std::string getY() const {
		return y;
	}
};
/**#in
	operatore di stream ridefinito per struct point 2D int
*/
std::ostream&operator<<(std::ostream &os, const point &p){
	std::cout<<"Valore x "<< p.x<< " Valore y "<<p.y<<std::endl;
	return os;
}
/**#in
	operatore di stream ridefinito per class point 2D double
*/
std::ostream&operator<<(std::ostream &os,  const pointdouble &p){
	std::cout<<"Valore x "<< p.getX()<< " Valore y "<<p.getY()<<std::endl;
	return os;
}
/**#in
	operatore di stream ridefinito per class point 2D string
*/
std::ostream&operator<<(std::ostream &os, const pointstring &p){
	std::cout<<"Prima stringa "<< p.getX()<< " Seconda stringa " <<p.getY()<<std::endl;
	return os;
}

//predicato che controlla se i due attributi sono uguali a 1.6 e 5.1 rispettivamente X e Y
bool eval_double_point(const pointdouble &c){
	return (c.getX() == 1.6 && c.getY() == 5.1);
}

//predicato che controlla se i due attributi string sono uguali a 'pippo' e 'pluto' rispettivamente X e Y
bool eval_string_point(const pointstring &c){
	return (c.getX() == "pippo" && c.getY() == "pluto");
}


void test_sparse_primitive_const(const SparseMatrix<int> &other){
	std::cout<<"///////////////////////////////////////////////////////"<<std::endl;
	
	/* Test su matrice sparsa di interi costanti */
	std::cout<<"ELEMENTI COSTANTI INTERI"<<std::endl;
	std::cout<<std::endl;
	
	// Stampa con print
	std::cout<<"Stampa con print"<<std::endl;
	other.print();
	
	//Dimensione sparse matrix
	std::cout<<"Dimensione matrice"<<std::endl;
	std::cout<<other.get_tot_value_sparse_matrix()<<std::endl;
	std::cout<<std::endl;
	
	std::cout<<"Dimensione matrice sparsa di soli elementi inseriti"<<std::endl;
	std::cout<<other.get_tot_insert_element()<<std::endl;
	std::cout<<std::endl;
	
	std::cout<<"Stampa con iterator costante"<<std::endl;
	
	// Stampa con iterator const
	SparseMatrix<int>::const_iterator i, ie;
	for(i = other.begin(), ie = other.end(); i!=ie; ++i){
		std::cout<<"Elemento riga " << (*i).index_row<<" Colonna " << (*i).index_colum<<" Valore " << (*i).value<<std::endl;
	}
	std::cout<<std::endl;
	
	//Test evaluate 
	std::cout<<"Test di evaluate con predicato che controlla quanti valori sono uguali a 2 (predicato is_equal_two)"<<std::endl;
	std::cout<<evaluate(other, is_equal_two)<<std::endl;
	std::cout<<std::endl;
	
	//Test copy costructor Q->T
	SparseMatrix<float> float_matrix(other);
	std::cout<<"Stampa elementi float const"<<std::endl;
	float_matrix.print();
}

void test_sparse_primitive(SparseMatrix<int> &other){
	std::cout<<"///////////////////////////////////////////////////////"<<std::endl;
	/* Test su matrice sparsa di interi  */
	std::cout<<"ELEMENTI NON COSTANTI INTERI"<<std::endl;
	std::cout<<std::endl;
	
	//Add elementi
	std::cout<<"Aggiunta elementi con valore 1(0,0), 2(2,2), 50(1,1), 4(4,4), 1(1,2), -2(3,3), 4(3,4), -2(2,2)"<<std::endl;
	std::cout<<"Elemento 2(2,2) sovrascritto da -2(2,2)"<<std::endl;
	other.add(1,0,0);
	other.add(2,2,2);
	other.add(50,1,1);
	other.add(4,4,4);
	other.add(1,1,2);
	other.add(-2,3,3);
	other.add(-4, 3,4);
	other.add(-2,2,2);
	std::cout<<std::endl;
	
	//Dimensione sparse matrix
	std::cout<<"Dimensione matrice"<<std::endl;
	std::cout<<other.get_tot_value_sparse_matrix()<<std::endl;
	std::cout<<std::endl;
	std::cout<<"Dimensione matrice sparsa di soli elementi inseriti"<<std::endl;
	std::cout<<other.get_tot_insert_element()<<std::endl;
	std::cout<<std::endl;
	
	// Stampa con print
	std::cout<<"Stampa con print"<<std::endl;
	other.print();
	std::cout<<"Stampa con iterator non costante"<<std::endl;
	
	// Stampa con iterator
	SparseMatrix<int>::iterator i, ie;
	for(i = other.begin(), ie = other.end(); i!=ie; ++i){
		std::cout<<"Elemento riga " << (*i).index_row<<" Colonna " << (*i).index_colum<<" Valore " << (*i).value<<std::endl;
	}
	std::cout<<std::endl;
	
	//Test evaluate 
	std::cout<<"Test di evaluate con predicato che controlla quanti valori sono uguali a 2 (predicato is_equal_two)"<<std::endl;
	std::cout<<evaluate(other, is_equal_two)<<std::endl;
	std::cout<<std::endl;
	
	//Test copy costructor con convertibilità Q->T
	std::cout<<"Test convertibilita Q->T sparse matrix"<<std::endl;
	SparseMatrix<float> float_matrix(other);
	std::cout<<"Stampa elementi float"<<std::endl;
	float_matrix.print();
}

void test_search_element(const SparseMatrix<int> &other){
	std::cout<<"///////////////////////////////////////////////////////"<<std::endl;
	// Test su matrice sparsa ricerca elementi
	std::cout<<"Elemento riga 2 colonna 2: valore ";
	std::cout<<other(2,2)<<std::endl;
	std::cout<<"Elemento in riga 1 colonna 4 e' di default ";
	std::cout<<other(1,4)<<std::endl;
	//Elemento fuori dal range (non valido)
	try{
	std::cout<<other(10,10)<<std::endl;
	}
	catch (std::runtime_error &e){
		std::cout<<e.what()<<" in posizione 10 10 "<< std::endl;
	}
}

void test_struct_point(void){
	std::cout<<"///////////////////////////////////////////////////////"<<std::endl;
	std::cout<<"TEST STRUCT POINT"<<std::endl;
	SparseMatrix<point> point_matrix(point(1,1));
	
	//Add elementi
	std::cout<<"Inserimento valori (1,2(1,1)) (1,5(2,2)) (3,5(1,2))"<<std::endl;
	point_matrix.add(point(1,2),1,1);
	point_matrix.add(point(1,5),2,2);
	point_matrix.add(point(3,5),1,2);
	std::cout<<std::endl;
	
	//Stampa valori
	std::cout<<" Stampa valori "<<std::endl;
	std::cout<<" Stampa con print " <<std::endl;
	point_matrix.print();
	std::cout<<std::endl;
	
	// Stampa con iterator
	std::cout<<"Stampa con iteratori"<<std::endl;
	SparseMatrix<point>::iterator i, ie;
	for(i = point_matrix.begin(), ie = point_matrix.end(); i!=ie; ++i){
		std::cout<<"Elemento riga " << (*i).index_row<<" Colonna " << (*i).index_colum<<" Valore " << (*i).value<<std::endl;
	}
	std::cout<<std::endl;
	
	//Dimensione sparse matrix
	std::cout<<"Dimensione matrice sparsa di point"<<std::endl;
	std::cout<<point_matrix.get_tot_value_sparse_matrix()<<std::endl;
	std::cout<<std::endl;
	
	//Dimensione sparse matrix soli elementi inseriti 
	std::cout<<"Dimensione matrice sparsa di soli elementi inseriti"<<std::endl;
	std::cout<<point_matrix.get_tot_insert_element()<<std::endl;
	std::cout<<std::endl;
	
	//Ricerca elemento in posizione 
	std::cout<<"Ricerca elemento in posizione corretta"<<std::endl;
	std::cout<<point_matrix(1,1)<<std::endl;
	std::cout<<"Sollevamento eccezione causa elemento fuori dal range della matrice"<<std::endl;
	try{
		std::cout<<point_matrix(3,3)<<std::endl;
	}
	catch(std::runtime_error &e){
		std::cout<<e.what()<<std::endl;
	}
}
void test_string_sparse_matrix(void){
	std::cout<<"///////////////////////////////////////////////////////"<<std::endl;
	std::cout<<"TEST STRING"<<std::endl;
	SparseMatrix<std::string> point_string("default");
	
	//Add elementi
	std::cout<<"Inserimento stringhe ('pippo', 1, 1) ('topolino', 2,3) ('minnie', 1,2)"<<std::endl;
	point_string.add("pippo", 1, 1);
	point_string.add("topolino", 2, 3);
	point_string.add("minnie",1,2);
	std::cout<<std::endl;
	
	//Stampa stringhe con print
	std::cout<<"Stampa stringhe"<<std::endl;
	point_string.print();
	
	//Stampa con iterator
	std::cout<<"Stampa con iteratori"<<std::endl;
	SparseMatrix<std::string>::iterator i, ie;
	for(i = point_string.begin(), ie = point_string.end(); i!=ie; ++i){
		std::cout<<"Elemento riga " << (*i).index_row<<" Colonna " << (*i).index_colum<<" Valore " << (*i).value<<std::endl;
	}
	std::cout<<std::endl;
	
	//Dimensione sparse matrix
	std::cout<<"Dimensione matrice sparsa di point"<<std::endl;
	std::cout<<point_string.get_tot_value_sparse_matrix()<<std::endl;
	std::cout<<std::endl;
}
void test_double_class_sparse_matrix(void){
	std::cout<<"///////////////////////////////////////////////////////"<<std::endl;
	std::cout<<"TEST CLASSE double"<<std::endl;
	SparseMatrix<pointdouble> sparse_class_double(pointdouble(2.0,4.9));
	
	//Add elementi
	std::cout<<"Inserimento valori (1.2,2.4(1,1)) (1.6,5.1(2,2)) (3.9,5.0(1,2))"<<std::endl;
	sparse_class_double.add(pointdouble(1.2,2.4),1,1);
	sparse_class_double.add(pointdouble(1.6,5.1),2,2);
	sparse_class_double.add(pointdouble(3.9,5.0),1,2);
	std::cout<<std::endl;
	
	//Stampa con print 
	std::cout<<" Stampa valori "<<std::endl;
	std::cout<<" Stampa con print " <<std::endl;
	sparse_class_double.print();
	std::cout<<std::endl;
	
	//Stampa con iterator 
	std::cout<<"Stampa con iteratori"<<std::endl;
	SparseMatrix<pointdouble>::iterator i, ie;
	for(i = sparse_class_double.begin(), ie = sparse_class_double.end(); i!=ie; ++i){
		std::cout<<"Elemento riga " << (*i).index_row<<" Colonna " << (*i).index_colum<<" Valore " << (*i).value<<std::endl;
	}
	std::cout<<std::endl;
	
	//Dimensione matrice sparsa 
	std::cout<<"Dimensione matrice sparsa di point"<<std::endl;
	std::cout<<sparse_class_double.get_tot_value_sparse_matrix()<<std::endl;
	std::cout<<std::endl;
	
	//Test evaluate 
	std::cout<<"Test Evaluate : predicato che ritorna numero di elementi se e solo se il primo e il secondo attributo di questi sono uguali a 1.6 e 5.1 (1 elemento)"<<std::endl;
	std::cout<<evaluate(sparse_class_double, eval_double_point)<<std::endl;
	std::cout<<std::endl;
	
	//Test copy costructor 
	std::cout<<"Normal copy costructor e print elementi"<<std::endl;
	SparseMatrix<pointdouble> second_sparse_double(sparse_class_double);
	second_sparse_double.print();
	std::cout<<std::endl;
}
void test_string_class_sparse_matrix(void){
	std::cout<<"///////////////////////////////////////////////////////"<<std::endl;
	std::cout<<"TEST CLASSE string"<<std::endl;
	SparseMatrix<pointstring> point_sparse_string(pointstring("primo", "secondo"));
	
	//Add elementi
	std::cout<<"Inserimento elementi (('pippo', 'pluto'), 1, 2), (('paperino', 'topolino', 2,5)), (('minnie', 'paperone'), 1, 5)"<<std::endl;
	point_sparse_string.add(pointstring("pippo", "pluto"), 1, 2);
	point_sparse_string.add(pointstring("paperino", "topolino"), 2, 5);
	point_sparse_string.add(pointstring("minnie", "paperone"), 1, 5);
	std::cout<<std::endl;
	std::cout<<"Add elemento riga o colonna minore di 0"<<std::endl;
	std::cout<<std::endl;
	try{
		point_sparse_string.add(pointstring("pap", "mem"), -1, 0);
	}
	catch(std::runtime_error &e){
		std::cout<<e.what()<<std::endl;
	}
	
	//Test copy costructor 
	std::cout<<"Copy costructor e print"<<std::endl;
	SparseMatrix<pointstring> second_point_string(point_sparse_string);
	second_point_string.print();
	
	//Test copy assignment 
	std::cout<<"Copy assignment e print"<<std::endl;
	SparseMatrix<pointstring> assign_point_string = second_point_string;
	assign_point_string.print();
	
	//Stampa con print 
	std::cout<<"Stampa con print"<<std::endl;
	std::cout<<std::endl;
	point_sparse_string.print();
	std::cout<<std::endl;
	
	//Stampa con iterator 
	std::cout<<"Stampa con iterator"<<std::endl;
	SparseMatrix<pointstring>::iterator i, ie;
	for(i = point_sparse_string.begin(), ie = point_sparse_string.end(); i!=ie; ++i){
		std::cout<<"Elemento riga " << (*i).index_row<<" Colonna " << (*i).index_colum<<" Valore " << (*i).value<<std::endl;
	}
	std::cout<<std::endl;
	
	//Dimensione matrice sparsa 
	std::cout<<"Dimensione matrice sparsa di point"<<std::endl;
	std::cout<<point_sparse_string.get_tot_value_sparse_matrix()<<std::endl;
	std::cout<<std::endl;
	
	//Ricerca elementi 
	std::cout<<"Elemento in posizione 2,5"<<std::endl;
	std::cout<<point_sparse_string(2, 5)<<std::endl;
	std::cout<<"Elemento di default"<<std::endl;
	std::cout<<point_sparse_string(1,1)<<std::endl;
	std::cout<<"Elemento fuori range"<<std::endl;
	try{
		std::cout<<point_sparse_string(10,10)<<std::endl;
	}
	catch(std::runtime_error &e){
		std::cout<<e.what()<<std::endl;
	}
	
	//Test evaluate 
	std::cout<<"Test Evaluate : predicato che ritorna numero di elementi se e solo se il primo e il secondo attributo di questi sono uguali a pippo e pluto (1 elemento)"<<std::endl;
	std::cout<<evaluate(point_sparse_string, eval_string_point)<<std::endl;
	std::cout<<std::endl;
}

int main(int argc, char *argv[]) {
	try{
		std::cout<<"///////////////////////////////////////////////////////"<<std::endl;
		std::cout<<"Matrice sparsa di interi"<<std::endl;
		
		//matrice sparsa int 
		SparseMatrix<int> sp(2);
		sp.add(1,0,0);
		sp.add(2,2,2);
		sp.add(50,1,1);
		sp.add(4,4,4);
		sp.add(1,1,2);
		sp.add(-2,2,2);
	
		//Elemento con riga e colonna minori di 0
		try{
			sp.add(4,-2,-2);
		}
		catch(std::runtime_error &e){
			std::cout<<e.what()<<" riga: -2 colonna: -2 "<<std::endl;
		}
		// test passaggio matrice a funzione con parametro matrice costante 
		test_sparse_primitive_const(sp);
		//test passaggio a interi non costanti
		test_sparse_primitive(sp);
		// test ricerca elementi
		test_search_element(sp);
		//test matrice con strutture point come value T
		test_struct_point();
		//test matrice con stringa come value T
		test_string_sparse_matrix();
		//test matrice con classe di parametri double come value T
		test_double_class_sparse_matrix();
		// test matrice con classi con classi di parametri string come value T
		test_string_class_sparse_matrix();
		return 0;
	}
	catch(std::runtime_error &e){
		std::cout<<e.what()<<std::endl;
	}
}

