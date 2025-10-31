#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

/* Função para cadastrar territórios */
void cadastrarTerritorios(Territorio *mapa, int n) {
    for (int i = 0; i < n; i++) {
        printf("\n----- Território %d -----\n", i + 1);
        printf("Nome: ");
        scanf("%29s", mapa[i].nome);
        printf("Cor (exército): ");
        scanf("%9s", mapa[i].cor);
        printf("Número de tropas: ");
        scanf("%d", &mapa[i].tropas);
        if (mapa[i].tropas < 0) mapa[i].tropas = 0;
    }
}

/* Exibe os territórios */
void exibirTerritorios(Territorio *mapa, int n) {
    printf("\n=== Dados dos Territórios ===\n");
    for (int i = 0; i < n; i++) {
        printf("[%d] Nome: %s | Cor: %s | Tropas: %d\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

/* Simulação de ataque */
void atacar(Territorio *atacante, Territorio *defensor) {
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("\n⚠️  Você não pode atacar um território da mesma cor!\n");
        return;
    }

    if (atacante->tropas < 1) {
        printf("\n⚠️  O território atacante não tem tropas suficientes para atacar!\n");
        return;
    }

    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n🎲 Dado do atacante: %d\n🎲 Dado do defensor: %d\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        defensor->tropas -= 1;
        printf("✅ O atacante venceu a rodada. '%s' perdeu 1 tropa.\n", defensor->nome);

        if (defensor->tropas <= 0) {
            strcpy(defensor->cor, atacante->cor);
            defensor->tropas = 1;
            printf("🏳️ '%s' foi conquistado! Agora pertence a '%s' (cor: %s) com 1 tropa.\n",
                   defensor->nome, atacante->nome, defensor->cor);
        }
    } else {
        atacante->tropas -= 1;
        printf("❌ O defensor resistiu. '%s' perdeu 1 tropa.\n", atacante->nome);

        if (atacante->tropas <= 0) {
            strcpy(atacante->cor, defensor->cor);
            atacante->tropas = 1;
            printf("🏳️ '%s' foi abatido! Agora pertence a '%s' (cor: %s) com 1 tropa.\n",
                   atacante->nome, defensor->nome, atacante->cor);
        }
    }
}

/* Função para atribuir missão aleatória */
void atribuirMissao(char **destino, char *missoes[], int totalMissoes, Territorio *mapa, int n) {
    int indiceMissao = rand() % totalMissoes;
    int indiceTerritorio = rand() % n;

    // Aloca memória para a string da missão
    *destino = (char *) malloc(200 * sizeof(char));
    if (*destino == NULL) {
        printf("Erro ao alocar memória para missão.\n");
        exit(1);
    }

    // Copia a missão sorteada
    strcpy(*destino, missoes[indiceMissao]);

    printf("\n🎯 A missão foi atribuída ao território '%s'!\n", mapa[indiceTerritorio].nome);
    printf("📝 Missão: %s\n", *destino);
}

/* Verifica se a missão foi cumprida (simples) */
int verificarMissao(char *missao, Territorio *mapa, int n) {
    if (strstr(missao, "dominar todos os territórios da cor vermelha")) {
        int todosVermelhos = 1;
        for (int i = 0; i < n; i++) {
            if (strcmp(mapa[i].cor, "vermelho") != 0) {
                todosVermelhos = 0;
                break;
            }
        }
        return todosVermelhos;
    } 
    else if (strstr(missao, "eliminar todos os territórios azuis")) {
        int existeAzul = 0;
        for (int i = 0; i < n; i++) {
            if (strcmp(mapa[i].cor, "azul") == 0) {
                existeAzul = 1;
                break;
            }
        }
        return !existeAzul;
    }
    else if (strstr(missao, "controlar pelo menos 3 territórios")) {
        char corRef[10];
        strcpy(corRef, mapa[0].cor);
        int count = 0;
        for (int i = 0; i < n; i++) {
            if (strcmp(mapa[i].cor, corRef) == 0)
                count++;
        }
        return count >= 3;
    }

    return 0; // caso não se aplique nenhuma verificação
}

/* Libera memória */
void liberarMemoria(Territorio *mapa, char *missao) {
    free(mapa);
    free(missao);
    printf("\nMemória liberada.\n");
}

/* Programa principal */
int main() {
    srand((unsigned int) time(NULL));

    int n;
    printf("Digite o número de territórios: ");
    scanf("%d", &n);

    Territorio *mapa = (Territorio *) calloc((size_t)n, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    cadastrarTerritorios(mapa, n);
    exibirTerritorios(mapa, n);

    // Criação das missões
    char *missoes[] = {
        "dominar todos os territórios da cor vermelha",
        "eliminar todos os territórios azuis",
        "controlar pelo menos 3 territórios de uma mesma cor",
        "fazer com que nenhum território tenha menos de 2 tropas",
        "conquistar um território inimigo em batalha"
    };

    char *missao = NULL;
    atribuirMissao(&missao, missoes, 5, mapa, n);

    // Loop principal do jogo
    while (1) {
        int a, d;
        printf("\nDigite o número do território atacante (0 para sair): ");
        scanf("%d", &a);
        if (a == 0) {
            printf("Jogo finalizado pelo usuário.\n");
            break;
        }

        printf("Digite o número do território defensor (0 para cancelar ataque): ");
        scanf("%d", &d);
        if (d == 0) continue;

        if (a < 1 || a > n || d < 1 || d > n || a == d) {
            printf("Entrada inválida.\n");
            continue;
        }

        atacar(&mapa[a - 1], &mapa[d - 1]);
        exibirTerritorios(mapa, n);

        // Verifica vitória
        if (verificarMissao(missao, mapa, n)) {
            printf("\n🎉 Missão cumprida! Vitória alcançada!\n");
            break;
        }
    }

    liberarMemoria(mapa, missao);
    return 0;
}
