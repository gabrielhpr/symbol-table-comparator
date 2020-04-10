#ifndef TS_ARVORES_RUBRO_NEGRAS_H
#define TS_ARVORES_RUBRO_NEGRAS_H

#include <iostream>
#include "estruturas.h"
using namespace std;


template <class par>
class TSArvoresRubroNegras { 
    public:
        TSArvoresRubroNegras();
        ~TSArvoresRubroNegras();

        /*Árvore Rubro Negra*/
        NoRN* raiz;
        NoRN* criaNo(Chave chave, Item valor);

        /*Número de elementos*/
        int n;//public

        /*Métodos da TS*/
        void insere(Chave chave, Item valor);
        Item devolve(Chave chave);
        NoRN* removeUtilRecur(NoRN* q, Chave chave, bool& removeu);
        void remove(Chave chave);
        int rank(Chave chave);
        Chave seleciona(int k); 

        //Método utilizado pelo destructor
        void destroiArvoreRubroNegra(NoRN* q);
        void exibeTSUtilRecur(NoRN* q);
        void exibeTS(); 
};

template <class par>
TSArvoresRubroNegras<par> :: TSArvoresRubroNegras() {
    n = 0;
    raiz = nullptr;
}

template <class par>
TSArvoresRubroNegras<par> :: ~TSArvoresRubroNegras() {
    //Executar em pós-ordem
    destroiArvoreRubroNegra(raiz);
}

template <class par> 
void TSArvoresRubroNegras<par> :: destroiArvoreRubroNegra(NoRN* q) {
    if(q != nullptr) {
        destroiArvoreRubroNegra(q->esq);
        destroiArvoreRubroNegra(q->dir);
        delete q;
    }
}

template <class par>
NoRN* TSArvoresRubroNegras<par> :: criaNo(Chave chave, Item valor) {
    NoRN* novo = new NoRN;
    novo->chave = chave;
    novo->valor = valor;
    novo->cor = 'V';
    novo->esq = nullptr;
    novo->dir = nullptr;
    novo->pai = nullptr;
    novo->quantNosSubArvEsq = 0;
    novo->quantNosSubArvDir = 0;
    return novo;
}

/* O(log n) */
template <class par>
void TSArvoresRubroNegras<par> :: insere(Chave chave, Item valor) {
    //Caso raiz nula
    if(raiz == nullptr) {
        raiz = criaNo(chave, valor);
        n++;
        return;
    }

    bool achou = false;
    NoRN *p = raiz;

    //Busca
    while(!achou) {
        if(chave < p->chave && p->esq != nullptr) {
            p = p->esq; 
        }
        else if(chave < p->chave && p->esq == nullptr) {
            achou = true;
        }
        else if(chave > p->chave && p->dir != nullptr) {
            p = p->dir;
        }
        else if(chave > p->chave && p->dir == nullptr) {
            achou = true;
        }
        else {
            achou = true;
        }
    }

    //Atualiza se já existe
    if(p->chave == chave) {
        p->valor++;
        return;
    }

    //Cria o novo nó
    NoRN* novo = criaNo(chave, valor);
    novo->pai = p;
    n++;

    //Insere o novo nó vermelho
    if(novo->chave > p->chave) {
        p->dir = novo;
    }
    else {
        p->esq = novo;
    }

    NoRN* filho = novo;

    while(p != nullptr) {
        //Caso pai preto, não altera nada
        if(p->cor == 'P') {
            break;
        }
        //Caso sem avô, troca a cor do pai
        NoRN* avo = p->pai;
        if(avo == nullptr) {
            p->cor = 'P';
            break;
        }
        //Caso tio vermelho
        NoRN* tio = (avo->esq == p) ? avo->dir : avo->esq;
        if(tio != nullptr && tio->cor == 'V') {
            avo->cor = 'V';
            p->cor = 'P';
            tio->cor = 'P';
            filho = avo;
            p = filho->pai;
        }
        //Caso tio preto ou null
        else {
            if(p == avo->esq && filho == p->dir) {
                //Rotação pra esquerda, p sendo o eixo
                p->dir = filho->esq;
                if(filho->esq != nullptr) filho->esq->pai = p;
                filho->esq = p;
                filho->pai = avo;
                avo->esq = filho;
                p->pai = filho;

                p = avo->esq;
                filho = filho->esq;
            }
            else if(p == avo->dir && filho == p->esq) {
                //Rotação pra direita, p sendo o eixo
                p->esq = filho->dir;
                if(filho->dir != nullptr) filho->dir->pai = p;
                filho->dir = p;
                filho->pai = avo;
                avo->dir = filho;
                p->pai = filho;

                p = avo->dir;
                filho = filho->dir;
            }
            else if(p == avo->esq && filho == p->esq) {
                //Rotação pra direita
                if(avo->pai != nullptr) {
                    if(avo->pai->esq == avo) avo->pai->esq = p;
                    else avo->pai->dir = p;
                }
                avo->esq = p->dir;
                if(p->dir != nullptr) p->dir->pai = avo;
                p->dir = avo;
                p->pai = avo->pai;
                avo->pai = p;
                p->cor = 'P';
                avo->cor = 'V';

                if(avo == raiz) {
                    raiz = p;
                    break;
                }
            }
            else if(p == avo->dir && filho == p->dir) {
                //Rotação pra esquerda
                if(avo->pai != nullptr) {
                    if(avo->pai->esq == avo) avo->pai->esq = p;
                    else avo->pai->dir = p;
                }
                avo->dir = p->esq;
                if(p->esq != nullptr) p->esq->pai = avo;
                p->esq = avo;
                p->pai = avo->pai;
                avo->pai = p;
                p->cor = 'P';
                avo->cor = 'V';

                if(avo == raiz) {
                    raiz = p;
                    break;
                }
            }            
        }
    }
}

/*Retorna o valor da chave correspondente ou -1 se a chave não existe*/
/* O(log n) */
template <class par>
Item TSArvoresRubroNegras<par> :: devolve(Chave chave) {
    Item valor = -1;
    NoRN* aux;

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
NoRN* TSArvoresRubroNegras<par> :: removeUtilRecur(NoRN* q, Chave chave, bool& removeu) {

}

/* O(log n) */
template <class par>
void TSArvoresRubroNegras<par> :: remove(Chave chave) {
    bool removeu = false;
    
}

/* O(log n) */
template <class par>
int TSArvoresRubroNegras<par> :: rank(Chave chave) {
    int n_elements = 0;
    int r;
    if(chave == raiz->chave) {
        n_elements = raiz->quantNosSubArvEsq;
    }
    //Caso sub árvore esquerda
    else {
        NoRN* aux;
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
Chave TSArvoresRubroNegras<par> :: seleciona(int k) {
    Chave chave = "";
    int r;
    NoRN* aux = raiz;
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
void TSArvoresRubroNegras<par> :: exibeTSUtilRecur(NoRN* q) {
    if(q != nullptr) {
        exibeTSUtilRecur(q->esq);
        cout << "Chave: " << q->chave << ", Valor: " << q->valor << endl;
        exibeTSUtilRecur(q->dir);
    }
}

template <class par>
void TSArvoresRubroNegras<par> :: exibeTS() {
    exibeTSUtilRecur(raiz);
    cout << n << endl;
}

#endif