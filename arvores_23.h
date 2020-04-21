#ifndef TS_ARVORE_23_H
#define TS_ARVORE_23_H

//Debug
#define debug(args...) fprintf(stderr, args)

#include <iostream>
#include <fstream>
#include "estruturas.h"
using namespace std;


template <class Chave, class Valor>
class arvore23 { 
    public:
        arvore23(string nome_arquivo);
        ~arvore23();

        /*Árvore de Busca Binária*/
        No23* raiz;
        void constroiDoisNo(No23* novo, Chave chave, Item valor);

        /*Número de elementos*/
        int n;//public

        /*Métodos da TS*/
        No23* put23(No23 *raiz, Chave chave, Item valor, bool &cresceu);
        void insere(Chave chave, Item valor);
        Item devolve(Chave chave);
        No23* removeUtilRecur(No23* q, Chave chave, bool& removeu);
        void remove(Chave chave);
        int rank(Chave chave);
        Chave seleciona(int k); 

        //Método utilizado pelo destructor
        void destroiArvore23(No23* q);

        void exibeTSUtilRecur(No23* q);
        void exibeTS(); 
};

template <class Chave, class Valor>
arvore23<Chave, Valor> :: arvore23(string nome_arquivo) {
    n = 0;
    raiz = nullptr;

    ifstream texto;
    string palavra;

    texto.open(nome_arquivo, ios::in);

    while(texto >> palavra) 
        insere(palavra, 1);

    texto.close();
}

template <class Chave, class Valor> 
void arvore23<Chave, Valor> :: destroiArvore23(No23* q) {
    if(q != nullptr) {
        destroiArvore23(q->ap1);
        destroiArvore23(q->ap2);
        if(!q->doisNo)
            destroiArvore23(q->ap3);
        delete q;
    }
}

template <class Chave, class Valor>
arvore23<Chave, Valor> :: ~arvore23() {
    //Executar em pós-ordem
    destroiArvore23(raiz);
}

template <class Chave, class Valor>
void arvore23<Chave, Valor> :: constroiDoisNo(No23* novo, Chave chave, Item valor) {
    novo->chave1 = chave;
    novo->valor1 = valor;
    novo->ap1 = nullptr;
    novo->ap2 = nullptr;
    novo->ap3 = nullptr;
    novo->doisNo = true;
    novo->quantNosSubArvEsq = 0;
    novo->quantNosSubArvDir = 0;
}

template <class Chave, class Valor>
No23* arvore23<Chave, Valor> :: put23(No23 *raizSub, Chave chave, Item valor, bool &cresceu) {
    //debug("entrou no put23 \n");
    //Árvore vazia
    if (raizSub == nullptr){
        //debug("raiz nula \n");
        No23 *raizSub = new No23;
        constroiDoisNo(raizSub, chave, valor);
        cresceu = true;
        return raizSub;
    }
    if (raizSub->ehFolha()) {
        //debug("é folha \n");
        //A raiz é folha
        if (raizSub->doisNo) {
            //debug("folha dois no \n");
            //Insere na célula que é 2-Nó, transformando ela em uma 3-Nó
            if (raizSub->chave1 > chave){
                raizSub->chave2 = raizSub->chave1;
                raizSub->valor2 = raizSub->valor1;
                raizSub->chave1 = chave;
                raizSub->valor1 = valor;
            }
            else {
                raizSub->chave2 = chave;
                raizSub->valor2 = valor;
            }
            raizSub->doisNo = false;
            cresceu = false;
            return raizSub;
        }
        else {
            //debug("folha três nó \n");
            //raizSub é 3-Nós, ou seja, vai "explodir"
            No23 *meio = new No23;
            No23 *maior = new No23;

            Chave menorChave, maiorChave, meioChave;
            Item menorValor, maiorValor, meioValor;
            if (chave > raizSub->chave2){
                maiorChave = chave;
                maiorValor = valor;
                meioChave = raizSub->chave2;
                meioValor = raizSub->valor2;
                menorChave = raizSub->chave1;
                menorValor = raizSub->valor1;
            }
            else if (chave < raizSub->chave1){
                maiorChave = raizSub->chave2;
                maiorValor = raizSub->valor2;
                meioChave = raizSub->chave1;
                meioValor = raizSub->valor1;
                menorChave = chave;
                menorValor = valor;
            }
            else{
                maiorChave = raizSub->chave2;
                maiorValor = raizSub->valor2;
                meioChave = chave;
                meioValor = valor;
                menorChave = raizSub->chave1;
                menorValor = raizSub->valor1;
            }
            constroiDoisNo(meio, meioChave, meioValor);
            meio->ap1 = raizSub;
            meio->ap2 = maior;
            constroiDoisNo(maior, maiorChave, maiorValor);
            constroiDoisNo(raizSub, menorChave, menorValor);
            cresceu = true;
            return meio;
        }
    }
    else{
        //debug("não folha \n");
        //A raizSub não é folha
        if (chave < raizSub->chave1) {
            //debug("chegou no primeiro caso \n");

            //Chama recursão para a esquerda do Nó
            No23 *aux = put23(raizSub->ap1, chave, valor, cresceu);
            if (!cresceu)
                return raizSub;
            else {
                //Cresceu
                if (raizSub->doisNo) {
                    //Cresceu e vai transformar a raizSub que é 2-Nó em uma 3-Nó
                    raizSub->chave2 = raizSub->chave1;
                    raizSub->valor2 = raizSub->valor1;
                    raizSub->chave1 = aux->chave1;
                    raizSub->valor1 = aux->valor1;
                    raizSub->ap3 = raizSub->ap2;
                    raizSub->ap1 = aux->ap1;
                    raizSub->ap2 = aux->ap2;
                    delete aux;
                    raizSub->doisNo = false;
                    cresceu = false;
                    return raizSub;
                }
                else {
                    //Cresceu e a raizSub é 3-Nó, ou seja, vai "explodir"
                    No23 *maior = new No23;
                    constroiDoisNo(maior, raizSub->chave2, raizSub->valor2);
                    maior->ap1 = raizSub->ap2;
                    maior->ap2 = raizSub->ap3;
                    raizSub->ap1 = aux;
                    raizSub->ap2 = maior;
                    raizSub->ap3 = nullptr;
                    raizSub->doisNo = true;
                    cresceu = true;
                    return raizSub;
                }

            }
        }
        else if (!raizSub->doisNo && chave > raizSub->chave2) {
            //debug("chegou no segundo caso \n");

            //Chama a recursão para a direita do Nó
            No23 *aux = put23(raizSub->ap3, chave, valor, cresceu);
            if (!cresceu) 
                return raizSub; 
            else {
                //cresceu e a raizSub é 3-Nó, ou seja, vai "explodir"
                No23 *menor = new No23;
                constroiDoisNo(menor, raizSub->chave1, raizSub->valor1);
                menor->ap1 = raizSub->ap1;
                menor->ap2 = raizSub->ap2;
                raizSub->ap1 = menor;
                raizSub->ap2 = aux;
                raizSub->ap3 = nullptr;
                raizSub->doisNo = true;
                cresceu = true;
                return raizSub;
            }
        }
        else {
            //debug("chegou no terceiro caso \n");
            //Chama a recursão para o meio do Nó
            No23 *aux = put23(raizSub->ap2, chave, valor, cresceu);
            if (!cresceu) 
                return raizSub;
            else {
                //cresceu
                if (raizSub->doisNo) {
                    //cresceu e vai transformar a raizSub que é 2-Nó em uma 3-Nó
                    raizSub->chave2 = aux->chave1;
                    raizSub->valor2 = aux->valor1;
                    raizSub->ap2 = aux->ap1;
                    raizSub->ap3 = aux->ap2;
                    delete aux;
                    raizSub->doisNo = false;
                    cresceu = false;
                    return raizSub;
                }
                else {
                    //cresceu e a raizSub é 3-Nó, ou seja, vai "explodir"
                    No23 *maior = new No23;
                    constroiDoisNo(maior, raizSub->chave2, raizSub->valor2);
                    maior->ap2 = raizSub->ap3;
                    maior->ap1 = aux->ap2;
                    raizSub->ap2 = aux->ap1;
                    raizSub->doisNo = true;
                    aux->ap2 = maior;
                    aux->ap1 = raizSub;
                    cresceu = true;
                    return aux;
                }
            }
        }
    }
}

/* O(log n) */
template <class Chave, class Valor>
void arvore23<Chave, Valor> :: insere(Chave chave, Item valor) {
    bool cresceu = false;
    bool achou = false;

    //Procura se a chave já existe O(log n)
    No23* noAux = raiz;
    while(noAux != nullptr && !achou) {   
        if(chave == noAux->chave1) {
            noAux->valor1 += 1;
            achou = true;
        }
        else if(chave < noAux->chave1) 
            noAux = noAux->ap1;
        else if(chave > noAux->chave1) {
            if(noAux->doisNo) 
                noAux = noAux->ap2;
            else {
                if(chave == noAux->chave2) {
                    noAux->valor2 += 1;
                    achou = true;
                }
                else if(chave < noAux->chave2) 
                    noAux = noAux->ap2;
                else 
                    noAux = noAux->ap3;
            }
        }
    }
    //Se a chave não existe, adiciona ela na árvore
    if(!achou) {
        raiz = put23(raiz, chave, valor, cresceu);
        n++;
    }
}

/*Retorna o valor da chave correspondente ou -1 se a chave não existe*/
/* O(log n) */
template <class Chave, class Valor>
Item arvore23<Chave, Valor> :: devolve(Chave chave) {
    bool achou = false;
    Item valor = -1;

    //Procura se a chave já existe O(log n)
    No23* noAux = raiz;
    while(noAux != nullptr && !achou) {   
        if(chave == noAux->chave1) {
            valor = noAux->valor1;
            achou = true;
        }
        else if(chave < noAux->chave1) 
            noAux = noAux->ap1;
        else if(chave > noAux->chave1) {
            if(noAux->doisNo) 
                noAux = noAux->ap2;
            else {
                if(chave == noAux->chave2) {
                    valor = noAux->valor2;
                    achou = true;
                }
                else if(chave < noAux->chave2) 
                    noAux = noAux->ap2;
                else 
                    noAux = noAux->ap3;
            }
        }
    }
    return valor;
} 

template <class Chave, class Valor>
No23* arvore23<Chave, Valor> :: removeUtilRecur(No23* q, Chave chave, bool& removeu) {

}

/* O(log n) */
template <class Chave, class Valor>
void arvore23<Chave, Valor> :: remove(Chave chave) {
    bool removeu = false;
    raiz = removeUtilRecur(raiz, chave, removeu);
}

/* O(log n) */
template <class Chave, class Valor>
int arvore23<Chave, Valor> :: rank(Chave chave) {
    
}

/* O(n) */
template <class Chave, class Valor>
Chave arvore23<Chave, Valor> :: seleciona(int k) {
    
}

template <class Chave, class Valor>
void arvore23<Chave, Valor> :: exibeTSUtilRecur(No23* q) {
    if(q != nullptr) {
        exibeTSUtilRecur(q->ap1);
        cout << "Chave-P: " << q->chave1 << ", Valor: " << q->valor1
        << " ,fap1: " << q->ap1 << ", fap2: " << q->ap2 << ", fap3: " << q->ap3 << ", 2no: "
        << q->doisNo << endl;
        exibeTSUtilRecur(q->ap2);
        
        if(q->doisNo == false) {
            cout << "Chave-S: " << q->chave2 << ", Valor: " << q->valor2
            << " ,fap1: " << q->ap1 << ", fap2: " << q->ap2 << ", fap3: " << q->ap3 << ", 2no: "
            << q->doisNo << endl;
            exibeTSUtilRecur(q->ap3);
        }
    }
}

template <class Chave, class Valor>
void arvore23<Chave, Valor> :: exibeTS() {
    exibeTSUtilRecur(raiz);
    cout << n << endl;
}

#endif