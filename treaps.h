#ifndef TS_TREAPS_H
#define TS_TREAPS_H

#include <iostream>
#include <ctime>
#include <climits>
#include "estruturas.h"
using namespace std;


template <class par>
class TSTreaps { 
    public:
        TSTreaps();
        ~TSTreaps();

        /*Treap*/
        NoTreap* raiz;
        NoTreap* criaNo(Chave chave, Item valor);

        /*Número de elementos*/
        int n;//public

        /*Rotações*/
        NoTreap* rotacaoEsq(NoTreap* p);
        NoTreap* rotacaoDir(NoTreap* p);

        /*Métodos da TS*/
        NoTreap* insereUtilRecur(NoTreap* q, Chave chave, Item valor, bool& criouNovoNo);
        void insere(Chave chave, Item valor);
        Item devolve(Chave chave);
        NoTreap* removeUtilRecur(NoTreap* q, Chave chave, bool& removeu);
        void remove(Chave chave);
        int rank(Chave chave);
        Chave seleciona(int k); 

        //Método utilizado pelo destructor
        void destroiTreap(NoTreap* q);
        void exibeTSUtilRecur(NoTreap* q);
        void exibeTS(); 
};

template <class par>
TSTreaps<par> :: TSTreaps() {
    n = 0;
    raiz = nullptr;
}

template <class par>
TSTreaps<par> :: ~TSTreaps() {
    //Executar em pós-ordem
    destroiTreap(raiz);
}

template <class par> 
void TSTreaps<par> :: destroiTreap(NoTreap* q) {
    if(q != nullptr) {
        destroiTreap(q->esq);
        destroiTreap(q->dir);
        delete q;
    }
}

template <class par>
NoTreap* TSTreaps<par> :: rotacaoEsq(NoTreap* p) {
    if(p == nullptr || p->dir == nullptr) {
        return p;
    }
    p->quantNosSubArvDir = p->dir->quantNosSubArvEsq;
    p->dir->quantNosSubArvEsq += 1 + p->quantNosSubArvEsq;
    NoTreap* aux = p->dir;
    p->dir = aux->esq;
    aux->esq = p;
    return aux;
}

template <class par>
NoTreap* TSTreaps<par> :: rotacaoDir(NoTreap* p) {
    if(p == nullptr || p->esq == nullptr) {
        return p;
    }
    p->quantNosSubArvEsq = p->esq->quantNosSubArvDir;
    p->esq->quantNosSubArvDir += 1 + p->quantNosSubArvDir;
    NoTreap* aux = p->esq;
    p->esq = aux->dir;
    aux->dir = p;
    return aux;
}

template <class par>
NoTreap* TSTreaps<par> :: criaNo(Chave chave, Item valor) {
    NoTreap* novo = new NoTreap;
    novo->chave = chave;
    novo->valor = valor;
    srand((int) time(0));
    novo->prioridade = rand()% 2000000;    
    novo->esq = nullptr;
    novo->dir = nullptr;
    novo->quantNosSubArvEsq = 0;
    novo->quantNosSubArvDir = 0;
    return novo;
}

template <class par>
NoTreap* TSTreaps<par> :: insereUtilRecur(NoTreap* p, Chave chave, Item valor, bool& criouNovoNo) {
    
    if(p == nullptr) {
        p = criaNo(chave, valor);
        criouNovoNo = true;
        n++;
    }
    else if(p->chave == chave) {
        p->valor++;
    }
    else if(chave < p->chave) {
        p->esq = insereUtilRecur(p->esq, chave, valor, criouNovoNo);
        if(criouNovoNo == true) {
            p->quantNosSubArvEsq++;
            if(p->prioridade < p->esq->prioridade) {
                p = rotacaoDir(p);
            }
        }
    }
    else if(chave > p->chave) {
        p->dir = insereUtilRecur(p->dir, chave, valor, criouNovoNo);
        if(criouNovoNo == true) {
            p->quantNosSubArvDir++;
            if(p->prioridade < p->dir->prioridade) {
                p = rotacaoEsq(p);
            }
        }
    }
    return p;
}

/* O(log n) */
template <class par>
void TSTreaps<par> :: insere(Chave chave, Item valor) {
    bool criouNovoNo = false;
    raiz = insereUtilRecur(raiz, chave, valor, criouNovoNo);
}

/*Retorna o valor da chave correspondente ou -1 se a chave não existe*/
/* O(log n) */
template <class par>
Item TSTreaps<par> :: devolve(Chave chave) {
    Item valor = -1;
    NoTreap* aux;

    aux = raiz;
    
    while(aux != nullptr) {
        if(aux->chave == chave) {
            valor = aux->valor;
            break;
        }
        else if(aux->chave > chave) {
            aux = aux->esq;
        }
        else {
            aux = aux->dir;
        }
    }
    return valor;
}

template <class par>
NoTreap* TSTreaps<par> :: removeUtilRecur(NoTreap* q, Chave chave, bool& removeu) {

    //Não achou o nó
    if(q == nullptr) return nullptr;

    //O nó é uma folha
    if(q->chave == chave && q->esq == nullptr && q->dir == nullptr) {
        delete q;
        q = nullptr;
        removeu = true;
    }
    else if(q->chave == chave && q->esq == nullptr) {
        NoABB* aux;
        aux = q->dir;
        delete q;
        q = aux;
        removeu = true;
    }
    else if(q->chave == chave) {
        NoABB* maxSubEsqNo;
        Item maxSubEsqItem;
        Chave chaveMaxSubEsq;

        chaveMaxSubEsq = seleciona(rank(q)-1);
        maxSubEsqItem = devolve(chaveMaxSubEsq);
        maxSubEsqNo = criaNo(chaveMaxSubEsq, maxSubEsqItem);

        removeu = false;
        q->esq = removeUtilRecur(q->esq, chaveMaxSubEsq, removeu);

        maxSubEsqNo->esq = q->esq;
        maxSubEsqNo->dir = q->dir;
        maxSubEsqNo->quantNosSubArvEsq = q->quantNosSubArvEsq - 1;
        maxSubEsqNo->quantNosSubArvDir = q->quantNosSubArvDir;

        delete q;
        q = maxSubEsqNo;

        removeu = true;
    }
    else if(q->chave > chave) {
        q->esq = removeUtilRecur(q->esq, chave);
        if(removeu == true) {
            q->quantNosSubArvEsq--;
        }
    }
    else {
        q->dir = removeUtilRecur(q->dir, chave);
        if(removeu == true) {
            q->quantNosSubArvDir--;
        }
    }
    return q;
}

/* O(log n) */
template <class par>
void TSTreaps<par> :: remove(Chave chave) {
    bool removeu = false;
    raiz = removeUtilRecur(raiz, chave, removeu);
}

/* O(log n) */
template <class par>
int TSTreaps<par> :: rank(Chave chave) {
    int n_elements = 0;
    int r;
    if(chave == raiz->chave) {
        n_elements = raiz->quantNosSubArvEsq;
    }
    //Caso sub árvore esquerda
    else {
        NoABB* aux;
        if(chave < raiz->chave) {
            aux = raiz->esq;
        }
        else {
            aux = raiz->dir;
            n_elements += raiz->quantNosSubArvEsq + 1;
        }
        while(aux != nullptr) {
            if(aux->chave == chave) {
                n_elements += aux->quantNosSubArvEsq;
                break;
            } 
            else if(chave < aux->chave) {
                aux = aux->esq;
            }
            else {
                aux = aux->dir;
                n_elements += aux->quantNosSubArvEsq+1;
            }
        }
    }
    return n_elements;
}

/* O(n) */
template <class par>
Chave TSTreaps<par> :: seleciona(int k) {
    Chave chave = "";
    int r;
    NoABB* aux = raiz;
    while(aux != nullptr) {
        r = rank(aux->chave);
        if(r == k) {
            chave = aux->chave;
            break;
        }
        else if(k < r) {
            aux = aux->esq;
        }
        else {
            aux = aux->dir;
        }
    }

    return chave;
}

template <class par>
void TSTreaps<par> :: exibeTSUtilRecur(NoTreap* q) {
    if(q != nullptr) {
        exibeTSUtilRecur(q->esq);
        cout << "Chave: " << q->chave << ", Valor: " << q->valor << endl;
        exibeTSUtilRecur(q->dir);
    }
}

template <class par>
void TSTreaps<par> :: exibeTS() {
    exibeTSUtilRecur(raiz);
    cout << n << endl;
}

#endif