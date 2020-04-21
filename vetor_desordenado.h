#ifndef TS_VETOR_DESORDENADO_H
#define TS_VETOR_DESORDENADO_H

#include <iostream>
#include <fstream>
#include "estruturas.h"
using namespace std;


template <class Chave, class Valor>
class vetorDes { 
    public:
        vetorDes(string nome_arquivo);
        ~vetorDes();

        /*Métodos da TS*/
        void insere(Chave chave, Item valor);
        Item devolve(Chave chave);
        void remove(Chave chave);
        int rank(Chave chave);
        Chave seleciona(int k); 

        void exibeTS();

        /*Vetor*/
        pair<Chave,Valor>* vd;
        /*Número de elementos*/
        int n;//public
        int tam;//tamanho do vetor
        void resize();
};

template <class Chave, class Valor>
vetorDes<Chave,Valor> :: vetorDes(string nome_arquivo) {
    n = 0;
    tam = 2;
    vd = new pair<Chave,Valor>[tam];

    ifstream texto;
    string palavra;

    texto.open(nome_arquivo, ios::in);

    while(texto >> palavra) 
        insere(palavra, 1);

    texto.close();
}

template <class Chave, class Valor>
vetorDes<Chave,Valor> :: ~vetorDes() {
    delete[] vd;
}

/* O(n) - sem resize */
template <class Chave, class Valor>
void vetorDes<Chave, Valor> :: insere(Chave chave, Item valor) {
    bool chaveExiste = false;
    //Checa se a chave já pertence ao vetor
    for (int i = 0; i < n && !chaveExiste; i++) {
        //Se a chave já existe, atualiza o seu valor
        if(vd[i].first == chave) {
            chaveExiste = true;
            vd[i].second += 1; 
        }
    }
    //O vetor está lotado
    if(n == tam && !chaveExiste) {
        //Dobra o vetor de tamanho
        resize();
        vd[n].first = chave;
        vd[n].second = valor;
        n++;
    }
    else if(!chaveExiste) {
        vd[n].first = chave;
        vd[n].second = valor;
        n++;
    }
}

/*Retorna o valor da chave correspondente ou -1 se a chave não existe*/
/* O(n) */
template <class Chave, class Valor>
Item vetorDes<Chave, Valor> :: devolve(Chave chave) {
    Item valor = -1;
    bool achou_chave = false;

    for (int i = 0; i < n && !achou_chave; i++) {
        if(vd[i].first == chave) {
            valor = vd[i].second;
            achou_chave = true;
        }
    }
    return valor;
}

/* O(n) - Sem lazy deletion*/
template <class Chave, class Valor>
void vetorDes<Chave, Valor> :: remove(Chave chave) {
    for (int i = 0; i < n; i++) {
        if(vd[i].first == chave) {
            vd[i] = vd[n-1];
            n--;
            break;
        }
    }
}

/* O(n) */
template <class Chave, class Valor>
int vetorDes<Chave, Valor> :: rank(Chave chave) {
    int n_elements = 0;

    for (int i = 0; i < n; i++) {
        if(vd[i].first < chave) n_elements++;
    }
    return n_elements;
}

/* O(n^2) */
template <class Chave, class Valor>
Chave vetorDes<Chave, Valor> :: seleciona(int k) {
    Chave chave = "";
    bool achei_chave = false;

    for (int i = 0; i < n && !achei_chave; i++) {
        if (rank(vd[i].first) == k) {
            chave = vd[i].first;
            achei_chave = true;
        }
    }
    return chave;
}

template <class Chave, class Valor>
void vetorDes<Chave, Valor> :: resize() {
    //Novo vetor desordenado
    pair<Chave,Valor>* vdn = new pair<Chave,Valor>[2*tam];
    tam = tam*2;

    for (int i = 0; i < n; i++) {
        vdn[i] = vd[i];
    }
    delete[] vd;
    vd = vdn;
}

template <class Chave, class Valor>
void vetorDes<Chave, Valor> :: exibeTS() {
    for (int i = 0; i < n; i++) {
        cout << "Chave: " << vd[i].first << ", Valor: " << vd[i].second << endl;
    }
    cout << n << endl;
}

#endif