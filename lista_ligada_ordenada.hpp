#ifndef TS_LISTA_LIGADA_ORDENADA_H
#define TS_LISTA_LIGADA_ORDENADA_H

#include <iostream>
#include <fstream>
#include "estruturas.hpp"
using namespace std;

/*
ideia guardar um ponteiro para o fim, assim é possível checar no insere se r == n
insere direto no final, assim fica n comparações e não 2n
*/

template <class Chave, class Valor>
class listaOrd { 
    public:
        listaOrd(string nome_arquivo);
        ~listaOrd();

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

template <class Chave, class Valor>
listaOrd<Chave, Valor> :: listaOrd(string nome_arquivo) {
    n = 0;
    cabeca_llo = new Celula;
    cabeca_llo->prox = nullptr;

    ifstream texto;
    string palavra;

    texto.open(nome_arquivo, ios::in);

    while(texto >> palavra) 
        insere(palavra, 1);

    texto.close();
}

template <class Chave, class Valor>
listaOrd<Chave, Valor> :: ~listaOrd() {
    Celula* aux;

    for (aux = cabeca_llo; aux != nullptr;) {
        cabeca_llo = aux->prox;
        delete aux;
        aux = cabeca_llo;
    }
}

/* O(n) */
template <class Chave, class Valor>
void listaOrd<Chave, Valor> :: insere(Chave chave, Item valor) {
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
template <class Chave, class Valor>
Item listaOrd<Chave, Valor> :: devolve(Chave chave) {
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
template <class Chave, class Valor>
void listaOrd<Chave, Valor> :: remove(Chave chave) {
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
template <class Chave, class Valor>
int listaOrd<Chave, Valor> :: rank(Chave chave) {
    int n_elements = 0;
    Celula* aux;

    for (aux = cabeca_llo->prox; aux != nullptr; aux = aux->prox) {
        if(aux->chave >= chave) break;
        n_elements++;
    }
    return n_elements;
}

/* O(n) */
template <class Chave, class Valor>
Chave listaOrd<Chave, Valor> :: seleciona(int k) {
    Chave chave = "";
    Celula* aux;
    int i;

    for (i = 0, aux = cabeca_llo->prox; i < k && aux != nullptr; i++, aux = aux->prox);

    if(i == k) chave = aux->chave;
    
    return chave;
}

template <class Chave, class Valor>
void listaOrd<Chave, Valor> :: exibeTS() {
    Celula* aux;
    for (aux = cabeca_llo->prox; aux != nullptr; aux = aux->prox) {
        cout << "Chave: " << aux->chave << ", Valor: " << aux->valor << endl;
    }
}

#endif