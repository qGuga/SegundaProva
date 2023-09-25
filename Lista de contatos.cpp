#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 11 // Tamanho da tabela de hash (deve ser um número primo)

// Estrutura para representar um contato
struct Contato {
    char nome[50];
    char telefone[15];
};

// Estrutura para representar um item na tabela hash
struct HashItem {
    struct Contato contato;
    int ocupado; // Flag para indicar se a posição está ocupada
};

// Função para calcular o hash de uma string (nome do contato)
unsigned int hash(char *str) {
    unsigned int hash = 0;
    while (*str) {
        hash = (hash * 31) + (*str++); // Algoritmo de hashing simples
    }
    return hash % TABLE_SIZE; // Retorna o índice na tabela hash
}

// Função para inserir um contato na tabela hash usando sondagem quadrática
void inserir(struct HashItem tabela[], struct Contato contato) {
    unsigned int chave = hash(contato.nome); // Calcula o hash do nome do contato
    int tentativa = 0;

    while (tabela[chave].ocupado) { // Trata colisões
        // Quadratic probing: tenta a próxima posição quadrática
        chave = (chave + tentativa * tentativa) % TABLE_SIZE;
        tentativa++;

        if (tentativa >= TABLE_SIZE) { // Verifica se a tabela está cheia
            printf("Tabela de hash esta cheia. Nao é possível inserir o contato.\n");
            return;
        }
    }

    tabela[chave].contato = contato; // Insere o contato na tabela
    tabela[chave].ocupado = 1; // Marca a posição como ocupada
    printf("Contato '%s' inserido na posicao %d da tabela.\n", contato.nome, chave);
}

// Função para buscar um contato na tabela hash
void buscar(struct HashItem tabela[], char *nome) {
    unsigned int chave = hash(nome); // Calcula o hash do nome a ser buscado
    int tentativa = 0;

    while (tabela[chave].ocupado) { // Trata colisões
        if (strcmp(tabela[chave].contato.nome, nome) == 0) { // Compara os nomes
            printf("Contato encontrado na posicao %d:\n", chave);
            printf("Nome: %s\nTelefone: %s\n", tabela[chave].contato.nome, tabela[chave].contato.telefone);
            return;
        }

        // Quadratic probing: tenta a próxima posição quadrática
        chave = (chave + tentativa * tentativa) % TABLE_SIZE;
        tentativa++;

        if (tentativa >= TABLE_SIZE) { // Verifica se a tabela está cheia
            printf("Contato nao encontrado.\n");
            return;
        }
    }

    printf("Contato nao encontrado.\n");
}

// Função para listar todos os contatos
void listar(struct HashItem tabela[]) {
    printf("Lista de contatos:\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (tabela[i].ocupado) {
            printf("Posição %d: Nome: %s, Telefone: %s\n", i, tabela[i].contato.nome, tabela[i].contato.telefone);
        }
    }
}

// Função para exportar contatos para um arquivo TXT
void exportarContatos(struct HashItem tabela[], char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "w"); // Abre o arquivo para escrita

    if (arquivo == NULL) { // Verifica se houve erro ao abrir o arquivo
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        if (tabela[i].ocupado) {
            fprintf(arquivo, "Nome: %s, Telefone: %s\n", tabela[i].contato.nome, tabela[i].contato.telefone);
        }
    }

    fclose(arquivo); // Fecha o arquivo após a escrita
    printf("Contatos exportados para %s com sucesso.\n", nomeArquivo);
}

int main() {
    struct HashItem tabela[TABLE_SIZE] = {0}; // Inicializa a tabela hash

    int opcao;
    char nomeBusca[50];
    char nomeArquivo[50];
    struct Contato novoContato;

    do {
        printf("\nMenu:\n");
        printf("1. Inserir Contato\n");
        printf("2. Listar Contatos\n");
        printf("3. Buscar Contato\n");
        printf("4. Exportar Contatos para Arquivo TXT\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite o nome do contato: ");
                scanf("%s", novoContato.nome);
                printf("Digite o telefone do contato: ");
                scanf("%s", novoContato.telefone);
                inserir(tabela, novoContato);
                break;
            case 2:
                listar(tabela);
                break;
            case 3:
                printf("Digite o nome do contato a ser buscado: ");
                scanf("%s", nomeBusca);
                buscar(tabela, nomeBusca);
                break;
            case 4:
                printf("Digite o nome do arquivo para exportar contatos: ");
                scanf("%s", nomeArquivo);
                exportarContatos(tabela, nomeArquivo);
                break;
            case 5:
                printf("Saindo do programa...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 5);

    return 0;
}

