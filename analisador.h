#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // isalpha, isdigit, isspace
#include "tabelaSimbolos.c"
#include "tabelaTokens.c"

#ifndef ANALISADOR_H
#define ANALISADOR_H
#define MAX_TOKEN_SIZE 100
#define MAX_LINE_LENGHT 1024

static TabelaSimbolos tabela;
static TabelaTokens tabelaTokens;
static int erroLexico = 0;

const char palavras_reservadas [12][20] = 
{
    "program", "var", "integer", "real", "boolean",
    "begin", "end", "if", "then", "else", "while", "do"
}; 

int comparar_tabela(char input[45])
{
    for (int i = 0; i < 12; i++)
    {
        if (!strcmp(input, palavras_reservadas[i])) return 1;
    }

    return 0;
}

int verificar_operador(char input[45])
{
    if (!strcmp(input, "=")) return 1; 
    if (!strcmp(input, ">=")) return 2;
    if (!strcmp(input, "*")) return 3;
    if (!strcmp(input, "<>")) return 4;
    if (!strcmp(input, "<=")) return 5;
    if (!strcmp(input, "/")) return 6;
    if (!strcmp(input, ">")) return 7; 
    if (!strcmp(input, "+")) return 8; 
    if (!strcmp(input, "<")) return 9;
    if (!strcmp(input, "-")) return 10;
    if (!strcmp(input, ":=")) return 11;
    if (!strcmp(input, ":")) return 12;
    return 0;
}

int verificar_operador_char(char input)
{
    return (input == '=' || input == '*' || input == '/' || input == '>' || input == '<' || input == '+' || input == '-' || input == ':'); 
}

int verificar_simbolo(char input[45])
{
    if (!strcmp(input, "{")) return 1; 
    if (!strcmp(input, ",")) return 2;
    if (!strcmp(input, "}")) return 3;
    if (!strcmp(input, ";")) return 4;
    if (!strcmp(input, "(")) return 5;
    if (!strcmp(input, ")")) return 6;
    if (!strcmp(input, ":")) return 7;
    return 0;
}

int verificar_simbolo_char(char input)
{
    return (input == '{' || input == '}' || input == ',' || input == ';' || input == '(' || input == ')' || input == ':'); 
}

int verificar_numero(char input[45])
{
    int res = -1;
    // 2.5
    for (int i = 0; res != 0 && input[i] != '\0'; i++)
    {
        if (res == 2 || (res == 1 && input[i] == '.')) res = 2;
        else
        {
            if (isdigit(input[i]) && res != 2) res = 1;
            else res = 0;
        }
    }

    return res;
}


// A função a seguir supõe que o arquivo de saída já tenha sido aberto com o modo de escrita.
// Portanto, antes do parâmetro ser passado, é necessário que o arquivo seja aberto e o modo de escrita esteja habilltado
void registrar_token(FILE * arquivo_saida, Token token)
{
    if (arquivo_saida) 
    {
        if (!strcmp(token.nome, "ERRO"))
            erroLexico = 1;

        fprintf(arquivo_saida, "< %s, %s > (linha: %d, coluna: %d)\n", token.nome, token.lexema, token.linha, token.coluna);
        if (!erroLexico)
            pushToken(&tabelaTokens, token);
    }
}


/*
    A função que irá representar o analisador léxico a seguir, recebe uma string como parâmetro
    e fará o filtro dos caracteres por tipo. 

    Filtros do analisador léxico: alfabético > digito > operadores > símbolos.
    
    Exemplo de funcionamento:

    Imagine que a string passada por parâmetro seja "Exemplo;".
    O analisador léxico irá filtrar primeiro por caracteres alfabéticos e armazenar isso em uma string chamada
    "filtro_input", ou seja, após a filtragem, a variável filtro_input terá "Exemplo" como conteúdo. 
    Enfim, o analisador registra o token "Exemplo" decidindo se ele é uma palavra reservada ou um identificador.
    
    Depois, o analisador irá filtrar por dígitos, operadores e, por último, símbolos. 
    Ao filtrar por símbolos, a variável filtro_input terá como conteúdo ";". 
    Assim, o analisador irá registrar o token ";" com a categoria de símbolo SMB_SEM.
*/

void analisador_lexicografico(char input[45], FILE * arquivo_saida, int linha, int coluna)
{
    Token token;
    token.linha = linha;

    // A variável a seguir será responsável por separar o input por um único tipo de caractere. 
    // Exemplo: caso a string seja "program;" então a variável pode conter apenas
    // caracteres alfabéticos "program" ou apenas o delimitador ";"

    char filtro_input[45];
    
    int index_input = 0, index_filtro = 0, res;

    while (input[index_input] != '\0')
    {
        if (isalpha(input[index_input]))
        {
            index_filtro = 0;
            while (isalpha(input[index_input]))
            {
                filtro_input[index_filtro++] = input[index_input++];
            }

            filtro_input[index_filtro] = '\0';

            strcpy(token.lexema, filtro_input);

            if (comparar_tabela(filtro_input)) strcpy(token.nome, "PAL-RES");
            else 
            {
                push(&tabela, filtro_input);
                char str[10], output[20];
                itoa(procurarSimbolo(&tabela, filtro_input), str, 10);
                sprintf(output, "Indice: %s", str);
                strcpy(token.lexema, output);
                strcpy(token.nome , "ID");
            }

            coluna++;
        }

        else if (isdigit(input[index_input]))
        {
            index_filtro = 0;
            while (isdigit(input[index_input]) || input[index_input] == '.')
            {
                filtro_input[index_filtro++] = input[index_input++];
            }

            filtro_input[index_filtro] = '\0';

            strcpy(token.lexema, filtro_input);

            if (res = verificar_numero(filtro_input))
            {
                switch (res)
                {
                    case 1:
                        strcpy(token.nome, "NUM_INT");
                        break;
                    case 2:
                        strcpy(token.nome, "NUM_FLT");
                }
            }

            coluna++;
        }

        else if (verificar_operador_char(input[index_input]))
        {
            
            index_filtro = 0;
            while (verificar_operador_char(input[index_input]))
            {
                filtro_input[index_filtro++] = input[index_input++];
            }
            
            filtro_input[index_filtro] = '\0';
            strcpy(token.lexema, filtro_input);

            res = verificar_operador(filtro_input);
            switch (res)
            {
                case 1:
                    strcpy(token.nome, "OP_EQ");
                    break;
                case 2:
                    strcpy(token.nome, "OP_GE");
                    break;
                case 3:
                    strcpy(token.nome, "OP_MUL");
                    break;
                case 4:
                    strcpy(token.nome, "OP_NE");
                    break;
                case 5:
                    strcpy(token.nome, "OP_LE");
                    break;
                case 6:
                    strcpy(token.nome, "OP_DIV");
                    break;
                case 7:
                    strcpy(token.nome, "OP_GT");
                    break;
                case 8:
                    strcpy(token.nome, "OP_AD");
                    break;
                case 9:
                    strcpy(token.nome, "OP_LT");
                    break;
                case 10:
                    strcpy(token.nome, "OP_MIN");
                    break;
                case 11:
                    strcpy(token.nome, "OP_ASS");
                    break;
                case 12:
                    strcpy(token.nome, "SMB_COL");
                    break;
            }

            coluna++;
        }

        else if (verificar_simbolo_char(input[index_input]))
        {
            index_filtro = 0;
            while (verificar_simbolo_char(input[index_input]))
            {
                filtro_input[index_filtro++] = input[index_input++];
            }

            filtro_input[index_filtro] = '\0';

            strcpy(token.lexema, filtro_input);

            res = verificar_simbolo(filtro_input);
            switch (res)
            {
                case 1:
                    strcpy(token.nome, "SMB_OBC");
                    break;
                case 2:
                    strcpy(token.nome, "SMB_COM");
                    break;
                case 3:
                    strcpy(token.nome, "SMB_CBC");
                    break;
                case 4:
                    strcpy(token.nome, "SMB_SEM");
                    break;
                case 5:
                    strcpy(token.nome, "SMB_OPA");
                    break;
                case 6:
                    strcpy(token.nome, "SMB_CPA");
                    break;
                case 7:
                    strcpy(token.nome, "SMB_COL");
                    break;
            }

            coluna++;
        }

        else if (input[index_input] == '.')
        {
            // Ponto isolado pode ser um ponto final ou um ponto decimal.
            // Para garantir que é um ponto final, verifica se o próximo caractere é espaço ou delimitador.
            if (input[index_input + 1] == '\0' || isspace(input[index_input + 1]))
            {
                strcpy(token.lexema, ".");
                strcpy(token.nome, "PONTO_FINAL");
                index_input++;
            }

            coluna++;
        }

        else 
        {
            strcpy(token.lexema, "Caractere nao identificado");
            strcpy(token.nome, "ERRO");
            index_input++;

            coluna++;
        }

        token.coluna = coluna;
        registrar_token(arquivo_saida, token);
    } 
    
}

// Aqui se encontra as funções do analisador sintático

static int consumirToken(TabelaTokens * tabela, const char * esperado)
{
    // para o funcionamento da declaração de variáveis

    if (!strcmp(esperado, "VAR"))
    {
        if (!strcmp(atualToken(tabela).lexema, "var"))
        {
            proximoToken(tabela);
            return 1;
        }
        return 0;
    }
    else if (!strcmp(esperado, "TIPO"))
    {
        if 
        (
            !strcmp(atualToken(tabela).lexema, "integer") ||
            !strcmp(atualToken(tabela).lexema, "real") ||
            !strcmp(atualToken(tabela).lexema, "boolean")
        )
        {
            proximoToken(tabela);
            return 1;
        }

        return 0;
    }
    else 
    {
        if (!strcmp(atualToken(tabela).nome, esperado))
        {
            proximoToken(tabela);
            return 1;
        }

        return 0;
    }
    
}

static void programa(TabelaTokens * tabela)
{
    if (strcmp(atualToken(tabela).lexema, "program"))
    {
        printf("ERRO: esperado palavra reservada 'program'\n");
        exit(1);
    }

    proximoToken(tabela);

    if (!consumirToken(tabela, "ID"))
    {
        printf("ERRO: esperado identificador apos 'PROGRAM'\n");
        printf("%s\n", atualToken(tabela).lexema);
        exit(1);
    }

    if (!consumirToken(tabela, "SMB_SEM"))
    {
        printf("ERRO: esperado ';' apos o identificador\n");
        exit(1);
    }

    bloco(tabela);

    if (!strcmp(atualToken(tabela).lexema, ".") && tabela->iterator->next == NULL)
    {
        printf("Analise sintatica concluida com sucesso.\n");
    }
    else
    {
        printf("ERRO: esperado unico '.' apos o fim de programa\n");
        exit(1);
    }

    
}

static void listaIdentificadores(TabelaTokens * tabela)
{
    if (!consumirToken(tabela, "ID"))
    {
        printf("ERRO: Identificador esperado após palavra reservada 'var'\n");
        exit(1);
    }

    while (consumirToken(tabela, "SMB_COM"))
    {
        if (!consumirToken(tabela, "ID"))
        {
            printf("ERRO: Identificador esperado após ','\n");
            exit(1);
        }
    }

}

static void tipo(TabelaTokens * tabela)
{
    if (!consumirToken(tabela, "TIPO"))
    {
        printf("ERRO: Esperado tipo (integer, real ou boolean) apos ':'\n");
        exit(1);
    }
}

static void declaracaoVariaveis(TabelaTokens * tabela)
{
    listaIdentificadores(tabela);

    if (!consumirToken(tabela, "SMB_COL"))
    {
        printf("ERRO: ':' esperado apos identificadores\n");
        exit(1);
    }

    tipo(tabela);
}

static void declaracaoVariaveisEscopo(TabelaTokens * tabela)
{
    while (consumirToken(tabela, "VAR"))
    {
        declaracaoVariaveis(tabela);

        if (!consumirToken(tabela, "SMB_SEM"))
        {
            printf("ERRO: Esperado ';' após declaração de variáveis\n");
            exit(1);
        }

        while (consumirToken(tabela, "ID"))
        {
            declaracaoVariaveis(tabela);

            if (!consumirToken(tabela, "SMB_SEM"))
            {
                printf("ERRO: Esperado ';' após declaração de variáveis\n");
                exit(1);
            }
        }
    }
}

void bloco(TabelaTokens * tabela)
{
    declaracaoVariaveisEscopo(tabela);
    comandoComposto(tabela);
}

static void variavel(TabelaTokens * tabela)
{
    if (!consumirToken(tabela, "ID"))
    {
        printf("ERRO: Esperado uma variável na atribuicao\n");
        exit(1);
    }
}

static void numero(TabelaTokens * tabela)
{
    if (!consumirToken(tabela, "NUM_INT") && !consumirToken(tabela, "NUM_FLT"))
    {
        printf("ERRO: Esperado um numero (integer ou float)\n");
        exit(1);
    }
}

static void fator(TabelaTokens * tabela)
{
    NoToken * temp = tabela->iterator;
    if (consumirToken(tabela, "ID"))
    {
        tabela->iterator = temp;
        variavel(tabela);
    }
    else
    {
        if (consumirToken(tabela, "NUM_INT") || consumirToken(tabela, "NUM_FLT"))
        {
            tabela->iterator = temp;
            numero(tabela);
        }
        else
        {
            printf("ERRO fator\n");
            exit(1);
        }
        
    }

}

static void termo(TabelaTokens * tabela)
{
    fator(tabela);

    if (!strcmp(atualToken(tabela).lexema, "OP_MUL") || !strcmp(atualToken(tabela).lexema, "OP_DIV"))
    {
        proximoToken(tabela);
        fator(tabela);
    }
}

static void expressaoSimples(TabelaTokens * tabela)
{
    if (consumirToken(tabela, "OP_AD") || consumirToken(tabela, "OP_MIN")) 
        termo(tabela);
    else 
        termo(tabela);
    while (consumirToken(tabela, "OP_AD") || consumirToken(tabela, "OP_MIN")) 
        termo(tabela);
}

static void expressao(TabelaTokens * tabela)
{
    expressaoSimples(tabela);

    if 
    (
        !strcmp(atualToken(tabela).nome, "OP_EQ") ||
        !strcmp(atualToken(tabela).nome, "OP_GT") ||
        !strcmp(atualToken(tabela).nome, "OP_LT") ||
        !strcmp(atualToken(tabela).nome, "OP_GE") ||
        !strcmp(atualToken(tabela).nome, "OP_NE")
    )
    {
        proximoToken(tabela);

        if 
        (
            !strcmp(atualToken(tabela).nome, "OP_EQ") ||
            !strcmp(atualToken(tabela).nome, "OP_GT") ||
            !strcmp(atualToken(tabela).nome, "OP_LT") ||
            !strcmp(atualToken(tabela).nome, "OP_GE") ||
            !strcmp(atualToken(tabela).nome, "OP_NE")
        )
        {
            proximoToken(tabela);
        }

        expressaoSimples(tabela);
    }
}

static void atribuicao(TabelaTokens * tabela)
{

    if (!consumirToken(tabela, "ID"))
    {
        printf("ERRO: identificador esperado\n");
        exit(1);
    }

    if (!consumirToken(tabela, "OP_ASS"))
    {
        printf("%s\n", atualToken(tabela).nome);
        printf("%s\n", atualToken(tabela).lexema);
        printf("ERRO: ':' esperado\n");
        exit(1);
    }

    
    expressao(tabela);
}

static void comando(TabelaTokens * tabela)
{
    // printf("%s\n", atualToken(tabela).lexema);
    // printf("%s\n", atualToken(tabela).nome);

    if (!strcmp(atualToken(tabela).nome, "ID"))
    {
        atribuicao(tabela);
    }
    else if (!strcmp(atualToken(tabela).lexema, "begin"))
    {
        comandoComposto(tabela);
    }
    else if (!strcmp(atualToken(tabela).lexema, "if"))
    {
        expressaoCondicional(tabela);
    }
    else if (!strcmp(atualToken(tabela).lexema, "while"))
    {
        comandoRepetitivo(tabela);
    }
    else
    {
        printf("COMANDO: %s\n", atualToken(tabela).lexema);
        printf("ERRO comando\n");
        exit(1);
    }
}

void comandoComposto(TabelaTokens * tabela)
{
    if (strcmp(atualToken(tabela).lexema, "begin"))
    {
        printf("ERRO: Esperado palavra reservada 'begin'\n");
        exit(1);
    }

    proximoToken(tabela);

    comando(tabela);

    while (strcmp(atualToken(tabela).lexema, "end"))
    {
        proximoToken(tabela);
        comando(tabela);
    }

    if (strcmp(atualToken(tabela).lexema, "end"))
    {
        printf("ERRO: Esperado palavra reservada 'end'\n");
        exit(1);
    }

    proximoToken(tabela);


}

void comandoRepetitivo(TabelaTokens* tabela)
{
    if(!consumirToken(tabela, "PAL-RES"))
    {
        printf("ERRO: while esperado\n");
        exit(1);
    }

    expressao(tabela);

    if (strcmp(atualToken(tabela).lexema, "do"))
    {
        printf("ERRO: do esperado\n");
        exit(1);  
    }

    proximoToken(tabela);

    comando(tabela);
}

void expressaoCondicional(TabelaTokens* tabela)
{
    if(!consumirToken(tabela, "PAL-RES"))
    {
        printf("ERRO: IF esperado\n");
        exit(1);
    }

    expressao(tabela);

    if (strcmp(atualToken(tabela).lexema, "then"))
    {
        printf("THEN: %s\n", atualToken(tabela).lexema);
        printf("ERRO: then esperado\n");
        exit(1);    
    }

    proximoToken(tabela);

    comando(tabela);

    if (!strcmp(atualToken(tabela).lexema, "else"))
    {
        proximoToken(tabela);
        comando(tabela);
    }

    // consumir ';'
    proximoToken(tabela);
    
}

#endif