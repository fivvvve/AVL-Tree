//
// Created by Vanessa on 27/04/2023.
//

#ifndef ARVOREBINARIA_ARVOREBINARIA_H
#define ARVOREBINARIA_ARVOREBINARIA_H

typedef struct no no;
typedef struct avl avl;

//Função que cria e retorna uma estrutura do tipo árvore
avl *criaArvore();

//Função que insere um elemento na árvore
//Retorna 1 se a inserção foi realizada com sucesso
//Retorna 0 se não foi possível realizar a inserção
int insereNo(avl *avl, int valor);

//Função que remove um elemento da árvore implementando a remoção por cópia usando o sucessor
//Retorna 1 se o elemento foi removido
//Retorna 0 se a árvore estiver vazia
//Retorna -1 caso o elemento a ser removido não esteja na árvore
int removeNo(avl *avl, int valor);

//Função que imprime a árvore em ordem
//printf("%d - %d - %d\n", raiz->chave, raiz->pai->chave, raiz->fb);
void imprimeOrdem(no *raiz);

//Função que imprime a árvore em pré-ordem
//printf("%d - %d - %d\n", raiz->chave, raiz->pai->chave, raiz->fb);
void imprimePreOrdem(no *raiz);

//Função que retorna a raiz da árvore
no *getRaiz(avl *avl);

//Função que retorna a quantidade de elementos da árvore
int getNumElementos(avl *avl);

//Função que lê um arquivo com números inteiros e carrega na árvore
void processaCarga(avl *avl, char *nomeArquivo);

//****************Funções de balancemento******************//
//atualiza o valor de balancemento dos nós após a inserção de um nó
//condições de parada: após o ajuste do FB => chegar no nó raiz ou o nó ficar com fb 0, 2 ou -2
void atualizaFB_Insercao(avl *avl, no *novoNo);

//atualiza o valor de balancemento dos nós após a inserção de um nó
//condições de parada: após o ajuste do FB => chegar no nó raiz ou o nó ficar com fb 1, -1, 2 ou -2
void atualizaFB_Remocao(avl *avl, no *paidoNo, int chave);

//verifica a estratégia de balancemento do nó e ajusta o fator de balanceamento
void balancemento(avl *avl, no *noDesbalanceado);

//rotação à esquerda no nó desbalanceado
void rotacaoEsq(no *noDesbalanceado);

//rotação à direita no nó desbalanceado
void rotacaoDir(no *noDesbalanceado);


#endif //ARVOREBINARIA_ARVOREBINARIA_H
