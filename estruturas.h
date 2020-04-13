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
        bool ehDuploPreto;
        NoRN* esq;
        NoRN* dir;
        NoRN* pai;
};

class No23 {
    public:
        Chave chave1, chave2;
        Item valor1, valor2;
        int quantNosSubArvEsq = 0;
        int quantNosSubArvDir = 0;
        No23 *ap1;
        No23 *ap2;
        No23 *ap3;
        bool doisNo;
        bool ehFolha();
};

bool No23 :: ehFolha() {
    //Retorna se o Nó é uma folha ou não
    return (ap1 == nullptr && ap2 == nullptr && ap3 == nullptr);
}
        
#endif