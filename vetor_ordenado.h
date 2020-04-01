#ifndef VETOR_ORDENADO_H
#define VETOR_ORDENADO_H

#include <string>
using namespace std;

typedef string Chave;
typedef int Item;

class VetorOrdenado{
    VetorOrdenado();
    ~VetorOrdenado();

    /*Métodos*/
    void insere(Chave chave, Item valor);
    Item devolve(Chave chave);
    void remove(Chave chave);
    int rank(Chave chave);
    Chave seleciona(int k); 

};
VetorOrdenado :: VetorOrdenado() {

}
VetorOrdenado :: ~VetorOrdenado() {

}
void VetorOrdenado :: insere(Chave chave, Item valor) {

}
Item VetorOrdenado :: devolve(Chave chave) {

}
void VetorOrdenado :: remove(Chave chave) {

}
int VetorOrdenado :: rank(Chave chave) {

}
Chave VetorOrdenado :: seleciona(int k) {
    
}







#endif