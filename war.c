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
#define TAM_NOME 30
#define TAM_COR 10

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
struct Territorio
{
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
};

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:

// --- Função para limpar o buffer de entrada ---
void limparBufferEntrada()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void cadastrarTerritorio(struct Territorio *mapa, int i)
{ // recebe o mapa e o índice do território a ser cadastrado
    printf("\n--- Cadastro Territorio %d ---\n\n", i + 1);

    printf("Digite o nome do Territorio: ");
    fgets(mapa[i].nome, TAM_NOME, stdin);
    mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0'; // remove o \n que fgets deixa

    printf("Digite a cor do Exercito: ");
    fgets(mapa[i].cor, TAM_COR, stdin);
    mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0'; // remove o \n que fgets deixa

    printf("Digite o numero de tropas: ");
    scanf("%d", &mapa[i].tropas);
    limparBufferEntrada(); // limpa o buffer depois do scanf
}

void listarTerritorios(struct Territorio *mapa, int total)
{ // recebe o mapa e o total de territórios cadastrados
    printf("\n===== MAPA =====\n");

    for (int i = 0; i < total; i++)
    { // percorre apenas os territórios cadastrados

        printf("\nTerritorio %d\n", i + 1);
        printf("Nome: %s\n", mapa[i].nome);
        printf("Cor do Exercito: %s\n", mapa[i].cor);
        printf("Tropas: %d\n", mapa[i].tropas);
    }
}

void atacar(struct Territorio *atacante, struct Territorio *defensor)
{

    int dadoAtaque = rand() % 6 + 1; // rola um dado de 6 lados para o atacante
    int dadoDefesa = rand() % 6 + 1; // rola um dado de 6 lados para o defensor

    printf("\n===== BATALHA =====\n");

    printf("Dado atacante: %d\n", dadoAtaque);
    printf("Dado defensor: %d\n", dadoDefesa);

    if (dadoAtaque >= dadoDefesa)
    { // atacante vence ou empata
        printf("Atacante vence a batalha!\n");
        defensor->tropas--; // defensor perde uma tropa

        if (defensor->tropas <= 0)
        { // se o defensor ficar sem tropas, o atacante conquista o território
            printf("Territorio conquistado!\n");
            strcpy(defensor->cor, atacante->cor); // defensor agora tem a cor do atacante
            defensor->tropas = 1;                 // move uma tropa para o território conquistado
            atacante->tropas--;                   // atacante perde uma tropa para mover
        }
    }
    else
    { // defensor vence
        printf("Defensor vence a batalha!\n");
        atacante->tropas--; // atacante perde uma tropa
    }
}

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main()
{
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.

    srand(time(NULL)); // Inicializa a semente para números aleatórios

    int totalTerritorios; // número de territórios pré-definidos para cadastro
    printf("Quantos territorios deseja cadastrar? ");
    scanf("%d", &totalTerritorios);
    limparBufferEntrada();

    int atacante, defensor; // variáveis para armazenar os índices dos territórios de ataque e defesa
    int opcao;              // variável para armazenar a escolha do jogador no menu

    struct Territorio *mapa; // ponteiro para o mapa do mundo, que será alocado dinamicamente

    mapa = calloc(totalTerritorios, sizeof(struct Territorio)); // aloca memória para o mapa

    if (mapa == NULL)
    { // verifica se a alocação foi bem-sucedida
        printf("Erro de memoria\n");
        return 1;
    }

    for (int i = 0; i < totalTerritorios; i++)
    {
        cadastrarTerritorio(mapa, i);
    }
    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.

    do
    {

        printf("==========================\n");
        printf("      WAR - PARTE 2\n");
        printf("==========================\n");
        printf("1 - Ver mapa\n");
        printf("2 - Atacar\n");
        printf("0 - Sair\n");

        scanf("%d", &opcao);
        limparBufferEntrada(); // Limpa o buffer para evitar problemas com scanf e getchar

        switch (opcao)
        {

        case 1:

            listarTerritorios(mapa, totalTerritorios); // exibe apenas os territórios cadastrados
            break;

        case 2:

            listarTerritorios(mapa, totalTerritorios); // exibe os territórios para o jogador escolher
            printf("\nEscolha territorio atacante (1 - %d): ", totalTerritorios);
            scanf("%d", &atacante);

            printf("Escolha territorio defensor (1 - %d): ", totalTerritorios);
            scanf("%d", &defensor);
            limparBufferEntrada(); // Limpa o buffer depois dos scanf

            atacante--; // ajusta para índice 0
            defensor--; // ajusta para índice 0

            if (atacante < 0 || atacante >= totalTerritorios || defensor < 0 || defensor >= totalTerritorios)
            {
                printf("Territorio invalido.\n");
                break;
            }

            if (strcmp(mapa[atacante].cor, mapa[defensor].cor) == 0)
            {
                printf("Voce nao pode atacar um territorio da mesma cor.\n");
                break;
            }

            atacar(&mapa[atacante], &mapa[defensor]);  // chama a função de ataque passando os territórios selecionados
            listarTerritorios(mapa, totalTerritorios); // exibe o mapa atualizado após o ataque
            break;
        }
    } while (opcao != 0);

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.
    free(mapa); // libera a memória alocada para o mapa
    return 0;
}

// --- Implementação das Funções ---

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
