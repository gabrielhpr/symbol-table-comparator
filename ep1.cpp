#include <iostream>
#include <fstream>
#include "vetor_desordenado.h"
#include "vetor_ordenado.h"
#include "lista_ligada_desordenada.h"
#include "lista_ligada_ordenada.h"
#include "arvore_busca_binaria.h"
#include "treaps.h"

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

    TSTreaps    <par> ts;

    ifstream texto;
    string palavra;

    texto.open("holy_bible.txt", ios::in);
    int teste = 0;

    while(texto >> palavra) {
        //cout << palavra << endl;
        //cout << palavra.size() << endl;

        //for (auto i = palavra.begin(); i != palavra.end(); i++)  {
          //  printf(" %c ", *i);
        //}
        //printf("\nteste %c \n", 76);
        
        ts.insere(palavra, 1);
        //teste++;
        //if(teste == 3) break;
    }
    string command = "";

    while(command != "stop") { 
        cin >> command;
        if(command == "exibe")
            ts.exibeTS();
        else if(command == "remove") {
            cout << "quem ?" << endl;
            string str;
            cin >> str;
            ts.remove(str);
        }
        //Item t = ts.devolve("Luís");
        //cout << t << endl;
    }

    


    return 0;
}