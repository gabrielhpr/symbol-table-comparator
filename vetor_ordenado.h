#ifndef TS_VETOR_ORDENADO_H
#define TS_VETOR_ORDENADO_H

#include <iostream>
using namespace std;

typedef string Chave;
typedef int Item;

template <class par>
class TSVetorOrdenado { 
    public:
        TSVetorOrdenado();
        ~TSVetorOrdenado();

        /*Métodos da TS*/
        void insere(Chave chave, Item valor);
        Item devolve(Chave chave);
        void remove(Chave chave);
        int rank(Chave chave);
        Chave seleciona(int k); 

        void exibeTS();

        /*Vetor*/
        par* vo;
        /*Número de elementos*/
        int n;//public
        int tam;//tamanho do vetor
        void resize();
};

template <class par>
TSVetorOrdenado<par> :: TSVetorOrdenado() {
    n = 0;
    tam = 2;
    vo = new par[tam];
}

template <class par>
TSVetorOrdenado<par> :: ~TSVetorOrdenado() {
    delete[] vo;
}

/* O(n+logn) - sem resize */
template <class par>
void TSVetorOrdenado<par> :: insere(Chave chave, Item valor) {
    //Checa se a chave já pertence ao vetor
    //Se a chave já existe, atualiza o seu valor
    int r = rank(chave);

    if(r != n && seleciona(r) == chave) {
        vo[r].valor += 1;
    }
    else {
        //O vetor está lotado, dobra seu tamanho
        if(n == tam) resize();

        if(r == n) {
            vo[n].chave = chave;
            vo[n].valor = valor;
            n++;
        }
        else {
            for (int i = n; i > r; i--) {
                vo[i] = vo[i-1];
            }
            vo[r].chave = chave;
            vo[r].valor = valor;
            n++;
        }
    }
}

/*Retorna o valor da chave correspondente ou -1 se a chave não existe*/
/* O(log n) */
template <class par>
Item TSVetorOrdenado<par> :: devolve(Chave chave) {
    int r = rank(chave);
    if (r < n) return vo[r].valor;
    return -1;
}

/* O(n+log n) - Sem lazy deletion*/
template <class par>
void TSVetorOrdenado<par> :: remove(Chave chave) {
    int r = rank(chave);
    if (r != n) {
        for (int i = r; i < n-1; i++) 
            vo[r] = vo[r+1];
        n--;
    }
}

/* O(log n) */
template <class par>
int TSVetorOrdenado<par> :: rank(Chave chave) {
    int ini, fim, mid;
    int r = n;
    ini = 0;
    fim = n-1;

    while(ini <= fim) {
        mid = (ini+fim)/2;
        if(vo[mid].chave == chave) {
            r = mid;
            break;
        } 
        else if(vo[mid].chave > chave && ( ((mid-1) >= 0 && vo[mid-1].chave < chave) || (mid-1) < 0 ) ) {
            r = mid;
            break;
        }
        else if(vo[mid].chave > chave) 
            fim = mid-1;
        else 
            ini = mid+1;
    }
    return r;
}

/* O(1) */
template <class par>
Chave TSVetorOrdenado<par> :: seleciona(int k) {
    if(k < n)
        return vo[k].chave;
    return "";
}

template <class par>
void TSVetorOrdenado<par> :: resize() {
    //Novo vetor desordenado
    par* von = new par[2*tam];
    tam = tam*2;

    for (int i = 0; i < n; i++) {
        von[i] = vo[i];
    }
    delete[] vo;
    vo = von;
}

template <class par>
void TSVetorOrdenado<par> :: exibeTS() {
    for (int i = 0; i < n; i++) {
        cout << "Chave: " << vo[i].chave << ", Valor: " << vo[i].valor << endl;
    }
}


#endif