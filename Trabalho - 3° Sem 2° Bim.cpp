#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <time.h>

//Definição de Structs (Registros de Dados)
struct TpData
{
	int dia, ano;
	char mes[4];	
};

struct TpPessoa
{
	int CodFunc;
	char Nome[30];
	TpData DtAdmissao;
	float Salario;
	int CodigoCargo;
	int Status;
};

struct TpCargo
{
	int Codigo;
	char Descricao[30];
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
	printf("[A] Cadastrar Funcionarios\n");
	printf("[B] Listar Funcionarios\n");
	printf("[C] Consultar Funcionarios\n");
	printf("[D] Deletar Funcionarios\n");
	printf("[E] Alterar Funcionarios\n");
	printf("[F] Ordenar por Bolhas pelo Nome do Funcionario de A-Z\n");
	printf("[G] Ordenar por Bolhas pelo Codigo do Funcionario (Crescente)\n");
	printf("[H] Atualizacao de Salario Geral\n");
	printf("[I] Tempo (em Anos) dos Funcionarios na Empresa\n");
	printf("[J] Ordenar Funcionarios pelo Salario (Decrescente)\n");
	printf("[K] Inserir Dados de Funcionarios Aleatorios\n");
	
	printf("\n[L] Cadastrar Cargos\n");
	printf("[M] Listar Cargos\n");
	printf("[N] Consultar Cargos\n");
	printf("[O] Deletar Cargos\n");
	printf("[P] Alterar Cargos\n");
	printf("[Q] Ordenar por Selecao Direta pelo Codigo do Cargo\n");
	printf("[R] Inserir Dados de Cargos Aleatorios\n");

	printf("\n[S] Cadastrar de Funcionarios em Filiais\n");
	printf("[T] Listar Funcionarios em Filiais\n");
	printf("[U] Inserir Dados de Funcionarios Aleatorios em Filiais\n");

	printf("\n[V] Listar Registros Completos\n");
	printf("[W] Listar Cargos e os Funcionarios que os ocupam\n");

	printf("\n[ESC] Sair do Programa\n");
	printf("\nPressione a opcao desejada: ");
	return toupper(getche());
}


//MÉTODOS DE BUSCA
//Busca Exaustiva com Sentinela
int BuscaCodigoFuncSent(int Codigo)
{
	FILE *PtrAux = fopen("Funcionario.dat", "ab+");
	TpPessoa RegAux1, RegAux2;
	fseek(PtrAux, 0, 2);
	int fim = ftell(PtrAux), i;
	RegAux1.CodFunc = Codigo;
	RegAux1.Status = 1;
	fwrite(&RegAux1, sizeof(TpPessoa), 1, PtrAux);
	rewind(PtrAux);
	fread(&RegAux1, sizeof(TpPessoa), 1, PtrAux);
	while (!(RegAux1.CodFunc==Codigo && RegAux1.Status==1))
		fread(&RegAux1, sizeof(TpPessoa), 1, PtrAux);	
	int parada = ftell(PtrAux);	
	if (parada<=fim && RegAux1.CodFunc==Codigo && RegAux1.Status==1)   //Achou o Codigo do Funcionario!
	{
		int retorna = ftell(PtrAux)-sizeof(TpPessoa);
		FILE *PtrBusca = fopen("Funcionario.dat", "rb+");
		fseek(PtrAux, 0, 2);
		i = ftell(PtrAux)-sizeof(TpPessoa);
		fseek(PtrBusca, i, 0);
		RegAux2.CodFunc = Codigo;
		RegAux2.Status = 0;
		fwrite(&RegAux2, sizeof(TpPessoa), 1, PtrBusca);
		fclose(PtrBusca);
		FILE *PtrTemp = fopen("Temp.dat", "wb");
		rewind(PtrAux);
		fread(&RegAux1, sizeof(TpPessoa), 1, PtrAux);
		while (!feof(PtrAux))
		{
			if (RegAux2.CodFunc!=RegAux1.CodFunc || RegAux1.Status==1)
				fwrite(&RegAux1, sizeof(TpPessoa), 1, PtrTemp);
			fread(&RegAux1, sizeof(TpPessoa), 1, PtrAux);	
		}				
		fclose(PtrTemp);
		fclose(PtrAux);						
		remove("Funcionario.dat");
		rename("Temp.dat", "Funcionario.dat");	
		return retorna;
	}
	else        //Nao Achou o Codigo do Funcionario procurado!
	{
		FILE *PtrBusca = fopen("Funcionario.dat", "rb+");
		fseek(PtrAux, 0, 2);
		i = ftell(PtrAux)-sizeof(TpPessoa);
		fseek(PtrBusca, i, 0);
		RegAux2.CodFunc = Codigo;
		RegAux2.Status = 0;
		fwrite(&RegAux2, sizeof(TpPessoa), 1, PtrBusca);
		fclose(PtrBusca);
		FILE *PtrTemp = fopen("Temp.dat", "wb");
		rewind(PtrAux);
		fread(&RegAux1, sizeof(TpPessoa), 1, PtrAux);
		while (!feof(PtrAux))
		{
			if (RegAux2.CodFunc!=RegAux1.CodFunc || RegAux1.Status==1)
				fwrite(&RegAux1, sizeof(TpPessoa), 1, PtrTemp);
			fread(&RegAux1, sizeof(TpPessoa), 1, PtrAux);	
		}				
		fclose(PtrTemp);
		fclose(PtrAux);						
		remove("Funcionario.dat");
		rename("Temp.dat", "Funcionario.dat");	
		return -1;
	}
	
}

//Busca Sequencial Indexada (apenas Ordenado)
int BuscaCodigoFuncSeque(FILE *Ptr, int Codigo)
{
	TpPessoa RegAux;
	rewind(Ptr);
	fread(&RegAux, sizeof(TpPessoa), 1, Ptr);
	while (!feof(Ptr) && Codigo>RegAux.CodFunc)
		fread(&RegAux, sizeof(TpPessoa), 1, Ptr);
	if (!feof(Ptr) && Codigo==RegAux.CodFunc)   //Achou o Nome na posicao 'i'!
		return ftell(Ptr)-sizeof(TpPessoa);
	else        //Nao Achou o Nome procurado!
		return -1;
}

//Busca Exaustiva
int BuscaCodigoCargo(FILE *Ptr, int Codigo)
{
	TpCargo RegAux;
	rewind(Ptr);
	fread(&RegAux, sizeof(TpCargo), 1, Ptr);
	while (!feof(Ptr) && RegAux.Codigo!=Codigo)
		fread(&RegAux, sizeof(TpCargo), 1, Ptr);
	if (!feof(Ptr))   //Achou o Codigo!
		return ftell(Ptr)-sizeof(TpCargo);
	else        //Nao Achou o Codigo procurado!
		return -1;
}

int BuscaCodigoFiliFunc(FILE *Ptr, char IdFIli[30], int Codigo)
{
	TpFilial RegAux;
	int igual=0, pos;
	rewind(Ptr);
	fread(&RegAux, sizeof(TpFilial), 1, Ptr);
	while (!feof(Ptr))
	{
		if (strcmp(RegAux.IdFilial, IdFIli)==0)
			if (RegAux.CodFunc==Codigo)
			{
				igual = 1;
				pos = ftell(Ptr)-sizeof(TpFilial);
			}
		fread(&RegAux, sizeof(TpFilial), 1, Ptr);
	}
	if (igual==1)   //Achou o Codigo!
		return pos;
	else        //Nao Achou o Codigo procurado!
		return -1;
}

//Busca Binária (apenas Ordenado)
int BuscaCodigoCargoBin(FILE *Ptr, char Codigo)
{
	TpCargo RegAux;
	int inicio=fseek(Ptr, 0, 0), fim, meio;
	fseek(Ptr, 0, 2);
	fim = ftell(Ptr)-sizeof(TpCargo);
	meio = fim/sizeof(TpCargo)/2*sizeof(TpCargo);
	fseek(Ptr, meio, 0);
	fread(&RegAux, sizeof(TpCargo), 1, Ptr);
	while (inicio<fim && Codigo!=RegAux.Codigo)
	{
		if (RegAux.Codigo<Codigo)
			inicio = meio+sizeof(TpCargo);
		else 
			fim = meio-sizeof(TpCargo);
		meio = (inicio+fim)/sizeof(TpCargo)/2*sizeof(TpCargo);
		fseek(Ptr, meio, 0);
		fread(&RegAux, sizeof(TpCargo), 1, Ptr);
	}
	if (Codigo==RegAux.Codigo)
		return ftell(Ptr)-sizeof(TpCargo);
	else 
		return -1;
}



void CadastroFuncionarios(void)
{
	TpPessoa RegFunc, RegAux;
	int i, fim;
	char resp;
	printf("\n\n*** CADASTRO DE FUNCIONARIOS ***\n");
	printf("\nCodigo do Funcionario: ");
	scanf("%d", &RegFunc.CodFunc);
	while (RegFunc.CodFunc>0)
	{
		printf("\nArquivo esta Ordenado?\n");
		if (toupper(getche())=='S')
		{
			FILE *Ptr = fopen("Funcionario.dat", "ab+");
			i = BuscaCodigoFuncSeque(Ptr, RegFunc.CodFunc);
			fclose(Ptr);
			printf("\n\n");
		}
		else
		{
			i = BuscaCodigoFuncSent(RegFunc.CodFunc);
			printf("\n\n");
		}
		if (i<0)
		{
			FILE *Ptr = fopen("Funcionario.dat", "ab+");
			printf("Nome do Funcionario: ");
			fflush(stdin);
			gets(RegFunc.Nome);
			printf("Dia da Admissao: ");
			scanf("%d", &RegFunc.DtAdmissao.dia);
			printf("Mes [as tres primeiras letras]: ");
			fflush(stdin);
			strupr(gets(RegFunc.DtAdmissao.mes));
			printf("Ano: ");
			scanf("%d", &RegFunc.DtAdmissao.ano);
			printf("Salario: R$ ");
			scanf("%f", &RegFunc.Salario);
			do
			{
				FILE *PtrCargo = fopen("Cargo.dat", "rb");
				printf("Codigo do Cargo: ");
				scanf("%d", &RegFunc.CodigoCargo);
				i = BuscaCodigoCargo(PtrCargo, RegFunc.CodigoCargo);
				fclose(PtrCargo);
				if (i>=0)
				{
					RegFunc.Status = 1;
					fwrite(&RegFunc, sizeof(TpPessoa), 1, Ptr);
					resp = 'N';
				}
				else
				{
					printf("\nCodido do Cargo nao Existente!");
					printf("\nDeseja tentar outro Codigo?\n");
					fflush(stdin);
					scanf("%c", &resp);
					printf("\n");
				}
			}while(toupper(resp)!='N');
			fclose(Ptr);
		}
		else
			printf("\nCodido do Funcionario ja existente!\n");
		printf("\nCodigo do Funcionario: ");
		scanf("%d", &RegFunc.CodFunc);	
	}
}

//Relatorio dos Funcionarios
void ListarFuncionarios(void)
{
	FILE *Ptr = fopen("Funcionario.dat", "rb");
	TpPessoa RegFunc;
	TpCargo RegCargo;
	char Descricao[30];
	int Codigo;
	printf("\n\n*** LISTA DE FUNCIONARIOS ***\n");
	if (Ptr == NULL)
		printf("\nErro de abertura do Arquivo");
	else
	{
		printf("--------------------------------------------------------------------------------------------------------\n");
		printf("CodFunc     Nome \t  Data de Admissao \t Salario R$ \t CodigoCargo \t Descricao do Cargo\n\n");
		fread(&RegFunc, sizeof(TpPessoa), 1, Ptr);
		while (!feof(Ptr))
		{
			if (RegFunc.Status==1)
			{
				Codigo = RegFunc.CodigoCargo;
				FILE *PtrCargo = fopen("Cargo.dat", "rb");
				rewind(PtrCargo);
				fread(&RegCargo, sizeof(TpCargo), 1, PtrCargo);
				while (!feof(Ptr) && RegCargo.Codigo!=Codigo)
					fread(&RegCargo, sizeof(TpCargo), 1, PtrCargo);
				fclose(PtrCargo);
				printf("%d \t    %s \t  %d/%s/%d \t\t R$%.2f \t %d \t\t %s\n", RegFunc.CodFunc, RegFunc.Nome, RegFunc.DtAdmissao.dia, RegFunc.DtAdmissao.mes, RegFunc.DtAdmissao.ano, RegFunc.Salario, RegFunc.CodigoCargo, RegCargo.Descricao);
			}
			fread(&RegFunc, sizeof(TpPessoa), 1, Ptr);
		}
		printf("--------------------------------------------------------------------------------------------------------\n");
		fclose(Ptr);
	}	
	getch();
}

void ConsultarFuncionarios(void)
{
	FILE *PtrAux = fopen("Funcionario.dat", "rb");
	TpPessoa RegFunc;
	int i;
	printf("\n\n*** CONSULTA DE FUNCIONARIOS ***\n");
	printf("\nCodigo a Consultar: ");
	scanf("%d", &RegFunc.CodFunc);
	if (PtrAux == NULL)
		printf("\nErro de abertura do Arquivo");
	else
	{
		fclose(PtrAux);
		while (RegFunc.CodFunc>0)
		{
			printf("\nArquivo esta Ordenado?\n");
			if (toupper(getche())=='S')
			{
				FILE *Ptr = fopen("Funcionario.dat", "rb");
				i = BuscaCodigoFuncSeque(Ptr, RegFunc.CodFunc);
				fclose(Ptr);
				printf("\n");
			}
			else
			{
				i = BuscaCodigoFuncSent(RegFunc.CodFunc);
				printf("\n");
			}
			if (i>=0)
			{
				FILE *Ptr = fopen("Funcionario.dat", "rb");
				fseek(Ptr, i, 0);
				fread(&RegFunc, sizeof(TpPessoa), 1, Ptr);
				printf("\n%d \t    %s \t  %d/%s/%d \t R$%.2f \t %d\nEncontrado em Funcinario.dat!\n", RegFunc.CodFunc, RegFunc.Nome, RegFunc.DtAdmissao.dia, RegFunc.DtAdmissao.mes, RegFunc.DtAdmissao.ano, RegFunc.Salario, RegFunc.CodigoCargo);
				fclose(Ptr);
			}
			else 
				printf("\nCodigo nao Cadastrado!\n");	
			getch();
			printf("\nCodigo a Consultar: ");
			scanf("%d", &RegFunc.CodFunc);
		}
	}
}

void DeletarFuncionarios(void)
{
	char resp;
	printf("\n\nExclusao Logica ou Fisica (L/F)?\n");
	resp = toupper(getche());
	do
	{		
		if (toupper(resp)=='L')
		{
			FILE *PtrAux = fopen("Funcionario.dat", "rb+");			
			TpPessoa RegFunc, BackRegFunc;
			int i;
			printf("\n\n*** EXCLUSAO LOGICA DE FUNCIONARIOS ***\n");
			printf("\nCodigo a Consultar: ");
			scanf("%d", &BackRegFunc.CodFunc);
			if (PtrAux == NULL)
				printf("\nErro de abertura do Arquivo");
			else
			{
				fclose(PtrAux);
				printf("\nArquivo esta Ordenado?\n");
				if (toupper(getche())=='S')
				{
					FILE *Ptr = fopen("Funcionario.dat", "rb+");
					i = BuscaCodigoFuncSeque(Ptr, BackRegFunc.CodFunc);
					fclose(Ptr);
					printf("\n");
				}
				else
				{
					i = BuscaCodigoFuncSent(BackRegFunc.CodFunc);
					printf("\n");
				}
				if (i>=0)
				{
					FILE *Ptr = fopen("Funcionario.dat", "rb+");
					fseek(Ptr, i, 0);
					fread(&RegFunc, sizeof(TpPessoa), 1, Ptr);
					printf("\n%d \t    %s \t  %d/%s/%d \t R$%.2f \t %d\nEncontrado em Funcinario.dat!\n", RegFunc.CodFunc, RegFunc.Nome, RegFunc.DtAdmissao.dia, RegFunc.DtAdmissao.mes, RegFunc.DtAdmissao.ano, RegFunc.Salario, RegFunc.CodigoCargo);
					printf("\nConfirma a Exclusao (S/N)?\n");
					if (toupper(getche())=='S')
					{
						strcpy(BackRegFunc.Nome, RegFunc.Nome);
						BackRegFunc.DtAdmissao.dia = RegFunc.DtAdmissao.dia;
						strcpy(BackRegFunc.DtAdmissao.mes, RegFunc.DtAdmissao.mes);
						BackRegFunc.DtAdmissao.ano = RegFunc.DtAdmissao.ano;
						BackRegFunc.Salario = RegFunc.Salario;
						BackRegFunc.CodigoCargo = RegFunc.CodigoCargo;
						RegFunc.Status = 0;
						fseek(Ptr, i, 0);
						fwrite(&RegFunc, sizeof(TpPessoa), 1, Ptr);			
						printf("\n\n%d \t    %s \t  %d/%s/%d \t R$%.2f \t %d\nFoi excluido!\n", BackRegFunc.CodFunc, BackRegFunc.Nome, BackRegFunc.DtAdmissao.dia, BackRegFunc.DtAdmissao.mes, BackRegFunc.DtAdmissao.ano, BackRegFunc.Salario, BackRegFunc.CodigoCargo);
					}
					else
						printf("\n");
					fclose(Ptr);
				}
				else 
					printf("\nCodigo nao Cadastrado!\n");	
				getch();
			}
		}
		else if (toupper(resp)=='F')
		{
			FILE *PtrAux = fopen("Funcionario.dat", "rb");
			TpPessoa RegFunc, BackRegFunc;
			int i;
			printf("\n\n*** EXCLUSAO FISICA DE FUNCIONARIOS ***\n");
			printf("\nCodigo a Consultar: ");
			scanf("%d", &BackRegFunc.CodFunc);
			if (PtrAux == NULL)
				printf("\nErro de abertura do Arquivo");
			else
			{
				fclose(PtrAux);
				printf("\nArquivo esta Ordenado?\n");
				if (toupper(getche())=='S')
				{
					FILE *Ptr = fopen("Funcionario.dat", "rb");
					i = BuscaCodigoFuncSeque(Ptr, BackRegFunc.CodFunc);
					fclose(Ptr);
					printf("\n");
				}
				else
				{
					i = BuscaCodigoFuncSent(BackRegFunc.CodFunc);
					printf("\n");
				}
				if (i>=0)
				{
					FILE *Ptr = fopen("Funcionario.dat", "rb");
					fseek(Ptr, i, 0);
					fread(&RegFunc, sizeof(TpPessoa), 1, Ptr);
					printf("\n%d \t    %s \t  %d/%s/%d \t R$%.2f \t %d\nEncontrado em Funcinario.dat!\n", RegFunc.CodFunc, RegFunc.Nome, RegFunc.DtAdmissao.dia, RegFunc.DtAdmissao.mes, RegFunc.DtAdmissao.ano, RegFunc.Salario, RegFunc.CodigoCargo);
					printf("\nConfirma a Exclusao (S/N)?\n");
					if (toupper(getche())=='S')
					{
						strcpy(BackRegFunc.Nome, RegFunc.Nome);
						BackRegFunc.DtAdmissao.dia = RegFunc.DtAdmissao.dia;
						strcpy(BackRegFunc.DtAdmissao.mes, RegFunc.DtAdmissao.mes);
						BackRegFunc.DtAdmissao.ano = RegFunc.DtAdmissao.ano;
						BackRegFunc.Salario = RegFunc.Salario;
						BackRegFunc.CodigoCargo = RegFunc.CodigoCargo;
						FILE *PtrTemp = fopen("Temp.dat", "wb");
						rewind(Ptr);
						fread(&RegFunc, sizeof(TpPessoa), 1, Ptr);
						while (!feof(Ptr))
						{
							if (BackRegFunc.CodFunc!=RegFunc.CodFunc)
								fwrite(&RegFunc, sizeof(TpPessoa), 1, PtrTemp);
							fread(&RegFunc, sizeof(TpPessoa), 1, Ptr);	
						}				
						fclose(PtrTemp);
						fclose(Ptr);						
						remove("Funcionario.dat");
						rename("Temp.dat", "Funcionario.dat");
						printf("\n\n%d \t    %s \t  %d/%s/%d \t R$%.2f \t %d\nFoi excluido!\n", BackRegFunc.CodFunc, BackRegFunc.Nome, BackRegFunc.DtAdmissao.dia, BackRegFunc.DtAdmissao.mes, BackRegFunc.DtAdmissao.ano, BackRegFunc.Salario, BackRegFunc.CodigoCargo);
					}
					else
						printf("\n");
				}
				else 
					printf("\nCodigo nao Cadastrado!\n");	
				getch();		
			}
		}
		printf("\n\nExclusao Logica ou Fisica (L/F)?\n");
		resp = toupper(getche());
	}while(resp!=27);
}

void AlterarFuncionarios(void)
{
	FILE *PtrAux = fopen("Funcionario.dat", "rb+");
	TpPessoa RegFunc, BackRegFunc;
	char resp, ordenado;
	int i, j;
	printf("\n\n*** ALTERACAO DE FUNCIONARIOS ***\n");	
	printf("\nCodigo a Consultar: ");
	scanf("%d", &BackRegFunc.CodFunc);
	if (PtrAux == NULL)
		printf("\nErro de abertura do Arquivo");
	else
	{
		fclose(PtrAux);
		while (BackRegFunc.CodFunc>0)
		{
			printf("\nArquivo esta Ordenado?\n");
			fflush(stdin);
			scanf("%c", &ordenado);
			if (toupper(ordenado)=='S')
			{
				FILE *Ptr = fopen("Funcionario.dat", "rb+");
				i = BuscaCodigoFuncSeque(Ptr, BackRegFunc.CodFunc);
				fclose(Ptr);
				printf("\n\n");
			}
			else
			{
				i = BuscaCodigoFuncSent(BackRegFunc.CodFunc);
				printf("\n");
			}
			if (i>=0)
			{
				FILE *Ptr = fopen("Funcionario.dat", "rb+");
				fseek(Ptr, i, 0);
				fread(&RegFunc, sizeof(TpPessoa), 1, Ptr);
				printf("\n%d \t    %s \t  %d/%s/%d \t R$%.2f \t %d\nEncontrado em Funcinario.dat!\n", RegFunc.CodFunc, RegFunc.Nome, RegFunc.DtAdmissao.dia, RegFunc.DtAdmissao.mes, RegFunc.DtAdmissao.ano, RegFunc.Salario, RegFunc.CodigoCargo);
				printf("\nConfirma a Alteracao (S/N)?\n");
				if (toupper(getche())=='S')
				{
					strcpy(BackRegFunc.Nome, RegFunc.Nome);
					BackRegFunc.DtAdmissao.dia = RegFunc.DtAdmissao.dia;
					strcpy(BackRegFunc.DtAdmissao.mes, RegFunc.DtAdmissao.mes);
					BackRegFunc.DtAdmissao.ano = RegFunc.DtAdmissao.ano;
					BackRegFunc.Salario = RegFunc.Salario;
					BackRegFunc.CodigoCargo = RegFunc.CodigoCargo;
					RegFunc.CodFunc = 0;
					fseek(Ptr, i, 0);
					fwrite(&RegFunc, sizeof(TpPessoa), 1, Ptr);
					do
					{
						fclose(Ptr);
						printf("\n\nNovo Codigo: ");
						scanf("%d", &RegFunc.CodFunc);
						if (toupper(ordenado)=='S')
						{
							FILE *Ptr = fopen("Funcionario.dat", "rb+");
							j = BuscaCodigoFuncSeque(Ptr, RegFunc.CodFunc);
							fclose(Ptr);
						}
						else
							j = BuscaCodigoFuncSent(RegFunc.CodFunc);
						if (j<0)
						{
							printf("Novo Nome: ");
							fflush(stdin);
							gets(RegFunc.Nome);
							printf("Novo Dia de Admissao: ");
							scanf("%d", &RegFunc.DtAdmissao.dia);
							printf("Novo Mes [as tres primeiras letras]: ");
							fflush(stdin);
							strupr(gets(RegFunc.DtAdmissao.mes));
							printf("Novo Ano: ");
							scanf("%d", &RegFunc.DtAdmissao.ano);
							printf("Novo Salario: ");
							scanf("%f", &RegFunc.Salario);
							do
							{
								FILE *PtrCargo = fopen("Cargo.dat", "rb");
								printf("Novo Codigo do Cargo: ");
								scanf("%d", &RegFunc.CodigoCargo);														
								j = BuscaCodigoCargo(PtrCargo, RegFunc.CodigoCargo);
								fclose(PtrCargo);
								if (j>=0)
								{
									FILE *Ptr = fopen("Funcionario.dat", "rb+");
									fseek(Ptr, i, 0);
									fwrite(&RegFunc, sizeof(TpPessoa), 1, Ptr);
									printf("\n\n%d \t    %s \t  %d/%s/%d \t R$%.2f \t %d\n\nFoi Alterado para\n", BackRegFunc.CodFunc, BackRegFunc.Nome, BackRegFunc.DtAdmissao.dia, BackRegFunc.DtAdmissao.mes, BackRegFunc.DtAdmissao.ano, BackRegFunc.Salario, BackRegFunc.CodigoCargo);
									printf("\n%d \t    %s \t  %d/%s/%d \t R$%.2f \t %d\n", RegFunc.CodFunc, RegFunc.Nome, RegFunc.DtAdmissao.dia, RegFunc.DtAdmissao.mes, RegFunc.DtAdmissao.ano, RegFunc.Salario, RegFunc.CodigoCargo);
									resp = 'N';
								}
								else
								{
									printf("\nCodigo do Cargo nao Existente!\n");
									printf("\nDeseja tentar outro Codigo?\n");
									fflush(stdin);
									scanf("%c", &resp);
									printf("\n");
								}
							}while(toupper(resp)!='N');							
						}
						else
						{
							printf("\nCodigo ja Cadastrado!\n");
							printf("\nDeseja tentar outro Codigo?\n");
							fflush(stdin);
							scanf("%c", &resp);
						}
					}while(toupper(resp)!='N');
					FILE *Ptr = fopen("Funcionario.dat", "rb+");
					RegFunc.CodFunc = BackRegFunc.CodFunc;
					fseek(Ptr, i, 0);
					fwrite(&RegFunc, sizeof(TpPessoa), 1, Ptr);
				}
				else
					printf("\n");
				fclose(Ptr);
			}
			else 
				printf("\nCodigo nao Cadastrado!\n");
			getch();
			printf("\nCodigo a Consultar: ");
			scanf("%d", &BackRegFunc.CodFunc);
		}
	}
}

//Ordenação por Bolhas
void OrdenaNomeBolhaFuncionarios(void)
{
	FILE *Ptr = fopen("Funcionario.dat", "rb+");
	TpPessoa RegAux1, RegAux2;
	int i, QuantReg;
	printf("\n\n *** ORDENACAO POR BOLHAS PELO NOME DO FUNCIONARIO DE A-Z***\n");
	if (Ptr == NULL)
		printf("\nErro de abertura do Arquivo!\n");
	else
	{
		fseek(Ptr, 0, 2);
		QuantReg = ftell(Ptr)/sizeof(TpPessoa);
		while (QuantReg>0)
		{
			for (i=0; i<QuantReg-1; i++)
			{
				fseek(Ptr, i*sizeof(TpPessoa), 0);
				fread(&RegAux1, sizeof(TpPessoa), 1, Ptr);
				fseek(Ptr, (i+1)*sizeof(TpPessoa), 0);
				fread(&RegAux2, sizeof(TpPessoa), 1, Ptr);
				if (stricmp(RegAux1.Nome, RegAux2.Nome)>0)
				{
					fseek(Ptr, i*sizeof(TpPessoa), 0);
					fwrite(&RegAux2, sizeof(TpPessoa), 1, Ptr);
					fseek(Ptr, (i+1)*sizeof(TpPessoa), 0);
					fwrite(&RegAux1, sizeof(TpPessoa), 1, Ptr);
				}
			}
			QuantReg--;
		}
		printf("\n*** TABELA DE FUNCIONARIOS ORDENADA ***\n");
		printf("-------------------------------------------------------------------------------\n");
		printf("CodFunc     Nome \t  Data de Admissao \t Salario R$ \t CodigoCargo\n\n");
		rewind(Ptr);
		fread(&RegAux1, sizeof(TpPessoa), 1, Ptr);
		while (!feof(Ptr))
		{
			if (RegAux1.Status==1)
				printf("%d \t    %s \t  %d/%s/%d \t\t R$%.2f \t %d\n", RegAux1.CodFunc, RegAux1.Nome, RegAux1.DtAdmissao.dia, RegAux1.DtAdmissao.mes, RegAux1.DtAdmissao.ano, RegAux1.Salario, RegAux1.CodigoCargo);
			fread(&RegAux1, sizeof(TpPessoa), 1, Ptr);
		}
		printf("-------------------------------------------------------------------------------\n");
		fclose(Ptr);
	}
	getch();
}

void OrdenaCodigoBolhaFuncionarios(void)
{
	FILE *Ptr = fopen("Funcionario.dat", "rb+");
	TpPessoa RegAux1, RegAux2;
	int i, QuantReg;
	printf("\n\n *** ORDENACAO POR BOLHAS PELO CODIGO DO FUNCIONARIO (CRESCENTE)***\n");
	if (Ptr == NULL)
		printf("\nErro de abertura do Arquivo!\n");
	else
	{
		fseek(Ptr, 0, 2);
		QuantReg = ftell(Ptr)/sizeof(TpPessoa);
		while (QuantReg>0)
		{
			for (i=0; i<QuantReg-1; i++)
			{
				fseek(Ptr, i*sizeof(TpPessoa), 0);
				fread(&RegAux1, sizeof(TpPessoa), 1, Ptr);
				fseek(Ptr, (i+1)*sizeof(TpPessoa), 0);
				fread(&RegAux2, sizeof(TpPessoa), 1, Ptr);
				if (RegAux1.CodFunc>RegAux2.CodFunc)
				{
					fseek(Ptr, i*sizeof(TpPessoa), 0);
					fwrite(&RegAux2, sizeof(TpPessoa), 1, Ptr);
					fseek(Ptr, (i+1)*sizeof(TpPessoa), 0);
					fwrite(&RegAux1, sizeof(TpPessoa), 1, Ptr);
				}
			}
			QuantReg--;
		}
		printf("\n*** TABELA DE FUNCIONARIOS ORDENADA ***\n");
		printf("-------------------------------------------------------------------------------\n");
		printf("CodFunc     Nome \t  Data de Admissao \t Salario R$ \t CodigoCargo\n\n");
		rewind(Ptr);
		fread(&RegAux1, sizeof(TpPessoa), 1, Ptr);
		while (!feof(Ptr))
		{
			if (RegAux1.Status==1)
				printf("%d \t    %s \t  %d/%s/%d \t\t R$%.2f \t %d\n", RegAux1.CodFunc, RegAux1.Nome, RegAux1.DtAdmissao.dia, RegAux1.DtAdmissao.mes, RegAux1.DtAdmissao.ano, RegAux1.Salario, RegAux1.CodigoCargo);
			fread(&RegAux1, sizeof(TpPessoa), 1, Ptr);
		}
		printf("-------------------------------------------------------------------------------\n");
		fclose(Ptr);
	}
	getch();
}

void AtualizaSalariosFuncionarios(void)
{
	FILE *Ptr = fopen("Funcionario.dat", "rb+");
	TpPessoa RegFunc;
	float Percentual;
	int i, QuantReg;
	printf("\n\n *** ATUALIZAR SALARIO GERAL ***\n");
	if (Ptr == NULL)
		printf("\nErro de abertura do Arquivo!\n");
	else
	{
		fseek(Ptr, 0, 2);
		QuantReg = ftell(Ptr)/sizeof(TpPessoa);
		printf("\nQual o percentual de Atualizacao?\n");
		scanf("%f", &Percentual);
		for (i=0; i<QuantReg; i++)
		{
			fseek(Ptr, i*sizeof(TpPessoa), 0);
			fread(&RegFunc, sizeof(TpPessoa), 1, Ptr);
			RegFunc.Salario = RegFunc.Salario+RegFunc.Salario*Percentual/100;
			fseek(Ptr, i*sizeof(TpPessoa), 0);
			fwrite(&RegFunc, sizeof(TpPessoa), 1, Ptr);
		}
		printf("\n*** SALARIOS ATUALIZADOS ***\n");
		printf("-------------------------------------------------------------------------------\n");
		printf("CodFunc     Nome \t  Data de Admissao \t Salario R$ \t CodigoCargo\n\n");
		rewind(Ptr);
		fread(&RegFunc, sizeof(TpPessoa), 1, Ptr);
		while (!feof(Ptr))
		{
			if (RegFunc.Status==1)
				printf("%d \t    %s \t  %d/%s/%d \t\t R$%.2f \t %d\n", RegFunc.CodFunc, RegFunc.Nome, RegFunc.DtAdmissao.dia, RegFunc.DtAdmissao.mes, RegFunc.DtAdmissao.ano, RegFunc.Salario, RegFunc.CodigoCargo);
			fread(&RegFunc, sizeof(TpPessoa), 1, Ptr);
		}
		printf("-------------------------------------------------------------------------------\n");
		fclose(Ptr);
	}
	getch();
}

void TempoEmpresaFuncionarios(void)
{
	FILE *Ptr = fopen("Funcionario.dat", "rb");
	TpPessoa RegFunc;
	int AnoAtual;
	printf("\n\n*** TEMPO DOS FUNCIONARIOS NA EMPRESA ***\n");
	if (Ptr == NULL)
		printf("\nErro de abertura do Arquivo!\n");
	else 
	{
		printf("\nInforme o Ano Atual: ");
		scanf("%d", &AnoAtual);
		printf("\n*** TEMPO DOS FUNCIONARIOS NA EMPRESA ***\n");
		printf("----------------------------------------------------------------------------------------------\n");
		printf("CodFunc     Nome \t  Data de Admissao \t Salario R$ \t CodigoCargo \t Tempo (anos)\n\n");
		fread(&RegFunc, sizeof(TpPessoa), 1, Ptr);
		while (!feof(Ptr))
		{
			if (RegFunc.Status==1)
				printf("%d \t    %s \t  %d/%s/%d \t\t R$%.2f \t %d \t\t %d\n", RegFunc.CodFunc, RegFunc.Nome, RegFunc.DtAdmissao.dia, RegFunc.DtAdmissao.mes, RegFunc.DtAdmissao.ano, RegFunc.Salario, RegFunc.CodigoCargo, AnoAtual-RegFunc.DtAdmissao.ano);
			fread(&RegFunc, sizeof(TpPessoa), 1, Ptr);
		}
		printf("----------------------------------------------------------------------------------------------\n");
		fclose(Ptr);
	}
	getch();
}

void OrdenaSalarioFuncionarios(void)
{
	FILE *Ptr = fopen("Funcionario.dat", "rb+");
	TpPessoa RegAux1, RegAux2;
	int i, j, QuantReg;
	printf("\n\n*** ORDENAR FUNCIONARIOS PELO SALARIO (DECRESCENTE) ***\n");
	if (Ptr == NULL)
		printf("\nErro de abertura do Arquivo!\n");
	else
	{
		fseek(Ptr, 0, 2);
		QuantReg = ftell(Ptr)/sizeof(TpPessoa);
		while (QuantReg>0)
		{
			for (i=0; i<QuantReg-1; i++)
			{
				fseek(Ptr, i*sizeof(TpPessoa), 0);
				fread(&RegAux1, sizeof(TpPessoa), 1, Ptr);
				fseek(Ptr, (i+1)*sizeof(TpPessoa), 0);
				fread(&RegAux2, sizeof(TpPessoa), 1, Ptr);
				if (RegAux1.Salario<RegAux2.Salario)
				{
					fseek(Ptr, i*sizeof(TpPessoa), 0);
					fwrite(&RegAux2, sizeof(TpPessoa), 1, Ptr);
					fseek(Ptr, (i+1)*sizeof(TpPessoa), 0);
					fwrite(&RegAux1, sizeof(TpPessoa), 1, Ptr);
				}
			}
			QuantReg--;
		}
		printf("\n*** ORDENAR FUNCIONARIOS PELO SALARIO (DECRESCENTE) ***\n");
		printf("-------------------------------------------------------------------------------\n");
		printf("CodFunc     Nome \t  Data de Admissao \t Salario R$ \t CodigoCargo\n\n");
		rewind(Ptr);
		fread(&RegAux1, sizeof(TpPessoa), 1, Ptr);
		while (!feof(Ptr))
		{
			if (RegAux1.Status==1)
				printf("%d \t    %s \t  %d/%s/%d \t\t R$%.2f \t %d\n", RegAux1.CodFunc, RegAux1.Nome, RegAux1.DtAdmissao.dia, RegAux1.DtAdmissao.mes, RegAux1.DtAdmissao.ano, RegAux1.Salario, RegAux1.CodigoCargo);
			fread(&RegAux1, sizeof(TpPessoa), 1, Ptr);
		}
		printf("-------------------------------------------------------------------------------\n");
		fclose(Ptr);
	}
	getch();
}

void InsereDadosFuncionarios(void)
{
	FILE *Ptr = fopen("Funcionario.dat","ab");
	TpPessoa RegFunc;
	
	RegFunc.CodFunc = 1;
	strcpy(RegFunc.Nome,"Rafael");
	RegFunc.DtAdmissao.dia = 01;
	strcpy(RegFunc.DtAdmissao.mes,"MAR");
	RegFunc.DtAdmissao.ano = 2002;
	RegFunc.Salario = 9600;
	RegFunc.CodigoCargo = 7;
	RegFunc.Status = 1;
	fwrite(&RegFunc, sizeof(TpPessoa), 1, Ptr);

	RegFunc.CodFunc = 5;
	strcpy(RegFunc.Nome,"Ivo");
	RegFunc.DtAdmissao.dia = 14;
	strcpy(RegFunc.DtAdmissao.mes,"SET");
	RegFunc.DtAdmissao.ano = 1990;
	RegFunc.Salario = 7800;
	RegFunc.CodigoCargo = 7;
	RegFunc.Status = 1;
	fwrite(&RegFunc, sizeof(TpPessoa), 1, Ptr);
	
	RegFunc.CodFunc = 15;
	strcpy(RegFunc.Nome,"Ana");
	RegFunc.DtAdmissao.dia = 15;
	strcpy(RegFunc.DtAdmissao.mes,"FEV");
	RegFunc.DtAdmissao.ano = 2018;
	RegFunc.Salario = 8900;
	RegFunc.CodigoCargo = 3;
	RegFunc.Status = 1;
	fwrite(&RegFunc, sizeof(TpPessoa), 1, Ptr);

	RegFunc.CodFunc = 11;
	strcpy(RegFunc.Nome,"Eva");
	RegFunc.DtAdmissao.dia = 06;
	strcpy(RegFunc.DtAdmissao.mes,"MAR");
	RegFunc.DtAdmissao.ano = 2021;
	RegFunc.Salario = 8500;
	RegFunc.CodigoCargo = 3;
	RegFunc.Status = 1;
	fwrite(&RegFunc, sizeof(TpPessoa), 1, Ptr);
	
	RegFunc.CodFunc = 7;
	strcpy(RegFunc.Nome,"Abel");
	RegFunc.DtAdmissao.dia = 02;
	strcpy(RegFunc.DtAdmissao.mes,"OUT");
	RegFunc.DtAdmissao.ano = 2020;
	RegFunc.Salario = 6570;
	RegFunc.CodigoCargo = 1;
	RegFunc.Status = 1;
	fwrite(&RegFunc, sizeof(TpPessoa), 1, Ptr);

	fclose(Ptr);
}


//Inserção Direta
void InsercaoNomeDiretaCargos(FILE *Ptr)
{
	TpCargo RegAux1, RegAux2;
	fseek(Ptr, 0, 2);
	int i=ftell(Ptr)/sizeof(TpCargo)-1;
	fseek(Ptr, i*sizeof(TpCargo), 0);
	fread(&RegAux1, sizeof(TpCargo), 1, Ptr);
	fseek(Ptr, (i-1)*sizeof(TpCargo), 0);
	fread(&RegAux2, sizeof(TpCargo), 1, Ptr);
	while (i>0 && RegAux1.Codigo<RegAux2.Codigo)
	{
		
		fseek(Ptr, i*sizeof(TpCargo), 0);
		fwrite(&RegAux2, sizeof(TpCargo), 1, Ptr);
		fseek(Ptr, (i-1)*sizeof(TpCargo), 0);
		fwrite(&RegAux1, sizeof(TpCargo), 1, Ptr);
		i--;
		fseek(Ptr, i*sizeof(TpCargo), 0);
		fread(&RegAux1, sizeof(TpCargo), 1, Ptr);
		fseek(Ptr, (i-1)*sizeof(TpCargo), 0);
		fread(&RegAux2, sizeof(TpCargo), 1, Ptr);
	}
}

//Relatorio dos Cargos
void CadastroCargos(void)
{
	FILE *Ptr = fopen("Cargo.dat", "ab+");
	TpCargo RegCargo;
	int i;
	char resp;
	printf("\n\n*** CADASTRO DE CARGOS ***\n");
	printf("\nCodigo do Cargo: ");
	scanf("%d", &RegCargo.Codigo);
	while (RegCargo.Codigo>0)
	{		
		printf("\nArquivo esta Ordenado?\n");
		if (toupper(getche())=='S')
			i = BuscaCodigoCargoBin(Ptr, RegCargo.Codigo);
		else
			i = BuscaCodigoCargo(Ptr, RegCargo.Codigo);
		if (i<0)
		{
			printf("\n\nDescricao do Cargo: ");
			fflush(stdin);
			gets(RegCargo.Descricao);
			resp = 'N';
			printf("\nArquivo esta Ordenado?\n");
			if (toupper(getche())=='S')
			{
				FILE *PtrInsercaoDireta = fopen("Cargo.dat", "rb+");
				fseek(PtrInsercaoDireta, 0, 2);
				fwrite(&RegCargo, sizeof(TpCargo), 1, PtrInsercaoDireta);
				InsercaoNomeDiretaCargos(PtrInsercaoDireta);
				fclose(PtrInsercaoDireta);
			}	
			else
				fwrite(&RegCargo, sizeof(TpCargo), 1, Ptr);
		}
		else
			printf("\nCodigo ja existente!");
		printf("\n\nCodigo do Cargo: ");
		scanf("%d", &RegCargo.Codigo);
	}
	fclose(Ptr);
}

void ListarCargos(void)
{
	FILE *Ptr = fopen("Cargo.dat", "rb");
	TpCargo RegCargo;
	printf("\n\n*** LISTA DE CARGOS ***\n");
	if (Ptr == NULL)
		printf("\nErro de abertura do Arquivo");
	else
	{
		printf("---------------------------------------\n");
		printf("Codigo \t Descricao do Cargo\n\n");
		fread(&RegCargo, sizeof(TpCargo), 1, Ptr);
		while (!feof(Ptr))
		{
			printf("%d \t %s\n", RegCargo.Codigo, RegCargo.Descricao);
			fread(&RegCargo, sizeof(TpCargo), 1, Ptr);
		}
		printf("---------------------------------------\n");
		fclose(Ptr);
	}	
	getch();
}

void ConsultarCargos(void) 
{
	FILE *Ptr = fopen("Cargo.dat", "rb");
	TpCargo RegCargo;
	int i;
	printf("\n\n*** CONSULTA DE CARGOS ***\n");
	printf("\nCodigo a Consultar: ");
	scanf("%d", &RegCargo.Codigo);
	if (Ptr == NULL)
		printf("\nErro de abertura do Arquivo");
	while (RegCargo.Codigo>0)
	{
		printf("\nArquivo esta Ordenado?\n");
		if (toupper(getche())=='S')
			i = BuscaCodigoCargoBin(Ptr, RegCargo.Codigo);
		else
			i = BuscaCodigoCargo(Ptr, RegCargo.Codigo);
		if (i>=0)
		{
			fseek(Ptr, i, 0);
			fread(&RegCargo, sizeof(TpCargo), 1, Ptr);
			printf("\n\n%d \t %s\nEncontrado Cargo.dat!\n", RegCargo.Codigo, RegCargo.Descricao, i);
		}
		else 
			printf("\n\nCodigo nao Cadastrado!\n");	
		getch();
		printf("\nCodigo a Consultar: ");
		scanf("%d", &RegCargo.Codigo);
	}
}

void DeletarCargos(void)
{
	FILE *Ptr = fopen("Cargo.dat", "rb");
	TpCargo RegCargo, BackRegCargo;
	int i;
	printf("\n\n*** EXCLUSAO FISICA DE CARGOS ***\n");
	printf("\nCodigo a Consultar: ");
	scanf("%d", &BackRegCargo.Codigo);
	if (Ptr == NULL)
		printf("\nErro de abertura do Arquivo");
	else
	{
		printf("\nArquivo esta Ordenado?\n");
		if (toupper(getche())=='S')
			i = BuscaCodigoCargoBin(Ptr, BackRegCargo.Codigo);
		else
			i = BuscaCodigoCargo(Ptr, BackRegCargo.Codigo);
		if (i>=0)
		{
			fseek(Ptr, i, 0);
			fread(&RegCargo, sizeof(TpCargo), 1, Ptr);
			printf("\n\n%d \t %s\nEncontrado em Cargo.dat!\n", RegCargo.Codigo, RegCargo.Descricao);
			printf("\nConfirma a Exclusao (S/N)?\n");
			if (toupper(getche())=='S')
			{
				strcpy(BackRegCargo.Descricao, RegCargo.Descricao);
				FILE *PtrTemp = fopen("Temp.dat", "wb");
				rewind(Ptr);
				fread(&RegCargo, sizeof(TpCargo), 1, Ptr);
				while (!feof(Ptr))
				{
					if (BackRegCargo.Codigo!=RegCargo.Codigo)
						fwrite(&RegCargo, sizeof(TpCargo), 1, PtrTemp);
					fread(&RegCargo, sizeof(TpCargo), 1, Ptr);	
				}				
				fclose(PtrTemp);
				fclose(Ptr);						
				remove("Cargo.dat");
				rename("Temp.dat", "Cargo.dat");
				printf("\n\n%d \t %s\nFoi excluido!\n", BackRegCargo.Codigo, BackRegCargo.Descricao);
			}
			else
				printf("\n");
		}
		else 
			printf("\n\nCodigo nao Cadastrado!\n");	
		getch();		
	}
}

void AlterarCargos(void)
{
	FILE *Ptr = fopen("Cargo.dat", "rb+");
	TpCargo RegCargo, BackRegCargo;
	char resp;
	int i, j;
	printf("\n\n*** ALTERACAO DE CARGOS ***\n");	
	printf("\nCodigo a Consultar: ");
	scanf("%d", &BackRegCargo.Codigo);
	if (Ptr == NULL)
		printf("\nErro de abertura do Arquivo");
	else
	{
		while (BackRegCargo.Codigo>0)
		{
			printf("\nArquivo esta Ordenado?\n");
			if (toupper(getche())=='S')
				i = BuscaCodigoCargoBin(Ptr, BackRegCargo.Codigo);
			else
				i = BuscaCodigoCargo(Ptr, BackRegCargo.Codigo);
			if (i>=0)
			{
				fseek(Ptr, i, 0);
				fread(&RegCargo, sizeof(TpCargo), 1, Ptr);
				printf("\n\n%d \t %s\nEncontrado em Cargo.dat!\n", RegCargo.Codigo, RegCargo.Descricao);
				printf("\nConfirma a Alteracao (S/N)?\n");
				if (toupper(getche())=='S')
				{
					strcpy(BackRegCargo.Descricao, RegCargo.Descricao);
					RegCargo.Codigo = 0;
					fseek(Ptr, i, 0);
					fwrite(&RegCargo, sizeof(TpCargo), 1, Ptr);
					do
					{
						printf("\n\nNovo Codigo: ");
						scanf("%d", &RegCargo.Codigo);
						j = BuscaCodigoCargo(Ptr, RegCargo.Codigo);
						if (j<0)
						{
							printf("Nova Descricao: ");
							fflush(stdin);
							gets(RegCargo.Descricao);
							fseek(Ptr, i, 0);
							fwrite(&RegCargo, sizeof(TpCargo), 1, Ptr);
							printf("\n%d \t %s\n\nFoi Alterado para\n", BackRegCargo.Codigo, BackRegCargo.Descricao);
							printf("\n%d \t %s\n", RegCargo.Codigo, RegCargo.Descricao);
							resp = 'N';											
						}
						else
						{
							printf("\nCodigo ja Cadastrado!\n");
							printf("\nDeseja tentar outro Codigo?\n");
							fflush(stdin);
							scanf("%c", &resp);
						}
					}while(toupper(resp)!='N');
					
				}
				else
					printf("\n");
			}
			else 
				printf("\n\nCodigo nao Cadastrado!\n");
			getch();
			printf("\nCodigo a Consultar: ");
			scanf("%d", &BackRegCargo.Codigo);
		}
		fclose(Ptr);
	}
}

//achar Posição Maior
int PosicaoMaior(FILE *Ptr, int fim)
{
	TpCargo RegMaior, RegAux;
	int posMaior, i, QuantReg;
	fseek(Ptr, 0, 0);
	fread(&RegMaior, sizeof(TpCargo), 1, Ptr);
	posMaior = 0;	
	for (i=1; i<fim; i++)
	{
		fseek(Ptr, i*sizeof(TpCargo), 0);
		fread(&RegAux, sizeof(TpCargo), 1, Ptr);
		if (RegMaior.Codigo<RegAux.Codigo)
		{
			fseek(Ptr, i*sizeof(TpCargo), 0);
			fread(&RegMaior, sizeof(TpCargo), 1, Ptr);
			posMaior = i;
		}
	}
	return posMaior;
}
//Seleção Direta
void SelecaoCodigoDiretaCargos(void)
{
	FILE *Ptr = fopen("Cargo.dat", "rb+");
	TpCargo RegAux1, RegAux2;
	int fim, posMaior;
	fseek(Ptr, 0, 2);
	fim = ftell(Ptr)/sizeof(TpCargo);
	printf("\n\n *** ORDENAR POR SELECAO DIRETA PELO CODIGO DO CARGO ***\n");
	if (Ptr == NULL)
		printf("\nErro de abertura do Arquivo!\n");
	else
	{
		while (fim>0)
		{
			posMaior = PosicaoMaior(Ptr, fim);
			fseek(Ptr, posMaior*sizeof(TpCargo), 0);
			fread(&RegAux1, sizeof(TpCargo), 1, Ptr);
			fseek(Ptr, (fim-1)*sizeof(TpCargo), 0);
			fread(&RegAux2, sizeof(TpCargo), 1, Ptr);
			if (posMaior<fim-1)
			{
				fseek(Ptr, posMaior*sizeof(TpCargo), 0);
				fwrite(&RegAux2, sizeof(TpCargo), 1, Ptr);
				fseek(Ptr, (fim-1)*sizeof(TpCargo), 0);
				fwrite(&RegAux1, sizeof(TpCargo), 1, Ptr);
			}
			fim--;
		}
		printf("\n*** TABELA DE FUNCIONARIOS ORDENADA ***\n");
		printf("\n---------------------------------------\n");
		printf("Codigo \t Descricao do Cargo\n\n");
		rewind(Ptr);
		fread(&RegAux1, sizeof(TpCargo), 1, Ptr);
		while (!feof(Ptr))
		{
			printf("%d \t %s\n", RegAux1.Codigo, RegAux1.Descricao);
			fread(&RegAux1, sizeof(TpCargo), 1, Ptr);
		}
		printf("---------------------------------------\n");
		fclose(Ptr);
	}
	getch();
}

void InserirCargos(void)
{
	FILE *Ptr = fopen("Cargo.dat", "ab");
	TpCargo RegCargo;

	RegCargo.Codigo = 3;
	strcpy(RegCargo.Descricao,"Gestor de TI");
	fwrite(&RegCargo, sizeof(TpCargo), 1, Ptr);

	RegCargo.Codigo = 2;
	strcpy(RegCargo.Descricao,"Gestor de Redes");
	fwrite(&RegCargo, sizeof(TpCargo), 1, Ptr);

	RegCargo.Codigo = 7;
	strcpy(RegCargo.Descricao,"Operador de Maquinas");
	fwrite(&RegCargo, sizeof(TpCargo), 1, Ptr);

	RegCargo.Codigo = 1;
	strcpy(RegCargo.Descricao,"Diretor Geral");
	fwrite(&RegCargo, sizeof(TpCargo), 1, Ptr);

	fclose(Ptr);
}


void CadastroFuncionariosFiliais(void)
{
	FILE *Ptr = fopen("Filial.dat", "ab+");
	TpFilial RegFili;
	int i;
	printf("\n\n*** CADASTRO DE FUNCIONARIOS EM FILIAIS ***\n");
	do
	{
		printf("\nID da Filial: ");
		fflush(stdin);
		gets(RegFili.IdFilial);
		printf("\nCodigo do Funcionario: ");
		scanf("%d", &RegFili.CodFunc);
		while (strcmp(RegFili.IdFilial,"\0")!=0 && RegFili.CodFunc>0)
		{
			i = BuscaCodigoFiliFunc(Ptr, RegFili.IdFilial, RegFili.CodFunc);
			if (i<0)
			{
				printf("Dia da Admissao: ");
				scanf("%d", &RegFili.DtVinculo.dia);
				printf("Mes [as tres/quatro primeiras letras]: ");
				fflush(stdin);
				strupr(gets(RegFili.DtVinculo.mes));
				printf("Ano: ");
				scanf("%d", &RegFili.DtVinculo.ano);
				fwrite(&RegFili, sizeof(TpFilial), 1, Ptr);
			}
			else
				printf("\nCodigo ja existente na %s!\n", RegFili.IdFilial);
			printf("\nCodigo do Funcionario: ");
			scanf("%d", &RegFili.CodFunc);	
		}
	}while(strcmp(RegFili.IdFilial,"\0")!=0);
	fclose(Ptr);
}

void ListarFuncionariosFiliais(void)
{
	FILE *Ptr = fopen("Filial.dat", "rb");
	TpFilial RegFili;
	printf("\n\n*** LISTA DE FUNCIONARIOS EM FILIAIS ***\n");
	if (Ptr == NULL)
		printf("\nErro de abertura do Arquivo");
	else
	{
		printf("-------------------------------------\n");
		printf("IdFilial  Codigo  Data de Admissao\n\n");
		fread(&RegFili, sizeof(TpFilial), 1, Ptr);
		while (!feof(Ptr))
		{
			printf("%s\t  %d \t  %d/%s/%d\n", RegFili.IdFilial, RegFili.CodFunc, RegFili.DtVinculo.dia, RegFili.DtVinculo.mes, RegFili.DtVinculo.ano);
			fread(&RegFili, sizeof(TpFilial), 1, Ptr);
		}
		printf("-------------------------------------\n");
		fclose(Ptr);
	}	
	getch();
}

void InsereDadosFuncionariosFiliais(void)
{
	FILE *Ptr = fopen("Filial.dat","ab");
	TpFilial RegFili;
	
	strcpy(RegFili.IdFilial,"FILIAL2");
	RegFili.CodFunc = 1;	
	RegFili.DtVinculo.dia = 25;
	strcpy(RegFili.DtVinculo.mes,"MAR");
	RegFili.DtVinculo.ano = 2002;
	fwrite(&RegFili, sizeof(TpFilial), 1, Ptr);

	strcpy(RegFili.IdFilial,"FILIAL2");
	RegFili.CodFunc = 5;	
	RegFili.DtVinculo.dia = 18;
	strcpy(RegFili.DtVinculo.mes,"SET");
	RegFili.DtVinculo.ano = 2003;
	fwrite(&RegFili, sizeof(TpFilial), 1, Ptr);

	strcpy(RegFili.IdFilial,"FILIAL1");
	RegFili.CodFunc = 15;	
	RegFili.DtVinculo.dia = 15;
	strcpy(RegFili.DtVinculo.mes,"FEV");
	RegFili.DtVinculo.ano = 2021;
	fwrite(&RegFili, sizeof(TpFilial), 1, Ptr);

	strcpy(RegFili.IdFilial,"FILIAL1");
	RegFili.CodFunc = 11;	
	RegFili.DtVinculo.dia = 26;
	strcpy(RegFili.DtVinculo.mes,"MAI");
	RegFili.DtVinculo.ano = 2021;
	fwrite(&RegFili, sizeof(TpFilial), 1, Ptr);

	strcpy(RegFili.IdFilial,"FILIAL3");
	RegFili.CodFunc = 7;	
	RegFili.DtVinculo.dia = 15;
	strcpy(RegFili.DtVinculo.mes,"MAR");
	RegFili.DtVinculo.ano = 2021;
	fwrite(&RegFili, sizeof(TpFilial), 1, Ptr);

	fclose(Ptr);
}


//Relatorio de Filiais com todos os dados registrados (Não Feito)
void ListarRegistrosCompletos(void)
{
	
}

//Relatorio de Cargos e os Funcionarios que os ocupam (Não Feito)
void ListarCargosFuncionarios(void)
{
	
}




int main(void)
{				
	char opcao;
	do
	{
		opcao=Menu();
		switch(opcao)
		{
		    case 'A': CadastroFuncionarios();
					  break;
					  
			case 'B': ListarFuncionarios();
					  break;

			case 'C': ConsultarFuncionarios();
					  break;
					  
			case 'D': DeletarFuncionarios();
					  break;
			
			case 'E': AlterarFuncionarios();
					  break;
					  
			case 'F': OrdenaNomeBolhaFuncionarios();
					  break;

			case 'G': OrdenaCodigoBolhaFuncionarios();
					  break;

			case 'H': AtualizaSalariosFuncionarios();
					  break;
					  
			case 'I': TempoEmpresaFuncionarios();
					  break;
					  
			case 'J': OrdenaSalarioFuncionarios();
					  break;

			case 'K': InsereDadosFuncionarios();
					  break;	 

			case 'L': CadastroCargos();
					  break;

			case 'M': ListarCargos();
					  break;

			case 'N': ConsultarCargos();
					  break;

			case 'O': DeletarCargos();
					  break;

			case 'P': AlterarCargos();
					  break;		  

			case 'Q': SelecaoCodigoDiretaCargos();
					  break; 

			case 'R': InserirCargos();
					  break;

			case 'S': CadastroFuncionariosFiliais();
					  break;

			case 'T': ListarFuncionariosFiliais();
					  break;

			case 'U': InsereDadosFuncionariosFiliais();
					  break;

			case 'V': ListarRegistrosCompletos();
					  break;

			case 'W': ListarCargosFuncionarios();
					  break;		  		  		  		  		   		 		  	     
		}	
	}while(opcao!=27);
	return 0;
}