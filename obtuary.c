#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#define MAX_MORTOS 1000

// Indice Global - quantidade de mortos
size_t gMortos = 0;

typedef struct{
    int dia,mes,ano;
}Data;

typedef struct{
    Data dataMorte;
	Data dataNascimento;
    char nomeMorto[64];
	char causaMorte[128];
	char descricaoMorto[128];
}Obituario;

// *inversao dos valores das primeiras duas telas
typedef enum{
    TELA_PRINCIPAL = -1,
    TELA_SAIR,        //0
    TELA_CADASTRAR,   //1
    TELA_DESCADASTRAR,//2
    TELA_LISTAR_TODOS //3
}Telas;

void limparBuffer(void);
void listarTodos(Obituario *obit);
void salvarEmDisco(Obituario *obit);
void cadastrarRegistro(Obituario *obit);
int compararFalecimento(Obituario *obit);
void descadastrarRegistro(Obituario *obit);
int checarDatas(int dia, int mes, int ano);
void carregarRegistrosParaMemoria(Obituario *obit);

int main()
{
	// permite o printf() de caracteres acentuados
	setlocale(LC_ALL,"Portuguese_Brazil");
	
    Telas menu;
	// usar calloc() no lugar de memset()
    Obituario *obit = (Obituario*) calloc(MAX_MORTOS,sizeof(Obituario));
    
    // coloca os registros na memoria do programa
    carregarRegistrosParaMemoria(obit);
    
    printf("===========================================\n");
    printf("\t\tOBITUÁRIO\n");
    printf("===========================================\n\n");
    
    do{
    	printf("-------------------------------------------\n");
    	printf("\t      TELA PRINCIPAL\n");
    	printf("-------------------------------------------\n");
    	printf("-Menu de Escolhas-\n\t");
    	printf("1 -> Cadastrar registro;\n\t");
    	printf("2 -> Descadastrar registro;\n\t");
    	printf("3 -> Listar todos os registros;\n\t");
    	printf("0 -> Encerra programa.\n\t?");
        	scanf("%d",&menu);
        	limparBuffer();
    	
    	switch(menu){
    		case TELA_CADASTRAR:
    			{
    				cadastrarRegistro(obit);	
    				menu = TELA_PRINCIPAL;
				}
    			break;
    		
    		case TELA_DESCADASTRAR:
    			{
    				descadastrarRegistro(obit);
    				menu = TELA_PRINCIPAL;
				}
    			break;
    		
    		case TELA_LISTAR_TODOS:
    			{
    				listarTodos(obit);
    				menu = TELA_PRINCIPAL;
				}
    			break;
    		
    		case TELA_SAIR:
    			{
    				salvarEmDisco(obit);
    				printf("===========================================\n");
    				printf("\t\tFINALIZADO\n");
    				printf("===========================================");
				}
    			break;
    		
    		default:
    			{
    				printf("*Opção inválida.\n");
    				menu = TELA_PRINCIPAL;
				}
    			break;
		}
		printf("\n");
    	
	} while(menu == TELA_PRINCIPAL);
    
    // liberar alocacao antes de terminar
	free(obit);
	
    return(EXIT_SUCCESS);
}

void salvarEmDisco(Obituario *obit){
	FILE *f;
		f = fopen("obituario.bin","wb");
		
		size_t salvos = fwrite(obit,sizeof(Obituario),gMortos,f);
		if(salvos != gMortos){
			printf("\n*ERRO: falha ao salvar em disco.");
			exit(EXIT_FAILURE);
		}
		
		fclose(f);
}

void descadastrarRegistro(Obituario *obit){
	int indice;
	if(gMortos == 0){
		printf("\nSEM REGISTROS PARA DESCADASTRAR.\n");
	}else{
		printf("-------------------------------------------\n");
    	printf("\t      TELA DE EXCLUSÃO\n");
    	printf("-------------------------------------------");
    	
    	while(1){
			printf("\nInsira um índice (entre 0 e %zd) para remover: ",gMortos-1);
				scanf("%d",&indice);
				limparBuffer();
			
			if(indice < 0 || indice > gMortos-1){
				printf("\n*Índice inválido. Tente novamente.");
			}else{
				break;
			}
		}
		
		for(int i = indice; i < gMortos - 1; i++){
			obit[i] = obit[i+1];
		}
		gMortos--;
		
		printf("REGISTRO EXCLUIDO COM SUCESSO.\n");
	}
}

void cadastrarRegistro(Obituario *obit){
    if(gMortos>=MAX_MORTOS){
		printf("\nOBITUÁRIO CHEIO. IMPOSSÍVEL ADICIONAR.\n");
	}else{
		printf("\n-------------------------------------------\n");
	    printf("\t     TELA DE CADASTRO\n");
	    printf("-------------------------------------------");
	    
		char confirma;
		do{
			printf("\nInsira o nome: ");
				scanf("%64[^\n]",obit[gMortos].nomeMorto);
				limparBuffer();
			printf("Insira a causa da morte: ");
				scanf("%128[^\n]",obit[gMortos].causaMorte);
				limparBuffer();
			printf("Agora uma breve descrição: ");
				scanf("%128[^\n]",obit[gMortos].descricaoMorto);
				limparBuffer();
			
			while(1){
				printf("Insira a data de Nascimento ->\nDia: ");
					scanf("%d",&obit[gMortos].dataNascimento.dia);
					limparBuffer();
				printf("Mes: ");
					scanf("%d",&obit[gMortos].dataNascimento.mes);
					limparBuffer();
				printf("Ano: ");
					scanf("%d",&obit[gMortos].dataNascimento.ano);
					limparBuffer();
				
				if( checarDatas(
				obit[gMortos].dataNascimento.dia,
				obit[gMortos].dataNascimento.mes,
				obit[gMortos].dataNascimento.ano) == 1 ){
					printf("\n*Data inválida inserida. Tente novamente.\n");
				}else{
					break;
				}
			}
			
			while(1){
				printf("Insira a data do Falecimento ->\nDia: ");
					scanf("%d",&obit[gMortos].dataMorte.dia);
					limparBuffer();
				printf("Mes: ");
					scanf("%d",&obit[gMortos].dataMorte.mes);
					limparBuffer();
				printf("Ano: ");
					scanf("%d",&obit[gMortos].dataMorte.ano);
					limparBuffer();	
					
				if( checarDatas(
				obit[gMortos].dataMorte.dia,
				obit[gMortos].dataMorte.mes,
				obit[gMortos].dataMorte.ano) == 1 ){
					printf("\n*Data inválida inserida. Tente novamente.\n");
				}else if( compararFalecimento(obit) == 1 ){
					printf("\n*O falecido não pode nascer antes de sua morte.\n");
				}else{
					break;
				}
			}
			
			printf("\nDeseja mesmo armazenar novo registro?\n(s/n): ");
				scanf("%c",&confirma);
		} while(confirma == 'n' || confirma == 'N');
		
		printf("REGISTRO ADICIONADO COM SUCESSO.\n");
		
		gMortos++;
	}
}

int compararFalecimento(Obituario *obit){
	if( obit[gMortos].dataMorte.ano < obit[gMortos].dataNascimento.ano ){
		return 1;
	}
	else if(
	obit[gMortos].dataMorte.ano == obit[gMortos].dataNascimento.ano
	&& obit[gMortos].dataMorte.mes < obit[gMortos].dataNascimento.mes ){
		return 1;
	}
	else if(
	obit[gMortos].dataMorte.ano == obit[gMortos].dataNascimento.ano
	&& obit[gMortos].dataMorte.mes == obit[gMortos].dataNascimento.mes 
	&& obit[gMortos].dataMorte.dia < obit[gMortos].dataNascimento.dia ){
		return 1;
	}
	
	return 0;
}

int checarDatas(int dia, int mes, int ano){
	if(dia<1 || dia>31){
		return 1;
	}
	else if(mes<1 || mes>12){
		return 1;
	}
	else if(dia>28 && mes==2){
		return 1;
	}
	else if(dia>30 && (mes==4 || mes==6 || mes==9 || mes==11)){
		return 1;
	}
	
	return 0;
}

void listarTodos(Obituario *obit){
	if(gMortos == 0){
		printf("\nSEM REGISTROS NO MOMENTO.\n");
	}else{
		printf("\n-------------------------------------------\n");
    	printf("\t     TELA DE LISTAGEM\n");
    	printf("-------------------------------------------\n");
		for(int i=0;i<gMortos;i++){
			printf("\n|Morto %d|\n",i);
			printf("-Nome: %s\n",obit[i].nomeMorto);
			printf("-Data de nascimento: %d/%d/%d\n",obit[i].dataNascimento.dia,
			obit[i].dataNascimento.mes,obit[i].dataNascimento.ano);
			printf("-Data da morte: %d/%d/%d\n",obit[i].dataMorte.dia,
			obit[i].dataMorte.mes,obit[i].dataMorte.ano);
			printf("-Causa da morte: %s\n",obit[i].causaMorte);
			printf("-Descrição: %s\n",obit[i].descricaoMorto);
		}
	}
}

void carregarRegistrosParaMemoria(Obituario *obit){
    FILE *f;
    f = fopen("obituario.bin","rb");
    if( f == NULL ){
        printf("*Falha ao abrir arquivo.\n(Lembre de criar 'obituario.bin' manualmente)");
        exit(EXIT_FAILURE);
    }
    
    gMortos = fread(obit,sizeof(Obituario),MAX_MORTOS,f);
    printf("Registros Lidos = %zd\n",gMortos);
    
    if(gMortos >= MAX_MORTOS){
    	printf("*AVISO: o obituário está cheio.\n");
	}
    
    fclose(f);
}

void limparBuffer(void){
    char c;
    while( (c=getchar()) != '\n' && c != EOF );
}

