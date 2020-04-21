#ifndef TS_VETOR_ORDENADO_H
#define TS_VETOR_ORDENADO_H

#include <iostream>
#include <fstream>
#include "estruturas.h"
using namespace std;

typedef string Chave;
typedef int Item;

template <class Chave, class Valor>
class vetorOrd { 
    public:
        vetorOrd(string nome_arquivo);
        ~vetorOrd();

        /*Métodos da TS*/
        void insere(Chave chave, Item valor);
        Item devolve(Chave chave);
        void remove(Chave chave);
        int rank(Chave chave);
        Chave seleciona(int k); 

        void exibeTS();

        /*Vetor*/
        pair<Chave,Valor>* vo;
        /*Número de elementos*/
        int n;//public
        int tam;//tamanho do vetor
        void resize();
};

template <class Chave, class Valor>
vetorOrd<Chave, Valor> :: vetorOrd(string nome_arquivo) {
    n = 0;
    tam = 2;
    vo = new pair<Chave,Valor>[tam];

    ifstream texto;
    string palavra;

    texto.open(nome_arquivo, ios::in);

    while(texto >> palavra) 
        insere(palavra, 1);

    texto.close();
}

template <class Chave, class Valor>
vetorOrd<Chave, Valor> :: ~vetorOrd() {
    delete[] vo;
}

/* O(n+logn) - sem resize */
template <class Chave, class Valor>
void vetorOrd<Chave, Valor> :: insere(Chave chave, Item valor) {
    //Checa se a chave já pertence ao vetor
    //Se a chave já existe, atualiza o seu valor
    int r = rank(chave);

    if(r != n && seleciona(r) == chave) {
        vo[r].second += 1;
    }
    else {
        //O vetor está lotado, dobra seu tamanho
        if(n == tam) resize();

        if(r == n) {
            vo[n].first = chave;
            vo[n].second = valor;
            n++;
        }
        else {
            for (int i = n; i > r; i--) {
                vo[i] = vo[i-1];
            }
            vo[r].first = chave;
            vo[r].second = valor;
            n++;
        }
    }
}

/*Retorna o valor da chave correspondente ou -1 se a chave não existe*/
/* O(log n) */
template <class Chave, class Valor>
Item vetorOrd<Chave, Valor> :: devolve(Chave chave) {
    int r = rank(chave);
    if (r < n) return vo[r].second;
    return -1;
}

/* O(n+log n) - Sem lazy deletion*/
template <class Chave, class Valor>
void vetorOrd<Chave, Valor> :: remove(Chave chave) {
    int r = rank(chave);
    if (r != n) {
        for (int i = r; i < n-1; i++) 
            vo[r] = vo[r+1];
        n--;
    }
}

/* O(log n) */
template <class Chave, class Valor>
int vetorOrd<Chave, Valor> :: rank(Chave chave) {
    int ini, fim, mid;
    int r = n;
    ini = 0;
    fim = n-1;

    while(ini <= fim) {
        mid = (ini+fim)/2;
        if(vo[mid].first == chave) {
            r = mid;
            break;
        } 
        else if(vo[mid].first > chave && ( ((mid-1) >= 0 && vo[mid-1].first < chave) || (mid-1) < 0 ) ) {
            r = mid;
            break;
        }
        else if(vo[mid].first > chave) 
            fim = mid-1;
        else 
            ini = mid+1;
    }
    return r;
}

/* O(1) */
template <class Chave, class Valor>
Chave vetorOrd<Chave, Valor> :: seleciona(int k) {
    if(k < n)
        return vo[k].first;
    return "";
}

template <class Chave, class Valor>
void vetorOrd<Chave, Valor> :: resize() {
    //Novo vetor desordenado
    pair<Chave,Valor>* von = new pair<Chave,Valor>[2*tam];
    tam = tam*2;

    for (int i = 0; i < n; i++) {
        von[i] = vo[i];
    }
    delete[] vo;
    vo = von;
}

template <class Chave, class Valor>
void vetorOrd<Chave, Valor> :: exibeTS() {
    for (int i = 0; i < n; i++) {
        cout << "Chave: " << vo[i].first << ", Valor: " << vo[i].second << endl;
    }
    cout << n << endl;
}


#endif