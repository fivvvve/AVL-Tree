#include <stdio.h>
#include <stdlib.h>
#include "arvorebinaria.h"


struct no {
    int chave;
    int fb;
    struct no *Fesq;
    struct no *Fdir;
    struct no *pai;
};


struct avl {
    struct no *sentinela;
    int numElementos;    
};


//Cria uma nova árvore inicializando a sentinela
avl *criaArvore(){
    avl *arv = (avl*) malloc(sizeof(avl));
    arv->sentinela = (no*) malloc(sizeof(no));
    arv->numElementos = 0;
    arv->sentinela->Fdir = NULL;
    arv->sentinela->Fesq = NULL;
    arv->sentinela->pai = NULL;
    arv->sentinela->chave = -1000;

    return(arv);
}
//Função que insere um elemento na árvore
//Retorna 1 se a inserção foi realizada com sucesso
//Retorna 0 se não foi possível realizar a inserção
int insereNo(avl *arv, int valor){
    if(arv == NULL) return 0;
    arv->numElementos++;

    no *atual = arv->sentinela->Fdir;

    if(atual == NULL){
        atual = (no*) malloc(sizeof(no));
        arv->sentinela->Fdir = atual;
        atual->fb = 0;
        atual->Fdir = NULL;
        atual->Fesq = NULL;
        atual->pai = arv->sentinela;
        atual->chave = valor;
        
        return 1;
    }

    no *ant;

    while(atual != NULL){
        if(valor >= atual->chave) {
            ant = atual;
            atual = atual->Fdir;
        } else {
            ant = atual;
            atual = atual->Fesq;
        }
    }

    atual = (no*) malloc(sizeof(no));
    atual->fb = 0;
    atual->Fdir = NULL;
    atual->Fesq = NULL;
    atual->pai = ant;
    atual->chave = valor;

    if(valor >= ant->chave) ant->Fdir = atual;
    else ant->Fesq = atual;

    atualizaFB_Insercao(arv, atual);

    return 1;
}

//Função que remove um elemento da árvore implementando a remoção por cópia usando o sucessor
//Retorna 1 se o elemento foi removido
//Retorna 0 se a árvore estiver vazia
//Retorna -1 caso o elemento a ser removido não esteja na árvore
int removeNo(avl *arv, int valor){
    if(arv->numElementos == 0) return 0;

    no *atual = arv->sentinela->Fdir;

    while(atual != NULL && atual->chave != valor){
        if(atual->chave <= valor) atual = atual->Fdir;
        else atual = atual->Fesq;
    }

    if(atual == NULL) return -1;

    arv->numElementos--;

    if(atual->Fdir == NULL && atual->Fesq == NULL){
        if(atual->chave >= atual->pai->chave) atual->pai->Fdir = NULL;
        else atual->pai->Fesq = NULL;
        atualizaFB_Remocao(arv, atual->pai, atual->chave);
        free(atual);
        return 1;
    }

    no *filho;
    if(atual->Fdir == NULL || atual->Fesq == NULL){
        if(atual->Fesq != NULL) filho = atual->Fesq;
        else filho = atual->Fdir;

        if(atual->chave >= atual->pai->chave) atual->pai->Fdir = filho;
        else atual->pai->Fesq = filho;

        filho->pai = atual->pai;

        atualizaFB_Remocao(arv, atual->pai, atual->chave);

        free(atual);

        return 1;
    }

    filho = atual->Fdir;
    while(filho->Fesq != NULL) filho = filho->Fesq;

    if(filho->Fdir != NULL){
        filho->Fdir->pai = filho->pai;
    }

    if(filho->pai == atual){
        filho->pai->Fdir = filho->Fdir;
    } else {
        filho->pai->Fesq = filho->Fdir;
    }
    

    atual->chave = filho->chave;

    atualizaFB_Remocao(arv, filho->pai, filho->chave);

    free(filho);
    return 1;
}


//Função que imprime a árvore em ordem
//printf("%d - %d - %d\n", raiz->chave, raiz->pai->chave, raiz->fb);
void imprimeOrdem(no *raiz){
    if(raiz==NULL) return;

    imprimeOrdem(raiz->Fesq);
    printf("%d - %d - %d\n", raiz->chave, raiz->pai->chave, raiz->fb);
    imprimeOrdem(raiz->Fdir);
}

//Função que imprime a árvore em pré-ordem
//printf("%d - %d - %d\n", raiz->chave, raiz->pai->chave, raiz->fb);
void imprimePreOrdem(no *raiz){
    if(raiz==NULL) return;

    printf("%d - %d - %d\n", raiz->chave, raiz->pai->chave, raiz->fb);
    imprimePreOrdem(raiz->Fesq);
    imprimePreOrdem(raiz->Fdir);
}


//Função que retorna a raiz da árvore
no *getRaiz(avl *arv){
    return arv->sentinela->Fdir;
}

//Função que retorna a quantidade de elementos da árvore
int getNumElementos(avl *arv){
    return arv->numElementos;
}

//Função que lê um arquivo com números inteiros e carrega na árvore
void processaCarga(avl *arv, char *nomeArquivo){

    FILE *arq = fopen(nomeArquivo, "r");

    int valor;
    int ret;

    while(!feof(arq)){
        ret = fscanf(arq, "%d\n", &valor);
        insereNo(arv, valor);
    }

}

//****************Funções de balancemento - inserção******************//
//atualiza o valor de balancemento dos nós após a inserção de um nó
//condições de parada: após o ajuste do FB => chegar no nó raiz ou o nó ficar com fb 0, 2 ou -2
void atualizaFB_Insercao(avl *arv, no *novoNo){
    
    no *aux = novoNo;

    do{
        if(aux->chave < aux->pai->chave){
            aux->pai->fb--;
        } else {
            aux->pai->fb++;
        }
        aux = aux->pai;
    }while(aux != arv->sentinela->Fdir && (aux->fb == 1 || aux->fb == -1));

    if(aux->fb == -2 || aux->fb == 2){
        balancemento(arv, aux);
    }

}

//verifica a estratégia de balancemento do nó e ajusta o fator de balanceamento
void balancemento(avl *arv, no *noDesbalanceado){

    no *filho;
    if(noDesbalanceado->fb == -2){
        filho = noDesbalanceado->Fesq;
        if(filho->fb == 1){
            //Rotação dupla
            int fbNeto = filho->Fdir->fb;
            rotacaoEsq(filho);
            rotacaoDir(noDesbalanceado);
            //ajustar fb
            if(fbNeto == 0){
                noDesbalanceado->fb = 0;
                filho->fb = 0;
            } else if(fbNeto == -1) {
                noDesbalanceado->fb = 1;
                filho->fb = 0;
                noDesbalanceado->pai->fb = 0;
            } else {
                noDesbalanceado->fb = 0;
                filho->fb = -1;
                noDesbalanceado->pai->fb = 0;
            }
        }
        else{
            //rotação simples
            int fbFilho = filho->fb;
            rotacaoDir(noDesbalanceado);
            filho->fb = 0;
            noDesbalanceado->fb = 0;
            if(fbFilho == 0){
                //só acontece na remoção
                //Ajustar o fb
                filho->fb = 1;
                noDesbalanceado->fb = -1;
            }
        }
    }

    if(noDesbalanceado->fb == 2){
        filho = noDesbalanceado->Fdir;
        if(filho->fb == -1){
            //Rotação dupla
            int fbNeto = filho->Fesq->fb;
            rotacaoDir(filho);
            rotacaoEsq(noDesbalanceado);
            //ajustar fb
            if(fbNeto == 0){
                noDesbalanceado->fb = 0;
                filho->fb = 0;
            } else if(fbNeto == -1) {
                noDesbalanceado->fb = 0;
                filho->fb = 1;
                noDesbalanceado->pai->fb = 0;
            } else {
                noDesbalanceado->fb = -1;
                filho->fb = 0;
                noDesbalanceado->pai->fb = 0;
            }
        }
        else{
            //rotação simples
            int fbFilho = filho->fb;
            rotacaoEsq(noDesbalanceado);
            filho->fb = 0;
            noDesbalanceado->fb = 0;
            if(fbFilho == 0){
                //só acontece na remoção
                //Ajustar o fb
                filho->fb = -1;
                noDesbalanceado->fb = 1;
            }
        }
    }
}

//****************Funções de balancemento - remocao******************//
//atualiza o valor de balancemento dos nós após a inserção de um nó
//condições de parada: após o ajuste do FB => chegar no nó raiz ou o nó ficar com fb 1, -1, 2 ou -2
void atualizaFB_Remocao(avl *arv, no *paidoNo, int chave){

    if(paidoNo == arv->sentinela) return;

    no *aux = paidoNo;

    if(chave >= aux->chave) aux->fb--;
    else aux->fb++;


    while(aux != arv->sentinela->Fdir && aux->fb == 0){
        if(aux->chave >= aux->pai->chave){
            aux->pai->fb--;
        } else {
            aux->pai->fb++;
        }
        
        aux = aux->pai;
    }


    if(aux->fb == 2 || aux->fb == -2){
        balancemento(arv, aux);
        if(aux->pai->fb == 0 && aux->pai->pai != arv->sentinela){
            atualizaFB_Remocao(arv, aux->pai->pai, aux->pai->chave);
        }
    }

}

//rotação à esquerda no nó desbalanceado
void rotacaoEsq(no *noDesbalanceado){

    no *pai_de_todos = noDesbalanceado->pai;

    noDesbalanceado->pai = noDesbalanceado->Fdir;
    noDesbalanceado->Fdir = noDesbalanceado->pai->Fesq;
    if(noDesbalanceado->pai->Fesq != NULL){
        noDesbalanceado->pai->Fesq->pai = noDesbalanceado;
    }
    noDesbalanceado->pai->Fesq = noDesbalanceado;
    noDesbalanceado->pai->pai = pai_de_todos;

    if(noDesbalanceado->chave >= pai_de_todos->chave){
        pai_de_todos->Fdir = noDesbalanceado->pai;
    } else {
        pai_de_todos->Fesq = noDesbalanceado->pai;
    }

}

//rotação à direita no nó desbalanceado
void rotacaoDir(no *noDesbalanceado){

    no *pai_de_todos = noDesbalanceado->pai;

    noDesbalanceado->pai = noDesbalanceado->Fesq;
    noDesbalanceado->Fesq = noDesbalanceado->pai->Fdir;
    if(noDesbalanceado->Fesq != NULL){
        noDesbalanceado->Fesq->pai  = noDesbalanceado;
    }
    noDesbalanceado->pai->Fdir = noDesbalanceado;
    noDesbalanceado->pai->pai = pai_de_todos;

    if(noDesbalanceado->chave >= pai_de_todos->chave){
        pai_de_todos->Fdir = noDesbalanceado->pai;
    } else {
        pai_de_todos->Fesq = noDesbalanceado->pai;
    }
}