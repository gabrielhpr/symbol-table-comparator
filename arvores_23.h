#ifndef TS_ARVORE_23_H
#define TS_ARVORE_23_H

#include <iostream>
#include "estruturas.h"
using namespace std;


template <class par>
class TSArvore23 { 
    public:
        TSArvore23();
        ~TSArvore23();

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

template <class par>
TSArvore23<par> :: TSArvore23() {
    n = 0;
    raiz = nullptr;
}

template <class par> 
void TSArvore23<par> :: destroiArvore23(No23* q) {
    if(q != nullptr) {
        destroiArvore23(q->ap1);
        destroiArvore23(q->ap2);
        destroiArvore23(q->ap3);
        delete q;
    }
}

template <class par>
TSArvore23<par> :: ~TSArvore23() {
    //Executar em pós-ordem
    destroiArvore23(raiz);
}

template <class par>
void TSArvore23<par> :: constroiDoisNo(No23* novo, Chave chave, Item valor) {
    novo->chave1 = chave;
    novo->valor1 = valor;
    novo->ap1 = nullptr;
    novo->ap2 = nullptr;
    novo->ap3 = nullptr;
    novo->doisNo = true;
    novo->quantNosSubArvEsq = 0;
    novo->quantNosSubArvDir = 0;
}

template <class par>
No23* TSArvore23<par> :: put23(No23 *raiz, Chave chave, Item valor, bool &cresceu) {
    if (raiz == nullptr){
        No23 *raiz = new No23;
        raiz->chave1 = chave;
        raiz->valor1 = valor;
        raiz->doisNo = true;
        cresceu = true;
        return raiz;
    }
    if (raiz->ehFolha()){
        //A raiz é folha
        if (raiz->doisNo){
            //Insere na célula que é 2-Nó, transformando ela em uma 3-Nó
            if (raiz->chave1 > chave){
                raiz->chave2 = raiz->chave1;
                raiz->valor2 = raiz->valor1;
                raiz->chave1 = chave;
                raiz->valor1 = valor;
            }
            else{
                raiz->chave2 = chave;
                raiz->valor2 = valor;
            }
            raiz->doisNo = false;
            cresceu = false;
            return raiz;
        }
        else{
            //Raiz é 3-Nós, ou seja, vai "explodir"
            No23 *meio = new No23, *maior = new No23;
            Chave menorChave, maiorChave, meioChave;
            Item menorValor, maiorValor, meioValor;
            if (chave > raiz->chave2){
                maiorChave = chave;
                maiorValor = valor;
                meioChave = raiz->chave2;
                meioValor = raiz->valor2;
                menorChave = raiz->chave1;
                menorValor = raiz->valor1;
            }
            else if (chave < raiz->chave1){
                maiorChave = raiz->chave2;
                maiorValor = raiz->valor2;
                meioChave = raiz->chave1;
                meioValor = raiz->valor1;
                menorChave = chave;
                menorValor = valor;
            }
            else{
                maiorChave = raiz->chave2;
                maiorValor = raiz->valor2;
                meioChave = chave;
                meioValor = valor;
                menorChave = raiz->chave1;
                menorValor = raiz->valor1;
            }
            constroiDoisNo(meio, meioChave, meioValor);
            meio->ap1 = raiz;
            meio->ap2 = maior;
            constroiDoisNo(maior, maiorChave, maiorValor);
            constroiDoisNo(raiz, menorChave, menorValor);
            cresceu = true;
            return meio;
        }
    }
    else{
        //A raiz não é folha
        if (raiz->chave1 > chave){
            //Chama recursão para a esquerda do Nó
            No23 *aux = put23(raiz->ap1, chave, valor, cresceu);
            if (!cresceu){
                return raiz;
            }
            else{
                //Cresceu
                if (raiz->doisNo){
                    //Cresceu e vai transformar a raiz que é 2-Nó em uma 3-Nó
                    raiz->chave2 = raiz->chave1;
                    raiz->valor2 = raiz->valor1;
                    raiz->chave1 = aux->chave1;
                    raiz->valor1 = aux->valor1;
                    raiz->ap3 = raiz->ap2;
                    raiz->ap1 = aux->ap1;
                    raiz->ap2 = aux->ap2;
                    delete aux;
                    raiz->doisNo = false;
                    cresceu = false;
                    return raiz;
                }
                else{
                    //Cresceu e a raiz é 3-Nó, ou seja, vai "explodir"
                    No23 *maior = new No23;
                    constroiDoisNo(maior, raiz->chave2, raiz->valor2);
                    maior->ap1 = raiz->ap2;
                    maior->ap2 = raiz->ap3;
                    raiz->ap1 = aux;
                    raiz->ap2 = maior;
                    raiz->doisNo = true;
                    cresceu = true;
                    return raiz;
                }

            }
        }
        else if (!raiz->doisNo && raiz->chave2 < chave){
            //Chama a recursão para a direita do Nó
            No23 *aux = put23(raiz->ap3, chave, valor, cresceu);
            if (!cresceu)
                return raiz;
            else{
                //cresceu e a raiz é 3-Nó, ou seja, vai "explodir"
                No23 *menor = new No23;
                menor->chave1 = raiz->chave1;
                menor->valor1 = raiz->valor1;
                menor->ap1 = raiz->ap1;
                menor->ap2 = raiz->ap2;
                raiz->ap1 = menor;
                raiz->ap2 = aux;
                raiz->doisNo = true;
                cresceu = true;
                return raiz;
            }
        }
        else{
            //Chama a recursão para o meio do Nó
            No23 *aux = put23(raiz->ap2, chave, valor, cresceu);
            if (!cresceu)
                return raiz;
            else{
                //cresceu
                if (raiz->doisNo){
                    //cresceu e vai transformar a raiz que é 2-Nó em uma 3-Nó
                    raiz->chave2 = aux->chave1;
                    raiz->valor2 = aux->valor1;
                    raiz->ap2 = aux->ap1;
                    raiz->ap3 = aux->ap2;
                    delete aux;
                    raiz->doisNo = false;
                    cresceu = false;
                    return raiz;
                }
                else{
                    //cresceu e a raiz é 3-Nó, ou seja, vai "explodir"
                    No23 *maior = new No23;
                    constroiDoisNo(maior, raiz->chave2, raiz->valor2);
                    maior->ap2 = raiz->ap3;
                    maior->ap1 = aux->ap2;
                    raiz->ap2 = aux->ap1;
                    raiz->doisNo = false;
                    aux->ap2 = maior;
                    aux->ap1 = raiz;
                    cresceu = true;
                    return aux;
                }
            }
        }
    }
}

/* O(log n) */
template <class par>
void TSArvore23<par> :: insere(Chave chave, Item valor) {
    bool cresceu = false;
    raiz = put23(raiz, chave, valor, cresceu);
}

/*Retorna o valor da chave correspondente ou -1 se a chave não existe*/
/* O(log n) */
template <class par>
Item TSArvore23<par> :: devolve(Chave chave) {
    
}

template <class par>
No23* TSArvore23<par> :: removeUtilRecur(No23* q, Chave chave, bool& removeu) {

}

/* O(log n) */
template <class par>
void TSArvore23<par> :: remove(Chave chave) {
    bool removeu = false;
    raiz = removeUtilRecur(raiz, chave, removeu);
}

/* O(log n) */
template <class par>
int TSArvore23<par> :: rank(Chave chave) {
    
}

/* O(n) */
template <class par>
Chave TSArvore23<par> :: seleciona(int k) {
    
}

template <class par>
void TSArvore23<par> :: exibeTSUtilRecur(No23* q) {
    if(q != nullptr) {
        exibeTSUtilRecur(q->ap1);
        cout << "Chave: " << q->chave1 << ", Valor: " << q->valor1
        << " ,fap1: " << q->ap1 << ", fap2: " << q->ap2 << ", fap3: " << q->ap3 << ", 2no: "
        << q->doisNo << endl;
        if(q->doisNo == false)
            cout << "Chave: " << q->chave2 << ", Valor: " << q->valor2
            << " ,fap1: " << q->ap1 << ", fap2: " << q->ap2 << ", fap3: " << q->ap3 << ", 2no: "
            << q->doisNo << endl;
        exibeTSUtilRecur(q->ap2);
        exibeTSUtilRecur(q->ap3);
    }
}

template <class par>
void TSArvore23<par> :: exibeTS() {
    exibeTSUtilRecur(raiz);
    cout << n << endl;

}

#endif