#include <iostream>
#include <fstream>
#include "vetor_desordenado.h"
#include "vetor_ordenado.h"

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

    TSVetorOrdenado<par> ts;

    ifstream texto;
    string palavra;

    texto.open("meuTexto.txt", ios::in);

    while(texto >> palavra) {
        //cout << palavra << endl;
        ts.insere(palavra, 1);
    }
    ts.exibeTS();
   



    return 0;
}