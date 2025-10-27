#include <stdio.h>
#include <stdlib.h>

// Definição da estrutura Territorio
struct Territorio{
	char nome[30];
	char cor[10];
	int tropas;
};

int main(){
	// Declaração de um array de 5 territórios
	struct Territorio Territorios[5];

	// Entrada de dados para cada território
	for(int i = 0; i < 5; i++){
		printf("----- Territorio %d -----\n", i + 1);
		printf("Digite o nome do territorio %d: ", i + 1);
		scanf("%s", Territorios[i].nome);
		printf("Digite a cor do territorio %d: ", i + 1);
		scanf("%s", Territorios[i].cor);
		printf("Digite o numero de tropas do territorio %d: ", i + 1);
		scanf("%d", &Territorios[i].tropas);
	}
	
	// Exibição dos dados dos territórios
	printf("\nDados dos territorios:\n");
	for(int i = 0; i < 5; i++){
		printf("Territorio %d:\n", i + 1);
		printf("Nome: %s\n", Territorios[i].nome);
		printf("Cor: %s\n", Territorios[i].cor);
		printf("Tropas: %d\n\n", Territorios[i].tropas);
	}
}