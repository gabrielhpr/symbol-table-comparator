#include <iostream>
#include <fstream>
#include "vetor_desordenado.h"
#include "vetor_ordenado.h"
#include "lista_ligada_desordenada.h"
#include "lista_ligada_ordenada.h"
#include "arvore_busca_binaria.h"
#include "treaps.h"
#include "arvores_rubro_negras.h"
#include "arvores_23.h"

using namespace std;

/*Métodos a serem implementados*/
/*
RANK    
int rank(Chave chave) - Devolve o número de elementos menores do que a chave (#elem < chave)
r = rank(Chave chave) - Dessa forma, se o retorno da função for n, a chave não está na tabela, se 
a estrutura for ordenada (ex vetor ordenado) r representa a posição da chave

SELECIONA
Chave seleciona(int k) - Devolve uma chave de rank k - facilita a busca em ordenados
*/

struct par {
    string chave;
    int valor;
};
typedef struct par par;


int main() {

    TSArvore23 <par> ts;

    ifstream texto;
    string palavra;

    texto.open("simple.txt", ios::in);
    int teste = 0;

    while(texto >> palavra) {
        ts.insere(palavra, 1);
    }
    
    string command = "";
    string str;

    while(command != "stop") { 
        cin >> command;
        if(command == "e")
            ts.exibeTS();
        else if(command == "r") {
            cout << "quem ?" << endl;
            cin >> str;
            ts.remove(str);
        }
        else if(command == "i") {
            cout << "manda a chave: " << endl;
            cin >> str;
            ts.insere(str, 1);
        }
        else if(command == "rank") {
            cout << "manda a chave: " << endl;
            cin >> str;
            int r = ts.rank(str);
            cout << r << endl;
        }
        //Item t = ts.devolve("Luís");
        //cout << t << endl;
    }
    
    


    return 0;
}