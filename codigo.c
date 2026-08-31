#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <errno.h>

#define ARQUIVO_ALUNOS "alunos.hro"
#define ARQUIVO_DISCIPLINAS "disciplinas.hro"

#define MIN_IDADE 15
#define MAX_ALUNOS 5
#define MAX_PERIODOS 3

#define LARGURA_MENU 63
#define DELAY_PROPOSITAL 1800

#define ID_ALUNO 'A'
#define ID_DISCIPLINA 'D'
#define ID_CADERNETA 'C'

typedef struct {
    char nome[256];
    int idade;
    char matricula[20];
    int periodo;
} Aluno;

typedef struct {
    char nome[256];
    int periodo;
    char codigo[20];
} Disciplina;

typedef struct {
    int ano;
    Disciplina disciplina;
    Aluno alunos[MAX_ALUNOS];
    float notas[MAX_ALUNOS];
    char codigo[20];
} Caderneta;

void menuPrincipal();
void submenuAluno();
void submenuDisciplina();
void submenuCaderneta();

void submenuListarAlunos();
void submenuListarDisciplinas();
void submenuListarCaderneta();

void formularioCadastroAluno();
void formularioEditarAluno();
void formularioCadastroDisciplina();
void formularioCadastroCaderneta();

bool mostrarTodosAlunos();
bool mostrarAlunosPorPeriodo(int periodo);
bool mostrarAlunoPorMatricula(char matricula[20]);

bool mostrarTodasDisciplinas();
bool mostrarDisciplinaPorCodigo(char codigo[20]);

bool cadastrarAluno(Aluno aluno);
bool cadastrarDisciplina(Disciplina disciplina);
bool cadastrarCaderneta(Caderneta caderneta);

bool editarAluno(Aluno aluno);
bool editarDisciplina(Disciplina aluno);
bool editarCaderneta(Caderneta caderneta);

bool gerarMatricula(char matricula[20]);
bool gerarCodigoDisciplina(char codigo[20]);

bool lerInteiro(int *valor);
void exibirMenu(const char *titulo, const char *opcoes[], int quantidade, bool limpar_tela);
void mostrarOpcao(void (*funcao[])(), int quantidade);

void encerrarPrograma();

bool encerrar_programa = false;

// system("cls");  
// printf("=========================== MENU PRINCIPAL ====================\n");
// printf("[0] GERENCIAR ALUNOS\n");
// printf("[1] GERENCIAR DISCIPLINAS\n");
// printf("[2] GERENCIAR CADERNETAS\n");
// printf("[3] LISTAR ALUNOS APROVADOS POR DISCIPLINA\n");
// printf("[4] LISTAR PERCENTUAL DE ALUNOS REPROVADOS POR DISCIPLINA\n");
// printf("[5] LISTAR QUANTIDADE DE ALUNOS POR DISCIPLINA\n");
// printf("[6] LISTAR ALUNO COM MAIOR NOTA POR DISCIPLINA\n");
// printf("[7] SAIR\n");
// printf("===============================================================\n");

void menuPrincipal(){ 
    const char *opcoes[] = {
        "GERENCIAR ALUNOS",
        "GERENCIAR DISCIPLINAS",
        "GERENCIAR CADERNETAS",
        "SAIR"
    };

    exibirMenu("MENU PRINCIPAL", opcoes, 4, true);
    
    void (*funcoes[4])() = {submenuAluno, submenuDisciplina, submenuCaderneta, encerrarPrograma};
    mostrarOpcao(funcoes, 4);
}

void submenuAluno(){
    const char *opcoes[] = {
        "CADASTRAR ALUNO",
        "EDITAR ALUNO",
        "LISTAR ALUNOS",
        "VOLTAR"
    };

    exibirMenu("GERENCIAR ALUNOS", opcoes, 4, true);
    
    void (*funcoes[4])() = {formularioCadastroAluno, formularioEditarAluno, submenuListarAlunos, NULL};
    mostrarOpcao(funcoes, 4);
}

void submenuDisciplina(){
    const char *opcoes[] = {
        "CADASTRAR DISCIPLINA",
        "EDITAR DISCIPLINAS",
        "LISTAR DISCIPLINAS",
        "VOLTAR AO MENU PRINCIPAL"
    };

    exibirMenu("GERENCIAR DISCIPLINAS", opcoes, 4, true);

    void (*funcoes[4])() = {formularioCadastroDisciplina, NULL, submenuListarDisciplinas, NULL};
    mostrarOpcao(funcoes, 4);
}

void submenuCaderneta(){
    const char *opcoes[] = {
        "CADASTRAR CADERNETA",
        "EDITAR CADERNETAS",
        "LISTAR CADERNETAS",
        "VOLTAR AO MENU PRINCIPAL"
    };

    exibirMenu("GERENCIAR CADERNETAS", opcoes, 4, true);

    void (*funcoes[4])() = {formularioCadastroCaderneta, NULL, NULL, NULL};
    mostrarOpcao(funcoes, 4);



}

void submenuListarAlunos(){
    const char *opcoes[] = {
        "PROCURAR ALUNO POR MATRICULA",
        "LISTAR TODOS ALUNOS CADASTRADOS",
        "LISTAR ALUNOS POR PERIODO",
        "VOLTAR"
    };

    exibirMenu("LISTAR ALUNOS", opcoes, 4, true);

    int opcao;
    printf("Digite uma opcao: ");
    
    if (!lerInteiro(&opcao)) {
        printf("\nEntrada invalida! Digite um numero valido.\n");
        return;
    }

    switch (opcao){
        case 0: 
            char matricula[20];
            printf("Digite a matricula: ");
            fgets(matricula, sizeof(matricula), stdin);
            matricula[strcspn(matricula, "\n")] = '\0'; 

            Sleep(DELAY_PROPOSITAL); 
            if(!mostrarAlunoPorMatricula(matricula)) printf("\nAluno nao encontrado!\n");
            break;
        case 1: 
            Sleep(DELAY_PROPOSITAL); 
            if(!mostrarTodosAlunos()) printf("\nNenhum aluno cadastrado!\n");
            break;
        case 2:
            int periodo;
        
            printf("Digite o periodo: ");
            if (!lerInteiro(&periodo)) {
                printf("\nEntrada invalida! Digite um numero valido.\n");
                break;
            }
            if (periodo < 0 || periodo >= MAX_PERIODOS) {
                printf("\nopcao invalida! digite um periodo valido.\n");
                break;
            }
            Sleep(DELAY_PROPOSITAL);
            if(!mostrarAlunosPorPeriodo(periodo)){ 
                printf("\nNenhum aluno cadastrado para o periodo informado!\n");
            } 
            break;
        case 3: submenuAluno(); return;
        default: 
            printf("\nOpcaooo invalida! digite uma opcao valida.\n"); 
            break;
    }
    
    printf("\n\n");
    const char *pos_opcoes[] = {
        "VOLTAR",
        "VOLTAR AO MENU PRINCIPAL"
    };

    exibirMenu("", pos_opcoes, 2, false);

    void (*funcoes[2])() = {submenuListarDisciplinas, NULL};
    mostrarOpcao(funcoes, 2);
}

void submenuListarDisciplinas(){
    const char *opcoes[] = {
        "PESQUISAR DSICIPLINA PELO CODIGO",
        "LISTAR TODAS DISCIPLINAS CADASTRADAS",
        "PESQUISAR DISCIPLINAS POR NOME",
        "LISTAR ALUNOS APROVADOS POR DISCIPLINA",
        "LISTAR PERCENTUAL DE ALUNOS REPROVADOS POR DISCIPLINA",
        "LISTAR QUANTIDADE DE ALUNOS POR DISCIPLINA",
        "LISTAR ALUNO COM MAIOR NOTA POR DISCIPLINA",
        "VOLTAR"
    };

    exibirMenu("LISTAR DISCIPLINAS", opcoes, 8, true);

    int opcao;
    printf("Digite uma opcao: ");
    
    if (!lerInteiro(&opcao)) {
        printf("\nEntrada invalida! Digite um numero valido.\n");
        return;
    }

    switch (opcao){
        case 0: 
            char codigo[20];
            printf("Digite o codigo : ");
            fgets(codigo, sizeof(codigo), stdin);
            codigo[strcspn(codigo, "\n")] = '\0'; 

            Sleep(DELAY_PROPOSITAL); 
            if(!mostrarDisciplinaPorCodigo(codigo)) printf("\nDisciplina nao encontrada!\n");
            break;
        case 1: 
            Sleep(DELAY_PROPOSITAL); 
            if(!mostrarTodasDisciplinas()) printf("\nNenhuma disciplina cadastrada!\n");
            break;
        case 2:
            int periodo;
        
            printf("Digite o nome: ");
            // if (!lerInteiro(&periodo)) {
            //     printf("\nEntrada invalida! Digite um numero valido.\n");
            //     break;
            // }
            // if (periodo < 0 || periodo >= MAX_PERIODOS) {
            //     printf("\nopcao invalida! digite um periodo valido.\n");
            //     break;
            // }
            // Sleep(DELAY_PROPOSITAL);
            // if(!mostrarAlunosPorPeriodo(periodo)){ 
            //     printf("\nNenhum aluno cadastrado para o periodo informado!\n");
            // } 
            break;
        case 3: submenuDisciplina(); return;
        default: 
            printf("\nOpcaooo invalida! digite uma opcao valida.\n"); 
            break;
    }
    
    printf("\n\n");
    const char *pos_opcoes[] = {
        "VOLTAR",
        "VOLTAR AO MENU PRINCIPAL"
    };

    exibirMenu("", pos_opcoes, 2, false);

    void (*funcoes[2])() = {submenuListarAlunos, NULL};
    mostrarOpcao(funcoes, 2);
}

void submenuListarCaderneta(){
    const char *opcoes[] = {
        "PESQUISAR DSICIPLINA PELO CODIGO",
        "LISTAR TODAS DISCIPLINAS CADASTRADAS",
        "PESQUISAR DISCIPLINAS POR NOME",
        "LISTAR ALUNOS APROVADOS POR DISCIPLINA",
        "LISTAR PERCENTUAL DE ALUNOS REPROVADOS POR DISCIPLINA",
        "LISTAR QUANTIDADE DE ALUNOS POR DISCIPLINA",
        "LISTAR ALUNO COM MAIOR NOTA POR DISCIPLINA",
        "VOLTAR"
    };

    exibirMenu("LISTAR DISCIPLINAS", opcoes, 8, true);

    int opcao;
    printf("Digite uma opcao: ");
    
    if (!lerInteiro(&opcao)) {
        printf("\nEntrada invalida! Digite um numero valido.\n");
        return;
    }

    switch (opcao){
        case 0: 
            char codigo[20];
            printf("Digite o codigo : ");
            fgets(codigo, sizeof(codigo), stdin);
            codigo[strcspn(codigo, "\n")] = '\0'; 

            Sleep(DELAY_PROPOSITAL); 
            if(!mostrarDisciplinaPorCodigo(codigo)) printf("\nDisciplina nao encontrada!\n");
            break;
        case 1: 
            Sleep(DELAY_PROPOSITAL); 
            if(!mostrarTodasDisciplinas()) printf("\nNenhuma disciplina cadastrada!\n");
            break;
        case 2:
            int periodo;
        
            printf("Digite o nome: ");
            // if (!lerInteiro(&periodo)) {
            //     printf("\nEntrada invalida! Digite um numero valido.\n");
            //     break;
            // }
            // if (periodo < 0 || periodo >= MAX_PERIODOS) {
            //     printf("\nopcao invalida! digite um periodo valido.\n");
            //     break;
            // }
            // Sleep(DELAY_PROPOSITAL);
            // if(!mostrarAlunosPorPeriodo(periodo)){ 
            //     printf("\nNenhum aluno cadastrado para o periodo informado!\n");
            // } 
            break;
        case 3: submenuDisciplina(); return;
        default: 
            printf("\nOpcaooo invalida! digite uma opcao valida.\n"); 
            break;
    }
    
    printf("\n\n");
    const char *pos_opcoes[] = {
        "VOLTAR",
        "VOLTAR AO MENU PRINCIPAL"
    };

    exibirMenu("", pos_opcoes, 2, false);

    void (*funcoes[2])() = {submenuListarAlunos, NULL};
    mostrarOpcao(funcoes, 2);
}


// deveria ter uma forma de eu utlizar essa função de outra forma polimorfismo 
// pq eu repito quase tudo e so coloco a condiçãod e periodo na outra
bool mostrarTodosAlunos(){
    FILE *arq = fopen(ARQUIVO_ALUNOS, "rb");
	Aluno aluno;
    bool tem_aluno = false;
    int contador = 0;

    system("cls");      
    while (fread(&aluno, sizeof(Aluno), 1, arq) == 1) {
        printf("================================================\n");
        printf("Matricula : %s\n", aluno.matricula);
        printf("Nome      : %s\n", aluno.nome);
        printf("Idade     : %d anos\n", aluno.idade);
        printf("Periodo   : %d\n", aluno.periodo);
        tem_aluno = true;
        contador++;
    }
	fclose(arq);
    
    printf("================================================\n");
    printf("Total de alunos cadastrados: %d", contador);
    return tem_aluno;
}

bool mostrarAlunosPorPeriodo(int periodo){
    FILE *arq = fopen(ARQUIVO_ALUNOS, "rb");
	Aluno aluno;
	bool tem_aluno = false;
    int contador = 0;

    system("cls");
	while(fread(&aluno, sizeof(Aluno), 1, arq) == 1){
        if (aluno.periodo == periodo){
            printf("================================================\n");
            printf("Matricula : %s\n", aluno.matricula);
            printf("Nome      : %s\n", aluno.nome);
            printf("Idade     : %d anos\n", aluno.idade);
            printf("Periodo   : %d\n", aluno.periodo);
            tem_aluno = true;
            contador++;
		}
	}
	fclose(arq);

    printf("================================================\n");
    printf("Total de alunos no '%d' periodo: %d", periodo, contador);
    return tem_aluno;
}

bool mostrarAlunoPorMatricula(char matricula[20]){
    FILE *arq = fopen(ARQUIVO_ALUNOS, "rb");
	Aluno aluno;
	bool tem_aluno = false;
    
	system("cls");
	while(fread(&aluno, sizeof(Aluno), 1, arq) == 1){
        if (strcmp(aluno.matricula, matricula) == 0){
        printf("================================================\n");
        printf("Matricula : %s\n", aluno.matricula);
        printf("Nome      : %s\n", aluno.nome);
        printf("Idade     : %d anos\n", aluno.idade);
        printf("Periodo   : %d\n", aluno.periodo);
        tem_aluno = true;
		}
	}

	fclose(arq);
    return tem_aluno;
}

bool editarAluno(Aluno aluno) {
    FILE *arq = fopen(ARQUIVO_ALUNOS, "r+b");

    if (arq == NULL) {
        return false;
    }

    Aluno alunoArquivo;

    while (fread(&alunoArquivo, sizeof(Aluno), 1, arq) == 1) {
        if (strcmp(alunoArquivo.matricula, aluno.matricula) == 0) {
            if (fseek(arq, -(long)sizeof(Aluno), SEEK_CUR) != 0) {
                fclose(arq);
                return false;
            }

            if (fwrite(&aluno, sizeof(Aluno), 1, arq) != 1) {
                fclose(arq);
                return false;
            }

            fclose(arq);
            return true;
        }
    }

    fclose(arq);
    return false;
}

bool mostrarTodasDisciplinas(){
    FILE *arq = fopen(ARQUIVO_DISCIPLINAS, "rb");
	Disciplina disciplina;
    bool tem_disciplina = false;
    int contador = 0;

    system("cls");      
    while (fread(&disciplina, sizeof(Disciplina), 1, arq) == 1) {
        printf("================================================\n");
        printf("Nome    : %s\n", disciplina.nome);
        printf("Codigo  : %s\n", disciplina.codigo);
        printf("Periodo : %d\n", disciplina.periodo);
        tem_disciplina = true;
        contador++;
    }
	fclose(arq);
    
    printf("================================================\n");
    printf("Total de disciplinas cadastradas: %d", contador);
    return tem_disciplina;
}

bool mostrarDisciplinaPorCodigo(char codigo[20]){
    FILE *arq = fopen(ARQUIVO_DISCIPLINAS, "rb");
	Disciplina disciplina;
    bool tem_disciplina = false;

    system("cls");      
    while (fread(&disciplina, sizeof(Disciplina), 1, arq) == 1) {
        if (strcmp(disciplina.codigo, codigo) == 0){
            printf("================================================\n");
            printf("Codigo  : %s\n", disciplina.codigo);
            printf("Periodo : %s\n", disciplina.periodo);
            printf("Nome    : %d\n", disciplina.nome);
            tem_disciplina = true;
        }
    }
	fclose(arq);
    return tem_disciplina;
}

bool editarDisciplina(Disciplina disciplina) {
    FILE *arq = fopen(ARQUIVO_DISCIPLINAS, "r+b");

    if (arq == NULL) {
        return false;
    }

    Disciplina disciplinaArquivo;

    while (fread(&disciplinaArquivo, sizeof(Disciplina), 1, arq) == 1) {
        if (strcmp(disciplinaArquivo.codigo, disciplinaArquivo.codigo) == 0) {
            if (fseek(arq, -(long)sizeof(Disciplina), SEEK_CUR) != 0) {
                fclose(arq);
                return false;
            }

            if (fwrite(&disciplinaArquivo, sizeof(Disciplina), 1, arq) != 1) {
                fclose(arq);
                return false;
            }

            fclose(arq);
            return true;
        }
    }

    fclose(arq);
    return false;
}

void formularioCadastroAluno(){
	Aluno aluno;
	
    printf("Digite o nome: ");
    fgets(aluno.nome, sizeof(aluno.nome), stdin);
    aluno.nome[strcspn(aluno.nome, "\n")] = '\0';

	printf("Digite a idade: ");
	if (!lerInteiro(&aluno.idade)) {
		return;
	}
	printf("Digite o periodo: ");
	if (!lerInteiro(&aluno.periodo)) {
		return;
	}
    //deveria perguntar confindadno a criação

    gerarMatricula(aluno.matricula);    
    cadastrarAluno(aluno);
}

void formularioEditarAluno(){
    Aluno aluno;
    exibirMenu("EDITAR ALUNO", NULL, 0, true);
    char matricula[20];
	
    printf("Digite a matricula: ");
    fgets(matricula, sizeof(matricula), stdin);
    matricula[strcspn(matricula, "\n")] = '\0';  
    
    Sleep(DELAY_PROPOSITAL);
    if(!mostrarAlunoPorMatricula(matricula)){
        printf("\nNao foi encontrado aluno com esssa matirucla.\n");
    }else{
        printf("Digite o nome: ");
        fgets(aluno.nome, sizeof(aluno.nome), stdin);
        aluno.nome[strcspn(aluno.nome, "\n")] = '\0';
        
        printf("Digite a idade: ");
        if (!lerInteiro(&aluno.idade)) {
            printf("\nEntrada invalida! Digite um numero valido.\n");
            return;
        }
        printf("Digite o periodo: ");
        if (!lerInteiro(&aluno.periodo)) {
            printf("\nEntrada invalida! Digite um numero valido.\n");
            return;
        }
        
        strcpy(aluno.matricula, matricula);

        //deveria perguntar confirmando a edicao
        if(!editarAluno(aluno)){
            Sleep(DELAY_PROPOSITAL);
            printf("\nErro ao editar aluno.\n");
        } else {             
            Sleep(DELAY_PROPOSITAL);
            printf("\nAluno atualizado com sucesso.\n");
        }
    }

    printf("\n\n");
    const char *pos_opcoes[] = {
        "VOLTAR",
        "VOLTAR AO MENU PRINCIPAL"
    };

    exibirMenu("", pos_opcoes, 2, false);

    void (*funcoes[2])() = {submenuAluno, NULL};
    mostrarOpcao(funcoes, 2);
}

void formularioCadastroDisciplina(){
    Disciplina disciplina;
	
    printf("Digite o nome: ");
    fgets(disciplina.nome, sizeof(disciplina.nome), stdin);
    disciplina.nome[strcspn(disciplina.nome, "\n")] = '\0';

	printf("Digite o periodo: ");
	if (!lerInteiro(&disciplina.periodo)) {
		return;
	}
    //deveria perguntar confindadno a criação

    
    gerarCodigoDisciplina(disciplina.codigo);    
    cadastrarDisciplina(disciplina);

}

void formularioCadastroCaderneta(){

}

bool cadastrarDisciplina(Disciplina disciplina){
    FILE *arq = fopen(ARQUIVO_DISCIPLINAS, "ab");
	fwrite(&disciplina, 1, sizeof(Disciplina), arq);
	fclose(arq);
    return true;
}

bool cadastrarAluno(Aluno aluno){
    FILE *arq = fopen(ARQUIVO_ALUNOS, "ab");
	fwrite(&aluno, 1, sizeof(Aluno), arq);
	fclose(arq);
    return true;
}

bool gerarMatricula(char matricula[20]) {
    FILE *arq = fopen(ARQUIVO_ALUNOS, "rb");

    time_t agora = time(NULL);
    struct tm *data = localtime(&agora);

    int anoAtual = data->tm_year + 1900;
    int maiorNumero = 0;

    Aluno aluno;

    while (fread(&aluno, sizeof(Aluno), 1, arq) == 1) {
        int ano;
        int numero;

        if (sscanf(aluno.matricula, "%4d%4d", &ano, &numero) == 2) {

            if (ano == anoAtual && numero > maiorNumero) {
                maiorNumero = numero;
            }
        }
    }

    fclose(arq);
    maiorNumero++;

    sprintf(matricula, "%c%04d%04d", ID_ALUNO, anoAtual, maiorNumero);

    return true;
}

bool gerarCodigoDisciplina(char codigo[20]) {
    FILE *arq = fopen(ARQUIVO_DISCIPLINAS, "rb");

    if (arq == NULL) {
        return false;
    }

    time_t agora = time(NULL);
    struct tm *data = localtime(&agora);

    int anoAtual = data->tm_year + 1900;
    int maiorNumero = 0;

    Disciplina disciplina;

    while (fread(&disciplina, sizeof(Disciplina), 1, arq) == 1) {
        int ano;
        int numero;

        if (sscanf(disciplina.codigo, "%4d%4d", &ano, &numero) == 2) {

            if (ano == anoAtual && numero > maiorNumero) {
                maiorNumero = numero;
            }
        }
    }

    fclose(arq);

    maiorNumero++;

    sprintf(codigo, "%c%04d%04d", ID_DISCIPLINA, anoAtual, maiorNumero);

    return true;
}

void exibirMenu(const char *titulo, const char *opcoes[], int quantidade, bool limpar_tela){
    if (limpar_tela) system("cls");  
    
    int tamanhoTitulo = strlen(titulo);
    int espacos = LARGURA_MENU - tamanhoTitulo - 2;

    printf("=");

    for (int i = 0; i < espacos / 2; i++)
        printf("=");

    printf(" %s ", titulo);

    for (int i = 0; i < (espacos + 1) / 2; i++)
        printf("=");

    printf("\n");

    for (int i = 0; i < quantidade; i++)
        printf("[%d] %s\n", i, opcoes[i]);

    for (int i = 0; i < LARGURA_MENU; i++)
        printf("=");

    printf("\n");
}

void mostrarOpcao(void (*funcao[])(), int quantidade){
    char entrada[100];

    printf("Digite uma opcao: ");
    fgets(entrada, sizeof(entrada), stdin);

    int opcao;

    if (sscanf(entrada, "%d", &opcao) != 1) {
        printf("\nOpcao invalida! digite uma opcao valida.\n");
        Sleep(DELAY_PROPOSITAL); 
        return;
    }

    if (opcao >= 0 && opcao < quantidade) {
        if (funcao[opcao] != NULL) funcao[opcao]();
    } 
    else{
        printf("\nOpcao invalida!\n");
        Sleep(DELAY_PROPOSITAL);
    }
}

void encerrarPrograma() { 
    printf("Tem certeza que deseja encerrar o programa? (S/N): "); 

    char resposta[5]; 
    fgets(resposta, sizeof(resposta), stdin); 

    if (resposta[0] != '\n' &&
        resposta[1] == '\n' &&
        (resposta[0] == 'S' || resposta[0] == 's')) {

        printf("\nSessao finalizada!");
        encerrar_programa = true; 
    }
}

bool lerInteiro(int *valor) {
    char entrada[100];

    if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
        printf("\nEntrada invalida! Digite um numero valido.\n");
        return false;
    }

    char *fim;
    long numero = strtol(entrada, &fim, 10);

    while (*fim == ' ' || *fim == '\t') {
        fim++;
    }

    if (fim == entrada || (*fim != '\n' && *fim != '\0')) {
        printf("\nEntrada invalida! Digite um numero valido.\n");
        return false;
    }

    *valor = (int)numero;

    return true;
}

int main() {
    while (!encerrar_programa) menuPrincipal();
    return 0;
}