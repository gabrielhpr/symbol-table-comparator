#ifndef TS_ARVORE_23_H
#define TS_ARVORE_23_H

//Debug
#define debug(args...) fprintf(stderr, args)

#include <iostream>
#include <fstream>
#include "estruturas.hpp"
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
        void remove(Chave chave);
        void rankRecur(No23* q, Chave chave, int& r);
        int rank(Chave chave);
        void selecionaRecur(No23* q, int k, Chave& chaveK);
        Chave seleciona(int k); 

        //Método utilizado pelo destructor
        void destroiArvore23(No23* q);

        void exibeElemento(No23* q, char c);
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
    novo->pai = nullptr;
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
            meio->pai = raizSub->pai;
            meio->ap1 = raizSub;
            meio->ap2 = maior;
            constroiDoisNo(maior, maiorChave, maiorValor);
            constroiDoisNo(raizSub, menorChave, menorValor);
            
            maior->pai = meio;
            raizSub->pai = meio;

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
            aux->pai = raizSub;
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
                    if (raizSub->ap1 != nullptr) raizSub->ap1->pai = raizSub;
                    raizSub->ap2 = aux->ap2;
                    if (raizSub->ap2 != nullptr) raizSub->ap2->pai = raizSub;
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
                    if (maior->ap1 != nullptr) maior->ap1->pai = maior;
                    maior->ap2 = raizSub->ap3;
                    if (maior->ap2 != nullptr) maior->ap2->pai = maior;
                    raizSub->ap1 = aux;
                    raizSub->ap2 = maior;
                    raizSub->ap3 = nullptr;
                    raizSub->doisNo = true;
                    cresceu = true;
                    maior->pai = raizSub;
                    aux->pai = raizSub;
                    return raizSub;
                }

            }
        }
        else if (!raizSub->doisNo && chave > raizSub->chave2) {
            //debug("chegou no segundo caso \n");

            //Chama a recursão para a direita do Nó
            No23 *aux = put23(raizSub->ap3, chave, valor, cresceu);
            aux->pai = raizSub;
            if (!cresceu) 
                return raizSub; 
            else {
                //cresceu e a raizSub é 3-Nó, ou seja, vai "explodir"
                No23 *menor = new No23;
                constroiDoisNo(menor, raizSub->chave1, raizSub->valor1);
                menor->pai = raizSub;
                aux->pai = raizSub;
                menor->ap1 = raizSub->ap1;
                if (menor->ap1 != nullptr) menor->ap1->pai = menor;
                menor->ap2 = raizSub->ap2;
                if (menor->ap2 != nullptr) menor->ap2->pai = menor;
                raizSub->ap1 = menor;
                raizSub->ap2 = aux;
                raizSub->ap3 = nullptr;
                raizSub->chave1 = raizSub->chave2;
                raizSub->valor1 = raizSub->valor2;  
                raizSub->doisNo = true;
                cresceu = true;
                return raizSub;
            }
        }
        else {
            //debug("chegou no terceiro caso \n");
            //Chama a recursão para o meio do Nó
            No23 *aux = put23(raizSub->ap2, chave, valor, cresceu);
            aux->pai = raizSub;
            if (!cresceu) 
                return raizSub;
            else {
                //cresceu
                if (raizSub->doisNo) {
                    //cresceu e vai transformar a raizSub que é 2-Nó em uma 3-Nó
                    raizSub->chave2 = aux->chave1;
                    raizSub->valor2 = aux->valor1;
                    raizSub->ap2 = aux->ap1;
                    if (raizSub->ap2 != nullptr) raizSub->ap2->pai = raizSub;
                    raizSub->ap3 = aux->ap2;
                    if (raizSub->ap3 != nullptr) raizSub->ap3->pai = raizSub;
                    aux->pai = raizSub->pai;
                    delete aux;
                    raizSub->doisNo = false;
                    cresceu = false;
                    return raizSub;
                }
                else {
                    //cresceu e a raizSub é 3-Nó, ou seja, vai "explodir"
                    No23 *maior = new No23;
                    constroiDoisNo(maior, raizSub->chave2, raizSub->valor2);
                    aux->pai = raizSub->pai;
                    maior->pai = aux;
                    raizSub->pai = aux;
                    
                    maior->ap2 = raizSub->ap3;
                    if (maior->ap2 != nullptr) maior->ap2->pai = maior;
                    maior->ap1 = aux->ap2;
                    if (maior->ap1 != nullptr) maior->ap1->pai = maior;
                    raizSub->ap2 = aux->ap1;
                    if (raizSub->ap2 != nullptr) raizSub->ap2->pai = raizSub;
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

/* O(log n) */
template <class Chave, class Valor>
void arvore23<Chave, Valor> :: remove(Chave chave) {
    debug("entrou na remoção \n");

    bool achou = false;
    No23 *aux = raiz;
    //Acha o nó a ser removido
    while (aux != nullptr && !achou) {
        if ( (aux->chave1 == chave) || (!aux->doisNo && aux->chave2 == chave) ) {
            achou = true;
        }
        else if (chave < aux->chave1) {
            aux = aux->ap1;
        }
        else if (!aux->doisNo && chave > aux->chave2) {
            aux = aux->ap3;
        }
        else {
            aux = aux->ap2;
        }
    }   

    //Se não achou, retorna
    if (!achou) {
        debug("Não achou \n");
        return;
    }
    
    //Não é folha, acha uma folha para substituir
    if ( (aux->ap1 != nullptr || aux->ap2 != nullptr) || (!aux->doisNo && aux->ap3 != nullptr) ) {
        debug("Não é folha \n");
        No23* aux2;
        if (chave == aux->chave1) {
            aux2 = aux->ap2;
            while (aux2->ap1 != nullptr) {
                aux2 = aux2->ap1;
            }
            aux->chave1 = aux2->chave1;
            aux->valor1 = aux2->valor1; 
            aux = aux2;
        }
        else if (!aux->doisNo && chave == aux->chave2) {
            aux2 = aux->ap3;
            while (aux2->ap1 != nullptr) {
                aux2 = aux2->ap1;
            }
            aux->chave2 = aux2->chave1;
            aux->valor2 = aux2->valor1;
            aux = aux2;
        }
    }
    //Caso folha 3 nó
    if (!aux->doisNo) {
        if(aux->chave2 == chave) {
            debug("caso folha 3 nó -> caso 2 \n");
            aux->doisNo = true;
        }
        else {
            debug("caso folha 3 nó -> caso 1 \n");
            aux->chave1 = aux->chave2;
            aux->valor1 = aux->valor2;
            aux->doisNo = true;
        }
        return;
    }
    //Caso folha 2 nó
    else {
        debug("caso folha 2 nó \n");
        No23* ponteiroValido = nullptr;

        while (aux != nullptr) {
            debug("entrou no loop \n");
            //Caso aux é a raiz
            if (aux == raiz) {
                debug("aux é a raiz \n");
                if(aux->chave1 == "") {
                    if(aux->ap1 != nullptr) {
                        raiz = aux->ap1;
                        aux->ap1 = nullptr;
                        //destroi raiz;
                    }
                    else {
                        raiz = aux->ap2;
                        aux->ap2 = nullptr;
                        //destroi raiz
                    }
                    delete aux;
                }
                else {
                    delete aux;
                    raiz = nullptr;
                }
                return;
            }
            No23* pai = aux->pai;
            No23* irmao;
            exibeElemento(aux, 'P');
            exibeElemento(pai, 'S');
            //Caso irmão rico
            //Caso cara da esquerda com irmão rico
            if (aux == pai->ap1 && pai->ap2->doisNo == false) {
                debug("cara da esquerda com irmão rico\n");
                irmao = pai->ap2;

                aux->chave1 = pai->chave1;
                pai->chave1 = irmao->chave1;
                irmao->chave1 = irmao->chave2;
                aux->ap2 = irmao->ap1;
                if (aux->ap2 != nullptr) aux->ap2->pai = aux;
                if (aux->ap2 == nullptr && ponteiroValido != nullptr) {
                    aux->ap2 = ponteiroValido;
                    ponteiroValido = nullptr;
                }
                else if (aux->ap1 == nullptr && ponteiroValido != nullptr) {
                    aux->ap1 = ponteiroValido;
                    ponteiroValido = nullptr;
                }
                irmao->ap1 = irmao->ap2;
                irmao->ap2 = irmao->ap3;
                irmao->ap3 = nullptr;
                irmao->doisNo = true;
                return;
            }
            //Caso cara da direita com irmão rico no meio
            else if (pai->doisNo == false && aux == pai->ap3 && pai->ap2->doisNo == false) {
                debug("cara da direita com irmão rico no meio\n");
                irmao = pai->ap2;

                aux->chave1 = pai->chave2;
                pai->chave2 = irmao->chave2;
                aux->ap1 = irmao->ap3;
                if (aux->ap1 != nullptr) aux->ap1->pai = aux;
                if (aux->ap2 == nullptr && ponteiroValido != nullptr) {
                    aux->ap2 = ponteiroValido;
                    ponteiroValido = nullptr;
                }
                else if (aux->ap1 == nullptr && ponteiroValido != nullptr) {
                    aux->ap1 = ponteiroValido;
                    ponteiroValido = nullptr;
                }
                irmao->ap3 = nullptr;
                irmao->doisNo = true;
                return;
            }
            //Caso cara do meio com irmão rico na esquerda ou direita
            else if (aux == pai->ap2 && (pai->ap1->doisNo == false || (pai->doisNo == false && pai->ap3->doisNo == false))) {
                debug("cara do meio com irmão rico\n");
                if (pai->ap1->doisNo == false) {
                    debug("cdmcir 1 \n");
                    irmao = pai->ap1;

                    aux->chave1 = pai->chave1;
                    aux->valor1 = pai->valor1;
                    
                    pai->chave1 = irmao->chave2;
                    pai->valor1 = irmao->valor2;

                    aux->ap1 = irmao->ap3;
                    if (aux->ap1 != nullptr) aux->ap1->pai = aux;
                    if (aux->ap2 == nullptr && ponteiroValido != nullptr) {
                        aux->ap2 = ponteiroValido;
                        ponteiroValido = nullptr;
                    }
                    else if (aux->ap1 == nullptr && ponteiroValido != nullptr) {
                        aux->ap1 = ponteiroValido;
                        ponteiroValido = nullptr;
                    }
                    irmao->ap3 = nullptr;
                    irmao->doisNo = true;
                }
                else if(pai->ap3->doisNo == false) {
                    debug("cdmcir 2 \n");
                    irmao = pai->ap3;

                    aux->chave1 = pai->chave2;
                    aux->valor1 = pai->valor2;
                    
                    pai->chave2 = irmao->chave1;
                    pai->valor2 = irmao->valor1;

                    irmao->chave1 = irmao->chave2;
                    irmao->valor1 = irmao->valor2;

                    aux->ap2 = irmao->ap1;
                    if (aux->ap2 != nullptr) aux->ap2->pai = aux;
                    if (aux->ap1 == nullptr && ponteiroValido != nullptr) {
                        aux->ap1 = ponteiroValido;
                        ponteiroValido = nullptr;
                    }
                    else if (aux->ap2 == nullptr && ponteiroValido != nullptr) {
                        aux->ap2 = ponteiroValido;
                        ponteiroValido = nullptr;
                    }
                    irmao->ap1 = irmao->ap2;
                    irmao->ap2 = irmao->ap3;
                    irmao->ap3 = nullptr;
                    irmao->doisNo = true;
                }
                return;
            }
            //Casos irmão pobre
            else {
                debug("irmão pobre \n");
                //Junta o pai com o irmão
                //Pai 3 nó
                if (pai->doisNo == false) {
                    debug("pai 3 nó \n");
                    //aux é o cara da esquerda
                    if(aux == pai->ap1) {
                        debug("cara da esquerda \n");
                        irmao = pai->ap2;
                        irmao->doisNo = false;

                        irmao->chave2 = irmao->chave1;
                        irmao->valor2 = irmao->valor1;
                        
                        irmao->chave1 = pai->chave1;
                        irmao->valor1 = pai->valor1;
                        
                        irmao->ap3 = irmao->ap2;
                        irmao->ap2 = irmao->ap1;
                        irmao->ap1 = aux->ap2;
                        if (irmao->ap1 != nullptr) irmao->ap1->pai = irmao;
                        if (irmao->ap1 == nullptr && ponteiroValido != nullptr) {
                            aux->ap1 = ponteiroValido;
                            ponteiroValido = nullptr;
                        }
                        else if (irmao->ap2 == nullptr && ponteiroValido != nullptr) {
                            aux->ap2 = ponteiroValido;
                            ponteiroValido = nullptr;
                        }
                        else if (!irmao->doisNo && irmao->ap3 == nullptr && ponteiroValido != nullptr) {
                            aux->ap3 = ponteiroValido;
                            ponteiroValido = nullptr;
                        }
                        pai->chave1 = pai->chave2;
                        pai->valor1 = pai->valor2;

                        delete aux;
                        pai->ap1 = pai->ap2;
                        pai->ap2 = pai->ap3;
                        pai->ap3 = nullptr;
                        pai->doisNo = true;
                        return;
                    }
                    else if (aux == pai->ap2) {
                        debug("cara do meio \n");
                        irmao = pai->ap3;

                        irmao->chave2 = irmao->chave1;
                        irmao->valor2 = irmao->valor1;
                        
                        irmao->chave1 = pai->chave2;
                        irmao->valor1 = pai->valor2;
                        
                        irmao->ap3 = irmao->ap2;
                        irmao->ap2 = irmao->ap1;
                        irmao->ap1 = aux->ap2;
                        if (irmao->ap1 != nullptr) irmao->ap1->pai = irmao;
                        if (aux->ap1 == nullptr && ponteiroValido != nullptr) {
                            aux->ap1 = ponteiroValido;
                            ponteiroValido = nullptr;
                        }
                        else if (irmao->ap2 == nullptr && ponteiroValido != nullptr) {
                            aux->ap2 = ponteiroValido;
                            ponteiroValido = nullptr;
                        }
                        else if (!irmao->doisNo && irmao->ap3 == nullptr && ponteiroValido != nullptr) {
                            aux->ap3 = ponteiroValido;
                            ponteiroValido = nullptr;
                        }
                        
                        irmao->doisNo = false;
                        delete aux;
                        pai->ap2 = pai->ap3;
                        pai->ap3 = nullptr;
                        pai->doisNo = true;
                        return;
                    }
                    else if (aux == pai->ap3) {
                        debug("cara da direita \n");
                        irmao = pai->ap2;

                        irmao->chave2 = pai->chave2;
                        irmao->valor2 = pai->valor2;
                        
                        irmao->ap3 = aux->ap1;
                        if (irmao->ap3 != nullptr) irmao->ap3->pai = irmao;
                        if (!irmao->doisNo && aux->ap3 == nullptr && ponteiroValido != nullptr) {
                            aux->ap3 = ponteiroValido;
                            ponteiroValido = nullptr;
                        }
                        else if (irmao->ap2 == nullptr && ponteiroValido != nullptr) {
                            aux->ap2 = ponteiroValido;
                            ponteiroValido = nullptr;
                        }
                        else if (irmao->ap1 == nullptr && ponteiroValido != nullptr) {
                            aux->ap1 = ponteiroValido;
                            ponteiroValido = nullptr;
                        }
                        
                        irmao->doisNo = false;
                        delete aux;
                        pai->ap3 = nullptr;
                        pai->doisNo = true;
                        return;
                    }
                }
                //Pai 2 nó
                else if (pai->doisNo == true) {  
                    pai = aux->pai;
                    
                    debug("pai 2 nó \n");
                    
                    if (aux == pai->ap1) {
                        debug("cara da esquerda \n");
                        irmao = pai->ap2;

                        irmao->chave2 = irmao->chave1;
                        irmao->valor2 = irmao->valor1;

                        irmao->chave1 = pai->chave1;
                        irmao->valor1 = pai->valor1;

                        irmao->ap3 = irmao->ap2;
                        irmao->ap2 = irmao->ap1;
                        irmao->ap1 = aux->ap2;
                        if (irmao->ap1 != nullptr) irmao->ap1->pai = irmao;
                        irmao->doisNo = false;
                        delete aux;
                        pai->ap1 = nullptr;
                        aux = pai;
                        aux->chave1 = "";
                        
                        if (ponteiroValido == nullptr) { 
                            debug("ponteiroValido nullptr \n");
                            ponteiroValido = irmao;
                        }
                        else {
                            debug("ponteiroValido n nullptr \n");
                            ponteiroValido->pai = irmao;
                            if(irmao->ap1 == nullptr) {
                                irmao->ap1 = ponteiroValido;
                                ponteiroValido = irmao;
                            }
                            
                            else if (irmao->ap2 == nullptr) {
                                irmao->ap2 = ponteiroValido;
                                ponteiroValido = irmao;
                            }
                            else if (!irmao->doisNo && irmao->ap3 == nullptr) {
                                irmao->ap3 = ponteiroValido;
                                ponteiroValido = irmao;
                            }
                            else {
                                ponteiroValido = irmao;
                            }
                            
                        }
                        
                    }
                    else if (aux == pai->ap2) {
                        debug("cara do meio \n");
                        irmao = pai->ap1;
                        
                        irmao->chave2 = pai->chave1;
                        irmao->valor2 = pai->valor1;

                        irmao->ap3 = aux->ap1;
                        if (irmao->ap3 != nullptr) irmao->ap3->pai = irmao;
                        irmao->doisNo = false;
                        delete aux;
                        pai->ap2 = nullptr;
                        //ou destoi aux;
                        aux = pai;
                        aux->chave1 = "";
                        
                        if (ponteiroValido == nullptr) { 
                            debug("ponteiroValido nullptr \n");
                            ponteiroValido = irmao;
                        }
                        else {
                            debug("ponteiroValido  n nullptr \n");
                            ponteiroValido->pai = irmao;
                            if(irmao->ap1 == nullptr) {
                                irmao->ap1 = ponteiroValido;
                                ponteiroValido = irmao;
                            }
                            else if (irmao->ap2 == nullptr) {
                                irmao->ap2 = ponteiroValido;
                                ponteiroValido = irmao;
                            }
                            else if (!irmao->doisNo && irmao->ap3 == nullptr) {
                                irmao->ap3 = ponteiroValido;
                                ponteiroValido = irmao;
                            }
                            else {
                                ponteiroValido = irmao;
                            }

                        }
                        
                    }
                }
            }
        }

    }
}

template <class Chave, class Valor>
void arvore23<Chave, Valor> :: rankRecur (No23* q, Chave chave, int& r) {
    if(q != nullptr) {
        rankRecur(q->ap1, chave, r);
        if (q->chave1 < chave) r++;
        rankRecur(q->ap2, chave, r);
        
        if(q->doisNo == false) {
            if (q->chave2 < chave) r++;
            rankRecur(q->ap3, chave, r);
        }
    }
}

/* O(n) */
template <class Chave, class Valor>
int arvore23<Chave, Valor> :: rank(Chave chave) {
    int r = 0;
    rankRecur(raiz, chave, r);
    return r;
}

template <class Chave, class Valor>
void arvore23<Chave, Valor> :: selecionaRecur(No23* q, int k, Chave& chaveK) {
    if(q != nullptr) {
        selecionaRecur(q->ap1, k, chaveK);
        if (rank(q->chave1) == k) {
            chaveK = q->chave1;
            return;
        }
        selecionaRecur(q->ap2, k, chaveK);
        
        if(q->doisNo == false) {
            if (rank(q->chave2) == k) {
                chaveK = q->chave2;
                return;
            }
            selecionaRecur(q->ap3, k, chaveK);
        }
    }
}

/* O(n^2) */
template <class Chave, class Valor>
Chave arvore23<Chave, Valor> :: seleciona(int k) {
    Chave chaveK = "";
    selecionaRecur(raiz, k, chaveK);
    return chaveK;
}

template <class Chave, class Valor>
void arvore23<Chave, Valor> :: exibeElemento(No23* q, char c) {
    cout << "Chave" << "-" << c << ", " << q->chave1 << ", Adress" << q << ", Valor: " << q->valor1
        << " ,fap1: " << q->ap1 << ", fap2: " << q->ap2 << ", fap3: " << q->ap3 << ", 2no: "
        << q->doisNo << "pai: "<< q->pai << endl;
}

template <class Chave, class Valor>
void arvore23<Chave, Valor> :: exibeTSUtilRecur(No23* q) {
    if(q != nullptr) {
        exibeTSUtilRecur(q->ap1);
        exibeElemento(q, 'P');
        n++;
        exibeTSUtilRecur(q->ap2);
        
        if(q->doisNo == false) {
            exibeElemento(q, 'S');
            n++;
            exibeTSUtilRecur(q->ap3);
        }
    }
}

template <class Chave, class Valor>
void arvore23<Chave, Valor> :: exibeTS() {
    n = 0;
    exibeTSUtilRecur(raiz);
    cout << n << endl;
}

#endif