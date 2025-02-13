#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

struct tree {
    char nome[30];
    struct tree *left;
    struct tree *right;
};

struct no {
    struct no *prox;
    char id[8];
    char dest[4];
    char empresa[20];
    char model[20];
    int np;
    struct tree *pl;
};

struct queue {
    struct no *front;
    struct no *rear;
};
struct queue *voos;


void printMenuVoo(char voo[8]) {
    printf("\n");
    printf("============= OPÇÕES DO VOO %s =============\n", voo);
    printf("1 - Cadastrar nomes\n");
    printf("2 - Remover nomes\n");
    printf("3 - Exportar manifesto de passageiros\n\n");
    printf("0 - Voltar\n");
    printf("\n");
}

int addPassageiro(char voo[8]) {
    return 0;
}

int remPassageiro(char voo[8]) {
    return 0;
}

void printPassageiros(char voo[8]) {

}

int menuVoo(char voo[8]) {
    int op = 0;

    printMenuVoo(voo);
    scanf("> %d", &op);
    getchar();

    switch(op) {
        case 1:
            while (addPassageiro(voo) > 0);
            break;
        case 2:
            while (remPassageiro(voo) > 0);
            break;
        case 3:
            printPassageiros(voo);
            break;
        default:
            break;
    }

    return op;
}

void printMenuPrincipal() {
    printf("\n");
    printf("===================== OPÇÕES =====================\n");
    printf("1 - Cadastrar voo\n");
    printf("2 - Acessar voo\n");
    printf("3 - Informações do primeiro voo\n");
    printf("4 - Informações de todos os voos\n");
    printf("5 - Autorizar decolagem do primeiro avião da fila\n");
    printf("6 - Listar quantidade de aviões na fila\n\n");
    printf("0 - Sair do programa\n");
    printf("\n");
}

void cadastroVoo() {
    struct no *voo = (struct no*)malloc(sizeof(struct no));
    voo->np = 0;
    voo->pl = NULL;
    voo->prox = NULL;

    printf("Entre com o id do voo: ");
    scanf("%s", voo->id);
    getchar();

    printf("\n");
    printf("Entre com o aeroporto destino: ");
    scanf("%s", voo->dest);
    getchar();

    printf("\n");
    printf("Entre com a empressa aérea: ");
    scanf("%s", voo->empresa);
    getchar();

    printf("\n");
    printf("Entre com o modelo do avião: ");
    scanf("%s", voo->model);
    getchar();

    if (voos->rear != NULL) {
        voos->rear->prox = voo;
        voos->rear = voo;
    } else {
        voos->front = voos->rear = voo;
    }
}

void printVoo(struct no *voo) {
    printf("%s\t\t%s\t\t%s\t\t%s\t\t%d\n", voo->id, voo->dest, voo->empresa, voo->model, voo->np);
}

void printVoos(int i) {
    struct no *tmp = voos->front;
    printf("ID do voo\t\tDestino\t\tEmpresa\t\tModelo\t\t\tN° Passageiros\n");

    for (; tmp != NULL; tmp = tmp->prox) {
        if (i-- >= 0) {
            printVoo(tmp);
            if (i < 0) break;
        } else {
            printVoo(tmp);
        }
    }
}

void decolar() {
    struct no *tmp = voos->front;
    if (tmp != NULL) {
        printf("Decolagem autorizada do voo %s\n", tmp->id);
        voos->front = voos->front->prox;
        free(tmp);
    } else {
        printf("Não há aviões na fila.");
    }
}

void printNVoos() {
    int i = 0;
    struct no *tmp = voos->front;

    for (; tmp != NULL; tmp = tmp->prox) i++;

    printf("Número de voos na fila: %i\n", i);
}

int menuPrincipal() {
    int op = 0;
    char id[8];

    printMenuPrincipal();
    scanf("%d", &op);
    getchar();

    switch(op) {
        case 1:
            cadastroVoo();
            break;
        case 2:
            printf("Entre com o id do voo: ");
            scanf("%s", id);
            getchar();
            while (menuVoo(id) > 0);
            break;
        case 3:
            printVoos(0);
            break;
        case 4:
            printVoos(-1);
            break;
        case 5:
            decolar();
            break;
        case 6:
            printNVoos();
            break;
        default:
            break;
    }

    return op;
}

void main() {
    int op = 1;
    setlocale(LC_ALL, "");

    voos = (struct queue*)malloc(sizeof(struct queue));
    voos->front = voos->rear = NULL;

    while(menuPrincipal() > 0);
}
