/**
  @Author Thiago de Assis Lima
  Este algoritmo foi adaptado, usando como base o algoritmo de Dijkstra desenvolvido por RITA PERES, no seguinte site:
  https://www.revista-programar.info/artigos/algoritmo-de-dijkstra/
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define FLSH gets(l)

/* definir variavies */
int NUMERO_CIDADES = 0;
int destino, origem, vertices = 0;
int custo, *custos = NULL;

void dijkstra(int vertices, int origem, int destino, int *custos, char *chave_das_cidades)
{
	int i, v, cont = 0;
	int *ant, *tmp;
	int *z; /* vertices para os quais se conhece o caminho minimo */
	double min;
	double dist[vertices]; /* vetor com os custos dos caminhos */
	/* aloca as linhas da matriz */
	ant = calloc(vertices, sizeof(int *));
	tmp = calloc(vertices, sizeof(int *));
	if (ant == NULL)
	{
		printf("** Erro: Memoria Insuficiente **");
		exit(-1);
	}

	z = calloc(vertices, sizeof(int *));
	if (z == NULL)
	{
		printf("** Erro: Memoria Insuficiente **");
		exit(-1);
	}

	for (i = 0; i < vertices; i++)
	{
		if (custos[(origem - 1) * vertices + i] != -1)
		{
			ant[i] = origem - 1;
			dist[i] = custos[(origem - 1) * vertices + i];
		}
		else
		{
			ant[i] = -1;
			dist[i] = HUGE_VAL;
		}
		z[i] = 0;
	}
	z[origem - 1] = 1;
	dist[origem - 1] = 0;

	/* Ciclo principal */
	do
	{
		/* Encontra o vertice que deve entrar em z */
		min = HUGE_VAL;
		for (i = 0; i < vertices; i++)
			if (!z[i])
				if (dist[i] >= 0 && dist[i] < min)
				{
					min = dist[i];
					v = i;
				}

		/* Calcula distancias dos novos vizinhos de z */
		if (min != HUGE_VAL && v != destino - 1)
		{
			z[v] = 1;
			for (i = 0; i < vertices; i++)
				if (!z[i])
				{
					if (custos[v * vertices + i] != -1 && dist[v] + custos[v * vertices + i] < dist[i])
					{
						dist[i] = dist[v] + custos[v * vertices + i];
						ant[i] = v;
					}
				}
		}
	} while (v != destino - 1 && min != HUGE_VAL);

	/* Mostra o Resultado da procura */
	printf("Menor percurso:");
	if (min == HUGE_VAL)
	{
		printf("Nao Existe\n");
		printf("\tCusto: \t- \n");
	}
	else
	{
		i = destino;
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
			printf(" %c", chave_das_cidades[tmp[i - 1] - 1]);
		}
		printf(" %c\n", chave_das_cidades[destino - 1]);

		printf("Distancia total: %d Km\n", (int)dist[destino - 1]);
	}
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
	char nome_arquivo[100];

	char cidadeX, cidadeY;
	int valor_distancia = 0;
	int count_arestas = 0;
	int numero_arestas = 0;
	char *cidades;
	char *chave_das_cidades;
	int *custos_from_file;
	int k = 0;
	int j = 0;
	printf("Digite o nome do arquivo de entrada: ");
	scanf("%s", nome_arquivo);
	arquivo = fopen(nome_arquivo, "r+");
	fscanf(arquivo, "%d\n", &numero_arestas);

	cidades = malloc(numero_arestas * 2 * sizeof(char));
	chave_das_cidades = malloc(numero_arestas * sizeof(char));
	custos_from_file = malloc(numero_arestas * sizeof(int));
	do
	{
		fscanf(arquivo, "%c %c %d\n", &cidadeX, &cidadeY, &valor_distancia);
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

	fscanf(arquivo, "%c %c\n", &cidadeX, &cidadeY);

	fclose(arquivo);

	int i;
	j = 0;

	vertices = NUMERO_CIDADES;
	origem = 0;
	destino = 3;

	custos = (int *)malloc(sizeof(int) * NUMERO_CIDADES * NUMERO_CIDADES);
	for (i = 0; i <= NUMERO_CIDADES * NUMERO_CIDADES; i++)
		custos[i] = -1;

	for (i = 0; i < numero_arestas * 2; i += 2)
	{
		origem = retornaChaveCidadePeloNome(chave_das_cidades, cidades[i]) + 1;
		destino = retornaChaveCidadePeloNome(chave_das_cidades, cidades[i + 1]) + 1;

		custos[(origem - 1) * NUMERO_CIDADES + destino - 1] = custos_from_file[j];
		custos[(destino - 1) * NUMERO_CIDADES + origem - 1] = custos_from_file[j];
		j++;
	}

	dijkstra(NUMERO_CIDADES, retornaChaveCidadePeloNome(chave_das_cidades, cidadeX) + 1, retornaChaveCidadePeloNome(chave_das_cidades, cidadeY) + 1, custos, chave_das_cidades);

	// free(chave_das_cidades);
	// free(custos_from_file);

	return 0;
}
