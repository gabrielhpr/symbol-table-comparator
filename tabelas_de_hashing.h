#ifndef TS_TABELAS_DE_HASHING_H
#define TS_TABELAS_DE_HASHING_H

#include <iostream>
#include <cstdlib>
#include "estruturas.h"
using namespace std;

typedef string Chave;
typedef int Item;

template <class par>
class TSTabelasDeHashing { 
    public:
        TSTabelasDeHashing();
        ~TSTabelasDeHashing();

        /*Métodos da TS*/
        void insere(Chave chave, Item valor);
        Item devolve(Chave chave);
        void remove(Chave chave);
        int rank(Chave chave);
        Chave seleciona(int k); 

        int hash(Chave chave);
        void exibeTS();
        void checaDistribuicaoDaTabela();

        /*Vetor*/
        CelulasHash** table;

        /*Número de chaves*/
        int n;
        /*Número de posições da tabela*/
        int m;
};

template <class par>
TSTabelasDeHashing<par> :: TSTabelasDeHashing() {
    n = 0;
    m = 7000;
    table = new CelulasHash*[m];

    for (int i = 0; i < m; i++)
        table[i] = nullptr;
}

template <class par>
TSTabelasDeHashing<par> :: ~TSTabelasDeHashing() {
    CelulasHash *aux, *aux2;
    for (int i = 0; i < m; i++) {
        if((aux = table[i]) != nullptr) {
            while (aux != nullptr) {
                aux2 = aux->prox;
                delete aux;
                aux = aux2;
            }
        }
    }
    delete[] table;
}

template <class par>
int TSTabelasDeHashing<par> :: hash(Chave chave) {
    int len = chave.length();
    int h = 0;

    for (int i = 0; i < len; i++) {
        h = (h*23+chave[i]) % m;
    }
    return h;
}
 
/* O(1) */
template <class par>
void TSTabelasDeHashing<par> :: insere(Chave chave, Item valor) {
    int pos = hash(chave);
    CelulasHash *novo, *aux;
    bool achou = false;

    //Verifica se o elemento já existe
    if( (aux = table[pos]) != nullptr) {
        while (aux != nullptr && !achou) {
            if(aux->chave == chave) {
                aux->valor += 1;
                achou = true;
            }
            aux = aux->prox;
        }
    }

    if(achou) return;
    //Caso não exista
    novo = new CelulasHash;
    novo->chave = chave;
    novo->valor = valor;
    novo->ant = novo->prox = nullptr;

    aux = table[pos];

    //Se não há ninguém na posição, insere
    if(aux == nullptr) {   
        table[pos] = novo;
    }
    else {
        //Insere no início
        novo->prox = aux;        
        table[pos] = novo;
        aux->ant = novo;   
    }
}

/*Retorna o valor da chave correspondente ou -1 se a chave não existe*/
/* O(log n) */
template <class par>
Item TSTabelasDeHashing<par> :: devolve(Chave chave) {
    Item valor = -1;
    int pos = hash(chave);
    CelulasHash* aux;

    if((aux = table[pos]) != nullptr) {
        while(aux != nullptr) {
            if(aux->chave == chave) {
                valor = aux->valor;
                break;
            }
            aux = aux->prox;
        }
    }

    return valor;
}

/* O(1) - Sem lazy deletion*/
template <class par>
void TSTabelasDeHashing<par> :: remove(Chave chave) {
    int pos = hash(chave);
    CelulasHash* aux;

    if((aux=table[pos]) != nullptr) {
        while(aux != nullptr) {
            if(aux->chave == chave) {
                if(aux->ant == nullptr) {
                    table[pos] = aux->prox;
                    if(aux->prox != nullptr)
                        aux->prox->ant = aux->ant;
                    delete aux;
                    break;
                }
                aux->ant->prox = aux->prox;
                if(aux->prox != nullptr)
                    aux->prox->ant = aux->ant;
                delete aux;
                break;
            }
            aux = aux->prox;
        }
    }
}

/* O(log n) */
template <class par>
int TSTabelasDeHashing<par> :: rank(Chave chave) {
    return -1;
}

/* O(1) */
template <class par>
Chave TSTabelasDeHashing<par> :: seleciona(int k) {
    
    return "";
}

template <class par>
void TSTabelasDeHashing<par> :: checaDistribuicaoDaTabela() {
    int quantEl;
    CelulasHash* aux;
    int maxLista = 0;

    for (int i = 0; i < m; i++) {
        quantEl = 0;
        if((aux = table[i]) != nullptr) {
            while(aux != nullptr) {
                quantEl++;
                aux = aux->prox;
            }
        }
        if(quantEl > maxLista) maxLista = quantEl;
        cout << "POS: " << i << " quantElem: " << quantEl << endl;
    }
    cout << "A maior lista tem: " << maxLista << endl;
}

template <class par>
void TSTabelasDeHashing<par> :: exibeTS() {
    CelulasHash* aux;
    int n = 0;

    for (int i = 0; i < m; i++) {
        if(table[i] == nullptr) continue;
        aux = table[i];
        while(aux != nullptr) {
            cout << "Chave: " << aux->chave << ", Valor: " << aux->valor << endl;
            n++;
            aux = aux->prox;
        }
    }
    cout << "N elements: " << n << endl;
    checaDistribuicaoDaTabela();
}


#endif