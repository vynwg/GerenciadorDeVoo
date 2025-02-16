#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>


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


struct pass {
    struct pass *prox;
    char nome[30];
};

struct pqueue {
    struct pass *front;
    struct pass *rear;
};
struct pqueue *passageiros;


void printMenuVoo(char voo[8]) {
    printf("\n");
    printf("============= OPÇÕES DO VOO \"%s\" =============\n", voo);
    printf("1 - Cadastrar nomes\n");
    printf("2 - Remover nomes\n");
    printf("3 - Manifesto de passageiros\n\n");
    printf("0 - Voltar\n");
    printf("\n");
}

struct tree *insert(struct tree *root, struct tree *leaf, char nome[30]) {
    if (!leaf) {
        leaf = (struct tree*)malloc(sizeof(struct tree));
        leaf->left = leaf->right = NULL;
        strcpy(leaf->nome, nome);
        if (!root) return leaf;

        if (strcmp(root->nome, leaf->nome) < 0) {
            root->right = leaf;
        } else {
            root->left = leaf;
        }

        return leaf;
    }

    if (strcmp(leaf->nome, nome) < 0) {
        insert(leaf, leaf->right, nome);
    } else {
        insert(leaf, leaf->left, nome);
    }
}

void addPassageiro(struct no *voo) {
    char nome[30];

    printf("Entre com o nome do passageiro: ");
    scanf("%s", nome);
    getchar();

    voo->np++;

    if (!voo->pl)  {
        voo->pl = insert(voo->pl, voo->pl, nome);
    } else {
        insert(voo->pl, voo->pl, nome);
    }
}

struct tree *removet(struct tree *root, char nome[30], int *status) {
    struct tree *tmp, *tmp2;

    if (strcmp(root->nome, nome) == 0) {
        *status = 1;

        if (!root->left && !root->right) {
            free(root);
            return NULL;
        }

        if (!root->right) tmp = root->left;
        if (!root->left) tmp = root->right;
        if (!!root->left && !!root->right) {
            tmp = tmp2 = root->right;
            for (; !!tmp2->left; tmp2 = tmp2->left);
            tmp2->left = root->left;
        }

        free(root);
        return tmp;
    }

    if (strcmp(root->nome, nome) < 0) {
        if (!!root->right)
            root->right = removet(root->right, nome, status);
    } else {
        if (!!root->left)
            root->left = removet(root->left, nome, status);
    }

    return root;
}

void remPassageiro(struct no *voo) {
    char nome[30];
    int status = 0;
    struct tree *tmp;

    if (!voo->pl) {
        printf("A lista de passageiros está vazia.\n");
        return;
    }

    printf("Entre com o nome do passageiro: ");
    scanf("%s", nome);
    getchar();

    voo->pl = removet(voo->pl, nome, &status);

    if (!status) {
        printf("Passageiro \"%s\" não encontrado!\n", nome);
        return;
    }

    printf("Passageiro \"%s\" removido com sucesso.\n", nome);
    voo->np--;
}

void enqueuePassageiro(char nome[30]) {
    struct pass *passageiro = (struct pass*)malloc(sizeof(struct pass));
    strcpy(passageiro->nome, nome);
    passageiro->prox = NULL;

    if (!!passageiros->rear) {
        passageiros->rear->prox = passageiro;
        passageiros->rear = passageiro;
    } else {
        passageiros->front = passageiros->rear = passageiro;
    }
}

void dequeuePassageiro(char **nome) {
    struct pass *tmp2 = passageiros->front;
    
    if (!!tmp2) {
        strcpy(*nome, tmp2->nome);
        passageiros->front = tmp2->prox;
        free(tmp2);
    } else {
        passageiros->front = passageiros->rear = NULL;
        *nome = "\0";
    }
}

void emOrdem(struct tree *pl) {
    if (!pl) return;

    emOrdem(pl->left);
    printf("%s ", pl->nome);
    enqueuePassageiro(pl->nome);
    emOrdem(pl->right);
}

void printPassageiros(struct no *voo) {
    char *nome = (char*)malloc(30*sizeof(char));
    int i;
    char exp = 'N';
    char filename[30];
    FILE *file;

    if (!voo->pl) {
        printf("Lista de passageiros vazia!\n");
        return;
    }

    printf("Lista de passageiros do voo \"%s\" em ordem alfabética: ", voo->id);
    emOrdem(voo->pl);
    printf("\n");
    printf("Deseja exportar o manifesto [s/N]? ");
    scanf("%c", &exp);

    if (exp == 's') {
        sprintf(filename, "%s.txt", voo->id);
        file = fopen(filename, "w+");
        fprintf(file, "ID do voo\t\tDestino\t\tEmpresa\t\tModelo\t\t\tN° Passageiros\n");
        fprintf(file, "%s\t\t%s\t\t%s\t\t%s\t\t%d\n\n", voo->id, voo->dest, voo->empresa, voo->model, voo->np);
        fprintf(file, "Passageiros:\n");
    }    


    for (i = 0;; i++) {
        dequeuePassageiro(&nome);
        if (nome[0] == '\0') break;

        if (exp != 's') continue;
        fprintf(file, "%d - %s\n", i+1, nome);
    }

    if (exp == 's') fclose(file);
}

int menuVoo(struct no *voo) {
    int op = 0;

    printMenuVoo(voo->id);
    scanf("%d", &op);
    getchar();

    switch(op) {
        case 1:
            addPassageiro(voo);
            break;
        case 2:
            remPassageiro(voo);
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

    if (!!voos->rear) {
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

    for (; !!tmp; tmp = tmp->prox) {
        if (i-- >= 0) {
            printVoo(tmp);
            if (i < 0) break;
        } else {
            printVoo(tmp);
        }
    }
}

void liberart(struct tree *root) {
    if (!!root) {
        liberart(root->right);
        liberart(root->left);
        free(root);
    }
}

void decolar() {
    struct no *tmp = voos->front;
    if (!!tmp) {
        printf("Decolagem autorizada do voo \"%s\"\n", tmp->id);
        voos->front = voos->front->prox;
        
        liberart(tmp->pl);

        if (!voos->front) voos->rear = NULL;
        free(tmp);
    } else {
        printf("Não há aviões na fila.");
    }
}

void printNVoos() {
    int i = 0;
    struct no *tmp = voos->front;

    for (; !!tmp; tmp = tmp->prox) i++;

    printf("Número de voos na fila: %i\n", i);
}

int menuPrincipal() {
    int op = 0;
    struct no *tmp = voos->front;
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

            for (; !!tmp; tmp = tmp->prox)
                if (strcmp(tmp->id, id) == 0) break;

            if (!tmp) {
                printf("Voo \"%s\" não encontrado.\n", id);
                break;
            }

            while (menuVoo(tmp) > 0);
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
    passageiros = (struct pqueue*)malloc(sizeof(struct pqueue));
    voos->front = voos->rear = NULL;
    passageiros->front = passageiros->rear = NULL;

    while(menuPrincipal() > 0);
}
