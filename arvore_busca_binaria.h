#ifndef TS_ARVORE_BUSCA_BINARIA_H
#define TS_ARVORE_BUSCA_BINARIA_H

#include <iostream>
#include "estruturas.h"
using namespace std;


template <class par>
class TSArvoreBuscaBinaria { 
    public:
        TSArvoreBuscaBinaria();
        ~TSArvoreBuscaBinaria();

        /*Árvore de Busca Binária*/
        NoABB* raiz;
        NoABB* criaNo(Chave chave, Item valor);

        /*Número de elementos*/
        int n;//public

        /*Métodos da TS*/
        void insere(Chave chave, Item valor);
        Item devolve(Chave chave);
        NoABB* removeUtilRecur(NoABB* q, Chave chave, bool& removeu);
        void remove(Chave chave);
        int rank(Chave chave);
        Chave seleciona(int k); 

        //Método utilizado pelo destructor
        void destroiArvoreABB(NoABB* q);
        void exibeTSUtilRecur(NoABB* q);
        void exibeTS(); 
};

template <class par>
TSArvoreBuscaBinaria<par> :: TSArvoreBuscaBinaria() {
    n = 0;
    raiz = nullptr;
}

template <class par>
TSArvoreBuscaBinaria<par> :: ~TSArvoreBuscaBinaria() {
    //Executar em pós-ordem
    destroiArvoreABB(raiz);
}

template <class par> 
void TSArvoreBuscaBinaria<par> :: destroiArvoreABB(NoABB* q) {
    if(q != nullptr) {
        destroiArvoreABB(q->esq);
        destroiArvoreABB(q->dir);
        delete q;
    }
}

template <class par>
NoABB* TSArvoreBuscaBinaria<par> :: criaNo(Chave chave, Item valor) {
    NoABB* novo = new NoABB;
    novo->chave = chave;
    novo->valor = valor;
    novo->esq = nullptr;
    novo->dir = nullptr;
    novo->quantNosSubArvEsq = 0;
    novo->quantNosSubArvDir = 0;
    return novo;
}

/* O(log n) */
template <class par>
void TSArvoreBuscaBinaria<par> :: insere(Chave chave, Item valor) {
    bool chaveExiste = false;
    NoABB* aux;
    bool criouNovoNo = false;

    if(raiz == nullptr) {
        raiz = criaNo(chave, valor);
        n++;
        return;
    }

    aux = raiz;
    //Atualiza o valor da chave ou cria um novo nó
    while(aux != nullptr) {
        if(aux->chave == chave) {
            aux->chave = chave;
            aux->valor += 1;
            break;
        }
        else if(aux->chave > chave) {
            if(aux->esq == nullptr) {
                NoABB* novo = criaNo(chave, valor);
                criouNovoNo = true;
                n++;
                aux->esq = novo;
                aux = aux->esq;
            }
            aux = aux->esq;
        }
        else {
            if(aux->dir == nullptr) {
                NoABB* novo = criaNo(chave, valor);
                criouNovoNo = true;
                n++;
                aux->dir = novo;
                aux = aux->dir;
            }
            aux = aux->dir;
        }
    }
    //Atualiza a quantidade de nós da subárvore esquerda e direita de cada nó
    if(criouNovoNo) {
        while(aux != nullptr) {
            if(aux->chave == chave) break;

            else if(aux->chave > chave) {
                aux->quantNosSubArvEsq++;
                aux = aux->esq;
            }
            else {
                aux->quantNosSubArvDir++;
                aux = aux->dir;
            }
        }
    }
}

/*Retorna o valor da chave correspondente ou -1 se a chave não existe*/
/* O(log n) */
template <class par>
Item TSArvoreBuscaBinaria<par> :: devolve(Chave chave) {
    Item valor = -1;
    NoABB* aux;

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
NoABB* TSArvoreBuscaBinaria<par> :: removeUtilRecur(NoABB* q, Chave chave, bool& removeu) {

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
void TSArvoreBuscaBinaria<par> :: remove(Chave chave) {
    bool removeu = false;
    raiz = removeUtilRecur(raiz, chave, removeu);
}

/* O(log n) */
template <class par>
int TSArvoreBuscaBinaria<par> :: rank(Chave chave) {
    int n_elements = 0;
    int r;
    if(chave == raiz->chave) {
        n_elements = raiz->quantNosSubArvEsq;
    }
    else {
        NoABB* aux;
        if(chave < raiz->chave) {
            aux = raiz->esq;
        }
        else {
            n_elements += raiz->quantNosSubArvEsq + 1;
            aux = raiz->dir;
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
                n_elements += aux->quantNosSubArvEsq+1;
                aux = aux->dir;
            }
        }
    }
    return n_elements;
}

/* O(n) */
template <class par>
Chave TSArvoreBuscaBinaria<par> :: seleciona(int k) {
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
void TSArvoreBuscaBinaria<par> :: exibeTSUtilRecur(NoABB* q) {
    if(q != nullptr) {
        exibeTSUtilRecur(q->esq);
        cout << "Chave: " << q->chave << ", Valor: " << q->valor << endl;
        exibeTSUtilRecur(q->dir);
    }
}

template <class par>
void TSArvoreBuscaBinaria<par> :: exibeTS() {
    exibeTSUtilRecur(raiz);
    cout << n << endl;

}

#endif