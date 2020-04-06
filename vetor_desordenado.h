#ifndef TS_VETOR_DESORDENADO_H
#define TS_VETOR_DESORDENADO_H

#include <iostream>
#include "estruturas.h"
using namespace std;


template <class par>
class TSVetorDesordenado { 
    public:
        TSVetorDesordenado();
        ~TSVetorDesordenado();

        /*Métodos da TS*/
        void insere(Chave chave, Item valor);
        Item devolve(Chave chave);
        void remove(Chave chave);
        int rank(Chave chave);
        Chave seleciona(int k); 

        void exibeTS();

        /*Vetor*/
        par* vd;
        /*Número de elementos*/
        int n;//public
        int tam;//tamanho do vetor
        void resize();
};

template <class par>
TSVetorDesordenado<par> :: TSVetorDesordenado() {
    n = 0;
    tam = 2;
    vd = new par[tam];
}

template <class par>
TSVetorDesordenado<par> :: ~TSVetorDesordenado() {
    delete[] vd;
}

/* O(n) - sem resize */
template <class par>
void TSVetorDesordenado<par> :: insere(Chave chave, Item valor) {
    bool chaveExiste = false;
    //Checa se a chave já pertence ao vetor
    for (int i = 0; i < n && !chaveExiste; i++) {
        //Se a chave já existe, atualiza o seu valor
        if(vd[i].chave == chave) {
            chaveExiste = true;
            vd[i].valor += 1; 
        }
    }
    //O vetor está lotado
    if(n == tam && !chaveExiste) {
        //Dobra o vetor de tamanho
        resize();
        vd[n].chave = chave;
        vd[n].valor = valor;
        n++;
    }
    else if(!chaveExiste) {
        vd[n].chave = chave;
        vd[n].valor = valor;
        n++;
    }
}

/*Retorna o valor da chave correspondente ou -1 se a chave não existe*/
/* O(n) */
template <class par>
Item TSVetorDesordenado<par> :: devolve(Chave chave) {
    Item valor = -1;
    bool achou_chave = false;

    for (int i = 0; i < n && !achou_chave; i++) {
        if(vd[i].chave == chave) {
            valor = vd[i].valor;
            achou_chave = true;
        }
    }
    return valor;
}

/* O(n) - Sem lazy deletion*/
template <class par>
void TSVetorDesordenado<par> :: remove(Chave chave) {
    for (int i = 0; i < n; i++) {
        if(vd[i].chave == chave) {
            vd[i] = vd[n-1];
            n--;
            break;
        }
    }
}

/* O(n) */
template <class par>
int TSVetorDesordenado<par> :: rank(Chave chave) {
    int n_elements = 0;

    for (int i = 0; i < n; i++) {
        if(vd[i].chave < chave) n_elements++;
    }
    return n_elements;
}

/* O(n^2) */
template <class par>
Chave TSVetorDesordenado<par> :: seleciona(int k) {
    Chave chave = "";
    bool achei_chave = false;

    for (int i = 0; i < n && !achei_chave; i++) {
        if (rank(vd[i].chave) == k) {
            chave = vd[i].chave;
            achei_chave = true;
        }
    }
    return chave;
}

template <class par>
void TSVetorDesordenado<par> :: resize() {
    //Novo vetor desordenado
    par* vdn = new par[2*tam];
    tam = tam*2;

    for (int i = 0; i < n; i++) {
        vdn[i] = vd[i];
    }
    delete[] vd;
    vd = vdn;
}

template <class par>
void TSVetorDesordenado<par> :: exibeTS() {
    for (int i = 0; i < n; i++) {
        cout << "Chave: " << vd[i].chave << ", Valor: " << vd[i].valor << endl;
    }
}

#endif