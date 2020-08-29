/**

  Este algoritmo foi adaptado, usando como base o algoritmo de Dijkstra desenvolvido por RITA PERES, no seguinte site da ufmg:
  https://www.revista-programar.info/artigos/algoritmo-de-dijkstra/
  Outras referencias:
  https://stackoverflow.com/questions/4834811/strcat-concat-a-char-onto-a-string
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define FLSH gets(l)

/* definir variavies */
int NUMERO_CIDADES = 1;
int DESTINO, ORIGEM, VERTICES = 0;
int custo, *custos = NULL;

typedef struct Cidade
{
	int key;
	char *nome;
} Cidade;

void dijkstra(int VERTICES, int ORIGEM, int DESTINO, int *custos)
{
	int i, v, cont = 0;
	int *ant, *tmp;
	int *z; /* VERTICES para os quais se conhece o caminho minimo */
	double min;
	double dist[VERTICES]; /* vetor com os custos dos caminhos */

	/* aloca as linhas da matriz */
	ant = calloc(VERTICES, sizeof(int *));
	tmp = calloc(VERTICES, sizeof(int *));
	if (ant == NULL)
	{
		printf("** Erro: Memoria Insuficiente **");
		exit(-1);
	}

	z = calloc(VERTICES, sizeof(int *));
	if (z == NULL)
	{
		printf("** Erro: Memoria Insuficiente **");
		exit(-1);
	}

	for (i = 0; i < VERTICES; i++)
	{
		if (custos[(ORIGEM - 1) * VERTICES + i] != -1)
		{
			ant[i] = ORIGEM - 1;
			dist[i] = custos[(ORIGEM - 1) * VERTICES + i];
		}
		else
		{
			ant[i] = -1;
			dist[i] = HUGE_VAL;
		}
		z[i] = 0;
	}
	z[ORIGEM - 1] = 1;
	dist[ORIGEM - 1] = 0;

	/* Ciclo principal */
	do
	{
		/* Encontra o vertice que deve entrar em z */
		min = HUGE_VAL;
		for (i = 0; i < VERTICES; i++)
			if (!z[i])
				if (dist[i] >= 0 && dist[i] < min)
				{
					min = dist[i];
					v = i;
				}

		/* Calcula distancias dos novos vizinhos de z */
		if (min != HUGE_VAL && v != DESTINO - 1)
		{
			z[v] = 1;
			for (i = 0; i < VERTICES; i++)
				if (!z[i])
				{
					if (custos[v * VERTICES + i] != -1 && dist[v] + custos[v * VERTICES + i] < dist[i])
					{
						dist[i] = dist[v] + custos[v * VERTICES + i];
						ant[i] = v;
					}
				}
		}
	} while (v != DESTINO - 1 && min != HUGE_VAL);

	/* Mostra o Resultado da procura */
	printf("\tDe %d para %d: \t", ORIGEM, DESTINO);
	if (min == HUGE_VAL)
	{
		printf("Nao Existe\n");
		printf("\tCusto: \t- \n");
	}
	else
	{
		i = DESTINO;
		i = ant[i - 1];
		while (i != -1)
		{
			// printf("<-%d",i+1);
			tmp[cont] = i + 1;
			cont++;
			i = ant[i];
		}

		for (i = cont; i > 0; i--)
		{
			printf("%d -> ", tmp[i - 1]);
		}
		printf("%d", DESTINO);

		printf("\n\tCusto: %d\n", (int)dist[DESTINO - 1]);
	}
}

void cabecalho(void)
{
	printf("Implementacao do Algoritmo de Dijasktra\n");
	printf("Comandos:\n");
	printf("\t d - Adicionar Grafo\n"
		   "\t r - Procura Os Menores Caminhos no Grafo\n"
		   "\t CTRL+C para Sair do programa\n");
	printf("-------- ");
}

void add(void)
{
	int i, j;

	if (!custos)
		free(custos);
	custos = (int *)malloc(sizeof(int) * VERTICES * VERTICES);
	for (i = 0; i <= VERTICES * VERTICES; i++)
		custos[i] = -1;

	printf("Insira as arestas:\n");
	do
	{
		do
		{
			printf("Origem da aresta (entre 1 e %d ou '0' para sair): ", VERTICES);
			scanf("%d", &ORIGEM);
		} while (ORIGEM < 0 || ORIGEM > VERTICES);

		if (ORIGEM)
		{
			do
			{
				printf("Destino da aresta (entre 1 e %d, menos %d): ", VERTICES, ORIGEM);
				scanf("%d", &DESTINO);
			} while (DESTINO < 1 || DESTINO > VERTICES || DESTINO == ORIGEM);

			do
			{
				printf("Custo (positivo) da aresta do vertice %d para o vertice %d: ",
					   ORIGEM, DESTINO);
				scanf("%d", &custo);
			} while (custo < 0);

			custos[(ORIGEM - 1) * VERTICES + DESTINO - 1] = custo;
		}
	} while (ORIGEM);
}

void procurar(void)
{
	int i, j;

	/* Azul */
	printf("Lista dos Menores Caminhos no Grafo Dado: \n");

	for (i = 1; i <= VERTICES; i++)
	{
		for (j = 1; j <= VERTICES; j++)
			dijkstra(VERTICES, i, j, custos);
		printf("\n");
	}

	printf("ENTER para voltar ao menu inicial>\n");
	/* Volta cor normal */
	printf("\033[m");
}

int verificaSeCidadeJaExiste(char *cidades, char nome_cidade)
{
	int i;
	for (i = 0; i < NUMERO_CIDADES; i++)
	{
		if (cidades[i] == nome_cidade)
			return 1;
	}
	return 0;
}

int retornaChaveCidadePeloNome(char *cidades, char nome_cidade)
{
	int i;
	for (i = 0; i < NUMERO_CIDADES; i++)
	{
		if (cidades[i] == nome_cidade)
			return i;
	}
	return -1;
}

int main(int argc, char **argv)
{
	FILE *arquivo;
	FILE *arquivo2;
	char nome_arquivo[100];

	char cidadeX, cidadeY;
	int valor_distancia = 0;
	char char_arquivo = '\0';
	int count_arestas = 0;
	int key_cidade = 0;
	int numero_arestas = 0;
	char *cidades;
	char *chave_das_cidades;
	int *custos_from_file;
	int k = 0;
	int j = 0;
	// cidades = (Cidade*) realloc(cidades, 2);
	printf("Digite o nome do arquivo de entrada: \n");
	// scanf("%s", nome_arquivo);
	arquivo = fopen("teste.txt", "r+");
	char_arquivo = fscanf(arquivo, "%d\n", &numero_arestas);

	cidades = malloc(numero_arestas * 2 * sizeof(char));
	chave_das_cidades = malloc(numero_arestas * sizeof(char));
	custos_from_file = malloc(numero_arestas * sizeof(int));
	do
	{
		char_arquivo = fscanf(arquivo, "%c %c %d\n", &cidadeX, &cidadeY, &valor_distancia);
		cidades[count_arestas] = cidadeX;
		cidades[count_arestas + 1] = cidadeY;
		custos_from_file[j] = valor_distancia;
		if (!verificaSeCidadeJaExiste(chave_das_cidades, cidadeX))
		{
			// printf("%d - %c\n", k, cidadeX);
			chave_das_cidades[k] = cidadeX;
			NUMERO_CIDADES++;
			k++;
		}
		if (!verificaSeCidadeJaExiste(chave_das_cidades, cidadeY))
		{
			// printf("%d - %c\n", k, cidadeY);
			chave_das_cidades[k] = cidadeY;
			NUMERO_CIDADES++;
			k++;
		}

		j++;
		count_arestas += 2;
	} while (count_arestas < numero_arestas * 2);
	printf("saiu do while\n");
	// fclose(arquivo);

	int i;
	j = 0;
	for (i = 0; i < numero_arestas * 2; i += 2)
	{
		int origem = retornaChaveCidadePeloNome(chave_das_cidades, cidades[i]);
		int destino = retornaChaveCidadePeloNome(chave_das_cidades, cidades[i + 1]);
		// custos[(origem - 1) * NUMERO_CIDADES + destino - 1] = custos_from_file[j];
		j++;
	}
	// free(cidades);
	// free(chave_das_cidades);
	// free(custos_from_file);
	printf("saiu do while? %d", NUMERO_CIDADES);

	return 0;
}
