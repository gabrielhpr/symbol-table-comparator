#ifndef TS_ARVORES_RUBRO_NEGRAS_H
#define TS_ARVORES_RUBRO_NEGRAS_H
#include <iostream>
#include "estruturas.h"
using namespace std;

template <class par>
class TSArvoresRubroNegras { 
    public:
        TSArvoresRubroNegras();
        ~TSArvoresRubroNegras();

        /*Árvore Rubro Negra*/
        NoRN* raiz;
        NoRN* criaNo(Chave chave, Item valor);

        /*Número de elementos*/
        int n;//public

        /*Métodos da TS*/
        void insere(Chave chave, Item valor);
        Item devolve(Chave chave);
        void remove(Chave chave);
        int rank(Chave chave);
        Chave seleciona(int k); 

        //Atualiza informações dos filhos dos nós - utilizado pelas rotações
        void atualizaInfQuantFilhosDosNos(NoRN* no1, NoRN* no2, char direction);

        //Busca a chave
        NoRN* buscaAChave(NoRN* p, bool& achou);

        //Método utilizado pelo destructor
        void destroiArvoreRubroNegra(NoRN* q);

        void exibeTSUtilRecur(NoRN* q);
        void exibeTS(); 
};

template <class par>
TSArvoresRubroNegras<par> :: TSArvoresRubroNegras() {
    n = 0;
    raiz = nullptr;
}

template <class par> 
void TSArvoresRubroNegras<par> :: destroiArvoreRubroNegra(NoRN* q) {
    if(q != nullptr) {
        destroiArvoreRubroNegra(q->esq);
        destroiArvoreRubroNegra(q->dir);
        delete q;
    }
}

template <class par>
TSArvoresRubroNegras<par> :: ~TSArvoresRubroNegras() {
    //Executar em pós-ordem
    destroiArvoreRubroNegra(raiz);
}

template <class par>
void TSArvoresRubroNegras<par> :: atualizaInfQuantFilhosDosNos(NoRN* no1, NoRN* no2, char direction) {
    //Atualiza a quantidade de filhos de cada nó
    //Esquerda
    if(direction == 'e') {
        no1->quantNosSubArvDir = no2->quantNosSubArvEsq;
        no2->quantNosSubArvEsq += 1 + no1->quantNosSubArvEsq;
    }
    //Direita
    else {
        no1->quantNosSubArvEsq = no2->quantNosSubArvDir;
        no2->quantNosSubArvDir += 1 + no1->quantNosSubArvDir;
    }
}

template <class par>
NoRN* TSArvoresRubroNegras<par> :: buscaAChave(NoRN* p, bool& achou) {
    //Busca
    while(!achou) {
        if(chave < p->chave && p->esq != nullptr) 
            p = p->esq; 
        else if(chave < p->chave && p->esq == nullptr) 
            achou = true;
        else if(chave > p->chave && p->dir != nullptr) 
            p = p->dir;
        else if(chave > p->chave && p->dir == nullptr) 
            achou = true;
        else 
            achou = true;
    }
    return p;
} 

template <class par>
NoRN* TSArvoresRubroNegras<par> :: criaNo(Chave chave, Item valor) {
    NoRN* novo = new NoRN;
    novo->chave = chave;
    novo->valor = valor;
    novo->cor = 'V';
    novo->esq = nullptr;
    novo->dir = nullptr;
    novo->pai = nullptr;
    novo->ehDuploPreto = false;
    novo->quantNosSubArvEsq = 0;
    novo->quantNosSubArvDir = 0;
    return novo;
}

/* O(log n) */
/*Atualizar o numero de elementos das subarvores por meio de uma busca em abb, e atualizar tbm
durante as rotacoes, fazer isso no insere e no remove*/
template <class par>
void TSArvoresRubroNegras<par> :: insere(Chave chave, Item valor) {
    //Caso raiz nula
    if(raiz == nullptr) {
        raiz = criaNo(chave, valor);
        n++;
        return;
    }

    bool achou = false;
    NoRN *p = raiz;

    //Busca
    p = buscaAChave(p, achou);

    //Atualiza se já existe
    if(p->chave == chave) {
        p->valor++;
        return;
    }
    //Se não existe, refazemos a busca atualizando a quantidade de nós das subárvores
    else {
        bool achou = false;
        NoRN *p = raiz;

        //Busca
        while(!achou) {
            if(chave < p->chave && p->esq != nullptr) {
                p->quantNosSubArvEsq++;
                p = p->esq; 
            }
            else if(chave < p->chave && p->esq == nullptr) {
                p->quantNosSubArvEsq++;
                achou = true;
            }
            else if(chave > p->chave && p->dir != nullptr) {
                p->quantNosSubArvDir++;
                p = p->dir;
            }
            else if(chave > p->chave && p->dir == nullptr) {
                p->quantNosSubArvDir++;
                achou = true;
            }
            else 
                achou = true;
        }
    }

    //Cria o novo nó
    NoRN* novo = criaNo(chave, valor);
    novo->pai = p;
    n++;

    //Insere o novo nó vermelho
    if(novo->chave > p->chave) 
        p->dir = novo;
    else 
        p->esq = novo;

    NoRN* filho = novo;

    while(p != nullptr) {
        //Caso pai preto, não altera nada
        if(p->cor == 'P') break;

        //Caso sem avô, troca a cor do pai
        NoRN* avo = p->pai;
        if(avo == nullptr) {
            p->cor = 'P';
            break;
        }
        //Caso tio vermelho
        NoRN* tio = (avo->esq == p) ? avo->dir : avo->esq;
        if(tio != nullptr && tio->cor == 'V') {
            avo->cor = 'V';
            p->cor = 'P';
            tio->cor = 'P';
            filho = avo;
            p = filho->pai;
        }
        //Caso tio preto ou null
        else {
            if(p == avo->esq && filho == p->dir) {
                //Atualiza a quantidade de filhos de cada nó
                atualizaInfQuantFilhosDosNos(p, filho, 'e');

                //Rotação pra esquerda, p sendo o eixo
                p->dir = filho->esq;
                if(filho->esq != nullptr) filho->esq->pai = p;
                filho->esq = p;
                filho->pai = avo;
                avo->esq = filho;
                p->pai = filho;

                p = avo->esq;
                filho = filho->esq;
            }
            else if(p == avo->dir && filho == p->esq) {
                //Atualiza a quantidade de filhos de cada nó
                atualizaInfQuantFilhosDosNos(p, filho, 'd');
            
                //Rotação pra direita, p sendo o eixo
                p->esq = filho->dir;
                if(filho->dir != nullptr) filho->dir->pai = p;
                filho->dir = p;
                filho->pai = avo;
                avo->dir = filho;
                p->pai = filho;

                p = avo->dir;
                filho = filho->dir;
            }
            else if(p == avo->esq && filho == p->esq) {
                //Atualiza a quantidade de filhos de cada nó
                atualizaInfQuantFilhosDosNos(avo, p, 'd');

                //Rotação pra direita
                if(avo->pai != nullptr) {
                    if(avo->pai->esq == avo) avo->pai->esq = p;
                    else avo->pai->dir = p;
                }
                avo->esq = p->dir;
                if(p->dir != nullptr) p->dir->pai = avo;
                p->dir = avo;
                p->pai = avo->pai;
                avo->pai = p;
                p->cor = 'P';
                avo->cor = 'V';

                if(avo == raiz) {
                    raiz = p;
                    break;
                }
            }
            else if(p == avo->dir && filho == p->dir) {

                //Atualiza a quantidade de filhos de cada nó
                atualizaInfQuantFilhosDosNos(avo, p, 'e');

                //Rotação pra esquerda
                if(avo->pai != nullptr) {
                    if(avo->pai->esq == avo) avo->pai->esq = p;
                    else avo->pai->dir = p;
                }
                avo->dir = p->esq;
                if(p->esq != nullptr) p->esq->pai = avo;
                p->esq = avo;
                p->pai = avo->pai;
                avo->pai = p;
                p->cor = 'P';
                avo->cor = 'V';

                if(avo == raiz) {
                    raiz = p;
                    break;
                }
            }            
        }
    }
}

/*Retorna o valor da chave correspondente ou -1 se a chave não existe*/
/* O(log n) */
template <class par>
Item TSArvoresRubroNegras<par> :: devolve(Chave chave) {
    Item valor = -1;
    NoRN* aux;

    aux = raiz;
    
    while(aux != nullptr) {
        if(aux->chave == chave) {
            valor = aux->valor;
            break;
        }
        else if(aux->chave > chave) 
            aux = aux->esq;
        else 
            aux = aux->dir;
    }
    return valor;
}

/* O(log n) */
template <class par>
void TSArvoresRubroNegras<par> :: remove(Chave chave) {
    bool removeu = false;
    NoRN* p = raiz;

    //Raiz nula
    if(raiz == nullptr) {
        cout << "Não há elementos para serem removidos" << endl;
        return;
    }

    //Caso só exista a raiz
    if(raiz->chave == chave && raiz->dir == nullptr && raiz->esq == nullptr) {
        delete p;
        raiz = nullptr;
        return;
    }

    //Procura a chave
    bool achou = 0;
    while(p != nullptr && !achou) {
        if(chave < p->chave) 
            p = p->esq;
        else if(chave > p->chave) 
            p = p->dir;
        else 
            achou = 1;
    }

    //Não achou o nó a ser removido
    if(achou == 0) return;
    //Achou o nó, atualiza os nós do percurso até ele
    else {
        //Procura a chave
        bool achou = 0;
        while(p != nullptr && !achou) {
            if(chave < p->chave) {
                p->quantNosSubArvEsq--;
                p = p->esq;
            }
            else if(chave > p->chave) {
                p->quantNosSubArvDir--;
                p = p->dir;
            }
            else 
                achou = 1;
        }   
    }
    
    NoRN* subs;
    //Enquanto p não é uma folha
    while( !(p->esq == nullptr && p->dir == nullptr) ) {
        //Pega o menor da sub árvore direita, ou o maior da sub árvore esquerda
        if (p->dir != nullptr) {
            p->quantNosSubArvDir--;
            subs = p->dir;
            while (subs->esq != nullptr) {
                subs->quantNosSubArvEsq--;
                subs = subs->esq;
            }
        }
        else {
            p->quantNosSubArvEsq--;
            subs = p->esq;
            while (subs->dir != nullptr) {
                subs->quantNosSubArvDir--;
                subs = subs->dir;
            }
        }
        //Substitue o p com o subs
        p->chave = subs->chave;
        p->valor = subs->valor;
        p = subs;
    }

    //Folha vermelha, só remover
    if(p->cor == 'V') {
        if(p == p->pai->esq) p->pai->esq = nullptr;
        else p->pai->dir = nullptr;     
        delete p;
    }
    //Folha preta
    else if (p->cor == 'P') {
        p->ehDuploPreto = true;
        while (p != nullptr && p->ehDuploPreto == true) {
            NoRN *pai = p->pai;
            //Caso 2.0 - Duplo preto é a raiz -> raiz fica preta
            if (p == raiz) {
                p->cor = 'P';
                //Deleta o p
                if(removeu == false) delete p;
                else p->ehDuploPreto = false;
                break;
            }
            //Caso 2.1 - Irmão preto com filhos pretos ou sem filhos
            //Acha o irmão
            NoRN *irmao = (p == pai->dir) ? pai->esq : pai->dir;

            //Acha os sobrinhos
            NoRN *sobrinhoPerto, *sobrinhoLonge;
            sobrinhoPerto = sobrinhoLonge = nullptr;
            if (irmao != nullptr && irmao == irmao->pai->dir){
                sobrinhoPerto = irmao->esq;
                sobrinhoLonge = irmao->dir;
            }
            else if (irmao != nullptr && irmao == irmao->pai->esq) {
                sobrinhoPerto = irmao->dir;
                sobrinhoLonge = irmao->esq;
            }

            //Se o irmão é null, o pai vira duplo preto
            if (irmao == nullptr){
                pai->ehDuploPreto = true;
                if(removeu == false) { 
                    delete p;
                    removeu = true;
                }
                else 
                    p->ehDuploPreto = false;
                p = pai;
            }

            else if (irmao->cor == 'P' && (irmao->dir == nullptr || irmao->dir->cor == 'P') && (irmao->esq == nullptr || irmao->esq->cor == 'P')) {
                //Irmão fica vermelho
                irmao->cor = 'V';
                pai->ehDuploPreto = true;
                //Se o pai é vermelho, fica preto, se já é preto fica duplo preto
                if (pai->cor == 'V') {
                    pai->cor = 'P';
                    pai->ehDuploPreto = false;
                    if(removeu == false) {
                        if(p == pai->esq) pai->esq = nullptr;
                        else pai->dir = nullptr;
                        delete p;
                        removeu = true;
                    }
                    break;
                }
                else p->ehDuploPreto = false;
                p = pai;
            }
            //Caso 2.2 - Irmão vermelho
            else if (irmao->cor == 'V') {
                //Troca a cor do pai com o irmão
                char corTroca;
                corTroca = pai->cor;
                pai->cor = irmao->cor;
                irmao->cor = corTroca;
                //Roda na direção do duplo preto
                //Roda pra esquerda
                if (p == pai->esq) {
                    //Atualiza a quantidade de filhos de cada nó
                    atualizaInfQuantFilhosDosNos(pai, pai->dir, 'e');

                    //Direita do pai recebe a subarvore esquerda do irmao
                    pai->dir = irmao->esq;  
                    
                    //Muda o pai do antigo no da subarvore esquerda do irmao
                    if (irmao->esq != nullptr)
                        irmao->esq->pai = pai;
                    irmao->esq = pai;

                    //O irmao vai virar o novo pai, e deixa de ser irmão do no p
                    if (pai->pai != nullptr) {
                        if (pai->pai->esq == pai)
                            pai->pai->esq = irmao;
                        else
                            pai->pai->dir = irmao;
                    }
                    irmao->pai = pai->pai;
                    pai->pai = irmao;

                    //Acha o novo irmão do no p
                    irmao = pai->dir;
                }
                //Roda pra direita
                else if (p == pai->dir) {

                    //Atualiza a quantidade de filhos de cada nó
                    atualizaInfQuantFilhosDosNos(pai, pai->esq, 'd');

                    //Esquerda do pai recebe a subarvore direita do irmao
                    pai->esq = irmao->dir;

                    //Muda o pai do antigo no da subarvore esquerda do irmao
                    if (irmao->dir != nullptr)
                        irmao->dir->pai = pai;
                    irmao->dir = pai;

                    //O irmao vai virar o novo pai, e deixa de ser irmão do no p
                    if (pai->esq != nullptr) {
                        if (pai->pai->dir == pai)
                            pai->pai->dir = irmao;
                        else
                            pai->pai->esq = irmao;
                    }
                    irmao->pai = pai->pai;
                    pai->pai = irmao;

                    //Acha o novo irmão do no p
                    irmao = pai->esq;
                }
            }
            //Caso 2.3 irmão preto e sobrinho perto vermelho e sobrinho longe preto ou null
            else if (irmao->cor == 'P' && (sobrinhoPerto != nullptr && sobrinhoPerto->cor == 'V') && (sobrinhoLonge == nullptr || sobrinhoLonge->cor == 'P')) {
                pai = irmao->pai;
                //Troca a cor do irmão com o sobrinho mais próximo
                irmao->cor = 'V';
                sobrinhoPerto->cor = 'P';

                //Rotaciona contra o sentido do DP, usando o irmão como eixo
                if (irmao == pai->dir) {

                    //Atualiza a quantidade de filhos de cada nó
                    atualizaInfQuantFilhosDosNos(irmao, sobrinhoPerto, 'd');

                    irmao->esq = sobrinhoPerto->dir;
                    if (sobrinhoPerto->dir != nullptr)
                        sobrinhoPerto->dir->pai = irmao;
                    sobrinhoPerto->dir = irmao;
                    pai->dir = sobrinhoPerto;
                    irmao->pai = sobrinhoPerto;
                    sobrinhoPerto->pai = pai;
                }
                else if (irmao == pai->esq) {

                    //Atualiza a quantidade de filhos de cada nó
                    atualizaInfQuantFilhosDosNos(irmao, sobrinhoPerto, 'e');

                    irmao->dir = sobrinhoPerto->esq;
                    if (sobrinhoPerto->esq != nullptr)
                        sobrinhoPerto->esq->pai = irmao;
                    sobrinhoPerto->esq = irmao;
                    pai->esq = sobrinhoPerto;
                    irmao->pai = sobrinhoPerto;
                    sobrinhoPerto->pai = pai;
                }
            }
            //Caso 2.4
            else if (irmao->cor == 'P' && (sobrinhoLonge != nullptr && sobrinhoLonge->cor == 'V') && (sobrinhoPerto == nullptr || sobrinhoPerto->cor == 'P' || sobrinhoLonge->cor == 'V')) {
                char corTroca;
                corTroca = pai->cor;
                pai->cor = irmao->cor;
                irmao->cor = corTroca;

                //Roda pra esquerda - na direção do duplopreto
                if (p == pai->esq) {

                    //Atualiza a quantidade de filhos de cada nó
                    atualizaInfQuantFilhosDosNos(pai, irmao, 'e');

                    pai->dir = irmao->esq;
                    if (irmao->esq != nullptr)
                        irmao->esq->pai = pai;
                    irmao->esq = pai;
                    if (pai->pai != nullptr) {
                        if (pai == pai->pai->esq)
                            pai->pai->esq = irmao;
                        else
                            pai->pai->dir = irmao;
                    }

                    //Se o pai do pai é nullptr, pai é raiz, então o irmão vira a raiz
                    if(pai->pai == nullptr) raiz = irmao;

                    irmao->pai = pai->pai;
                    pai->pai = irmao;
                    sobrinhoLonge->cor = 'P';

                    if(removeu == false) {
                        if(p == p->pai->esq) p->pai->esq = nullptr;
                        else pai->dir = nullptr;
                        delete p;
                    }
                    else p->ehDuploPreto = false;
                    break;
                }
                //Roda pra direita
                else if (p == pai->dir) {

                    //Atualiza a quantidade de filhos de cada nó
                    atualizaInfQuantFilhosDosNos(pai, irmao, 'd');

                    pai->esq = irmao->dir;
                    if (irmao->dir != nullptr)
                        irmao->dir->pai = pai;
                    irmao->dir = pai;
                    if (pai->pai != nullptr) {
                        if (pai == pai->pai->esq)
                            pai->pai->esq = irmao;
                        else
                            pai->pai->dir = irmao;
                    }

                    //Se o pai do pai é nullptr, pai é raiz, então o irmão vira a raiz
                    if(pai->pai == nullptr) raiz = irmao;

                    irmao->pai = pai->pai;
                    pai->pai = irmao;
                    sobrinhoLonge->cor = 'P';
                    if(removeu == false) {
                        if(p == p->pai->esq) pai->esq = nullptr;
                        else pai->dir = nullptr;
                        delete p;
                    }
                    else p->ehDuploPreto = false;
                    break;
                }
            }
        }
    }
}

/* O(log n) */
template <class par>
int TSArvoresRubroNegras<par> :: rank(Chave chave) {
    int n_elements = 0;
    int r;
    if(chave == raiz->chave) {
        n_elements = raiz->quantNosSubArvEsq;
    }
    else {
        NoRN* aux;
        if(chave < raiz->chave) 
            aux = raiz->esq;
        else {
            n_elements += raiz->quantNosSubArvEsq + 1;
            aux = raiz->dir;
        }
        while(aux != nullptr) {
            if(aux->chave == chave) {
                n_elements += aux->quantNosSubArvEsq;
                break;
            } 
            else if(chave < aux->chave) 
                aux = aux->esq;
            else {
                n_elements += aux->quantNosSubArvEsq+1;
                aux = aux->dir;
            }
        }
    }
    return n_elements;
}

/* O(logn * logn) */
template <class par>
Chave TSArvoresRubroNegras<par> :: seleciona(int k) {
    Chave chave = "";
    int r;
    NoRN* aux = raiz;
    while(aux != nullptr) {
        r = rank(aux->chave);
        if(r == k) {
            chave = aux->chave;
            break;
        }
        else if(k < r) 
            aux = aux->esq;
        else 
            aux = aux->dir;
    }

    return chave;
}

template <class par>
void TSArvoresRubroNegras<par> :: exibeTSUtilRecur(NoRN* q) {
    if(q != nullptr) {
        exibeTSUtilRecur(q->esq);
        cout << "Chave: " << q->chave << ", Valor: " << q->valor << ", Cor: "<< q->cor
        << " ,fesq: " << q->esq << ", fdir: " << q->dir << "quantEsq: " << q->quantNosSubArvEsq
        << ", quantDir: " << q->quantNosSubArvDir << endl;
        exibeTSUtilRecur(q->dir);
    }
}

template <class par>
void TSArvoresRubroNegras<par> :: exibeTS() {
    if(raiz == nullptr) {
        cout << "Não há elementos" << endl;
        return; 
    }
    exibeTSUtilRecur(raiz);
    cout << n << endl;
}
#endif
//Tinha 697 linhas