#ifndef TS_LISTA_LIGADA_DESORDENADA_H
#define TS_LISTA_LIGADA_DESORDENADA_H

#include <iostream>
#include <fstream>
#include "estruturas.hpp"
using namespace std;


template <class Chave, class Valor>
class listaDes { 
    public:
        listaDes(string nome_arquivo);
        ~listaDes();

        /*Métodos da TS*/
        void insere(Chave chave, Item valor);
        Item devolve(Chave chave);
        void remove(Chave chave);
        int rank(Chave chave);
        Chave seleciona(int k); 

        void exibeTS();

        /*Lista Ligada Desordenada*/
        Celula* cabeca_lld;

        /*Número de elementos*/
        int n;//public
};

template <class Chave, class Valor>
listaDes<Chave, Valor> :: listaDes(string nome_arquivo) {
    n = 0;
    cabeca_lld = new Celula;
    cabeca_lld->prox = nullptr;

    ifstream texto;
    string palavra;

    texto.open(nome_arquivo, ios::in);

    while(texto >> palavra) 
        insere(palavra, 1);

    texto.close();
}

template <class Chave, class Valor>
listaDes<Chave, Valor> :: ~listaDes() {
    Celula* aux;

    for (aux = cabeca_lld; aux != nullptr;) {
        cabeca_lld = aux->prox;
        delete aux;
        aux = cabeca_lld;
    }
}

/* O(n) */
template <class Chave, class Valor>
void listaDes<Chave, Valor> :: insere(Chave chave, Item valor) {
    bool chaveExiste = false;
    Celula* aux;
    //Checa se a chave já pertence a lista
    for (aux = cabeca_lld->prox; aux != nullptr && !chaveExiste; aux = aux->prox) {
        //Se a chave já existe, atualiza o seu valor
        if(aux->chave == chave) {
            chaveExiste = true;
            aux->valor += 1; 
        }
    }
    //Cria uma nova célula e a insere na lista
    if(!chaveExiste) {
        Celula* nova = new Celula;
        nova->chave = chave;
        nova->valor = valor;
        nova->prox = cabeca_lld->prox;
        cabeca_lld->prox = nova;
        n++;
    }
}

/*Retorna o valor da chave correspondente ou -1 se a chave não existe*/
/* O(n) */
template <class Chave, class Valor>
Item listaDes<Chave, Valor> :: devolve(Chave chave) {
    Item valor = -1;
    bool achou_chave = false;
    Celula* aux;

    for (aux = cabeca_lld->prox; aux != nullptr && !achou_chave; aux = aux->prox) {
        if(aux->chave == chave) {
            valor = aux->valor;
            achou_chave = true;
        }
    }
    return valor;
}

/* O(n) */
template <class Chave, class Valor>
void listaDes<Chave, Valor> :: remove(Chave chave) {
    Celula *aux, *ant;
    ant = cabeca_lld;

    for (aux = cabeca_lld->prox; aux != nullptr; aux = aux->prox, ant = ant->prox) {
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
int listaDes<Chave, Valor> :: rank(Chave chave) {
    int n_elements = 0;
    Celula* aux;

    for (aux = cabeca_lld->prox; aux != nullptr; aux = aux->prox) {
        if(aux->chave < chave) n_elements++;
    }
    return n_elements;
}

/* O(n^2) */
template <class Chave, class Valor>
Chave listaDes<Chave, Valor> :: seleciona(int k) {
    Chave chave = "";
    bool achei_chave = false;
    Celula* aux;

    for (aux = cabeca_lld->prox; aux != nullptr && !achei_chave; aux = aux->prox) {
        if (rank(aux->chave) == k) {
            chave = aux->chave;
            achei_chave = true;
        }
    }
    return chave;
}

template <class Chave, class Valor>
void listaDes<Chave, Valor> :: exibeTS() {
    Celula* aux;
    for (aux = cabeca_lld->prox; aux != nullptr; aux = aux->prox) {
        cout << "Chave: " << aux->chave << ", Valor: " << aux->valor << endl;
    }
}

#endif