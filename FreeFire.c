#include <stdio.h>    // Para funções básicas de entrada e saída (como printf e scanf)
#include <stdlib.h>   // Para funções de utilidade geral
#include <string.h>   // Para manipulação de strings (como strcmp)
#include <stdbool.h>  // Para usar o tipo de dado booleano (true/false)

// Define a "ficha" de cada peça.
typedef struct {
    char nome[30];      // Nome da peça (ex: "Chip Central")
    char tipo[20];      // Categoria da peça (ex: "Controle")
    int prioridade;     // Importância da peça (número de 1 a 10)
} Componente;

// Define o número máximo de peças que podemos ter
#define MAX_COMPONENTES 20
// Variável que guarda quantas vezes o computador comparou duas peças (para análise)
long long comparacoes = 0; 

// ----------------------------------------------------
// Funções Auxiliares (Ajudantes)
// ----------------------------------------------------

// Função que troca a posição de duas peças no vetor
void trocar(Componente *a, Componente *b) {
    Componente temp = *a;
    *a = *b;
    *b = temp;
}

// Limpa qualquer lixo que tenha ficado na memória após ler um número
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função que lê um texto do usuário de forma mais segura (evita erros grandes)
void lerStringSegura(char *str, int tamanho) {
    if (fgets(str, tamanho, stdin) != NULL) {
        size_t len = strlen(str);
        if (len > 0 && str[len - 1] == '\n') {
            str[len - 1] = '\0'; // Remove o ENTER (nova linha)
        } else {
            limparBuffer(); // Se a linha for muito longa, limpa o resto
        }
    }
}

// Função que mostra todas as peças cadastradas em um formato organizado (tabela)
void mostrarComponentes(Componente vetor[], int n) {
    if (n == 0) {
        printf("\n[INFO] Nenhuma peça cadastrada para mostrar.\n");
        return;
    }
    printf("\n--- Lista de Componentes (%d) ---\n", n);
    printf("----------------------------------------------------\n");
    printf("| %-28s | %-18s | %-10s |\n", "NOME", "TIPO", "PRIORIDADE");
    printf("----------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("| %-28s | %-18s | %-10d |\n", 
               vetor[i].nome, vetor[i].tipo, vetor[i].prioridade);
    }
    printf("----------------------------------------------------\n");
}

// ----------------------------------------------------
// Função Principal de Entrada de Dados (Cadastro)
// ----------------------------------------------------

int cadastrarComponentes(Componente vetor[]) {
    int n = 0;
    char resposta[5];

    printf("\n### Cadastro de Componentes (Máx: %d) ###\n", MAX_COMPONENTES);

    while (n < MAX_COMPONENTES) {
        printf("\n--- Peça #%d ---\n", n + 1);

        printf("Nome da peça: ");
        lerStringSegura(vetor[n].nome, sizeof(vetor[n].nome));

        printf("Tipo da peça: ");
        lerStringSegura(vetor[n].tipo, sizeof(vetor[n].tipo));
        
        // Pede a prioridade e garante que seja um número entre 1 e 10
        do {
            printf("Prioridade (1 a 10): ");
            if (scanf("%d", &vetor[n].prioridade) != 1) {
                printf("[ERRO] Por favor, digite um número inteiro.\n");
                limparBuffer();
            } else if (vetor[n].prioridade < 1 || vetor[n].prioridade > 10) {
                printf("[ERRO] A prioridade deve ser entre 1 e 10.\n");
            }
        } while (vetor[n].prioridade < 1 || vetor[n].prioridade > 10);
        
        limparBuffer(); // Limpa a memória após ler o número

        n++;

        if (n < MAX_COMPONENTES) {
            printf("\nAdicionar outra peça? (s/n): ");
            lerStringSegura(resposta, sizeof(resposta));
            // Se a resposta não for 's' ou 'S', para o cadastro
            if (strcmp(resposta, "s") != 0 && strcmp(resposta, "S") != 0) {
                break;
            }
        }
    }
    printf("\n[SUCESSO] %d peças cadastradas.\n", n);
    return n;
}

// ----------------------------------------------------
// Funções de Ordenação (Estratégias de Montagem)
// ----------------------------------------------------

// 1. Ordena o vetor pelo NOME da peça usando Bubble Sort (Método da Bolha)
void bubbleSortNome(Componente vetor[], int n) {
    comparacoes = 0; // Zera o contador

    // O loop principal passa por toda a lista
    for (int i = 0; i < n - 1; i++) {
        // O loop interno move a "peça mais pesada" (maior nome) para o final
        for (int j = 0; j < n - 1 - i; j++) {
            comparacoes++; // Conta uma comparação
            // Se o nome atual for maior que o próximo (em ordem alfabética), troca
            if (strcmp(vetor[j].nome, vetor[j + 1].nome) > 0) {
                trocar(&vetor[j], &vetor[j + 1]);
            }
        }
    }

    printf("\n--- Análise (Bubble Sort - Nome) ---\n");
    printf("Total de Comparações: %lld\n", comparacoes);
}

// 2. Ordena o vetor pelo TIPO da peça usando Insertion Sort (Método da Inserção)
void insertionSortTipo(Componente vetor[], int n) {
    comparacoes = 0; // Zera o contador

    // Começa na segunda peça (índice 1)
    for (int i = 1; i < n; i++) {
        Componente chave = vetor[i]; // Pega a peça atual para ser inserida
        int j = i - 1;

        // Move as peças maiores para frente, abrindo espaço para a "chave"
        while (j >= 0) {
            comparacoes++; // Conta uma comparação
            // Se o tipo da peça anterior for maior que o tipo da chave, move a peça
            if (strcmp(vetor[j].tipo, chave.tipo) > 0) {
                 vetor[j + 1] = vetor[j];
                 j = j - 1;
            } else {
                break; // Achou o lugar certo
            }
        }
        vetor[j + 1] = chave; // Coloca a chave no lugar correto
    }

    printf("\n--- Análise (Insertion Sort - Tipo) ---\n");
    printf("Total de Comparações: %lld\n", comparacoes);
}

// 3. Ordena o vetor pela PRIORIDADE da peça usando Selection Sort (Método da Seleção)
void selectionSortPrioridade(Componente vetor[], int n) {
    comparacoes = 0; // Zera o contador

    // O loop externo percorre o vetor para encontrar o próximo menor elemento
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i; // Assume que o atual é o de menor prioridade

        // O loop interno procura a peça com a menor prioridade no restante da lista
        for (int j = i + 1; j < n; j++) {
            comparacoes++; // Conta uma comparação
            if (vetor[j].prioridade < vetor[min_idx].prioridade) {
                min_idx = j; // Encontrou um com prioridade menor
            }
        }
        // Se a peça de menor prioridade não era a atual, troca
        if (min_idx != i) {
            trocar(&vetor[i], &vetor[min_idx]);
        }
    }

    printf("\n--- Análise (Selection Sort - Prioridade) ---\n");
    printf("Total de Comparações: %lld\n", comparacoes);
}

// ----------------------------------------------------
// Função de Busca (Para Achar a Peça Chave)
// ----------------------------------------------------

// Procura uma peça pelo Nome usando Busca Binária (Super Rápida, mas precisa de ordem)
int buscaBinariaPorNome(Componente vetor[], int n, char nomeChave[]) {
    int esquerda = 0;
    int direita = n - 1;
    int resultado = -1;
    int comp_busca = 0;

    // Enquanto a área de busca não se fechar
    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2; // Calcula o meio
        comp_busca++; // Conta a comparação do meio

        int comparacao = strcmp(vetor[meio].nome, nomeChave); // Compara o meio com a chave

        if (comparacao == 0) {
            resultado = meio; // Achou a peça!
            break;
        } else if (comparacao < 0) {
            esquerda = meio + 1; // A chave está na metade direita
        } else {
            direita = meio - 1; // A chave está na metade esquerda
        }
    }

    printf("\n[INFO] Comparações na Busca Binária: %d\n", comp_busca);
    return resultado;
}

// ----------------------------------------------------
// Função Principal (main)
// ----------------------------------------------------

int main() {
    Componente torre[MAX_COMPONENTES];
    int numComponentes = 0;
    int opcao;
    // Variável que diz se a lista está organizada por NOME (precisa para a busca binária)
    bool ordenadoPorNome = false; 

    do {
        printf("\n\n#############################################\n");
        printf("### MÓDULO DE ORGANIZAÇÃO DOS COMPONENTES ###\n");
        printf("#############################################\n");
        printf("1. Cadastrar Componentes (Montar a lista de peças)\n");
        printf("2. Ordenar por NOME & Buscar Peça Chave (Estratégia Bubble)\n");
        printf("3. Ordenar por TIPO (Estratégia Insertion)\n");
        printf("4. Ordenar por PRIORIDADE (Estratégia Selection)\n");
        printf("5. Visualizar Peças Atuais\n");
        printf("0. Sair do Programa\n");
        printf("---------------------------------------------\n");
        printf("Escolha uma opção: ");

        if (scanf("%d", &opcao) != 1) {
            opcao = -1; 
            limparBuffer();
        } else {
            limparBuffer(); 
        }

        switch (opcao) {
            case 1:
                numComponentes = cadastrarComponentes(torre);
                ordenadoPorNome = false; 
                break;
            case 2:
                if (numComponentes == 0) {
                    printf("[ALERTA] Cadastre as peças primeiro (Opção 1).\n");
                    break;
                }
                bubbleSortNome(torre, numComponentes);
                mostrarComponentes(torre, numComponentes);
                ordenadoPorNome = true; // Sim, agora a lista está ordenada por nome

                char chaveBusca[30];
                printf("\n-- BUSCA BINÁRIA --\n");
                printf("Digite o NOME exato da peça-chave para ativar a torre: ");
                lerStringSegura(chaveBusca, sizeof(chaveBusca));

                int indice = buscaBinariaPorNome(torre, numComponentes, chaveBusca);

                if (indice != -1) {
                    printf("\n[ATIVAÇÃO SUCESSO] Peça '%s' encontrada! Montagem pode começar.\n",
                           torre[indice].nome);
                    printf(">>> Torre de Resgate PRONTA! <<<\n");
                } else {
                    printf("\n[FALHA] Peça-chave '%s' NÃO encontrada na lista.\n", chaveBusca);
                }
                break;
            case 3:
                if (numComponentes == 0) {
                    printf("[ALERTA] Cadastre as peças primeiro (Opção 1).\n");
                    break;
                }
                insertionSortTipo(torre, numComponentes);
                mostrarComponentes(torre, numComponentes);
                ordenadoPorNome = false; // A ordenação por nome foi desfeita
                break;
            case 4:
                if (numComponentes == 0) {
                    printf("[ALERTA] Cadastre as peças primeiro (Opção 1).\n");
                    break;
                }
                selectionSortPrioridade(torre, numComponentes);
                mostrarComponentes(torre, numComponentes);
                ordenadoPorNome = false; // A ordenação por nome foi desfeita
                break;
            case 5:
                mostrarComponentes(torre, numComponentes);
                break;
            case 0:
                printf("\nEncerrando o sistema. Tchau!\n");
                break;
            default:
                printf("\n[ERRO] Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}