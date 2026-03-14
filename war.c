// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define MAX_TERRITORIOS 5
#define TAM_NOME 30
#define TAM_COR 10

#define MISSAO_DESTRUIR_EXERCITO 1
#define MISSAO_CONQUISTAR_TERRITORIOS 2

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
struct Territorio {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
};

struct Missao {
    int tipo; // tipo da missão (destruir exército ou conquistar territórios).
    char alvo[TAM_COR]; // cor do exército a ser destruído (para missão de destruir) ou cor do exército a conquistar (para missão de conquistar).
};

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
void inicializarTerritorios(struct Territorio *mapa);
void sortearMissao(struct Missao *missao);

// Funções de interface com o usuário:
void exibirMenu();
void exibirMapa(const struct Territorio *mapa);

// Funções de lógica principal do jogo:
void atacar(struct Territorio *mapa);
int verificarMissao(const struct Territorio *mapa, struct Missao missao, const char *corJogador);

// Função utilitária:
void limparBufferEntrada();

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.

    srand(time(NULL)); // Inicializa a semente para números aleatórios

    struct Territorio *mapa = calloc(MAX_TERRITORIOS, sizeof(struct Territorio)); // aloca memória para o mapa
    if (mapa == NULL) { // verifica se a alocação foi bem-sucedida
        printf("Erro de memoria\n");
        return 1;
    }

    inicializarTerritorios(mapa); // Preenche os territórios com seus dados iniciais

    char corJogador[TAM_COR];

    printf("Digite a cor do seu exercito: ");
    fgets(corJogador, TAM_COR, stdin);
    corJogador[strcspn(corJogador, "\n")] = '\0'; // Remove o caractere de nova linha


    struct Missao missao;
    sortearMissao(&missao); // Sorteia a missão do jogador

    int opcao;

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.

    do {
        exibirMapa(mapa); // Exibe o mapa atualizado a cada rodada.
        exibirMenu(); // Exibe o menu de opções para o jogador.

        scanf("%d", &opcao);
        limparBufferEntrada(); // Limpa o buffer para evitar problemas com scanf e getchar

        switch(opcao) {
            case 1:
                atacar(mapa); // Gerencia a fase de ataque.
                break;

            case 2:
                if (verificarMissao(mapa, missao, corJogador)) {
                    printf("\n***** MISSAO CUMPRIDA! VOCE VENCEU! *****\n");
                    opcao = 0; // Encerra o jogo após a vitória.
                } else {
                    printf("\nMissao ainda nao concluida.\n");
                }

                printf("\nPressione Enter para continuar...");
                getchar(); // Aguarda o jogador pressionar Enter para continuar.
                break;

            case 0:
                printf("\nSaindo do jogo...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.
    free(mapa); // libera a memória alocada para o mapa
    return 0;
}

// --- Implementação das Funções ---

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // Limpa o buffer de entrada
}

void exibirMenu() { // Exibe o menu de opções para o jogador.
    printf("\n============ MENU ============\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar Missao\n");
    printf("0 - Sair\n");
    printf("==============================\n");
    printf("Escolha: ");
}

void inicializarTerritorios(struct Territorio *mapa) { // Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
    strcpy(mapa[0].nome, "Brasil");
    strcpy(mapa[0].cor, "Azul");
    mapa[0].tropas = 5;

    strcpy(mapa[1].nome, "Argentina");
    strcpy(mapa[1].cor, "Verde");
    mapa[1].tropas = 4;

    strcpy(mapa[2].nome, "Chile");
    strcpy(mapa[2].cor, "Vermelho");
    mapa[2].tropas = 3;

    strcpy(mapa[3].nome, "Peru");
    strcpy(mapa[3].cor, "Amarelo");
    mapa[3].tropas = 6;

    strcpy(mapa[4].nome, "Colombia");
    strcpy(mapa[4].cor, "Verde");
    mapa[4].tropas = 4;
}

void exibirMapa(const struct Territorio *mapa) { // Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
    printf("\n================= MAPA =================\n");

    for (int i = 0; i < MAX_TERRITORIOS; i++) {

        printf("\nTerritorio %d\n", i + 1);
        printf("Nome: %s\n", mapa[i].nome);
        printf("Cor do Exercito: %s\n", mapa[i].cor);
        printf("Numero de Tropas: %d\n", mapa[i].tropas);
    }
}

void atacar(struct Territorio *mapa) { // Gerencia a fase de ataque, solicitando ao jogador os territórios de origem e destino.
    int atacante;
    int defensor;

    printf("\nEscolha territorio atacante (1-5): ");
    scanf("%d", &atacante);

    printf("Escolha territorio defensor (1-5): ");
    scanf("%d", &defensor);

    limparBufferEntrada(); // Limpa o buffer para evitar problemas com scanf e getchar

    atacante--;
    defensor--;

    if (atacante < 0 || atacante >= MAX_TERRITORIOS || defensor < 0 || defensor >= MAX_TERRITORIOS) {

        printf("Territorio invalido!\n");
        return;
    }

    if (strcmp(mapa[atacante].cor, mapa[defensor].cor) == 0) {

        printf("Voce nao pode atacar um territorio da mesma cor!\n");
        return;
    }

    int dadoAtaque = rand() % 6 + 1;
    int dadoDefesa = rand() % 6 + 1;

    printf("\nDado de Ataque: %d\n", dadoAtaque); // Rola o dado para o atacante (1-6).
    printf("Dado de Defesa: %d\n", dadoDefesa); // Rola o dado para o defensor (1-6).

    if (dadoAtaque >= dadoDefesa) {
        printf("Atacante venceu!\n");
        mapa[defensor].tropas--; // Atacante vence, defensor perde uma tropa.

        if (mapa[defensor].tropas <= 0) {
            printf("Territorio conquistado!\n");
            strcpy(mapa[defensor].cor, mapa[atacante].cor); // Conquista o território, atualiza a cor.

            mapa[defensor].tropas = 1; // Move uma tropa para o território conquistado.
            mapa[atacante].tropas--; // Atacante perde uma tropa para mover.
        }
    } else {

        printf("Defensor venceu!\n");
        mapa[atacante].tropas--; // Defensor vence, atacante perde uma tropa.
    }
}

void sortearMissao(struct Missao *missao) {
    missao->tipo = rand() % 2 + 1; // Sorteia o tipo da missão (1 ou 2).

    if (missao->tipo == MISSAO_DESTRUIR_EXERCITO) {
        strcpy(missao->alvo, "Verde"); // Para missão de destruir, o alvo é a cor do exército a ser destruído.
        printf("Missao: Destruir o exercito %s\n", missao->alvo);
    } else {
        printf("Missao: Conquistar 3 territorios\n"); // Para missão de conquistar, o alvo é conquistar um número de territórios.
    }
}

int verificarMissao(const struct Territorio *mapa, struct Missao missao, const char *corJogador) {
    int contador = 0;

    if (missao.tipo == MISSAO_DESTRUIR_EXERCITO) {
        for (int i = 0; i < MAX_TERRITORIOS; i++) {
            if (strcmp(mapa[i].cor, missao.alvo) == 0) {
                return 0; // Missão de destruir não cumprida se ainda houver territórios do exército alvo.
            }            
        }
        return 1; // Missão de destruir cumprida se não houver mais territórios do exército alvo.
    }

    if (missao.tipo == MISSAO_CONQUISTAR_TERRITORIOS) {
        for (int i = 0; i < MAX_TERRITORIOS; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                contador++; // Conta quantos territórios o jogador possui.
            }
        }
        if (contador >= 3) {
            return 1; // Missão de conquistar cumprida se o jogador tiver 3 ou mais territórios.
        }
    }

    return 0; // Missão não cumprida.
}

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
