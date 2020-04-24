#ifndef TS_TABELAS_DE_HASHING_H
#define TS_TABELAS_DE_HASHING_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include "estruturas.hpp"
using namespace std;

typedef string Chave;
typedef int Item;

template <class Chave, class Valor>
class hashTable { 
    public:
        hashTable(string nome_arquivo);
        ~hashTable();

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

template <class Chave, class Valor>
hashTable<Chave, Valor> :: hashTable(string nome_arquivo) {
    n = 0;
    m = 7000;
    table = new CelulasHash*[m];

    for (int i = 0; i < m; i++)
        table[i] = nullptr;

    ifstream texto;
    string palavra;

    texto.open(nome_arquivo, ios::in);

    while(texto >> palavra) {
        insere(palavra, 1);
        cout << palavra << endl;
    }

    texto.close();
}

template <class Chave, class Valor>
hashTable<Chave, Valor> :: ~hashTable() {
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

template <class Chave, class Valor>
int hashTable<Chave, Valor> :: hash(Chave chave) {
    int len = chave.length();
    int h = 0;

    for (int i = 0; i < len; i++) {
        h = (h*23+chave[i]) % m;
    }
    return h;
}
 
/* O(1) */
template <class Chave, class Valor>
void hashTable<Chave, Valor> :: insere(Chave chave, Item valor) {
    int pos = hash(chave);
    CelulasHash *novo, *aux;
    bool achou = false;

    //Verifica se o elemento já existe
    if( (aux = table[pos]) != nullptr) {
        while (aux != nullptr && !achou) {
            if(aux->chave == chave) {
                aux->valor++;
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
template <class Chave, class Valor>
Item hashTable<Chave, Valor> :: devolve(Chave chave) {
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
template <class Chave, class Valor>
void hashTable<Chave, Valor> :: remove(Chave chave) {
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

/* O(n) */
template <class Chave, class Valor>
int hashTable<Chave, Valor> :: rank(Chave chave) {
    CelulasHash* aux;
    int r = 0;

    for (int i = 0; i < m; i++) {
        aux = table[i];
        while(aux != nullptr) {
            if(aux->chave < chave) {
                r++;
            }
            aux = aux->prox;
        }    
    }
    return r;
}

/* O(n^2) */
template <class Chave, class Valor>
Chave hashTable<Chave, Valor> :: seleciona(int k) {
    CelulasHash* aux;
    for (int i = 0; i < m; i++) {
        aux = table[i];
        while(aux != nullptr) {
            if(rank(aux->chave) == k) {
                return aux->chave;
            }
            aux = aux->prox;
        }    
    }
    return "";
}

template <class Chave, class Valor>
void hashTable<Chave, Valor> :: checaDistribuicaoDaTabela() {
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

template <class Chave, class Valor>
void hashTable<Chave, Valor> :: exibeTS() {
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