# Sistema de Gerenciamento Acadêmico em C

Sistema desenvolvido em linguagem C para gerenciamento de **alunos, disciplinas e cadernetas acadêmicas**, utilizando arquivos binários para persistência dos dados.

## 📋 Sobre o projeto

O sistema permite realizar o cadastro, edição e consulta de alunos e disciplinas, além do gerenciamento de cadernetas acadêmicas.

As informações são armazenadas em arquivos binários, permitindo que os dados permaneçam salvos mesmo após o encerramento do programa.

O projeto também conta com um sistema de navegação por menus, validação de entradas, geração automática de identificadores e gerenciamento do estado das cadernetas.

## ⚙️ Funcionalidades

### 👨‍🎓 Alunos

* Cadastrar aluno
* Gerar matrícula automaticamente
* Editar dados do aluno
* Pesquisar aluno pela matrícula
* Listar todos os alunos
* Listar alunos por período
* Validação de idade mínima
* Validação do período

### 📚 Disciplinas

* Cadastrar disciplina
* Gerar código automaticamente
* Editar disciplina
* Pesquisar disciplina pelo código
* Listar todas as disciplinas
* Listar disciplinas por período
* Validação do período

### 📖 Cadernetas

* Cadastrar caderneta vinculada a uma disciplina
* Gerar código automaticamente
* Adicionar alunos à caderneta
* Atualizar notas dos alunos
* Fechar caderneta
* Impedir alterações após o fechamento
* Listar cadernetas cadastradas
* Pesquisar caderneta pelo código

### 📊 Consultas específicas

Para cada caderneta, o sistema permite:

* Listar alunos aprovados
* Listar alunos reprovados
* Identificar o aluno com maior média
* Calcular o percentual de alunos aprovados e reprovados

A média é calculada com base nas avaliações cadastradas.

**Nota mínima para aprovação:** `7.0`

## 💾 Armazenamento

O sistema utiliza arquivos binários para armazenar os registros:

```text
alunos.hro
disciplinas.hro
cadernetas.hro
```

Cada arquivo armazena os dados correspondentes às suas respectivas estruturas.

### Estruturas principais

**Aluno**

* Nome
* Idade
* Matrícula
* Período

**Disciplina**

* Nome
* Período
* Código

**Caderneta**

* Ano
* Disciplina
* Alunos
* Notas
* Código
* Estado da caderneta (aberta/fechada)

## 🔢 Geração de identificadores

Os identificadores são gerados automaticamente pelo sistema.

O formato utilizado é:

```text
A20260001
D20260001
C20260001
```

Onde:

* `A` identifica um aluno
* `D` identifica uma disciplina
* `C` identifica uma caderneta
* `2026` representa o ano
* Os quatro últimos dígitos representam a sequência do registro naquele ano

A sequência é obtida a partir dos registros existentes no respectivo arquivo.

## 🔒 Fechamento da caderneta

Uma caderneta pode ser fechada após o seu preenchimento.

Quando uma caderneta está fechada, o sistema impede operações que alterem seus dados, como:

* Adicionar alunos
* Alterar notas

A caderneta continua disponível para consulta e listagem.

## 🧭 Funcionamento do programa

Ao iniciar o programa, é apresentado o **Menu Principal**:

```text
[0] GERENCIAR ALUNOS
[1] GERENCIAR DISCIPLINAS
[2] GERENCIAR CADERNETAS
[3] SAIR
```

Cada módulo possui seus próprios menus para realizar as operações disponíveis.

Após a conclusão de uma operação, o sistema apresenta opções para:

```text
[0] VOLTAR
[1] VOLTAR AO MENU PRINCIPAL
```

## 🛠️ Tecnologias utilizadas

* Linguagem: **C**
* Compilador: **GCC 16.1.0**
* Ambiente: **MinGW/MSYS2**
* Sistema operacional utilizado no desenvolvimento: **Windows**
* Armazenamento: **Arquivos binários**
* IDE/Editor: **Visual Studio Code**

## ▶️ Como compilar

É necessário ter o GCC instalado e configurado no PATH do sistema.

Para verificar a versão instalada:

```bash
gcc --version
```

A versão utilizada no desenvolvimento deste projeto foi:

```text
GCC 16.1.0
```

### Compilação

Abra o terminal na pasta onde está o arquivo `.c` e execute:

```bash
gcc main.c -o codigo.exe
```

Caso o arquivo fonte possua outro nome, substitua `main.c` pelo nome correspondente.

### Executando o programa

No Windows:

```bash
.\codigo.exe
```

Ou, no terminal:

```bash
codigo.exe
```

## 📁 Estrutura dos arquivos

Uma estrutura simples para o projeto pode ser:

```text
.
├── main.c
├── README.md
├── alunos.hro
├── disciplinas.hro
└── cadernetas.hro
```

Os arquivos `.hro` são criados/utilizados pelo programa para armazenar os dados.

> Recomenda-se manter os arquivos `.hro` no mesmo diretório do executável para que o programa consiga acessá-los corretamente.

## ✅ Validações

O sistema possui validações para evitar entradas inválidas, incluindo:

* Entradas numéricas inválidas
* Idade abaixo do mínimo permitido
* Período fora do intervalo permitido
* Notas fora do intervalo `0 a 10`
* Avaliações inexistentes
* Matrículas ou códigos não encontrados
* Cadernetas inexistentes
* Tentativa de alteração de caderneta fechada
* Limite máximo de alunos por caderneta

## 📌 Limites definidos

Atualmente, o sistema utiliza os seguintes limites:

| Configuração                   |   Valor |
| ------------------------------ | ------: |
| Idade mínima                   | 15 anos |
| Máximo de alunos por caderneta |       5 |
| Número de avaliações           |       2 |
| Número máximo de períodos      |       3 |
| Nota mínima para aprovação     |     7.0 |

## 👥 Equipe

* **Hilquias**
* **Ernani**
* **Maiane**
* **Sabrina**

## 🔗 Repositório

GitHub:

**https://github.com/hilquiasro/PROJETO_C_IFBA**

## 📄 Observações

Este projeto foi desenvolvido como atividade acadêmica, com foco na aplicação de conceitos de programação em C, incluindo:

* Structs
* Enumerações
* Ponteiros
* Funções
* Funções genéricas por meio de ponteiros para função
* Manipulação de arquivos
* Arquivos binários
* Alocação dinâmica de memória
* Validação de entrada
* Menus e modularização
* Busca e atualização de registros
* Persistência de dados

---

**Compilador utilizado:** GCC 16.1.0 (MinGW/MSYS2)
**Plataforma de desenvolvimento:** Windows
