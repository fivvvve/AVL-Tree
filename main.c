#include <stdio.h>
#include <stdlib.h>
#include "arvorebinaria.h"

int main(){
    avl *arv;
    int opt = 0, ret, valor;
    char nomeArquivo[31];
    arv = criaArvore()

    do{
        printf("Digite a opção que desejar:\n");
        printf("1 - Carregar arquivo\n");
        printf("2 - Inserir nó\n");
        printf("3 - Remover nó\n");
        printf("4 - Percorrer árvore\n");
        printf("5 - Sair\n");
        scanf("%d", &opt);

        system("clear || cls");

        switch (opt) {

        case 1:
            printf("Digite o nome do arquivo que deseja utilizar: ");
            scanf(" %[^\n]", nomeArquivo);
            processaCarga(arv, nomeArquivo);
            printf("Árvore carregada\n");
            break;    
        
        case 2:
            printf("Digite o valor do nó que deseja inserir: ");
            scanf("%d", &valor);
            ret = insereNo(arv, valor);
            if(ret == 1) printf("Nó inserido\n");
            else printf("A árvore ainda não foi criada\n");
            break;

        case 3:
            printf("Digite o valor do nó que deseja remover: ");
            scanf("%d", &valor);
            ret = removeNo(arv, valor);
            if(ret == 1) printf("Nó removido\n");
            else if(ret==0) printf("Não há elementos na árvore para serem removidos\n");
            else printf("Não há nó com esse valor na árvore\n");
            break;

        case 4:
            printf("Valores da árvore em pré-ordem:\n");
            imprimePreOrdem(getRaiz(arv));
            printf("\n");
            break;
        
        case 5:
            printf("Saindo...");
            break;

        default:
            printf("Opção inválida\n");
            break;
        }

        printf("\n");
    } while (opt != 5);

    free(arv);
    
}