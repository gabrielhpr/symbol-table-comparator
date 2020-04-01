#ifndef VETOR_DESORDENADO_H
#define VETOR_DESORDENADO_H

#include <string>
using namespace std;

typedef string Chave;
typedef int Item;

class VetorDesordenado { 
    VetorDesordenado();
    ~VetorDesordenado();

    /*Métodos*/
    void insere(Chave chave, Item valor);
    Item devolve(Chave chave);
    void remove(Chave chave);
    int rank(Chave chave);
    Chave seleciona(int k); 

};
VetorDesordenado :: VetorDesordenado() {

}
VetorDesordenado :: ~VetorDesordenado() {

}
void VetorDesordenado :: insere(Chave chave, Item valor) {

}
Item VetorDesordenado :: devolve(Chave chave) {

}
void VetorDesordenado :: remove(Chave chave) {

}
int VetorDesordenado :: rank(Chave chave) {

}
Chave VetorDesordenado :: seleciona(int k) {
    
}







#endif