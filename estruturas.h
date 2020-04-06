#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <string>
using namespace std;

typedef string Chave;
typedef int Item;

class Celula {
    public:
        Chave chave;
        Item valor;
        Celula* prox;
};

class NoABB {
    public:
        Chave chave;
        Item valor;
        int quantNosSubArvEsq;
        int quantNosSubArvDir;
        NoABB* esq;
        NoABB* dir;
};







#endif