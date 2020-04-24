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

/*
 * MAC0323 Algoritmos e Estruturas de Dados II
 * 
 * Contador de frequencia de palavras em um texto
 *
 *     http://www.ime.usp.br/~pf/mac0122-2003/aulas/symb-table.html
 * 
 * AVISO: Este programa quebra se o conjunto de caracters nao 
 *        for de 7-bits ASCII, ou seja, textos iso-latin 8859-1
 *        com acentos, cedilha, ... fazem o programa quebrar.
 *        UTF-8 nem pensar.
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string.h>
#include <time.h>
using namespace std;

#include "util.hpp"    /* emalloc(), ecalloc(), strCmp(), getLine(), stringCopy(), 
//                         ERRO(), AVISO(), getLine() */

/* tabela de símbolos */
//#include "tabeladesimbolos.hpp"  /* devolve(), insere(), remove(), ... */
#include "vetor_desordenado.h"
#include "vetor_ordenado.h"
#include "lista_ligada_desordenada.h"
#include "lista_ligada_ordenada.h"
#include "arvore_busca_binaria.h"
#include "treaps.h"
#include "arvores_rubro_negras.h"
#include "arvores_23.h"
#include "tabelas_de_hashing.h"

#define PROMPT      cout << ">>> ";
#define MIN_ST      "minST"
#define DELMIN_ST   "delminST"
#define GET_ST      "getST"
#define RANK_ST     "rankST"
#define DELETE_ST   "deleteST"
#define SELECT_ST   "selectST"

/*---------------------------------------------------------------*/
/*
 *  P R O T Ó T I P O S
 */
static void mostreUso (char *nomePrograma);

template <class ST>
void testeOperacoes(ST st);

/*---------------------------------------------------------------*/
/* 
 *  M A I N 
 */
int main(int argc, char *argv[]) {

    fstream arqTexto;

    if (argc < 3) mostreUso(argv[0]);

    /* abra arquivo com texto */
    arqTexto.open(argv[1]);

    if (arqTexto.fail()) {
        cout << "ERRO: arquivo" << argv[1] << "nao pode ser aberto.\n";
        exit(EXIT_FAILURE);
    }

    arqTexto.close();

    string nome_arquivo = argv[1];
    string tipo = argv[2];

    /* crie a ST*/
    cout << "criando ST...\n";

    /* usadas para medir tempo de processamento */
    clock_t start, end; 
    double elapsed = 0;

    if (tipo == "VD"){
        start = clock(); 
        TSVetorDesordenado<string, int> *st = new TSVetorDesordenado<string, int>;
        //vetorDes<String,Integer> *st = new vetorDes<String, Integer>(nome_arquivo);
        end = clock();

        /* calcule o tempo */
        elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;
        cout << "arquivo lido e ST construida em " << elapsed << " segundos\n";

        testeOperacoes(st);
		delete st;
    }
    else if (tipo == "VO"){
        start = clock(); 
        TSVetorOrdenado<string, int> *st = new TSVetorOrdenado<string, int>;
        //vetorOrd<String,Integer> *st = new vetorOrd<String,Integer>(nome_arquivo);
        end = clock();

        /* calcule o tempo */
        elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;
        cout << "arquivo lido e ST construida em " << elapsed << " segundos\n";

        testeOperacoes(st);
		delete st;
    }
    else if (tipo == "LD"){
        start = clock(); 
        TSListaLigadaDesordenada<string, int> *st = new TSListaLigadaDesordenada<string, int>;
        //listaDes<String,Integer> *st = new listaDes<String,Integer>(nome_arquivo);
        end = clock();

        /* calcule o tempo */
        elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;
        cout << "arquivo lido e ST construida em " << elapsed << " segundos\n";

        testeOperacoes(st);
		delete st;
    }
    else if (tipo == "LO"){
        start = clock(); 
        TSListaLigadaOrdenada<string, int> *st = new TSListaLigadaOrdenada<string, int>;
        //listaOrd<String,Integer> *st = new listaOrd<String,Integer>(nome_arquivo);
        end = clock();

        /* calcule o tempo */
        elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;
        cout << "arquivo lido e ST construida em " << elapsed << " segundos\n";

        testeOperacoes(st);
		delete st;
    }
    else if (tipo == "AB"){
        start = clock(); 
        TSArvoreBuscaBinaria<string, int> *st = new TSArvoreBuscaBinaria<string, int>;
        //arvoreBin<String,Integer> *st = new arvoreBin<String,Integer>(nome_arquivo);
        end = clock();

        /* calcule o tempo */
        elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;
        cout << "arquivo lido e ST construida em " << elapsed << " segundos\n";

        testeOperacoes(st);
		delete st;
    }
    else if (tipo == "TR"){
        start = clock(); 
        TSTreaps<string, int> *st = new TSTreaps<string, int>;
        //treap<String,Integer> *st = new treap<String,Integer>(nome_arquivo);
        end = clock();

        /* calcule o tempo */
        elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;
        cout << "arquivo lido e ST construida em " << elapsed << " segundos\n";

        testeOperacoes(st);
		delete st;
    }
    else if (tipo == "A23"){
        start = clock(); 
        TSArvore23<string, int> *st = new TSArvore23<string, int>;
        //arvore23<String,Integer> *st = new arvore23<String,Integer>(nome_arquivo);
        end = clock();

        /* calcule o tempo */
        elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;
        cout << "arquivo lido e ST construida em " << elapsed << " segundos\n";

        testeOperacoes(st);
		delete st;
    }
    else if (tipo == "RN"){
        start = clock(); 
        TSArvoresRubroNegras<string, int> *st = new TSArvoresRubroNegras<string, int>;
        //arvoreRN<String,Integer> *st = new arvoreRN<String,Integer>(nome_arquivo);
        end = clock();

        /* calcule o tempo */
        elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;
        cout << "arquivo lido e ST construida em " << elapsed << " segundos\n";

        testeOperacoes(st);
		delete st;
    }
    else if (tipo == "HS"){
        start = clock(); 
        TSTabelasDeHashing<string, int> *st = new TSTabelasDeHashing<string, int>;
        //hashTable<String,Integer> *st = new hashTable<String,Integer>(nome_arquivo);
        end = clock();

        /* calcule o tempo */
        elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;
        cout << "arquivo lido e ST construida em " << elapsed << " segundos\n";

        testeOperacoes(st);
		delete st;
    }
    else {
        cout << "A estrutura" << tipo << "não é válida";
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

/*-------------------------------------------------------*/
/* 
 *  TESTEOPERACOES(ST) 
 *
 *  RECEBE uma tabela de símbolos ST e testa várias operações
 *  interativamente.
 *
 *  A função supõe que as chaves são do tipo String e os
 *  valores são do tipo Integer (ver util.h).
 */
template <class ST>
void testeOperacoes(ST st) {
    //String linha    = NULL;
    /* mostre uso */
    cout << "Possiveis operacoes do teste interativo:\n";
    cout << "minST, delminST, getST <chave>; rankST <chave>, deleteST <chave>, selectST <int>\n";
    cout << "CRTL-D para encerrar.\n";
    PROMPT;

    ifstream texto;
    string operacao;

    while (texto >> operacao) {
        /* pegue operacao a ser testada */        
        if (operacao == "") {
             ERROR(operacao esperada);
        }
        /*---------------------------------*/
        else if (operacao == MIN_ST) {
            //String key = st->seleciona(0);
            string key = st->seleciona(0);
            if (key == "") {
                cout << "ST vazia\n";
            } 
            else {
                cout << key << "\n";
            }
        }
        /*---------------------------------*/
        else if (operacao == DELMIN_ST) {
            //String key = st->seleciona(0);
            string key = st->seleciona(0);
            if (key == "") {
                cout << "ST já está vazia\n";
            } 
            else {
                cout << "\"" << key; 
                st->remove(key);
                cout << "\" foi removida\n";
            }
        }
        /*---------------------------------*/
        else {
            /* operaÃ§Ã£o necessita de argumento key */
            string key;
            texto >> key; //getNextToken(NULL);
            if (key == "") {
                ERROR(operacao necessita uma palavra);
            } 
            else {
                /*---------------------------------*/
                if (operacao == GET_ST) {
                    int frequencia = 0;
                    frequencia = st->devolve(key); /* consulte a ST */
                    /* mostre o resultado da consulta */
                    if (frequencia == 0) {
                        cout << key << ": 0\n";
                    } 
                    else {
                        cout << key << ": " << frequencia << "\n";
                    }
                }
                /*---------------------------------*/
                else if (operacao == RANK_ST) {
                    int r = st->rank(key);
                    cout << r << "\n";
                }
                /*---------------------------------*/
                else if (operacao == DELETE_ST) {
                    st->remove(key);
                    cout << "\"" << key << "\" foi removida\n";
                }
                else if (operacao == SELECT_ST) {
                    int pos = atoi(key);
                    string chave = st->seleciona(pos);
                    cout << "Posição " << key << " = " << chave << "\n";
                }
                else {
                    ERROR(operacao nao reconhecida);
                }
            }
        }
        //if (operacao != "") free(operacao);
        //free(linha);
        PROMPT;
    }
    cout << "\n";
}


/*-----------------------------------------------------------*/
/* 
 *  I M P L E M E N T A Ç Ã O   D A S   F U N Ç Õ E S
 *                 A U X I L I A R E S 
 */
static void 
mostreUso (char *nomePrograma)
{
    cout << "Uso \n"
	     << "prompt> " << nomePrograma << " nome-arquivo tipo-tabela\n"
	     << "    nome-arquivo = nome do arquivo com o texto\n"
         << "    tipo-tabela  = sigla de estrutura que deve ser usada\n";
    exit(EXIT_FAILURE);
}   


/*
int main() {

    TSTabelasDeHashing <Chave, Valor> ts;

    ifstream texto;
    string palavra;

    texto.open("meuTexto.txt", ios::in);
    int teste = 0;

    while(texto >> palavra) {
        ts.insere(palavra, 1);
    }
    return 0;
    //cout << "done" << endl;
    //cout << ts.n << endl;
    
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
*/