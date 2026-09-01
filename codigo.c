#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <errno.h>

#define ARQUIVO_ALUNOS "alunos.hro"
#define ARQUIVO_DISCIPLINAS "disciplinas.hro"
#define ARQUIVO_CADERNETAS "cadernetas.hro"

#define MIN_IDADE 15
#define MAX_ALUNOS 5
#define MAX_NOTAS 2
#define MAX_PERIODOS 3

#define LARGURA_MENU 63
#define DELAY_PROPOSITAL 1800

#define ID_ALUNO 'A'
#define ID_DISCIPLINA 'D'
#define ID_CADERNETA 'C'

typedef enum {
    SUCESSO,
    NENHUM_REGISTRO,
    ERRO_ABRIR_ARQUIVO,
    ERRO_FECHAR_ARQUIVO,
    ERRO_GRAVAR_ARQUIVO,
    ERRO_POSICIONAR_ARQUIVO,
    ERRO_REGISTRO_NAO_ENCONTRADO,
    ERRO_MEMORIA,
    ERRO_DATA,
    ERRO_NAO_ENCONTRADO
} Resultado;

typedef enum {
    NOVO,
    ATUALIZAR
} OperacaoArquivo;

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
    float notas[MAX_ALUNOS][MAX_NOTAS];
    char codigo[20];
} Caderneta;

void menuPrincipal(); //OK
void submenuAluno(); //OK
void submenuDisciplina();
void submenuCaderneta();

void submenuListarAlunos(); //OK
void submenuListarDisciplinas();
void submenuListarCaderneta();

void formularioCadastrarAluno();
void formularioCadastrarDisciplina();
void formularioCadastrarCaderneta();

void formularioEditarAluno();
void formularioEditarDisciplina();
void formularioEditarCaderneta();

void exibirAluno(const void *registro);
void exibirDisciplina(const void *registro);
void exibirCaderneta(const void *registro);

bool mostrarAlunoPorMatricula(char matricula[20]);
bool mostrarDisciplinaPorCodigo(char codigo[20]);
bool mostrarCadernetaPorCodigo(char codigo[20]);

bool mostrarAlunosPorPeriodo(int periodo);

bool editarAluno(Aluno aluno); //OK
bool editarDisciplina(Disciplina aluno);
bool editarCaderneta(Caderneta caderneta);

Resultado listarGenerico(const char *nomeArquivo, size_t tamanhoRegistro, void (*mostrar)(const void *registro), int *contador); // OK
Resultado salvarGenerico(const void *dado, size_t tamanho, const char *nome_arquivo, OperacaoArquivo operacao, size_t offset_identificador); // OK
Resultado gerarCodigoGenerico(char codigo[20], const char *nomeArquivo, size_t tamanhoRegistro, size_t offsetCodigo, char identificador); // OK

bool lerInteiro(int *valor);
void exibirMenu(const char *titulo, const char *opcoes[], int quantidade, bool limpar_tela);
void menuPosOperacao(void (*voltar)());
void mostrarOpcao(void (*funcao[])(), int quantidade);

bool confirmarEscolha(const char *mensagem);
void sairPrograma();

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
    
    void (*funcoes[4])() = {submenuAluno, submenuDisciplina, submenuCaderneta, sairPrograma};
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
    
    void (*funcoes[4])() = {formularioCadastrarAluno, formularioEditarAluno, submenuListarAlunos, NULL};
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

    void (*funcoes[4])() = {formularioCadastrarDisciplina, formularioEditarDisciplina, submenuListarDisciplinas, NULL};
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

    void (*funcoes[4])() = {formularioCadastrarCaderneta, formularioEditarCaderneta, submenuListarCaderneta, NULL};
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

    if(!lerInteiro(&opcao)){
        printf("\nEntrada invalida! Digite um numero valido.\n");
        return;
    }

    switch(opcao){
        case 0:{
            char matricula[20];

            printf("Digite a matricula: ");
            fgets(matricula, sizeof(matricula), stdin);
            matricula[strcspn(matricula, "\n")] = '\0';

            Sleep(DELAY_PROPOSITAL);

            Resultado resultado = mostrarAlunoPorMatricula(matricula);

            if(resultado == ERRO_NAO_ENCONTRADO)
                printf("\nAluno nao encontrado!\n");
            else if(resultado != SUCESSO)
                printf("\nErro ao procurar aluno!\n");

            break;
        }
        case 1:{
            int contador;

            Sleep(DELAY_PROPOSITAL);
            Resultado resultado = listarGenerico(ARQUIVO_ALUNOS, sizeof(Aluno), exibirAluno, &contador);

            if(resultado == NENHUM_REGISTRO) printf("\nNenhum aluno cadastrado!\n");
            else if(resultado == SUCESSO) printf("\n================================================\nTotal de alunos cadastrados: %d\n", contador);
            else printf("\nErro ao listar alunos!\n");

            break;
        }
        case 2:{
            int periodo;

            printf("Digite o periodo: ");

            if(!lerInteiro(&periodo)){
                printf("\nEntrada invalida! Digite um numero valido.\n");
                break;
            }

            if(periodo < 0 || periodo > MAX_PERIODOS){
                printf("\nOpcao invalida! Digite um periodo valido.\n");
                break;
            }

            Sleep(DELAY_PROPOSITAL);

            Resultado resultado = mostrarAlunosPorPeriodo(periodo);

            if(resultado == NENHUM_REGISTRO)
                printf("\nNenhum aluno cadastrado para o periodo informado!\n");
            else if(resultado != SUCESSO)
                printf("\nErro ao listar alunos!\n");

            break;
        }
        case 3: submenuAluno(); return;

        default: printf("\nOpcao invalida! Digite uma opcao valida.\n"); break;
    }

    menuPosOperacao(submenuListarAlunos);
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

    // int opcao;
    // printf("Digite uma opcao: ");
    
    // if (!lerInteiro(&opcao)) {
    //     printf("\nEntrada invalida! Digite um numero valido.\n");
    //     return;
    // }

    // switch (opcao){
    //     case 0: 
    //         char codigo[20];
    //         printf("Digite o codigo : ");
    //         fgets(codigo, sizeof(codigo), stdin);
    //         codigo[strcspn(codigo, "\n")] = '\0'; 

    //         Sleep(DELAY_PROPOSITAL); 
    //         if(!mostrarDisciplinaPorCodigo(codigo)) printf("\nDisciplina nao encontrada!\n");
    //         break;
    //     case 1: 
    //         Sleep(DELAY_PROPOSITAL); 
    //         if(!mostrarTodasDisciplinas()) printf("\nNenhuma disciplina cadastrada!\n");
    //         break;
    //     case 2:
    //         int periodo;
        
    //         printf("Digite o nome: ");
    //         // if (!lerInteiro(&periodo)) {
    //         //     printf("\nEntrada invalida! Digite um numero valido.\n");
    //         //     break;
    //         // }
    //         // if (periodo < 0 || periodo >= MAX_PERIODOS) {
    //         //     printf("\nopcao invalida! digite um periodo valido.\n");
    //         //     break;
    //         // }
    //         // Sleep(DELAY_PROPOSITAL);
    //         // if(!mostrarAlunosPorPeriodo(periodo)){ 
    //         //     printf("\nNenhum aluno cadastrado para o periodo informado!\n");
    //         // } 
    //         break;
    //     case 3: submenuDisciplina(); return;
    //     default: 
    //         printf("\nOpcaooo invalida! digite uma opcao valida.\n"); 
    //         break;
    // }
    
    // printf("\n\n");
    // const char *pos_opcoes[] = {
    //     "VOLTAR",
    //     "VOLTAR AO MENU PRINCIPAL"
    // };

    // exibirMenu("", pos_opcoes, 2, false);

    // void (*funcoes[2])() = {submenuListarDisciplinas, NULL};
    // mostrarOpcao(funcoes, 2);
}

void submenuListarCaderneta(){
    // const char *opcoes[] = {
    //     "PROCURAR CADERNETA PELO CODIGO",
    //     "LISTAR TODAS AS CADERNTENAS",
    //     "VOLTAR"
    // };

    // exibirMenu("LISTAR CADERNETAS", opcoes, 3, true);

    // int opcao;
    // printf("Digite uma opcao: ");
    
    // if (!lerInteiro(&opcao)) {
    //     printf("\nEntrada invalida! Digite um numero valido.\n");
    //     return;
    // }

    // switch (opcao){
    //     case 0: 
    //         char codigo[20];
    //         printf("Digite o codigo : ");
    //         fgets(codigo, sizeof(codigo), stdin);
    //         codigo[strcspn(codigo, "\n")] = '\0'; 

    //         Sleep(DELAY_PROPOSITAL); 
    //         if(!mostrarCadernetaPorCodigo(codigo)) printf("\nCaderneta nao encontrada!\n");
    //         break;
    //     case 1: 
    //         Sleep(DELAY_PROPOSITAL); 
    //         if(!mostrarTodasCadernetas()) printf("\nNenhuma caderneta cadastrada!\n");
    //         break;
    //     case 2: submenuCaderneta(); return;
    //     default: 
    //         printf("\nOpcaooo invalida! digite uma opcao valida.\n"); 
    //         break;
    // }
    
    // printf("\n\n");
    // const char *pos_opcoes[] = {
    //     "VOLTAR",
    //     "VOLTAR AO MENU PRINCIPAL"
    // };

    // exibirMenu("", pos_opcoes, 2, false);

    // void (*funcoes[2])() = {submenuListarCaderneta, NULL};
    // mostrarOpcao(funcoes, 2);
}

Resultado listarGenerico(const char *nomeArquivo, size_t tamanhoRegistro, void (*mostrar)(const void *registro), int *contador){
    FILE *arq = fopen(nomeArquivo, "rb");

    if(arq == NULL)
        return ERRO_ABRIR_ARQUIVO;

    void *registro = malloc(tamanhoRegistro);

    if(registro == NULL){
        fclose(arq);
        return ERRO_MEMORIA;
    }

    int total = 0;

    system("cls");

    while(fread(registro, tamanhoRegistro, 1, arq) == 1){
        mostrar(registro);
        total++;
    }

    free(registro);

    if(fclose(arq) != 0)
        return ERRO_FECHAR_ARQUIVO;

    if(contador != NULL)
        *contador = total;

    if(total == 0)
        return NENHUM_REGISTRO;

    return SUCESSO;
}

void exibirAluno(const void *registro){
    const Aluno *aluno = registro;

    printf("================================================\n");
    printf("Matricula : %s\n", aluno->matricula);
    printf("Nome      : %s\n", aluno->nome);
    printf("Idade     : %d anos\n", aluno->idade);
    printf("Periodo   : %d\n", aluno->periodo);
}

void exibirDisciplina(const void *registro){
    const Disciplina *disciplina = registro;

    printf("================================================\n");
    printf("Nome    : %s\n", disciplina->nome);
    printf("Codigo  : %s\n", disciplina->codigo);
    printf("Periodo : %d\n", disciplina->periodo);
}

void exibirCaderneta(const void *registro){
    const Caderneta *caderneta = registro;

    printf("================================================\n");
    printf("Codigo     : %s\n", caderneta->codigo);
    printf("Disciplina : %s\n", caderneta->disciplina.nome);
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

// printf("================================================\n");
// printf("Total de disciplinas cadastradas: %d", contador);
// return tem_disciplina;

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

bool mostrarCadernetaPorCodigo(char codigo[20]) {

    FILE *arq = fopen(ARQUIVO_CADERNETAS, "rb");

    if (arq == NULL) {
        return false;
    }

    Caderneta caderneta;
    bool tem_caderneta = false;

    system("cls");

    while (fread(&caderneta, sizeof(Caderneta), 1, arq) == 1) {

        if (strcmp(caderneta.codigo, codigo) == 0) {

            tem_caderneta = true;

            printf("==============================================================\n");
            printf("%s - %s\n", caderneta.codigo, caderneta.disciplina.nome);
            printf("==============================================================\n");
            printf("%-30s %8s %8s %8s\n", "ALUNO", "NOTA 1", "NOTA 2", "MEDIA");
            printf("--------------------------------------------------------------\n");

            for (int i = 0; i < MAX_ALUNOS; i++) {

                float media = (caderneta.notas[i][0] +
                               caderneta.notas[i][1]) / 2.0;

                printf("%-30s %8.2f %8.2f %8.2f\n",
                       caderneta.alunos[i].nome,
                       caderneta.notas[i][0],
                       caderneta.notas[i][1],
                       media);
            }
            printf("==============================================================\n");
        }
    }

    fclose(arq);

    return tem_caderneta;
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

void formularioCadastrarAluno(){
	Aluno aluno;
	
    exibirMenu("CADASTRAR ALUNO", NULL, 0, true);

    printf("Digite o nome: ");
    fgets(aluno.nome, sizeof(aluno.nome), stdin);
    aluno.nome[strcspn(aluno.nome, "\n")] = '\0';

	printf("Digite a idade: ");
	if (!lerInteiro(&aluno.idade)) return;
	printf("Digite o periodo: ");
	if (!lerInteiro(&aluno.periodo)) return;

    Resultado resultado = gerarCodigoGenerico(aluno.matricula, ARQUIVO_ALUNOS, sizeof(Aluno), offsetof(Aluno, matricula), ID_ALUNO);

    if(resultado != SUCESSO){
        printf("\nErro ao gerar matricula.\n");
        return;
    }

    if(confirmarEscolha("Deseja confirmar o cadastro?")){
        resultado = salvarGenerico(&aluno, sizeof(Aluno), ARQUIVO_ALUNOS, NOVO, 0);
        Sleep(DELAY_PROPOSITAL);
        if(resultado == SUCESSO) printf("\nAluno cadastrado com sucesso!\n");
        else printf("\nErro ao cadastrar aluno!\n");
    } 

    menuPosOperacao(submenuAluno);
}

void formularioEditarAluno(){
    Aluno aluno;
    char matricula[20];

    exibirMenu("EDITAR ALUNO", NULL, 0, true);
	
    printf("Digite a matricula: ");
    fgets(matricula, sizeof(matricula), stdin);
    matricula[strcspn(matricula, "\n")] = '\0';  
    
    Sleep(DELAY_PROPOSITAL);
    if(!mostrarAlunoPorMatricula(matricula)){
        printf("\nNao foi encontrado aluno com esssa matirucla.\n");
    }else{
        printf("Digite o novo nome: ");
        fgets(aluno.nome, sizeof(aluno.nome), stdin);
        aluno.nome[strcspn(aluno.nome, "\n")] = '\0';
        
        printf("Digite a nova idade: ");
        if (!lerInteiro(&aluno.idade)) {
            printf("\nEntrada invalida! Digite um numero valido.\n");
            return;
        }
        printf("Digite o novo periodo: ");
        if (!lerInteiro(&aluno.periodo)) {
            printf("\nEntrada invalida! Digite um numero valido.\n");
            return;
        }
        
        strcpy(aluno.matricula, matricula);

        if(confirmarEscolha("Deseja confirmar a edicao?")){
            Resultado resultado = salvarGenerico(&aluno, sizeof(Aluno), ARQUIVO_ALUNOS, ATUALIZAR, offsetof(Aluno, matricula));
            Sleep(DELAY_PROPOSITAL);

            if(resultado != SUCESSO){
                printf("\nErro ao editar aluno.\n");
            }else{
                printf("\nAluno atualizado com sucesso.\n");
            }
        }
    }
    menuPosOperacao(submenuAluno);
}

void formularioCadastrarDisciplina(){
    Disciplina disciplina;
	
    printf("Digite o nome: ");
    fgets(disciplina.nome, sizeof(disciplina.nome), stdin);
    disciplina.nome[strcspn(disciplina.nome, "\n")] = '\0';

	printf("Digite o periodo: ");
	if (!lerInteiro(&disciplina.periodo)) {
		return;
	}
    //deveria perguntar confindadno a criação
    gerarCodigoGenerico(disciplina.codigo, ARQUIVO_DISCIPLINAS, sizeof(Disciplina), offsetof(Disciplina, codigo), ID_DISCIPLINA);    
    salvarGenerico(&disciplina, sizeof(Caderneta), ARQUIVO_DISCIPLINAS, NOVO, 0); 

}

void formularioEditarDisciplina(){

}

void formularioEditarCaderneta(){

}

void formularioCadastrarCaderneta(){
    Caderneta caderneta;
	Disciplina disciplina;

    char codigo[20];

    printf("Digite o codigo da disciplina: ");
    fgets(codigo, sizeof(codigo), stdin);
    codigo[strcspn(codigo, "\n")] = '\0';

    //deveria perguntar confindadno a criação

    gerarCodigoGenerico(caderneta.codigo, ARQUIVO_CADERNETAS, sizeof(Caderneta), offsetof(Caderneta, codigo), ID_CADERNETA); 
    salvarGenerico(&caderneta, sizeof(Caderneta), ARQUIVO_CADERNETAS, NOVO, 0); 
}

Resultado salvarGenerico( const void *dado, size_t tamanho, const char *nome_arquivo, OperacaoArquivo operacao, size_t offset_identificador){
    FILE *arq;

    if (operacao == NOVO) {
        arq = fopen(nome_arquivo, "ab");
        
        if (fwrite(dado, tamanho, 1, arq) != 1) {
            fclose(arq);
            return ERRO_GRAVAR_ARQUIVO;
        }
        if (fclose(arq) != 0)
            return ERRO_FECHAR_ARQUIVO;
        return SUCESSO;
    }

    arq = fopen(nome_arquivo, "r+b");

    if (arq == NULL) return ERRO_ABRIR_ARQUIVO;

    void *registro = malloc(tamanho);

    if (registro == NULL) {
        fclose(arq);
        return ERRO_MEMORIA;
    }

    while (fread(registro, tamanho, 1, arq) == 1) {
        char *id_arquivo = (char *)registro + offset_identificador;
        char *id_dado = (char *)dado + offset_identificador;

        if (strcmp(id_arquivo, id_dado) == 0) {
            if (fseek(arq, -(long)tamanho, SEEK_CUR) != 0) {
                free(registro);
                fclose(arq);
                return ERRO_POSICIONAR_ARQUIVO;
            }
            if (fwrite(dado, tamanho, 1, arq) != 1) {
                free(registro);
                fclose(arq);
                return ERRO_GRAVAR_ARQUIVO;
            }
            free(registro);
            fclose(arq);

            return SUCESSO;
        }
    }
    free(registro);
    fclose(arq);

    return false;
}

Resultado gerarCodigoGenerico(char codigo[20], const char *nomeArquivo, size_t tamanhoRegistro, size_t offsetCodigo, char identificador){
    FILE *arq = fopen(nomeArquivo, "rb");

    time_t agora = time(NULL);
    struct tm *data = localtime(&agora);

    if (data == NULL) return ERRO_DATA;

    int anoAtual = data->tm_year + 1900;
    int maiorNumero = 0;

    if (arq != NULL) {
        void *registro = malloc(tamanhoRegistro);

        if (registro == NULL) {
            fclose(arq);
            return ERRO_MEMORIA;
        }

        while (fread(registro, tamanhoRegistro, 1, arq) == 1) {
            char *campoCodigo = (char *)registro + offsetCodigo;
            int ano;
            int numero;

            if (sscanf(campoCodigo, "%*c%4d%4d", &ano, &numero) == 2) {
                if (ano == anoAtual && numero > maiorNumero) maiorNumero = numero;
            }
        }
        free(registro);
        if (fclose(arq) != 0) return ERRO_FECHAR_ARQUIVO;
    }
    maiorNumero++;

    sprintf(codigo, "%c%04d%04d", identificador, anoAtual, maiorNumero);
    return SUCESSO;
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

bool confirmarEscolha(const char *mensagem){
    printf("\n");
    char resposta[5];

    printf("%s (S/N): ", mensagem);
    fgets(resposta, sizeof(resposta), stdin);

    return resposta[0] != '\n' &&
           resposta[1] == '\n' &&
           (resposta[0] == 'S' || resposta[0] == 's');
}

void sairPrograma(){
    if(confirmarEscolha("Deseja encerrar o programa?")){
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

void menuPosOperacao(void (*voltar)()){
    printf("\n\n");
    const char *opcoes[] = {
        "VOLTAR",
        "VOLTAR AO MENU PRINCIPAL"
    };

    exibirMenu("", opcoes, 2, false);

    void (*funcoes[2])() = {voltar, NULL};
    mostrarOpcao(funcoes, 2);
}

int main() {
    while (!encerrar_programa) menuPrincipal();
    return 0;
}