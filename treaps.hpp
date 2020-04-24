#ifndef TS_TREAPS_H
#define TS_TREAPS_H

#include <iostream>
#include <fstream>
#include <ctime> 
#include <climits> /*INT_MAX*/
#include "estruturas.hpp" /* NoTreap* */
using namespace std;


template <class Chave, class Valor>
class treap { 
    public:
        treap(string nome_arquivo);
        ~treap();

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

template <class Chave, class Valor>
treap<Chave, Valor> :: treap(string nome_arquivo) {
    n = 0;
    srand((int) time(0));
    raiz = nullptr;

    ifstream texto;
    string palavra;

    texto.open(nome_arquivo, ios::in);

    while(texto >> palavra) 
        insere(palavra, 1);

    texto.close();
}

template <class Chave, class Valor>
treap<Chave, Valor> :: ~treap() {
    //Executar em pós-ordem
    destroiTreap(raiz);
}

template <class Chave, class Valor> 
void treap<Chave, Valor> :: destroiTreap(NoTreap* q) {
    if(q != nullptr) {
        destroiTreap(q->esq);
        destroiTreap(q->dir);
        delete q;
    }
}

template <class Chave, class Valor>
NoTreap* treap<Chave, Valor> :: rotacaoEsq(NoTreap* p) {
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

template <class Chave, class Valor>
NoTreap* treap<Chave, Valor> :: rotacaoDir(NoTreap* p) {
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

template <class Chave, class Valor>
NoTreap* treap<Chave, Valor> :: criaNo(Chave chave, Item valor) {
    NoTreap* novo = new NoTreap;
    novo->chave = chave;
    novo->valor = valor;
    novo->prioridade = rand()% INT_MAX;    
    novo->esq = nullptr;
    novo->dir = nullptr;
    novo->quantNosSubArvEsq = 0;
    novo->quantNosSubArvDir = 0;
    return novo;
}

template <class Chave, class Valor>
NoTreap* treap<Chave, Valor> :: insereUtilRecur(NoTreap* p, Chave chave, Item valor, bool& criouNovoNo) {
    
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
template <class Chave, class Valor>
void treap<Chave, Valor> :: insere(Chave chave, Item valor) {
    bool criouNovoNo = false;
    raiz = insereUtilRecur(raiz, chave, valor, criouNovoNo);
}

/*Retorna o valor da chave correspondente ou -1 se a chave não existe*/
/* O(log n) */
template <class Chave, class Valor>
Item treap<Chave, Valor> :: devolve(Chave chave) {
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

template <class Chave, class Valor>
NoTreap* treap<Chave, Valor> :: removeUtilRecur(NoTreap* q, Chave chave, bool& removeu) {

    //Não achou o nó
    if(q == nullptr) return nullptr;

    //O nó é uma folha
    if(q->chave == chave && q->esq == nullptr && q->dir == nullptr) {
        delete q;
        q = nullptr;
        removeu = true;
        n--;
    }
    else if(q->chave == chave) {
        if(q->dir == nullptr) {
            q = rotacaoDir(q);
            q->dir = removeUtilRecur(q->dir, chave, removeu);
            if(removeu == true) {
                q->quantNosSubArvDir++;
            }
        }
        else if(q->esq == nullptr) {
            q = rotacaoEsq(q);
            q->esq = removeUtilRecur(q->esq, chave, removeu);
            if(removeu == true) {
                q->quantNosSubArvEsq++;
            }
        }
        else {
            if(q->dir->prioridade > q->esq->prioridade) {
                q = rotacaoEsq(q);
                q->esq = removeUtilRecur(q->esq, chave, removeu);
                if(removeu == true) {
                    q->quantNosSubArvEsq++;
                }

            }
            else {
                q = rotacaoDir(q);
                q->dir = removeUtilRecur(q->dir, chave, removeu);
                if(removeu == true) {
                    q->quantNosSubArvDir++;
                }
            }
        }
    }
    else if(chave < q->chave) {
        q->esq = removeUtilRecur(q->esq, chave, removeu);
        if(removeu == true) {
            q->quantNosSubArvEsq++;
        }
    }
    else if(chave > q->chave) {
        q->dir = removeUtilRecur(q->dir, chave, removeu);
        if(removeu == true) {
            q->quantNosSubArvDir++;
        }
    }
    
    return q;
}

/* O(log n) */
template <class Chave, class Valor>
void treap<Chave, Valor> :: remove(Chave chave) {
    bool removeu = false;
    raiz = removeUtilRecur(raiz, chave, removeu);
}

/* O(log n) */
template <class Chave, class Valor>
int treap<Chave, Valor> :: rank(Chave chave) {
    int n_elements = 0;
    if(chave == raiz->chave) {
        n_elements = raiz->quantNosSubArvEsq;
    }
    //Caso sub árvore esquerda
    else {
        NoTreap* aux;
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
Chave treap<Chave, Valor> :: seleciona(int k) {
    Chave chave = "";
    int r;
    NoTreap* aux = raiz;
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
void treap<Chave, Valor> :: exibeTSUtilRecur(NoTreap* q) {
    if(q != nullptr) {
        exibeTSUtilRecur(q->esq);
        cout << "Chave: " << q->chave << ", Valor: " << q->valor << endl;
        exibeTSUtilRecur(q->dir);
    }
}

template <class Chave, class Valor>
void treap<Chave, Valor> :: exibeTS() {
    exibeTSUtilRecur(raiz);
    cout << n << endl;
}

#endif