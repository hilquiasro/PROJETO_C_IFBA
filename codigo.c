#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <stddef.h>

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

#define NOTA_APROVACAO 7.0

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
    ERRO_NAO_ENCONTRADO,
    ERRO_CADERNETA_FECHADA
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
    bool fechada;
} Caderneta;

void menuPrincipal(); 
void submenuAluno(); 
void submenuDisciplina(); 
void submenuCaderneta();

void submenuListarAlunos(); 
void submenuListarDisciplinas();
void submenuListarCaderneta();
void submenuListagemEspecificaCaderneta(const Caderneta *caderneta);

void submenuEditarCaderneta();

void formularioCadastrarAluno(); 
void formularioCadastrarDisciplina();
void formularioCadastrarCaderneta();
void formularioFecharCaderneta();

void formularioEditarAluno();
void formularioEditarDisciplina();
void formularioEditarNotasCaderneta();
void formularioAdicionarAlunoCaderneta();

void exibirAluno(const void *registro); 
void exibirDisciplina(const void *registro); 
void exibirCaderneta(const void *registro); 
void exibirTabelaCaderneta(const Caderneta *caderneta);

Resultado exibirAprovados(const Caderneta *caderneta);
Resultado exibirReprovados(const Caderneta *caderneta);
Resultado exibirAlunoMaiorNota(const Caderneta *caderneta);
Resultado exibirPercentualAprovadosReprovados(const Caderneta *caderneta);

void editarNotasCaderneta(const char *codigo);
void adicionarAlunoCaderneta(const char *codigo);
void fecharCaderneta(const char *codigo);
float calcularMediaAluno(const Caderneta *caderneta, int indiceAluno);

Resultado listarGenerico(const char *nomeArquivo, size_t tamanhoRegistro, bool (*filtro)(const void *, const void *), const void *criterio, void (*exibir)(const void *), int *contador); 
Resultado buscarGenerico(const char *nomeArquivo, size_t tamanhoRegistro, bool (*filtro)(const void *, const void *), const void *criterio, void *resultado);
Resultado salvarGenerico(const void *dado, size_t tamanho, const char *nome_arquivo, OperacaoArquivo operacao, size_t offset_identificador); 
Resultado gerarCodigoGenerico(char codigo[20], const char *nomeArquivo, size_t tamanhoRegistro, size_t offsetCodigo, char identificador); 

bool filtroMatricula(const void *registro, const void *criterio); 
bool filtroCodigoDisciplina(const void *registro, const void *criterio); 
bool filtroCodigoCaderneta(const void *registro, const void *criterio); 
bool filtroPeriodoAluno(const void *registro, const void *criterio); 
bool filtroPeriodoDisciplina(const void *registro, const void *criterio); 

bool lerInteiro(int *valor); 
bool lerFloat(float *valor);
void exibirMenu(const char *titulo, const char *opcoes[], int quantidade, bool limpar_tela, bool mostrar_rodape); 
void menuPosOperacao(void (*voltar)()); 
void mostrarOpcao(void (*funcao[])(), int quantidade); 
bool confirmarEscolha(const char *mensagem); 
void sairPrograma(); 

bool encerrar_programa = false;

void menuPrincipal(){ 
    const char *opcoes[] = {
        "GERENCIAR ALUNOS",
        "GERENCIAR DISCIPLINAS",
        "GERENCIAR CADERNETAS",
        "SAIR"
    };

    exibirMenu("MENU PRINCIPAL", opcoes, 4, true, true);
    
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

    exibirMenu("GERENCIAR ALUNOS", opcoes, 4, true, true);
    
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

    exibirMenu("GERENCIAR DISCIPLINAS", opcoes, 4, true, true);

    void (*funcoes[4])() = {formularioCadastrarDisciplina, formularioEditarDisciplina, submenuListarDisciplinas, NULL};
    mostrarOpcao(funcoes, 4);
}

void submenuCaderneta(){
    const char *opcoes[] = {
        "CADASTRAR CADERNETA",
        "EDITAR CADERNETA",
        "LISTAR CADERNETAS",
        "VOLTAR AO MENU PRINCIPAL"
    };

    exibirMenu("GERENCIAR CADERNETAS", opcoes, 4, true, true);

    void (*funcoes[4])() = {formularioCadastrarCaderneta, submenuEditarCaderneta, submenuListarCaderneta, NULL};
    mostrarOpcao(funcoes, 4);
}

void submenuEditarCaderneta(){
    const char *opcoes[] = {
        "ADICIONAR ALUNO",
        "ATUALIZAR NOTAS",
        "FECHAR CADERNETA",
        "VOLTAR"
    };

    exibirMenu("GERENCIAR CADERNETAS", opcoes, 4, true, true);

    void (*funcoes[4])() = {formularioAdicionarAlunoCaderneta, formularioEditarNotasCaderneta, formularioFecharCaderneta, submenuCaderneta};
    mostrarOpcao(funcoes, 4);
}

void submenuListarAlunos(){
    const char *opcoes[] = {
        "PROCURAR ALUNO POR MATRICULA",
        "LISTAR TODOS ALUNOS CADASTRADOS",
        "LISTAR ALUNOS POR PERIODO",
        "VOLTAR"
    };

    exibirMenu("LISTAR ALUNOS", opcoes, 4, true, true);

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
            exibirMenu("ALUNO", NULL, 0, true, false);
            Resultado resultado = listarGenerico(ARQUIVO_ALUNOS, sizeof(Aluno), filtroMatricula, matricula, exibirAluno, NULL);
            
            if(resultado == NENHUM_REGISTRO)
                printf("\nAluno nao encontrado!\n");
            else if(resultado != SUCESSO)
                printf("\nErro ao procurar aluno!\n");

            break;
        }
        case 1:{
            int contador;

            Sleep(DELAY_PROPOSITAL);
            exibirMenu("ALUNOS", NULL, 0, true, false);
            Resultado resultado = listarGenerico(ARQUIVO_ALUNOS, sizeof(Aluno), NULL, NULL, exibirAluno, &contador);

            if(resultado == NENHUM_REGISTRO) printf("\nNenhum aluno cadastrado!\n");
            else if(resultado == SUCESSO) 
                printf("\n================================================\nTotal de alunos cadastrados: %d\n", contador);
            else printf("\nErro ao listar alunos!\n");

            break;
        }
        case 2:{
            int periodo;
            int contador;

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

            exibirMenu("ALUNOS", NULL, 0, true, false);
            Resultado resultado = listarGenerico(ARQUIVO_ALUNOS, sizeof(Aluno), filtroPeriodoAluno, &periodo, exibirAluno, &contador);

            if(resultado == NENHUM_REGISTRO) printf("\nNenhum aluno cadastrado para o periodo informado!\n");
            else if(resultado == SUCESSO) 
                printf("================================================\n\nTotal de alunos no periodo '%d': %d\n", periodo, contador);
            else printf("\nErro ao listar alunos!\n");
            break;
        }
        case 3: submenuAluno(); return;

        default: printf("\nOpcao invalida! Digite uma opcao valida.\n"); break;
    }

    menuPosOperacao(submenuListarAlunos);
}

void submenuListarDisciplinas(){
    const char *opcoes[] = {
        "PESQUISAR DISCIPLINA PELO CODIGO",
        "LISTAR TODAS DISCIPLINAS CADASTRADAS",
        "LISTAR DISCIPLINAS POR PERIODO",
        "VOLTAR"
    };

    exibirMenu("LISTAR DISCIPLINAS", opcoes, 4, true, true);

    int opcao;
    printf("Digite uma opcao: ");

    if(!lerInteiro(&opcao)){
        printf("\nEntrada invalida! Digite um numero valido.\n");
        return;
    }

    switch(opcao){
        case 0:{
            char codigo[20];

            printf("Digite o codigo: ");
            fgets(codigo, sizeof(codigo), stdin);
            codigo[strcspn(codigo, "\n")] = '\0';

            Sleep(DELAY_PROPOSITAL);

            exibirMenu("DISCIPLINA", NULL, 0, true, false);
            Resultado resultado = listarGenerico(ARQUIVO_DISCIPLINAS, sizeof(Disciplina), filtroCodigoDisciplina, codigo, exibirDisciplina, NULL);

            if(resultado == NENHUM_REGISTRO)
                printf("\nDisciplina nao encontrada!\n");
            else if(resultado != SUCESSO)
                printf("\nErro ao pesquisar disciplina!\n");

            break;
        }
        case 1:{
            int contador;

            Sleep(DELAY_PROPOSITAL);

            exibirMenu("DISCIPLINAS", NULL, 0, true, false);
            Resultado resultado = listarGenerico(ARQUIVO_DISCIPLINAS, sizeof(Disciplina), NULL, NULL, exibirDisciplina, &contador);

            if(resultado == NENHUM_REGISTRO)
                printf("\nNenhuma disciplina cadastrada!\n");
            else if(resultado == SUCESSO)
                printf("================================================\nTotal de disciplinas cadastradas: %d\n", contador);
            else
                printf("\nErro ao listar disciplinas!\n");

            break;
        }
        case 2:{
            int periodo;
            int contador;

            printf("Digite o periodo: ");

            if(!lerInteiro(&periodo)){
                printf("\nEntrada invalida! Digite um numero valido.\n");
                break;
            }

            if(periodo < 0 || periodo > MAX_PERIODOS){
                printf("\nPeriodo invalido!\n");
                break;
            }

            Sleep(DELAY_PROPOSITAL);

            exibirMenu("DISCIPLINAS", NULL, 0, true, false);
            Resultado resultado = listarGenerico(ARQUIVO_DISCIPLINAS, sizeof(Disciplina), filtroPeriodoDisciplina, &periodo, exibirDisciplina, &contador);

            if(resultado == NENHUM_REGISTRO)
                printf("\nNenhuma disciplina encontrada para o periodo informado!\n");
            else if(resultado == SUCESSO) 
                printf("================================================\nTotal de disciplinas no periodo '%d': %d\n", periodo, contador);
            else printf("\nErro ao listar alunos!\n");

            break;
        }
        case 3:
            submenuDisciplina();
            return;
        default:
            printf("\nOpcao invalida! Digite uma opcao valida.\n");
            break;
    }
    menuPosOperacao(submenuListarDisciplinas);
}

void submenuListarCaderneta(){
    const char *opcoes[] = {
        "PROCURAR CADERNETA PELO CODIGO",
        "LISTAR TODAS AS CADERNETAS",
        "LISTAGENS ESPECIFICAS",
        "VOLTAR"
    };

    exibirMenu("LISTAR CADERNETAS", opcoes, 4, true, true);

    int opcao;
    printf("Digite uma opcao: ");

    if(!lerInteiro(&opcao)){
        printf("\nEntrada invalida! Digite um numero valido.\n");
        return;
    }

    switch(opcao){
        case 0:{
            char codigo[20];
            Caderneta caderneta;

            printf("Digite o codigo da caderneta: ");
            fgets(codigo, sizeof(codigo), stdin);
            codigo[strcspn(codigo, "\n")] = '\0';

            Resultado resultado = buscarGenerico(ARQUIVO_CADERNETAS, sizeof(Caderneta), filtroCodigoCaderneta, codigo, &caderneta);
            Sleep(DELAY_PROPOSITAL);

            if(resultado == SUCESSO){
                exibirMenu("CADERNETA", NULL, 0, true, false);
                exibirCaderneta(&caderneta);
                exibirTabelaCaderneta(&caderneta);
            }
            else if(resultado == ERRO_NAO_ENCONTRADO)
                printf("\nCaderneta nao encontrada!\n");
            else
                printf("\nErro ao procurar caderneta!\n");

            break;
        }
        case 1:{
            int contador;

            Sleep(DELAY_PROPOSITAL);
            exibirMenu("CADERNETAS", NULL, 0, true, false);
            Resultado resultado = listarGenerico(ARQUIVO_CADERNETAS, sizeof(Caderneta), NULL, NULL, exibirCaderneta, &contador);

            if(resultado == NENHUM_REGISTRO)
                printf("\nNenhuma caderneta cadastrada!\n");
            else if(resultado == SUCESSO)
                printf("================================================\nTotal de cadernetas cadastradas: %d\n", contador);
            else
                printf("\nErro ao listar cadernetas!\n");

            break;
        }
        case 2:{
            char codigo[20];
            Caderneta caderneta;

            printf("Digite o codigo da caderneta: ");
            fgets(codigo, sizeof(codigo), stdin);
            codigo[strcspn(codigo, "\n")] = '\0';

            Resultado resultado = buscarGenerico(ARQUIVO_CADERNETAS, sizeof(Caderneta), filtroCodigoCaderneta, codigo, &caderneta);
            Sleep(DELAY_PROPOSITAL);

            if(resultado == SUCESSO)
                submenuListagemEspecificaCaderneta(&caderneta);
            else if(resultado == ERRO_NAO_ENCONTRADO)
                printf("\nCaderneta nao encontrada!\n");
            else
                printf("\nErro ao procurar caderneta!\n");

            return;
        }
        case 3:
            submenuCaderneta();
            return;
        default:
            printf("\nOpcao invalida! Digite uma opcao valida.\n");
            break;
    }

    menuPosOperacao(submenuListarCaderneta);
}

void submenuListagemEspecificaCaderneta(const Caderneta *caderneta){
    const char *opcoes[] = {
        "LISTAR ALUNOS APROVADOS",
        "LISTAR ALUNOS REPROVADOS",
        "LISTAR ALUNO COM MAIOR NOTA",
        "LISTAR PERCENTUAL DE ALUNOS APROVADOS E REPROVADOS",
        "VOLTAR"
    };

    exibirMenu("LISTAGENS ESPECIFICAS", opcoes, 5, true, true);

    int opcao;
    printf("Digite uma opcao: ");

    if(!lerInteiro(&opcao)){
        printf("\nEntrada invalida! Digite um numero valido.\n");
        return;
    }

    Resultado resultado;

    switch(opcao){
        case 0:
            exibirMenu("ALUNOS APROVADOS", NULL, 0, true, false);
            resultado = exibirAprovados(caderneta);
            break;

        case 1:
            exibirMenu("ALUNOS REPROVADOS", NULL, 0, true, false);
            resultado = exibirReprovados(caderneta);
            break;

        case 2:
            exibirMenu("ALUNO COM MAIOR NOTA", NULL, 0, true, false);
            resultado = exibirAlunoMaiorNota(caderneta);
            break;

        case 3:
            exibirMenu("PERCENTUAL DE ALUNOS APROVADOS E REPROVADOS", NULL, 0, true, false);
            resultado = exibirPercentualAprovadosReprovados(caderneta);
            break;

        case 4:
            submenuListarCaderneta();
            return;

        default:
            printf("\nOpcao invalida! Digite uma opcao valida.\n");
            return;
    }

    if(resultado == NENHUM_REGISTRO)
        printf("\nNao ha alunos cadastrados nesta caderneta!\n");
    else if(resultado != SUCESSO)
        printf("\nErro ao realizar listagem!\n");

    menuPosOperacao(submenuListarCaderneta);
}

Resultado listarGenerico(const char *nomeArquivo, size_t tamanhoRegistro, bool (*filtro)(const void *, const void *), const void *criterio, void (*exibir)(const void *), int *contador){
    FILE *arq = fopen(nomeArquivo, "rb");

    if(arq == NULL)
        return ERRO_ABRIR_ARQUIVO;

    void *registro = malloc(tamanhoRegistro);

    if(registro == NULL){
        fclose(arq);
        return ERRO_MEMORIA;
    }

    int total = 0;

    while(fread(registro, tamanhoRegistro, 1, arq) == 1){
        if(filtro == NULL || filtro(registro, criterio)){
            exibir(registro);
            total++;
        }
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

Resultado buscarGenerico(const char *nomeArquivo, size_t tamanhoRegistro, bool (*filtro)(const void *, const void *), const void *criterio, void *resultado){
    FILE *arq = fopen(nomeArquivo, "rb");

    if(arq == NULL)
        return ERRO_ABRIR_ARQUIVO;

    void *registro = malloc(tamanhoRegistro);

    if(registro == NULL){
        fclose(arq);
        return ERRO_MEMORIA;
    }

    while(fread(registro, tamanhoRegistro, 1, arq) == 1){
        if(filtro(registro, criterio)){
            memcpy(resultado, registro, tamanhoRegistro);
            free(registro);

            if(fclose(arq) != 0)
                return ERRO_FECHAR_ARQUIVO;

            return SUCESSO;
        }
    }

    free(registro);

    if(fclose(arq) != 0)
        return ERRO_FECHAR_ARQUIVO;

    return ERRO_NAO_ENCONTRADO;
}

void exibirAluno(const void *registro){
    const Aluno *aluno = registro;

    printf("--------------------------------------------------------\n");
    printf("Matricula : %s\n", aluno->matricula);
    printf("Nome      : %s\n", aluno->nome);
    printf("Idade     : %d anos\n", aluno->idade);
    printf("Periodo   : %d\n", aluno->periodo);
}

void exibirDisciplina(const void *registro){
    const Disciplina *disciplina = registro;

    printf("--------------------------------------------------------\n");
    printf("Nome    : %s\n", disciplina->nome);
    printf("Codigo  : %s\n", disciplina->codigo);
    printf("Periodo : %d\n", disciplina->periodo);
}

void exibirCaderneta(const void *registro){
    const Caderneta *caderneta = registro;

    printf("--------------------------------------------------------\n");
    printf("Codigo     : %s\n", caderneta->codigo);
    printf("Disciplina : %s\n", caderneta->disciplina.nome);
    printf("Periodo : %d\n", caderneta->disciplina.periodo);
}

void exibirTabelaCaderneta(const Caderneta *caderneta){
    printf("\n========================================================\n");
    printf("%-4s", "");

    for(int j = 0; j < MAX_NOTAS; j++)
        printf("AV%-5d", j + 1);

    printf("%-16s %-25s\n", "MATRICULA", "NOME");
    printf("--------------------------------------------------------\n");

    for(int i = 0; i < MAX_ALUNOS; i++){
        printf("%-4d", i + 1);

        for(int j = 0; j < MAX_NOTAS; j++)
            printf("%-8.1f", caderneta->notas[i][j]);

        printf("%-16s %-25.25s\n", caderneta->alunos[i].matricula, caderneta->alunos[i].nome);
    }

    printf("========================================================\n");
}

Resultado exibirAprovados(const Caderneta *caderneta){
    bool encontrou = false;

    exibirCaderneta(caderneta);
    printf("\nALUNOS APROVADOS\n");

    for(int i = 0; i < MAX_ALUNOS; i++){
        if(caderneta->alunos[i].matricula[0] == '\0')
            continue;

        float media = calcularMediaAluno(caderneta, i);

        if(media >= NOTA_APROVACAO){
            printf("--------------------------------------------------------\n");
            printf("Matricula : %s\n", caderneta->alunos[i].matricula);
            printf("Nome      : %s\n", caderneta->alunos[i].nome);
            printf("Media     : %.2f\n", media);

            encontrou = true;
        }
    }

    printf("========================================================\n");

    if(!encontrou)
        return NENHUM_REGISTRO;

    return SUCESSO;
}

Resultado exibirReprovados(const Caderneta *caderneta){
    bool encontrou = false;

    exibirCaderneta(caderneta);
    printf("\nALUNOS REPROVADOS\n");

    for(int i = 0; i < MAX_ALUNOS; i++){
        if(caderneta->alunos[i].matricula[0] == '\0')
            continue;

        float media = calcularMediaAluno(caderneta, i);

        if(media < NOTA_APROVACAO){
            printf("--------------------------------------------------------\n");
            printf("Matricula : %s\n", caderneta->alunos[i].matricula);
            printf("Nome      : %s\n", caderneta->alunos[i].nome);
            printf("Media     : %.2f\n", media);

            encontrou = true;
        }
    }

    printf("========================================================\n");

    if(!encontrou)
        return NENHUM_REGISTRO;

    return SUCESSO;
}

Resultado exibirAlunoMaiorNota(const Caderneta *caderneta){
    int indiceMaior = -1;
    float maiorMedia = -1;

    for(int i = 0; i < MAX_ALUNOS; i++){
        if(caderneta->alunos[i].matricula[0] == '\0')
            continue;

        float media = calcularMediaAluno(caderneta, i);

        if(media > maiorMedia){
            maiorMedia = media;
            indiceMaior = i;
        }
    }

    if(indiceMaior == -1)
        return NENHUM_REGISTRO;

    exibirCaderneta(caderneta);

    printf("\nALUNO COM MAIOR MEDIA\n");
    printf("--------------------------------------------------------\n");
    printf("Matricula : %s\n", caderneta->alunos[indiceMaior].matricula);
    printf("Nome      : %s\n", caderneta->alunos[indiceMaior].nome);
    printf("Media     : %.2f\n", maiorMedia);
    printf("========================================================\n");

    return SUCESSO;
}

Resultado exibirPercentualAprovadosReprovados(const Caderneta *caderneta){
    int totalAlunos = 0;
    int aprovados = 0;
    int reprovados = 0;

    for(int i = 0; i < MAX_ALUNOS; i++){
        if(caderneta->alunos[i].matricula[0] == '\0')
            continue;

        totalAlunos++;

        float media = calcularMediaAluno(caderneta, i);

        if(media >= NOTA_APROVACAO)
            aprovados++;
        else
            reprovados++;
    }

    if(totalAlunos == 0)
        return NENHUM_REGISTRO;

    float percentualAprovados = (aprovados * 100.0) / totalAlunos;
    float percentualReprovados = (reprovados * 100.0) / totalAlunos;

    exibirCaderneta(caderneta);

    printf("\nPERCENTUAL DE ALUNOS\n");
    printf("--------------------------------------------------------\n");
    printf("Total de alunos : %d\n", totalAlunos);
    printf("Aprovados       : %.2f%%\n", percentualAprovados);
    printf("Reprovados      : %.2f%%\n", percentualReprovados);
    printf("========================================================\n");

    return SUCESSO;
}

bool filtroMatricula(const void *registro, const void *criterio){
    const Aluno *aluno = registro;
    const char *matricula = criterio;

    return strcmp(aluno->matricula, matricula) == 0;
}

bool filtroCodigoDisciplina(const void *registro, const void *criterio){
    const Disciplina *disciplina = registro;
    const char *codigo = criterio;

    return strcmp(disciplina->codigo, codigo) == 0;
}

bool filtroCodigoCaderneta(const void *registro, const void *criterio){
    const Caderneta *caderneta = registro;
    const char *codigo = criterio;

    return strcmp(caderneta->codigo, codigo) == 0;
}

bool filtroPeriodoAluno(const void *registro, const void *criterio){
    const Aluno *aluno = registro;
    const int *periodo = criterio;
    
    return aluno->periodo == *periodo;
}

bool filtroPeriodoDisciplina(const void *registro, const void *criterio){
    const Disciplina *disciplina = registro;
    const char *periodo = criterio;

    return disciplina->periodo == *periodo;
}

float calcularMediaAluno(const Caderneta *caderneta, int indiceAluno){
    float soma = 0;

    for(int j = 0; j < MAX_NOTAS; j++)
        soma += caderneta->notas[indiceAluno][j];

    return soma / MAX_NOTAS;
}

void formularioCadastrarAluno(){
    Aluno aluno;

    exibirMenu("CADASTRAR ALUNO", NULL, 0, true, false);

    printf("Digite o nome: ");
    fgets(aluno.nome, sizeof(aluno.nome), stdin);
    aluno.nome[strcspn(aluno.nome, "\n")] = '\0';

    printf("Digite a idade: ");
    if(!lerInteiro(&aluno.idade)){
        printf("\nEntrada invalida!\n");
        goto fim;
    }

    if(aluno.idade < MIN_IDADE){
        printf("\nIdade invalida! A idade minima e %d anos.\n", MIN_IDADE);
        goto fim;
    }

    printf("Digite o periodo: ");
    if(!lerInteiro(&aluno.periodo)){
        printf("\nEntrada invalida!\n");
        goto fim;
    }

    if(aluno.periodo < 1 || aluno.periodo > MAX_PERIODOS){
        printf("\nPeriodo invalido! Digite um periodo entre 1 e %d.\n", MAX_PERIODOS);
        goto fim;
    }

    Resultado resultado = gerarCodigoGenerico(aluno.matricula, ARQUIVO_ALUNOS, sizeof(Aluno), offsetof(Aluno, matricula), ID_ALUNO);

    if(resultado != SUCESSO){
        Sleep(DELAY_PROPOSITAL);
        printf("\nErro ao gerar matricula.\n");
        goto fim;
    }

    if(confirmarEscolha("Deseja confirmar o cadastro?")){
        resultado = salvarGenerico(&aluno, sizeof(Aluno), ARQUIVO_ALUNOS, NOVO, 0);
        Sleep(DELAY_PROPOSITAL);

        if(resultado == SUCESSO)
            printf("Aluno cadastrado com sucesso!\n");
        else
            printf("Erro ao cadastrar aluno!\n");
    }

fim:
    menuPosOperacao(submenuAluno);
}

void formularioEditarAluno(){
    Aluno aluno;
    char matricula[20];

    exibirMenu("EDITAR ALUNO", NULL, 0, true, false);

    printf("Digite a matricula: ");
    fgets(matricula, sizeof(matricula), stdin);
    matricula[strcspn(matricula, "\n")] = '\0';

    Resultado resultado = buscarGenerico(ARQUIVO_ALUNOS, sizeof(Aluno), filtroMatricula, matricula, &aluno);
    Sleep(DELAY_PROPOSITAL);

    if(resultado == ERRO_NAO_ENCONTRADO){
        printf("\nNao foi encontrado aluno com essa matricula.\n");
    }else if(resultado != SUCESSO){
        printf("\nErro ao procurar aluno.\n");
    }else{
        exibirAluno(&aluno);

        if(confirmarEscolha("Esse e o aluno que deseja editar?")){
            if(confirmarEscolha("Deseja alterar o nome?")){
                printf("Digite o novo nome: ");
                fgets(aluno.nome, sizeof(aluno.nome), stdin);
                aluno.nome[strcspn(aluno.nome, "\n")] = '\0';
            }

            if(confirmarEscolha("Deseja alterar a idade?")){
                printf("Digite a nova idade: ");

                if(!lerInteiro(&aluno.idade)){
                    printf("\nEntrada invalida!\n");
                    goto fim;
                }

                if(aluno.idade < MIN_IDADE){
                    printf("\nIdade invalida! A idade minima e %d anos.\n", MIN_IDADE);
                    goto fim;
                }
            }

            if(confirmarEscolha("Deseja alterar o periodo?")){
                printf("Digite o novo periodo: ");

                if(!lerInteiro(&aluno.periodo)){
                    printf("\nEntrada invalida!\n");
                    goto fim;
                }

                if(aluno.periodo < 1 || aluno.periodo > MAX_PERIODOS){
                    printf("\nPeriodo invalido! Digite um periodo entre 1 e %d.\n", MAX_PERIODOS);
                    goto fim;
                }
            }

            if(confirmarEscolha("Deseja confirmar a edicao?")){
                resultado = salvarGenerico(&aluno, sizeof(Aluno), ARQUIVO_ALUNOS, ATUALIZAR, offsetof(Aluno, matricula));

                Sleep(DELAY_PROPOSITAL);

                if(resultado == SUCESSO)
                    printf("Aluno atualizado com sucesso.\n");
                else
                    printf("Erro ao editar aluno.\n");

            }
        }
    }

fim:
    menuPosOperacao(submenuAluno);
}

void formularioCadastrarDisciplina(){
    Disciplina disciplina;

    exibirMenu("CADASTRAR DISCIPLINA", NULL, 0, true, false);

    printf("Digite o nome: ");
    fgets(disciplina.nome, sizeof(disciplina.nome), stdin);
    disciplina.nome[strcspn(disciplina.nome, "\n")] = '\0';

    printf("Digite o periodo: ");
    if(!lerInteiro(&disciplina.periodo)){
        printf("\nEntrada invalida!\n");
        goto fim;
    }

    if(disciplina.periodo < 1 || disciplina.periodo > MAX_PERIODOS){
        printf("\nPeriodo invalido! Digite um periodo entre 1 e %d.\n", MAX_PERIODOS);
        goto fim;
    }

    Resultado resultado = gerarCodigoGenerico(disciplina.codigo, ARQUIVO_DISCIPLINAS, sizeof(Disciplina), offsetof(Disciplina, codigo), ID_DISCIPLINA);

    if(resultado != SUCESSO){
        Sleep(DELAY_PROPOSITAL);
        printf("\nErro ao gerar codigo.\n");
        goto fim;
    }

    if(confirmarEscolha("Deseja confirmar o cadastro?")){
        resultado = salvarGenerico(&disciplina, sizeof(Disciplina), ARQUIVO_DISCIPLINAS, NOVO, 0);
        Sleep(DELAY_PROPOSITAL);

        if(resultado == SUCESSO)
            printf("Disciplina cadastrada com sucesso!\n");
        else
            printf("Erro ao cadastrar disciplina!\n");

    }

fim:
    menuPosOperacao(submenuDisciplina);
}

void formularioEditarDisciplina(){
    Disciplina disciplina;
    char codigo[20];

    exibirMenu("EDITAR DISCIPLINA", NULL, 0, true, false);

    printf("Digite o codigo: ");
    fgets(codigo, sizeof(codigo), stdin);
    codigo[strcspn(codigo, "\n")] = '\0';

    Resultado resultado = buscarGenerico(ARQUIVO_DISCIPLINAS, sizeof(Disciplina), filtroCodigoDisciplina, codigo, &disciplina);
    Sleep(DELAY_PROPOSITAL);

    if(resultado == ERRO_NAO_ENCONTRADO){
        printf("\nNao foi encontrada disciplina com esse codigo.\n");
    }else if(resultado != SUCESSO){
        printf("\nErro ao procurar disciplina.\n");
    }else{
        exibirDisciplina(&disciplina);

        if(confirmarEscolha("Essa e a disciplina que deseja editar?")){
            if(confirmarEscolha("Deseja alterar o nome?")){
                printf("Digite o novo nome: ");
                fgets(disciplina.nome, sizeof(disciplina.nome), stdin);
                disciplina.nome[strcspn(disciplina.nome, "\n")] = '\0';
            }

            if(confirmarEscolha("Deseja alterar o periodo?")){
                printf("Digite o novo periodo: ");

                if(!lerInteiro(&disciplina.periodo)){
                    printf("\nEntrada invalida!\n");
                    goto fim;
                }

                if(disciplina.periodo < 1 || disciplina.periodo > MAX_PERIODOS){
                    printf("\nPeriodo invalido! Digite um periodo entre 1 e %d.\n", MAX_PERIODOS);
                    goto fim;
                }
            }

            if(confirmarEscolha("Deseja confirmar a edicao?")){
                resultado = salvarGenerico(&disciplina, sizeof(Disciplina), ARQUIVO_DISCIPLINAS, ATUALIZAR, offsetof(Disciplina, codigo));
                Sleep(DELAY_PROPOSITAL);

                if(resultado == SUCESSO)
                    printf("Disciplina atualizada com sucesso.\n");
                else
                    printf("Erro ao editar disciplina.\n");

            }
        }
    }

fim:
    menuPosOperacao(submenuDisciplina);
}

void formularioEditarNotasCaderneta(){
    char codigo[20];

    exibirMenu("EDITAR NOTAS DA CADERNETA", NULL, 0, true, false);

    printf("Digite o codigo da caderneta: ");
    fgets(codigo, sizeof(codigo), stdin);
    codigo[strcspn(codigo, "\n")] = '\0';

    editarNotasCaderneta(codigo);
}

void formularioCadastrarCaderneta(){
    Caderneta caderneta = {0};
    Disciplina disciplina;
    char codigo[20];

    exibirMenu("CADASTRAR CADERNETA", NULL, 0, true, false);

    printf("Digite o codigo da disciplina: ");
    fgets(codigo, sizeof(codigo), stdin);
    codigo[strcspn(codigo, "\n")] = '\0';

    Resultado resultado = buscarGenerico(ARQUIVO_DISCIPLINAS, sizeof(Disciplina), filtroCodigoDisciplina, codigo, &disciplina);
    Sleep(DELAY_PROPOSITAL);

    if(resultado == ERRO_NAO_ENCONTRADO){
        printf("\nDisciplina nao encontrada!\n");
    }else if(resultado != SUCESSO){
        printf("\nErro ao procurar disciplina!\n");
    }else{
        printf("\nDisciplina encontrada:\n");
        printf("--------------------------------------------------------\n");
        printf("Codigo: %s\n", disciplina.codigo);
        printf("Nome: %s\n", disciplina.nome);
        printf("Periodo: %d\n", disciplina.periodo);

        if(confirmarEscolha("\nDeseja confirmar esta disciplina?")){
            caderneta.disciplina = disciplina;
            caderneta.fechada = false;

            resultado = gerarCodigoGenerico(caderneta.codigo, ARQUIVO_CADERNETAS, sizeof(Caderneta), offsetof(Caderneta, codigo), ID_CADERNETA);

            if(resultado != SUCESSO){
                Sleep(DELAY_PROPOSITAL);
                printf("Erro ao gerar codigo da caderneta!\n");
            }else{
                resultado = salvarGenerico(&caderneta, sizeof(Caderneta), ARQUIVO_CADERNETAS, NOVO, 0);
                Sleep(DELAY_PROPOSITAL);

                if(resultado == SUCESSO){
                    printf("Caderneta cadastrada com sucesso!\n");
                    Sleep(DELAY_PROPOSITAL);
                    exibirMenu("ADICIONAR ALUNO NA CADERNETA", NULL, 0, true, false);
                    exibirCaderneta(&caderneta);
                    adicionarAlunoCaderneta(caderneta.codigo);
                    return;
                }else{
                    printf("Erro ao cadastrar caderneta!\n");
                }
            }
        }else{
            printf("Operacao cancelada.\n");
        }
    }

    menuPosOperacao(submenuCaderneta);
}

void formularioFecharCaderneta(){
    char codigo[20];

    exibirMenu("FECHAR CADERNETA", NULL, 0, true, false);

    printf("Digite o codigo da caderneta: ");
    fgets(codigo, sizeof(codigo), stdin);
    codigo[strcspn(codigo, "\n")] = '\0';

    fecharCaderneta(codigo);
}

void formularioAdicionarAlunoCaderneta(){
    char codigo[20];

    exibirMenu("ADICIONAR ALUNO NA CADERNETA", NULL, 0, true, false);

    printf("Digite o codigo da caderneta: ");
    fgets(codigo, sizeof(codigo), stdin);
    codigo[strcspn(codigo, "\n")] = '\0';

    adicionarAlunoCaderneta(codigo);
}

void editarNotasCaderneta(const char *codigo){
    Caderneta caderneta;

    Resultado resultado = buscarGenerico(ARQUIVO_CADERNETAS, sizeof(Caderneta), filtroCodigoCaderneta, codigo, &caderneta);
    Sleep(DELAY_PROPOSITAL);

    if(resultado == ERRO_NAO_ENCONTRADO){
        printf("\nCaderneta nao encontrada!\n");
    }else if(resultado != SUCESSO){
        printf("\nErro ao procurar caderneta!\n");
    }else if(caderneta.fechada){
        printf("\nA caderneta esta fechada e nao pode mais ser alterada!\n");
    }else{
        while(1){
            exibirTabelaCaderneta(&caderneta);

            int aluno, avaliacao;
            float nota;

            printf("Digite o numero do aluno: ");
            if(!lerInteiro(&aluno)){
                printf("\nEntrada invalida!\n");
                break;
            }else if(aluno < 1 || aluno > MAX_ALUNOS){
                printf("\nAluno invalido!\n");
                break;
            }else if(caderneta.alunos[aluno - 1].matricula[0] == '\0'){
                printf("\nNao existe aluno cadastrado nessa posicao!\n");
                break;
            }

            exibirAluno(&caderneta.alunos[aluno - 1]);

            if(!confirmarEscolha("Deseja alterar as notas deste aluno?"))
                break;

            printf("Digite a avaliacao (1-%d): ", MAX_NOTAS);

            if(!lerInteiro(&avaliacao)){
                printf("\nEntrada invalida!\n");
                break;
            }else if(avaliacao < 1 || avaliacao > MAX_NOTAS){
                printf("\nAvaliacao invalida!\n");
                break;
            }

            printf("Nota atual: %.2f\n", caderneta.notas[aluno - 1][avaliacao - 1]);
            printf("Digite a nova nota: ");

            if(!lerFloat(&nota)){
                printf("\nEntrada invalida!\n");
                break;
            }else if(nota < 0 || nota > 10){
                printf("\nNota invalida! Digite uma nota entre 0 e 10.\n");
                break;
            }

            printf("Nova nota: %.2f\n", nota);

            if(confirmarEscolha("Deseja confirmar a alteracao da nota?")){
                caderneta.notas[aluno - 1][avaliacao - 1] = nota;

                resultado = salvarGenerico(&caderneta, sizeof(Caderneta), ARQUIVO_CADERNETAS, ATUALIZAR, offsetof(Caderneta, codigo));
                Sleep(DELAY_PROPOSITAL);

                if(resultado == SUCESSO){
                    printf("\nNota atualizada com sucesso!\n");
                }else{
                    printf("\nErro ao atualizar nota!\n");
                    break;
                }
            }

            if(!confirmarEscolha("Deseja alterar outra nota?"))
                break;
        }
    }

    menuPosOperacao(submenuEditarCaderneta);
}

void adicionarAlunoCaderneta(const char *codigo){
    Caderneta caderneta;
    Aluno aluno;
    char matricula[20];
    int posicao;

    Resultado resultado = buscarGenerico(ARQUIVO_CADERNETAS, sizeof(Caderneta), filtroCodigoCaderneta, codigo, &caderneta);
    Sleep(DELAY_PROPOSITAL);

    if(resultado == ERRO_NAO_ENCONTRADO){
        printf("\nCaderneta nao encontrada!\n");
    }else if(resultado != SUCESSO){
        printf("\nErro ao procurar caderneta!\n");
    }else if(caderneta.fechada){
        printf("\nA caderneta esta fechada e nao pode mais ser alterada!\n");
    }else{
        while(1){
            posicao = -1;

            for(int i = 0; i < MAX_ALUNOS; i++){
                if(caderneta.alunos[i].matricula[0] == '\0'){
                    posicao = i;
                    break;
                }
            }

            if(posicao == -1){
                printf("\nA caderneta atingiu o limite de %d alunos!\n", MAX_ALUNOS);
                break;
            }

            exibirTabelaCaderneta(&caderneta);

            printf("\nDigite a matricula do aluno: ");
            fgets(matricula, sizeof(matricula), stdin);
            matricula[strcspn(matricula, "\n")] = '\0';

            resultado = buscarGenerico(ARQUIVO_ALUNOS, sizeof(Aluno), filtroMatricula, matricula, &aluno);
            Sleep(DELAY_PROPOSITAL);

            if(resultado == ERRO_NAO_ENCONTRADO){
                printf("\nAluno nao encontrado!\n");
            }else if(resultado != SUCESSO){
                printf("\nErro ao procurar aluno!\n");
            }else{
                printf("\nAluno encontrado:\n");
                exibirAluno(&aluno);

                if(!confirmarEscolha("Deseja cadastrar este aluno na caderneta?"))
                    break;

                caderneta.alunos[posicao] = aluno;

                resultado = salvarGenerico(&caderneta, sizeof(Caderneta), ARQUIVO_CADERNETAS, ATUALIZAR, offsetof(Caderneta, codigo));
                Sleep(DELAY_PROPOSITAL);

                if(resultado != SUCESSO){
                    printf("\nErro ao cadastrar aluno na caderneta!\n");
                    break;
                }

                printf("\nAluno cadastrado na caderneta com sucesso!\n");

                if(posicao == MAX_ALUNOS - 1){
                    printf("\nA caderneta atingiu o limite de %d alunos!\n", MAX_ALUNOS);
                    break;
                }

                if(!confirmarEscolha("Deseja cadastrar outro aluno?"))
                    break;
            }
        }
    }

    menuPosOperacao(submenuEditarCaderneta);
}

void fecharCaderneta(const char *codigo){
    Caderneta caderneta;
    
    Resultado resultado = buscarGenerico(ARQUIVO_CADERNETAS, sizeof(Caderneta), filtroCodigoCaderneta, codigo, &caderneta);
    Sleep(DELAY_PROPOSITAL);

    if(resultado == ERRO_NAO_ENCONTRADO){
        printf("\nCaderneta nao encontrada!\n");
    }else if(resultado != SUCESSO){
        printf("\nErro ao procurar caderneta!\n");
    }else if(caderneta.fechada){
        printf("\nA caderneta ja esta fechada!\n");
    }else{
        exibirTabelaCaderneta(&caderneta);
        if(confirmarEscolha("\nDeseja realmente fechar esta caderneta?")){
            caderneta.fechada = true;
            
            resultado = salvarGenerico(&caderneta, sizeof(Caderneta), ARQUIVO_CADERNETAS, ATUALIZAR, offsetof(Caderneta, codigo));
            Sleep(DELAY_PROPOSITAL);
            
            if(resultado == SUCESSO){
                printf("\nCaderneta fechada com sucesso!\n");
            }else{
                printf("\nErro ao fechar caderneta!\n");
            }
        }
    }
    menuPosOperacao(submenuEditarCaderneta);
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

void exibirMenu(const char *titulo, const char *opcoes[], int quantidade, bool limpar_tela, bool mostrar_rodape){ 
    if(limpar_tela) 
        system("cls"); 

    if(strlen(titulo) == 0){ 
        for(int i = 0; i < LARGURA_MENU; i++) 
            printf("="); 

        printf("\n"); 
    }else{ 
        int tamanhoTitulo = strlen(titulo); 
        int espacos = LARGURA_MENU - tamanhoTitulo - 2; 

        printf("="); 

        for(int i = 0; i < espacos / 2; i++) 
            printf("="); 

        printf(" %s ", titulo); 

        for(int i = 0; i < (espacos + 1) / 2; i++) 
            printf("="); 

        printf("\n"); 
    } 

    for(int i = 0; i < quantidade; i++) 
        printf("[%d] %s\n", i, opcoes[i]); 

    if(mostrar_rodape){
        for(int i = 0; i < LARGURA_MENU; i++) 
            printf("="); 

        printf("\n"); 
    }
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
        printf("Sessao finalizada!");
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

    while (*fim == ' ' || *fim == '\t') fim++;

    if (fim == entrada || (*fim != '\n' && *fim != '\0')) {
        printf("\nEntrada invalida! Digite um numero valido.\n");
        return false;
    }

    *valor = (int)numero;
    return true;
}

bool lerFloat(float *valor){
    char entrada[100];

    if(fgets(entrada, sizeof(entrada), stdin) == NULL){
        printf("\nEntrada invalida! Digite um numero valido.\n");
        return false;
    }

    char *fim;
    float numero = strtof(entrada, &fim);

    while(*fim == ' ' || *fim == '\t') fim++;

    if(fim == entrada || (*fim != '\n' && *fim != '\0')){
        printf("\nEntrada invalida! Digite um numero valido.\n");
        return false;
    }

    *valor = numero;
    return true;
}

void menuPosOperacao(void (*voltar)()){
    printf("\n\n");
    const char *opcoes[] = {
        "VOLTAR",
        "VOLTAR AO MENU PRINCIPAL"
    };

    exibirMenu("", opcoes, 2, false, true);

    void (*funcoes[2])() = {voltar, NULL};
    mostrarOpcao(funcoes, 2);
}

int main() {
    while (!encerrar_programa) menuPrincipal();
    return 0;
}