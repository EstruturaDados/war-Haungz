#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

/* Cadastra territórios no vetor (uso de ponteiros) */
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

/* Exibe o estado atual dos territórios */
void exibirTerritorios(Territorio *mapa, int n) {
    printf("\n=== Dados dos Territórios ===\n");
    for (int i = 0; i < n; i++) {
        printf("[%d] Nome: %s | Cor: %s | Tropas: %d\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

/*
      Regra de ataque:
  - Ambos os lados rolam 1..6.
  - Quem tirar menor perde 1 tropa.
  - Empate: atacante perde 1 tropa.
  - Se um território ficar com 0 tropas (tinha 1 e perdeu),
    ele passa a pertencer (cor) ao vencedor e recebe 1 tropa.
*/
void atacar(Territorio *atacante, Territorio *defensor) {
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("\n⚠️  Você não pode atacar um território da mesma cor!\n");
        return;
    }

    if (atacante->tropas < 1) {
        printf("\n⚠️  O território atacante não tem tropas suficientes para atacar!\n");
        return;
    }

    // Rola dados
    int dadoAtacante = (rand() % 6) + 1; // 1 a 6
    int dadoDefensor = (rand() % 6) + 1; // 1 a 6

    printf("\n🎲 Dado do atacante: %d\n🎲 Dado do defensor: %d\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        // defensor perde 1 tropa
        defensor->tropas -= 1;
        printf("✅ O atacante venceu a rodada. '%s' perdeu 1 tropa.\n", defensor->nome);

        if (defensor->tropas <= 0) {
            // território foi abatido — troca de cor para o vencedor
            strcpy(defensor->cor, atacante->cor);
            // o território conquistado recebe 1 tropa (pra evitar 0 tropas)
            defensor->tropas = 1;
            printf("🏳️ '%s' foi conquistado! Agora pertence a '%s' (cor: %s) com 1 tropa.\n",
                   defensor->nome, atacante->nome, defensor->cor);
        }
    } else {
        // empate ou defensor maior -> atacante perde 1 tropa
        atacante->tropas -= 1;
        printf("❌ O defensor resistiu. '%s' perdeu 1 tropa.\n", atacante->nome);

        if (atacante->tropas <= 0) {
            // atacante foi abatido — troca de cor para o defensor
            strcpy(atacante->cor, defensor->cor);
            atacante->tropas = 1;
            printf("🏳️ '%s' foi abatido! Agora pertence a '%s' (cor: %s) com 1 tropa.\n",
                   atacante->nome, defensor->nome, atacante->cor);
        }
    }
}

/* Libera memória alocada para o mapa */
void liberarMemoria(Territorio *mapa) {
    free(mapa);
    printf("\nMemória liberada.\n");
}

int main() {
    srand((unsigned int) time(NULL)); // inicializa aleatoriedade

    int n;
    printf("Digite o número de territórios: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Número inválido de territórios. Encerrando.\n");
        return 1;
    }

    // alocação dinâmica
    Territorio *mapa = (Territorio *) calloc((size_t)n, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    cadastrarTerritorios(mapa, n);
    exibirTerritorios(mapa, n);

    // Loop permanente de ataques até o usuário digitar 0 para sair
    while (1) {
        int a, d;
        printf("\nDigite o número do território atacante (0 para sair): ");
        if (scanf("%d", &a) != 1) {
            printf("Entrada inválida. Encerrando.\n");
            break;
        }
        if (a == 0) {
            printf("Jogo finalizado pelo usuário.\n");
            break;
        }
        if (a < 1 || a > n) {
            printf("Atacante inválido. Escolha um índice entre 1 e %d.\n", n);
            continue;
        }

        printf("Digite o número do território defensor (0 para cancelar ataque): ");
        if (scanf("%d", &d) != 1) {
            printf("Entrada inválida. Encerrando.\n");
            break;
        }
        if (d == 0) {
            printf("Ataque cancelado pelo usuário.\n");
            continue;
        }
        if (d < 1 || d > n) {
            printf("Defensor inválido. Escolha um índice entre 1 e %d.\n", n);
            continue;
        }

        if (a == d) {
            printf("Não é possível atacar o mesmo território. Tente novamente.\n");
            continue;
        }

        // Não permitir ataque se atacante tiver 0 tropas
        if (mapa[a - 1].tropas <= 0) {
            printf("Território atacante não tem tropas suficientes.\n");
            continue;
        }

        // executa ataque (passando ponteiros para as posições corretas)
        atacar(&mapa[a - 1], &mapa[d - 1]);

        // exibe o mapa atualizado após cada ataque
        exibirTerritorios(mapa, n);
    }

    liberarMemoria(mapa);
    return 0;
}
