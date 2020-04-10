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

class NoTreap {
    public: 
        Chave chave;
        Item valor;
        int prioridade;
        int quantNosSubArvEsq;
        int quantNosSubArvDir;
        NoTreap* esq;
        NoTreap* dir;
};  

class NoRN {
    public: 
        Chave chave;
        Item valor;
        char cor;
        int quantNosSubArvEsq;
        int quantNosSubArvDir;
        NoRN* esq;
        NoRN* dir;
        NoRN* pai;
};






#endif