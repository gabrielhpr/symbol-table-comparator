#ifndef TS_LISTA_LIGADA_ORDENADA_H
#define TS_LISTA_LIGADA_ORDENADA_H

#include <iostream>
#include "estruturas.h"
using namespace std;

/*
ideia guardar um ponteiro para o fim, assim é possível checar no insere se r == n
insere direto no final, assim fica n comparações e não 2n
*/

template <class par>
class TSListaLigadaOrdenada { 
    public:
        TSListaLigadaOrdenada();
        ~TSListaLigadaOrdenada();

        /*Métodos da TS*/
        void insere(Chave chave, Item valor);
        Item devolve(Chave chave);
        void remove(Chave chave);
        int rank(Chave chave);
        Chave seleciona(int k); 

        void exibeTS();

        /*Lista Ligada Ordenada*/
        Celula* cabeca_llo;

        /*Número de elementos*/
        int n;//public
};

template <class par>
TSListaLigadaOrdenada<par> :: TSListaLigadaOrdenada() {
    n = 0;
    cabeca_llo = new Celula;
    cabeca_llo->prox = nullptr;
}

template <class par>
TSListaLigadaOrdenada<par> :: ~TSListaLigadaOrdenada() {
    Celula* aux;

    for (aux = cabeca_llo; aux != nullptr; aux = aux->prox) {
        cabeca_llo = aux->prox;
        delete aux;
        aux = cabeca_llo;
    }
}

/* O(n) */
template <class par>
void TSListaLigadaOrdenada<par> :: insere(Chave chave, Item valor) {
    bool chaveExiste = false;
    Celula *aux, *ant;
    int r = rank(chave);
    int i;

    //Avança até a posição de índice r
    for(i = 0, ant = cabeca_llo, aux=cabeca_llo->prox; i < r && !chaveExiste; i++, ant = ant->prox, aux = aux->prox);

    //Checa se a chave já pertence a lista
    //Se a chave já existe, atualiza o seu valor
    if (aux != nullptr && aux->chave == chave) {
        chaveExiste = true;
        aux->valor += 1;
    }
    //Cria uma nova célula e a insere na lista
    else {
        Celula* nova = new Celula;
        nova->chave = chave;
        nova->valor = valor;
        nova->prox = aux;
        ant->prox = nova;
        n++;
    }
}

/*Retorna o valor da chave correspondente ou -1 se a chave não existe*/
/* O(n) */
template <class par>
Item TSListaLigadaOrdenada<par> :: devolve(Chave chave) {
    Item valor = -1;
    bool achou_chave = false;
    Celula* aux;

    for (aux = cabeca_llo->prox; aux != nullptr && !achou_chave; aux = aux->prox) {
        if(aux->chave == chave) {
            valor = aux->valor;
            achou_chave = true;
        }
    }
    return valor;
}

/* O(n) */
template <class par>
void TSListaLigadaOrdenada<par> :: remove(Chave chave) {
    Celula *aux, *ant;
    ant = cabeca_llo;

    for (aux = cabeca_llo->prox; aux != nullptr; aux = aux->prox, ant = ant->prox) {
        if(aux->chave == chave) {
            //A célula anterior passa a apontar para a célula posterior a aux
            ant->prox = aux->prox;
            delete aux;
            n--;
            break;
        }
    }
}

/* O(n) */
template <class par>
int TSListaLigadaOrdenada<par> :: rank(Chave chave) {
    int n_elements = 0;
    Celula* aux;

    for (aux = cabeca_llo->prox; aux != nullptr; aux = aux->prox) {
        if(aux->chave >= chave) break;
        n_elements++;
    }
    return n_elements;
}

/* O(n) */
template <class par>
Chave TSListaLigadaOrdenada<par> :: seleciona(int k) {
    Chave chave = "";
    Celula* aux;
    int i;

    for (i = 0, aux = cabeca_llo->prox; i < k && aux != nullptr; i++, aux = aux->prox);

    if(i == k) chave = aux->chave;
    
    return chave;
}

template <class par>
void TSListaLigadaOrdenada<par> :: exibeTS() {
    Celula* aux;
    for (aux = cabeca_llo->prox; aux != nullptr; aux = aux->prox) {
        cout << "Chave: " << aux->chave << ", Valor: " << aux->valor << endl;
    }
}

#endif