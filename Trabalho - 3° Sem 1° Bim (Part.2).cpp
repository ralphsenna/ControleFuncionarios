#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>


#define TF 20

struct TpData
{
	int dia, ano;
	char mes[3];	
};

struct TpPessoa
{
	int CodFunc;
	char Nome[30];
	TpData DtAdmissao;
	float Salario;
	int CodigoCargo;
};

struct TpCargo
{
	int Codigo;
	char Descricao[40];
};

struct TpFilial
{
	char IdFilial[30];
	int CodFunc;
	TpData DtVinculo;
};




char Menu(void)
{
	system("cls");
	printf("### Menu de Opcoes ###\n");
	printf("[A] Cadastro de Funcionarios\n");
	printf("[B] Listar Funcionarios\n");
	printf("[C] Consultar Funcionarios\n");
	printf("[D] Deletar Funcionarios\n");
	printf("[E] Alterar Funcionarios\n");
	printf("[F] Ordenar pelo Nome do Funcionario\n");
	printf("[G] Atualizacao de Salario Geral\n");
	printf("[H] Listar Tempo (em Anos) de Empresa para cada Funcionario\n");
	printf("[I] Ordenar pelo Salario - Decrescente\n");
	printf("[J] (*Teste*) Inserir Dados Aleatorios\n");

	printf("\n[K] Cadastro de Cargos Ordenados por Codigo - Crescente\n");
	printf("[L] Listar Cargos\n");
	printf("[M] Consultar Cargos\n");
	printf("[N] Deletar Cargos\n");
	printf("[O] Altetar Cargos\n");
	printf("[P] (*Teste*) Inserir Dados de Cargos Aleatorios\n");

	printf("\n[Q] Cadastro de Funcionarios em Filiais Ordenado por Nome de Filial\n");
	printf("[R] Listar Cadastro de Funcionarios em Filiais\n");
	printf("[S] Consultar Filiais\n");
	printf("[T] Deletar Filiais\n");
	printf("[U] Alterar Filiais\n");
	printf("[V] (*Teste*) Inserir Filiais com Funcionarios\n");
	
	printf("\n[ESC] Sair do Programa\n");
	printf("\nPressione a opcao desejada: ");
	return toupper(getche());
}

int BuscaCodFunc(TpPessoa TabFunc[TF], int TL, int codigo)
{
	int i=0;
	while (i<TL && TabFunc[i].CodFunc!=codigo)
		i++;
	if (i<TL)   //Achou o Codigo na posicao 'i'!
		return i;
	else        //Nao Achou o Codigo procurado!
		return -1;
}

int BuscaCodigoCarg(TpCargo TabCarg[TF], int TLC, int codigo)
{
	int i=0;
	while (i<TLC && TabCarg[i].Codigo!=codigo)
		i++;
	if (i<TLC)   //Achou o Codigo na posicao 'i'!
		return i;
	else       	 //Nao Achou o Codigo procurado!
		return -1;
}

int BuscaNomeCarg(TpCargo TabCarg[TF], int TLC, int codigo)
{
	int i=0;
	while (i<TLC && TabCarg[i].Codigo!=codigo)
		i++;
	return i; //Achou o Codigo na posicao 'i'!
}

void Cadastro(TpPessoa TabFunc[TF], int &TL, TpCargo TabCarg[TF], int TLC)
{
	int codigo, codcargo, i, j;
	char NomeFunc[30], resp;
	printf("\n\n*** CADASTRO DE FUNCIONARIOS ***\n");
	//Entra lido
	printf("\n[%d] Nome do Funcionario: ", TL);
	fflush(stdin);
	gets(NomeFunc);
	while (TL<TF && strcmp(NomeFunc,"\0")!=0)
	{
		strcpy(TabFunc[TL].Nome, NomeFunc);
		printf("Codigo do Funcionario: ");
		scanf("%d", &codigo);
		i = BuscaCodFunc(TabFunc, TL, codigo);
		if (i<0)
		{
			TabFunc[TL].CodFunc = codigo;
			printf("Dia da Admissao: ");
			scanf("%d", &TabFunc[TL].DtAdmissao.dia);
			printf("Mes [as tres/quatro primeiras letras]: ");
			fflush(stdin);
			strupr(gets(TabFunc[TL].DtAdmissao.mes));
			printf("Ano: ");
			scanf("%d", &TabFunc[TL].DtAdmissao.ano);
			printf("Salario: R$ ");
			scanf("%f", &TabFunc[TL].Salario);
			do
			{
				printf("Codigo do Cargo: ");
				scanf("%d", &codcargo);
				j = BuscaCodigoCarg(TabCarg, TLC, codcargo);	
				if (j>=0)
				{
					TabFunc[TL].CodigoCargo = codcargo;
					resp = 'N';
					TL++;
				}
				if (j<0)
				{
					printf("\nCodigo nao existente!\n");
					printf("\nDeseja tentar outro Codigo?\n");
					fflush(stdin);
					scanf("%c", &resp);
					printf("\n");
				}
			}while(toupper(resp)!='N');
		}	
		else
			printf("\nCodigo ja existente!\n");
		//Sai lido
		printf("\n[%d] Nome do Funcionario: ", TL);
		fflush(stdin);
		gets(NomeFunc);
	}
}

void Relatorio(TpPessoa TabFunc[TF], int TL, TpCargo TabCarg[TF], int TLC)
{
	char DescCarg[40];
	int i, j, codigo;
	printf("\n\n*** RELATORIO DE FUNCIONARIOS ***\n");
	if (TL==0)
		printf("\nNao ha Registros!\n");
	else
	{
		printf("------------------------------------------------------------------------------\n");
		printf("CodFunc  Nome \t Data de Admissao  Salario R$   CodCargo   Descricao do Cargo\n\n");
		for (i=0; i<TL; i++)
		{
			codigo = TabFunc[i].CodigoCargo;
			j = BuscaNomeCarg(TabCarg, TLC, codigo);
			printf("%d \t %s \t %d/%s/%d \t   R$%.2f \t %d \t   %s\n", TabFunc[i].CodFunc, TabFunc[i].Nome, TabFunc[i].DtAdmissao.dia, TabFunc[i].DtAdmissao.mes, TabFunc[i].DtAdmissao.ano, TabFunc[i].Salario, TabFunc[i].CodigoCargo, TabCarg[j].Descricao);
		}
		printf("------------------------------------------------------------------------------\n");
	}	
	getch();
}

int BuscaNome(TpPessoa TabFunc[TF], int TL, char NomeFunc[30])
{
	int i=0;
	while (i<TL && stricmp(TabFunc[i].Nome, NomeFunc)!=0)
		i++;
	if (i<TL)   //Achou o Nome na posicao 'i'!
		return i;
	else        //Nao Achou o Nome procurado!
		return -1;
}

void Consultar(TpPessoa TabFunc[TF], int TL)
{
	char NomeFunc[30];
	int i;
	printf("\n\n*** CONSULTA DE FUNCIONARIOS ***\n");
	printf("\nNome a Consultar: ");
	fflush(stdin);
	gets(NomeFunc);
	if (TL==0)
	{
		printf("\nNao ha Registros!\n");
		Sleep(1000);
	}
	while (TL>0 && strcmp(NomeFunc,"\0")!=0)
	{
		i = BuscaNome(TabFunc, TL, NomeFunc);
		if (i>=0)
		{
			printf("\nCodFunc  Nome \t Data de Admissao  Salario R$   CodCargo\n");
			printf("%d \t %s \t %d/%s/%d \t   R$%.2f \t%d\nEncontrado na posicao [%d]!\n", TabFunc[i].CodFunc, TabFunc[i].Nome, TabFunc[i].DtAdmissao.dia, TabFunc[i].DtAdmissao.mes, TabFunc[i].DtAdmissao.ano, TabFunc[i].Salario, TabFunc[i].CodigoCargo, i);
		}
		else 
			printf("\nNome nao Cadastrado!\n");	
		getch();
		printf("\nNome a Consultar: ");
		fflush(stdin);
		gets(NomeFunc);
	}
}

void Deletar(TpPessoa TabFunc[TF], int &TL)
{
	char NomeFunc[30], BackMes[TF], BackNome[TF];
	int i, BackDia, BackAno, BackCodFunc, BackCodCarg;
	float BackSalario;
	printf("\n\n*** EXCLUSAO DE FUNCIONARIOS ***\n");
	printf("\nNome a deletar: ");
	fflush(stdin);
	gets(NomeFunc);
	if (TL==0)
	{
		printf("\nNao ha Registros!\n");
		Sleep(1000);
	}
	while (TL>0 && strcmp(NomeFunc,"\0")!=0)
	{
		i = BuscaNome(TabFunc, TL, NomeFunc);
		if (i>=0)
		{
			printf("\nCodFunc  Nome \t Data de Admissao  Salario R$   CodCargo\n");
			printf("%d \t %s \t %d/%s/%d \t   R$%.2f \t%d\nEncontrado na posicao [%d]!\n", TabFunc[i].CodFunc, TabFunc[i].Nome, TabFunc[i].DtAdmissao.dia, TabFunc[i].DtAdmissao.mes, TabFunc[i].DtAdmissao.ano, TabFunc[i].Salario, TabFunc[i].CodigoCargo, i);
			printf("\nConfirma a Exclusao (S/N)?\n");
			if (getche()=='S')
			{
				strcpy(BackNome, TabFunc[i].Nome);
				BackCodFunc = TabFunc[i].CodFunc;
				BackDia = TabFunc[i].DtAdmissao.dia;
				strcpy(BackMes, TabFunc[i].DtAdmissao.mes);
				BackAno = TabFunc[i].DtAdmissao.ano;
				BackSalario = TabFunc[i].Salario;
				BackCodCarg = TabFunc[i].CodigoCargo;
				for(; i<TL-1; i++)
					TabFunc[i] = TabFunc[i+1];							
				TL--;
				printf("\n\nCodFunc  Nome \t Data de Admissao  Salario R$   CodCargo\n\n");
				printf("%d \t %s \t %d/%s/%d \t   R$%.2f \t%d\nFoi excluido!\n", BackCodFunc, BackNome, BackDia, BackMes, BackAno, BackSalario, BackCodCarg);
			}
		}
		else 
			printf("\nNome nao Cadastrado!\n");			
		getch();
		printf("\n\nNome a Excluir: ");
		fflush(stdin);
		gets(NomeFunc);
	}
}

void Alterar(TpPessoa TabFunc[TF], int TL, TpCargo TabCarg[TF], int TLC)
{
	char NomeFunc[30], BackMes[TF], BackNome[TF], resp;
	int i, j, k, AuxCodFunc, AuxCodCarg, BackDia, BackAno, BackCodFunc, BackCodCarg;
	float BackSalario;
	printf("\n\n*** ALTERACAO DE FUNCIONARIOS ***\n");
	printf("\nNome a Alterar: ");
	fflush(stdin);
	gets(NomeFunc);
	if (TL==0)
	{
		printf("\nNao ha Registros!\n");
		Sleep(1000);
	}
	while (TL>0 && strcmp(NomeFunc,"\0")!=0)
	{
		i = BuscaNome(TabFunc, TL, NomeFunc);
		if (i>=0)
		{
			printf("\nCodFunc  Nome \t Data de Admissao  Salario R$   CodCargo\n");
			printf("%d \t %s \t %d/%s/%d \t   R$%.2f \t%d\nEncontrado na posicao [%d]!\n", TabFunc[i].CodFunc, TabFunc[i].Nome, TabFunc[i].DtAdmissao.dia, TabFunc[i].DtAdmissao.mes, TabFunc[i].DtAdmissao.ano, TabFunc[i].Salario, TabFunc[i].CodigoCargo, i);
			printf("\nConfirma a Alteracao (S/N)?\n");
			if (toupper(getche())=='S')
			{
				strcpy(BackNome, TabFunc[i].Nome);
				BackCodFunc = TabFunc[i].CodFunc;
				BackDia = TabFunc[i].DtAdmissao.dia;
				strcpy(BackMes, TabFunc[i].DtAdmissao.mes);
				BackAno = TabFunc[i].DtAdmissao.ano;
				BackSalario = TabFunc[i].Salario;
				BackCodCarg = TabFunc[i].CodigoCargo;
				printf("\n\nNovo Nome[%d]: ", i);
				fflush(stdin);
				gets(NomeFunc);
				strcpy(TabFunc[i].Nome, NomeFunc);
				do
				{
					printf("Novo Codigo do Funcionario[%d]: ", i);
					scanf("%d", &AuxCodFunc);
					TabFunc[i].CodFunc = 0;
					j = BuscaCodFunc(TabFunc, TL, AuxCodFunc);
					if (j<0)
					{
						TabFunc[i].CodFunc = AuxCodFunc;
						printf("Novo Dia de Admissao[%d]: ", i);
						scanf("%d", &TabFunc[i].DtAdmissao.dia);
						printf("Novo Mes[%d]: ", i);
						fflush(stdin);
						strupr(gets(TabFunc[i].DtAdmissao.mes));
						printf("Novo Ano[%d]: ", i);
						scanf("%d", &TabFunc[i].DtAdmissao.ano);
						printf("Novo Salario[%d]: ", i);
						scanf("%f", &TabFunc[i].Salario);
						do
						{
							printf("Novo Codigo do Cargo[%d]: ", i);
							scanf("%d", &AuxCodCarg);
							k = BuscaCodigoCarg(TabCarg, TLC, AuxCodCarg);	
							if (k>=0)
							{
								TabFunc[TL].CodigoCargo = AuxCodCarg;
								printf("\n\nCodFunc  Nome \t Data de Admissao  Salario R$   CodCargo\n");
								printf("%d \t %s \t %d/%s/%d \t   R$%.2f \t%d\n\nFoi Alterado para \n", BackCodFunc, BackNome, BackDia, BackMes, BackAno, BackSalario, BackCodCarg);
								printf("\nCodFunc  Nome \t Data de Admissao  Salario R$   CodCargo\n");
								printf("%d \t %s \t %d/%s/%d \t   R$%.2f \t%d\n", TabFunc[i].CodFunc, TabFunc[i].Nome, TabFunc[i].DtAdmissao.dia, TabFunc[i].DtAdmissao.mes, TabFunc[i].DtAdmissao.ano, TabFunc[i].Salario, TabFunc[i].CodigoCargo);
								resp = 'N';
							}
							if (k<0)
							{
								printf("\nCodigo nao existente!\n");
								printf("\nDeseja tentar outro Codigo?\n");
								fflush(stdin);
								scanf("%c", &resp);
								printf("\n");
							}
						}while(toupper(resp)!='N');
					}
					if (j>=0)
					{
						printf("\nCodigo ja existente!");
						printf("\nDeseja tentar outro Codigo?\n");
						fflush(stdin);
						scanf("%c", &resp);
						printf("\n");
					}
				}while(toupper(resp)!='N');
			}
		}
		else 
			printf("\nNome nao Cadastrado!\n");
		getch();
		printf("\n\nNome a Alterar: ");
		fflush(stdin);
		gets(NomeFunc);
	}
}

void OrdenaNomes(TpPessoa TabFunc[TF], int TL)
{
	int i, j;
	TpPessoa RegAux;
	printf("\n\n *** ORDENAR TABELA DE FUNCIONARIOS PELO NOME ***\n");
	if (TL==0)
		printf("\nNao ha dados para Ordenar!\n");
	else
	{
		for (i=0; i<TL-1; i++)
			for (j=i+1; j<TL; j++)
				if (stricmp(TabFunc[i].Nome, TabFunc[j].Nome)>0)
				{
					RegAux = TabFunc[i];
					TabFunc[i] = TabFunc[j];
					TabFunc[j] = RegAux;
				}
		printf("\n::.. TABELA DE FUNCIONARIOS ORDENADA ..::\n");
		printf("--------------------------------------------------------\n");
		printf("CodFunc  Nome \t Data de Admissao  Salario R$   CodCargo\n\n");
		for (i=0; i<TL; i++)
			printf("%d \t %s \t %d/%s/%d \t   R$%.2f \t%d\n", TabFunc[i].CodFunc, TabFunc[i].Nome, TabFunc[i].DtAdmissao.dia, TabFunc[i].DtAdmissao.mes, TabFunc[i].DtAdmissao.ano, TabFunc[i].Salario, TabFunc[i].CodigoCargo);
		printf("--------------------------------------------------------\n");
	}
	getch();
}

void OrdenaSalario(TpPessoa TabFunc[TF], int TL)
{
	int i, j;
	TpPessoa RegAux;
	printf("\n\n *** ORDENAR TABELA DE FUNCIONARIOS PELO SALARIO ***\n");
	if (TL==0)
		printf("\nNao ha dados para Ordenar!\n");
	else
	{
		for (i=0; i<TL-1; i++)
			for (j=i+1; j<TL; j++)
				if (TabFunc[i].Salario<TabFunc[j].Salario)
				{
					RegAux = TabFunc[i];
					TabFunc[i] = TabFunc[j];
					TabFunc[j] = RegAux;
				}
		printf("\n::.. TABELA DE FUNCIONARIOS ORDENADA POR SALARIO DECRESCENTE ..::\n");
		printf("--------------------------------------------------------\n");
		printf("CodFunc  Nome \t Data de Admissao  Salario R$   CodCargo\n\n");
		for (i=0; i<TL; i++)
			printf("%d \t %s \t %d/%s/%d \t   R$%.2f \t%d\n", TabFunc[i].CodFunc, TabFunc[i].Nome, TabFunc[i].DtAdmissao.dia, TabFunc[i].DtAdmissao.mes, TabFunc[i].DtAdmissao.ano, TabFunc[i].Salario, TabFunc[i].CodigoCargo);
		printf("--------------------------------------------------------\n");
	}
	getch();
}

void AtualizaSalarios(TpPessoa TabFunc[TF], int TL)
{
	float percentual;
	int i;
	printf("\n\n *** ATUALIZAR SALARIO ***\n");
	if (TL==0)
		printf("\nNao ha dados para Atualizar!\n");
	else
	{
		printf("Qual o percentual de Atualizacao?\n");
		scanf("%f", &percentual);
		for (i=0; i<TL; i++)
			TabFunc[i].Salario = TabFunc[i].Salario+TabFunc[i].Salario*percentual/100;
		printf("\n------- SALARIOS ATUALIZADOS -------\n");
		printf("--------------------------------------------------------\n");
		printf("CodFunc  Nome \t Data de Admissao  Salario R$   CodCargo\n\n");
		for (i=0; i<TL; i++)
			printf("%d \t %s \t %d/%s/%d \t   R$%.2f \t%d\n", TabFunc[i].CodFunc, TabFunc[i].Nome, TabFunc[i].DtAdmissao.dia, TabFunc[i].DtAdmissao.mes, TabFunc[i].DtAdmissao.ano, TabFunc[i].Salario, TabFunc[i].CodigoCargo);
		printf("--------------------------------------------------------\n");
	}
	getch();
}

void TempoEmpresa(TpPessoa TabFunc[TF], int TL)
{
	int i, AnoAtual;
	printf("\n\n*** TEMPO DOS FUNCIONARIOS NA EMPRESA ***\n");
	if (TL==0)
		printf("\nNao ha Funcionarios para mostrar o Tempo de Servico!\n");	
	else 
	{
		printf("Informe o Ano Atual: ");
		scanf("%d",&AnoAtual);
		printf("\n------- TEMPO DOS FUNCIONARIOS DA EMPRESA -------\n");
		printf("--------------------------------------------------------\n");
		printf("CodFunc  Nome \t Data de Admissao  Salario R$   CodCargo\n\n");
		for (i=0; i<TL; i++)
			printf("%d \t %s \t %d/%s/%d \t   R$%.2f \t%d\n", TabFunc[i].CodFunc, TabFunc[i].Nome, TabFunc[i].DtAdmissao.dia, TabFunc[i].DtAdmissao.mes, TabFunc[i].DtAdmissao.ano, TabFunc[i].Salario, TabFunc[i].CodigoCargo);
		printf("--------------------------------------------------------\n");
	}
	getch();
}

void InsereDados(TpPessoa TabFunc[TF], int &TL)
{
	strcpy(TabFunc[TL].Nome, "Abel");
	TabFunc[TL].CodFunc = 01;
	TabFunc[TL].DtAdmissao.dia = 01;
	strcpy(TabFunc[TL].DtAdmissao.mes, "MAR");
	TabFunc[TL].DtAdmissao.ano = 2017;
	TabFunc[TL].Salario = 15800.70;
	TabFunc[TL].CodigoCargo = 01;
	TL++;
	
	strcpy(TabFunc[TL].Nome, "Ana");
	TabFunc[TL].CodFunc = 15;
	TabFunc[TL].DtAdmissao.dia = 06;
	strcpy(TabFunc[TL].DtAdmissao.mes, "MAR");
	TabFunc[TL].DtAdmissao.ano = 2021;
	TabFunc[TL].Salario = 8500;
	TabFunc[TL].CodigoCargo = 03;
	TL++;
	
	strcpy(TabFunc[TL].Nome, "Eva");
	TabFunc[TL].CodFunc = 11;
	TabFunc[TL].DtAdmissao.dia = 15;
	strcpy(TabFunc[TL].DtAdmissao.mes, "FEV");
	TabFunc[TL].DtAdmissao.ano = 2018;
	TabFunc[TL].Salario = 8900;
	TabFunc[TL].CodigoCargo = 03;
	TL++;
	
	strcpy(TabFunc[TL].Nome, "Rafa");
	TabFunc[TL].CodFunc = 07;
	TabFunc[TL].DtAdmissao.dia = 02;
	strcpy(TabFunc[TL].DtAdmissao.mes, "OUT");
	TabFunc[TL].DtAdmissao.ano = 2020;
	TabFunc[TL].Salario = 6570;
	TabFunc[TL].CodigoCargo = 07;
	TL++;
}


int BuscaDescricaocomCargo(TpPessoa TabFunc[TF], int TL, int codigo)
{
	int i=0;
	while (i<TL && TabFunc[i].CodigoCargo!=codigo)
		i++;
	if (i<TL)   //Achou o Código na posicao 'i'!
		return 1;
	else        //Não Achou o Código procurado!
		return -1;
}

void CadastroCargosOrdenaCodCres(TpCargo TabCarg[TF], int &TLC)
{
	char DescCargo[TF];
	int codigo, i, j;
	TpCargo RegAux;
	printf("\n\n*** CADASTRO DE CARGOS ORDENADOS POR CODIGO CRESCENTE ***\n");
	printf("\n[%d] Descricao do Cargo: ", TLC);
	fflush(stdin);
	gets(DescCargo);
	while (TLC<TF && strcmp(DescCargo,"\0")!=0)
	{
		strcpy(TabCarg[TLC].Descricao, DescCargo);
		printf("Codigo do Cargo: ");
		scanf("%d", &codigo);
		i = BuscaCodigoCarg(TabCarg, TLC, codigo);
		if (i<0)
		{
			TabCarg[TLC].Codigo = codigo;
			TLC++;
		}	
		else
		{
			printf("\nCodigo ja existente!\n");
			getch();
		}
		printf("\n[%d] Descricao do Cargo: ", TLC);
		fflush(stdin);
		gets(DescCargo);
	}
	for (i=0; i<TLC-1; i++)
		for (j=i+1; j<TLC; j++)
			if (TabCarg[i].Codigo>TabCarg[j].Codigo)
			{		
				RegAux = TabCarg[i];
				TabCarg[i] = TabCarg[j];
				TabCarg[j] = RegAux;
			}
}

void RelatorioCargos(TpCargo TabCarg[TF], int TLC, TpPessoa TabFunc[TF], int TL)
{
	int i, j, cargo, achou, codigo, achou2, aux;
	printf("\n\n*** RELATORIO DE CARGOS ***\n");
	if (TLC==0)
		printf("\nNao ha Registros!\n");
	else
	{
		printf("------------------------------------------------------------------------------------\n");
		for (i=0; i<TLC; i++)
		{
			cargo = TabCarg[i].Codigo;
			achou = BuscaDescricaocomCargo(TabFunc, TL, cargo);
			if (achou>0)
			{
				if (aux==1)
				{
					j = i-1;
					aux = 0;
				}
				else
					j = i;
				printf("%s\n", TabCarg[i].Descricao);
				for ( ; j<TL && TabFunc[j].CodigoCargo==TabCarg[i].Codigo; j++)
				{
					codigo = TabFunc[j].CodigoCargo;
					achou2 = BuscaNomeCarg(TabCarg, TLC, codigo);
					printf("%d \t %s \t %d/%s/%d \t   R$%.2f \t %d \t   %s\n", TabFunc[j].CodFunc, TabFunc[j].Nome, TabFunc[j].DtAdmissao.dia, TabFunc[j].DtAdmissao.mes, TabFunc[j].DtAdmissao.ano, TabFunc[j].Salario, TabFunc[j].CodigoCargo, TabCarg[achou2].Descricao);
				}
				printf("\n");
				i = j-1;
			}
			else
				aux = 1;
		}
		printf("------------------------------------------------------------------------------------\n");
	}	
	getch();
}

void ConsultarCargos(TpCargo TabCarg[TF], int TLC)
{
	int i, codigo;
	printf("\n\n*** CONSULTA DE CARGOS ***\n");
	printf("\nCodigo a Consultar: ");
	scanf("%d", &codigo);
	if (TLC==0)
	{
		printf("\nNao ha Registros!\n");
		Sleep(1000);
	}
	while (TLC>0 && codigo>0)
	{
		i = BuscaCodigoCarg(TabCarg, TLC, codigo);
		if (i>=0)
			printf("\n%d \t %s\nEncontrado na posicao [%d]\n", TabCarg[i].Codigo, TabCarg[i].Descricao, i);
		else 
			printf("\nCodigo nao Cadastrado!\n");	
		getch();
		printf("\nCodigo a Consultar: ");
		scanf("%d", &codigo);
	}
}

void DeletarCargos(TpCargo TabCarg[TF], int &TLC)
{
	char BackDesc[40];
	int i, CodCargo, BackCodCarg;
	printf("\n\n*** EXCLUSAO DE CARGOS ***\n");
	printf("\nCodigo do Cargo a deletar: ");
	scanf("%d", &CodCargo);
	if (TLC==0)
	{
		printf("\nNao ha Registros!\n");
		Sleep(1000);
	}
	while (TLC>0 && CodCargo>0)
	{
		i = BuscaCodigoCarg(TabCarg, TLC, CodCargo);
		if (i>=0)
		{
			printf("\nCodigo \t Descricao do Cargo\n");
			printf("%d \t %s\nEncontrado na posicao [%d]!\n", TabCarg[i].Codigo, TabCarg[i].Descricao, i);
			printf("\nConfirma a Exclusao (S/N)?\n");
			if (toupper(getche())=='S')
			{
				BackCodCarg = TabCarg[i].Codigo;
				strcpy(BackDesc, TabCarg[i].Descricao);
				for(; i<TLC-1; i++)
					TabCarg[i] = TabCarg[i+1];							
				TLC--;
				printf("\n\nCodigo \t Descricao do Cargo\n");
				printf("%d \t %s\nFoi excluido!\n", BackCodCarg, BackDesc);
			}
		}
		else 
			printf("\nCodigo nao Cadastrado!\n");			
		getch();
		printf("\nCodigo do Cargo a deletar: ");
		scanf("%d", &CodCargo);
	}
}

void AlterarCargos(TpCargo TabCarg[TF], int TLC)
{
	char BackDesc[40], resp;
	int i, j, CodCargo, BackCodCarg;
	printf("\n\n*** ALTERACAO DE CARGOS ***\n");
	printf("\nCodigo do Cargo a Alterar: ");
	scanf("%d", &CodCargo);
	if (TLC==0)
	{
		printf("\nNao ha Registros!\n");
		Sleep(1000);
	}
	while (TLC>0 && CodCargo>0)
	{
		i = BuscaCodigoCarg(TabCarg, TLC, CodCargo);
		if (i>=0)
		{
			printf("\nCodigo \t Descricao do Cargo\n");
			printf("%d \t %s\nEncontrado na posicao [%d]!\n", TabCarg[i].Codigo, TabCarg[i].Descricao, i);
			printf("\nConfirma a Alteracao (S/N)?\n");
			if (toupper(getche())=='S')
			{
				BackCodCarg = TabCarg[i].Codigo;
				strcpy(BackDesc, TabCarg[i].Descricao);
				do
				{
					printf("\n\nNovo Codigo do Cargo[%d]: ", i);
					scanf("%d", &CodCargo);
					TabCarg[i].Codigo = 0;
					j = BuscaCodigoCarg(TabCarg, TLC, CodCargo);
					if (j<0)
					{
						TabCarg[i].Codigo = CodCargo;
						printf("Nova Descricao do Cargo[%d]: ", i);
						fflush(stdin);
						gets(TabCarg[i].Descricao);
						printf("\nCodigo \t Descricao do Cargo\n");
						printf("%d \t %s\n\nFoi Alterado para \n", BackCodCarg, BackDesc);
						printf("\nCodigo \t Descricao do Cargo\n");
						printf("%d \t %s\n", TabCarg[i].Codigo, TabCarg[i].Descricao);
						resp = 'N';
					}
					else
					{
						printf("\nCodigo ja Cadastrado!");
						printf("\nDeseja tentar outro Codigo?\n");
						fflush(stdin);
						scanf("%c", &resp);
						printf("\n");
					}
				}while(resp!='N');
			}
		}
		else
			printf("\nCodigo nao Cadastrado!");
		getch();
		printf("\n\nCodigo do Cargo a Alterar: ");
		scanf("%d", &CodCargo);
	}
}

void InserirCargos(TpCargo TabCarg[TF], int &TLC)
{
	TabCarg[TLC].Codigo = 01;
	strcpy(TabCarg[TLC].Descricao,"Diretor Geral");
	TLC++;

	TabCarg[TLC].Codigo = 02;
	strcpy(TabCarg[TLC].Descricao,"Gestor de Redes");
	TLC++;

	TabCarg[TLC].Codigo = 03;
	strcpy(TabCarg[TLC].Descricao,"Gestor de TI");
	TLC++;

	TabCarg[TLC].Codigo = 07;
	strcpy(TabCarg[TLC].Descricao,"Operador de Computadores");
	TLC++;	
}


int BuscaNomeFilial(TpFilial TabFili[TF], int TLF, char Filial[30])
{
	int i=0;
	while (i<TLF && stricmp(TabFili[i].IdFilial, Filial)!=0)
		i++;
	if (i<TLF)   //Achou o Nome na posicao 'i'!
		return i;
	else        //Nao Achou o Nome procurado!
		return -1;
}

int BuscaCodFiliFunc(TpFilial TabFili[TF], int TLF, int codigo, char idfili[30])
{
	int i, igual=0;
	for (i=0; i<TLF; i++)
		if (strcmp(TabFili[i].IdFilial, idfili)==0)
			if (TabFili[i].CodFunc==codigo)
				igual=1;
	if (igual==1)   //Achou o Codigo na posicao 'i'!
		return i;
	else        	//Nao Achou o Codigo procurado!
		return -1;
}

void CadastroFiliaisOrdenaFili(TpFilial TabFili[TF], int &TLF)
{
	TpFilial RegAux;
	int codigo, codcargo, i, j;
	char idfili[30], resp;
	printf("\n\n*** CADASTRO DE FILIAIS ***\n");
	//Entra lido
	printf("\n[%d] Nome da Filial: ", TLF);
	fflush(stdin);
	gets(idfili);
	while (TLF<TF && strcmp(idfili,"\0")!=0)	
	{
		strcpy(TabFili[TLF].IdFilial, idfili);
		printf("Codigo do Funcionario: ");
		scanf("%d", &codigo);
		i = BuscaCodFiliFunc(TabFili, TLF, codigo, idfili);
		if (i<0)
		{
			TabFili[TLF].CodFunc = codigo;
			printf("Dia do Vinculo: ");
			scanf("%d", &TabFili[TLF].DtVinculo.dia);
			printf("Mes [as tres/quatro primeiras letras]: ");
			fflush(stdin);
			strupr(gets(TabFili[TLF].DtVinculo.mes));
			printf("Ano: ");
			scanf("%d", &TabFili[TLF].DtVinculo.ano);
			TLF++;
		}	
		else
			printf("\nCodigo ja existente na %s!\n", idfili);
		//Sai lido
		printf("\n[%d] Nome da Filial: ", TLF);
		fflush(stdin);
		gets(idfili);
	}
	for (i=0; i<TLF-1; i++)
		for (j=i+1; j<TLF; j++)
			if (strcmp(TabFili[i].IdFilial, TabFili[j].IdFilial)>0)
			{		
				RegAux = TabFili[i];
				TabFili[i] = TabFili[j];
				TabFili[j] = RegAux;
			}
}

void RelatorioFiliais(TpFilial TabFili[TF], int TLF, TpPessoa TabFunc[TF], TpCargo TabCarg[TF], int TLC)
{
	int i, j, k, codigo, aux=0;
	printf("\n\n*** RELATORIO DE CADASTRO DE FUNCIONARIOS EM FILIAIS ***\n");
	if (TLF==0)
		printf("\nNao ha Registros!\n");
	else
	{
		printf("-------------------------------------\n");
		for (i=0; i<TLF; i++)
		{
			printf("%s\n", TabFili[i].IdFilial);
			for (j=i; j<TLF && TabFunc[j].CodFunc==TabFili[j].CodFunc && strcmp(TabFili[i].IdFilial, TabFili[j].IdFilial)==0; j++)
			{
				codigo = TabFunc[j].CodigoCargo;
				k = BuscaNomeCarg(TabCarg, TLC, codigo);
				printf("%d \t %s \t %d/%s/%d \t   R$%.2f \t %d \t   %s\n", TabFunc[j].CodFunc, TabFunc[j].Nome, TabFunc[j].DtAdmissao.dia, TabFunc[j].DtAdmissao.mes, TabFunc[j].DtAdmissao.ano, TabFunc[j].Salario, TabFunc[j].CodigoCargo, TabCarg[k].Descricao);
			}
			printf("\n");
			i = j-1;
		}
		printf("-------------------------------------\n");
	}	
	getch();
}

void ConsultarFiliais(TpFilial TabFili[TF], int TLF)
{
	char Filial[30];
	int i;
	printf("\n\n*** CONSULTA DE FILIAIS ***\n");
	printf("\nFilial a Consultar: ");
	fflush(stdin);
	gets(Filial);
	if (TLF==0)
	{
		printf("\nNao ha Registros!\n");
		Sleep(1000);
	}
	while (TLF>0 && strcmp(Filial,"\0")!=0)
	{
		i = BuscaNomeFilial(TabFili, TLF, Filial);
		if (i>=0)
			printf("\n%s \t %d \t %d/%s/%d\nEncontrado na posicao [%d]\n", TabFili[i].IdFilial, TabFili[i].CodFunc, TabFili[i].DtVinculo.dia, TabFili[i].DtVinculo.mes, TabFili[i].DtVinculo.ano, i);
		else 
			printf("\nFilial nao Cadastrada!\n");	
		getch();
		printf("\nFilial a Consultar: ");
		fflush(stdin);
		gets(Filial);
	}
}

void DeletarFiliais(TpFilial TabFili[TF], int &TLF)
{
	char Filial[40], BackFilial[40], BackMes[3];
	int i, BackCodFunc, BackDia, BackAno;
	printf("\n\n*** EXCLUSAO DE FILIAIS ***\n");
	printf("\nFilial a Excluir: ");
	fflush(stdin);
	gets(Filial);
	if (TLF==0)
	{
		printf("\nNao ha Registros!\n");
		Sleep(1000);
	}
	while (TLF>0 && strcmp(Filial,"\0")!=0)
	{
		i = BuscaNomeFilial(TabFili, TLF, Filial);
		if (i>=0)
		{
			printf("\n%s \t %d \t %d/%s/%d\nEncontrado na posicao [%d]\n", TabFili[i].IdFilial, TabFili[i].CodFunc, TabFili[i].DtVinculo.dia, TabFili[i].DtVinculo.mes, TabFili[i].DtVinculo.ano, i);
			printf("\nConfirma a Exclusao (S/N)?\n");
			if (toupper(getche())=='S')
			{
				strcpy(BackFilial, TabFili[i].IdFilial);
				BackCodFunc = TabFili[i].CodFunc;
				BackDia = TabFili[i].DtVinculo.dia;
				strcpy(BackMes, TabFili[i].DtVinculo.mes);
				BackAno = TabFili[i].DtVinculo.ano;
				for(; i<TLF-1; i++)
					TabFili[i] = TabFili[i+1];							
				TLF--;
				printf("\n%s \t %d \t %d/%s/%d\nFoi excluido!\n", BackFilial, BackCodFunc, BackDia, BackMes, BackAno);
			}
		}
		else 
			printf("\nFilial nao Cadastrada!\n");	
		getch();
		printf("\nFilial a Consultar: ");
		fflush(stdin);
		gets(Filial);
	}
}

void AlterarFiliais(TpFilial TabFili[TF], int TLF)
{
	char Filial[40], BackFilial[40], BackMes[3], resp;
	int i, j, BackCodFunc, codigo, BackDia, BackAno;
	printf("\n\n*** ALTERACAO DE FILIAIS ***\n");
	printf("\nFilial a Alterar: ");
	fflush(stdin);
	gets(Filial);
	if (TLF==0)
	{
		printf("\nNao ha Registros!\n");
		Sleep(1000);
	}
	while (TLF>0 && strcmp(Filial,"\0")!=0)
	{
		i = BuscaNomeFilial(TabFili, TLF, Filial);
		if (i>=0)
		{
			printf("\n%s \t %d \t %d/%s/%d\nEncontrado na posicao [%d]\n", TabFili[i].IdFilial, TabFili[i].CodFunc, TabFili[i].DtVinculo.dia, TabFili[i].DtVinculo.mes, TabFili[i].DtVinculo.ano, i);
			printf("\nConfirma a Exclusao (S/N)?\n");
			if (toupper(getche())=='S')
			{
				strcpy(BackFilial, TabFili[i].IdFilial);
				BackCodFunc = TabFili[i].CodFunc;
				BackDia = TabFili[i].DtVinculo.dia;
				strcpy(BackMes, TabFili[i].DtVinculo.mes);
				BackAno = TabFili[i].DtVinculo.ano;
				printf("\n\nNova Filial[%d]: ", i);
				fflush(stdin);
				gets(Filial);
				strcpy(TabFili[i].IdFilial, Filial);
				do
				{
					printf("Novo Codigo do Funcionario[%d]: ", i);
					scanf("%d", &codigo);
					j = BuscaCodFiliFunc(TabFili, TLF, codigo, Filial);
					if (j<0)
					{
						TabFili[i].CodFunc = codigo;
						printf("Novo Dia de Vinculo[%d]: ", i);
						scanf("%d", &TabFili[i].DtVinculo.dia);
						printf("Novo Mes[%d]: ", i);
						fflush(stdin);
						strupr(gets(TabFili[i].DtVinculo.mes));
						printf("Novo Ano[%d]: ", i);
						scanf("%d", &TabFili[i].DtVinculo.ano);
						printf("\n%s \t %d \t %d/%s/%d\nFoi alterado para \n\n%s \t %d \t %d/%s/%d\n", BackFilial, BackCodFunc, BackDia, BackMes, BackAno, TabFili[i].IdFilial, TabFili[i].CodFunc, TabFili[i].DtVinculo.dia, TabFili[i].DtVinculo.mes, TabFili[i].DtVinculo.ano);
						resp = 'N';
					}
					else
					{
						printf("\nCodigo do Funcionario ja Cadastrado!");
						printf("\nDeseja tentar outro Codigo?\n");
						fflush(stdin);
						scanf("%c", &resp);
						printf("\n");
					}
				}while(resp!='N');
			}
		}
		else 
			printf("\nFilial nao Cadastrada!\n");	
		getch();
		printf("\nFilial a Consultar: ");
		fflush(stdin);
		gets(Filial);
	}
}

void InserirFiliais(TpFilial TabFili[TF], int &TLF)
{
	strcpy(TabFili[TLF].IdFilial, "Filial1");
	TabFili[TLF].CodFunc = 01;
	TabFili[TLF].DtVinculo.dia = 10;
	strcpy(TabFili[TLF].DtVinculo.mes, "MAR");
	TabFili[TLF].DtVinculo.ano = 2017;
	TLF++;

	strcpy(TabFili[TLF].IdFilial, "Filial1");
	TabFili[TLF].CodFunc = 15;
	TabFili[TLF].DtVinculo.dia = 21;
	strcpy(TabFili[TLF].DtVinculo.mes, "AGO");
	TabFili[TLF].DtVinculo.ano = 2018;
	TLF++;

	strcpy(TabFili[TLF].IdFilial, "Filial2");
	TabFili[TLF].CodFunc = 11;
	TabFili[TLF].DtVinculo.dia = 17;
	strcpy(TabFili[TLF].DtVinculo.mes, "JAN");
	TabFili[TLF].DtVinculo.ano = 2022;
	TLF++;
	
	strcpy(TabFili[TLF].IdFilial, "Filial3");
	TabFili[TLF].CodFunc = 07;
	TabFili[TLF].DtVinculo.dia = 04;
	strcpy(TabFili[TLF].DtVinculo.mes, "OUT");
	TabFili[TLF].DtVinculo.ano = 2020;
	TLF++;
}



int main(void)
{				
	char opcao;
	TpPessoa TabFuncionarios[TF];
	TpCargo TabCargos[TF];
	TpFilial TabFilial[TF];
	int TamL=0, TamLC=0, TamLF=0;
	do
	{
		opcao=Menu();
		switch(opcao)
		{
		    case 'A': Cadastro(TabFuncionarios, TamL, TabCargos, TamLC);
					  break;
					  
			case 'B': Relatorio(TabFuncionarios, TamL, TabCargos, TamLC);
					  break;

			case 'C': Consultar(TabFuncionarios, TamL);
					  break;
					  
			case 'D': Deletar(TabFuncionarios, TamL);
					  break;
			
			case 'E': Alterar(TabFuncionarios, TamL, TabCargos, TamLC);
					  break;
					  
		    case 'F': OrdenaNomes(TabFuncionarios, TamL);
					  break;
					  
			case 'G': AtualizaSalarios(TabFuncionarios, TamL);
					  break;
					  
			case 'H': TempoEmpresa(TabFuncionarios, TamL);
					  break;
					  
			case 'I': OrdenaSalario(TabFuncionarios, TamL);
					  break;

			case 'J': InsereDados(TabFuncionarios, TamL);
					  break;

			case 'K': CadastroCargosOrdenaCodCres(TabCargos, TamLC);
					  break;

			case 'L': RelatorioCargos(TabCargos, TamLC, TabFuncionarios, TamL);
					  break;

			case 'M': ConsultarCargos(TabCargos, TamLC);
					  break;

			case 'N': DeletarCargos(TabCargos, TamLC);
					  break;

			case 'O': AlterarCargos(TabCargos, TamLC);
					  break;

			case 'P': InserirCargos(TabCargos, TamLC);
					  break;

			case 'Q': CadastroFiliaisOrdenaFili(TabFilial, TamLF);
					  break;

			case 'R': RelatorioFiliais(TabFilial, TamLF, TabFuncionarios, TabCargos, TamLC);
					  break;
			
			case 'S': ConsultarFiliais(TabFilial, TamLF);
					  break;

			case 'T': DeletarFiliais(TabFilial, TamLF);
					  break;

			case 'U': AlterarFiliais(TabFilial, TamLF);
					  break;		  

			case 'V': InserirFiliais(TabFilial, TamLF);
					  break;		  		    		 		  	     
		}	
	}while(opcao!=27);
	return 0;
}