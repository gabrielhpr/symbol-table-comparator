#ifndef TS_ARVORE_BUSCA_BINARIA_H
#define TS_ARVORE_BUSCA_BINARIA_H

#include <iostream>
#include <fstream>
#include "estruturas.hpp"
using namespace std;


template <class Chave, class Valor>
class arvoreBin { 
    public:
        arvoreBin(string nome_arquivo);
        ~arvoreBin();

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

template <class Chave, class Valor>
arvoreBin<Chave, Valor> :: arvoreBin(string nome_arquivo) {
    n = 0;
    raiz = nullptr;

    ifstream texto;
    string palavra;

    texto.open(nome_arquivo, ios::in);

    while(texto >> palavra) 
        insere(palavra, 1);

    texto.close();
}

template <class Chave, class Valor>
arvoreBin<Chave, Valor> :: ~arvoreBin() {
    //Executar em pós-ordem
    destroiArvoreABB(raiz);
}

template <class Chave, class Valor> 
void arvoreBin<Chave, Valor> :: destroiArvoreABB(NoABB* q) {
    if(q != nullptr) {
        destroiArvoreABB(q->esq);
        destroiArvoreABB(q->dir);
        delete q;
    }
}

template <class Chave, class Valor>
NoABB* arvoreBin<Chave, Valor> :: criaNo(Chave chave, Item valor) {
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
template <class Chave, class Valor>
void arvoreBin<Chave, Valor> :: insere(Chave chave, Item valor) {
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
template <class Chave, class Valor>
Item arvoreBin<Chave, Valor> :: devolve(Chave chave) {
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

template <class Chave, class Valor>
NoABB* arvoreBin<Chave, Valor> :: removeUtilRecur(NoABB* q, Chave chave, bool& removeu) {

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

        chaveMaxSubEsq = seleciona(rank(q->chave)-1);
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
        q->esq = removeUtilRecur(q->esq, chave, removeu);
        if(removeu == true) {
            q->quantNosSubArvEsq--;
        }
    }
    else {
        q->dir = removeUtilRecur(q->dir, chave, removeu);
        if(removeu == true) {
            q->quantNosSubArvDir--;
        }
    }
    return q;
}

/* O(log n) */
template <class Chave, class Valor>
void arvoreBin<Chave, Valor> :: remove(Chave chave) {
    bool removeu = false;
    raiz = removeUtilRecur(raiz, chave, removeu);
}

/* O(log n) */
template <class Chave, class Valor>
int arvoreBin<Chave, Valor> :: rank(Chave chave) {
    int n_elements = 0;
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
template <class Chave, class Valor>
Chave arvoreBin<Chave, Valor> :: seleciona(int k) {
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

template <class Chave, class Valor>
void arvoreBin<Chave, Valor> :: exibeTSUtilRecur(NoABB* q) {
    if(q != nullptr) {
        exibeTSUtilRecur(q->esq);
        cout << "Chave: " << q->chave << ", Valor: " << q->valor << endl;
        exibeTSUtilRecur(q->dir);
    }
}

template <class Chave, class Valor>
void arvoreBin<Chave, Valor> :: exibeTS() {
    exibeTSUtilRecur(raiz);
    cout << n << endl;

}

#endif