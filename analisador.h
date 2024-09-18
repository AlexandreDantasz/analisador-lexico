#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // isalpha, isdigit, isspace

#ifndef ANALISADOR_H
#define ANALISADOR_H

const char tabela_simbolos [12][20] = 
{
    "program", "var", "integer", "real", "boolean",
    "begin", "end", "if", "then", "else", "while", "do"
}; 

typedef struct Token
{
    char nome[45], lexema[45];
    int linha, coluna;
} Token;

int comparar_tabela(char input[45])
{
    for (int i = 0; i < 12; i++)
    {
        if (!strcmp(input, tabela_simbolos[i])) return 1;
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
    return 0;
}

int verificar_operador_char(char input)
{
    return (input == '=' || input == '*' || input == '/' || input == '>' || input == '<' || input == '+' || input == '-'); 
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
    for (int i = 0; res != 0 && input[i] != '\0'; i++)
    {
        if (res == 1 && input[i] == '.') res = 2;
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
    if (arquivo_saida) fprintf(arquivo_saida, "< %s, %s > (linha: %d, coluna: %d)\n", token.nome, token.lexema, token.linha, token.coluna);
}

void analisador_lexicografico(char input[45], FILE * arquivo_saida, int linha, int coluna)
{
    Token token;
    token.linha = linha;
    token.coluna = coluna;

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
            else strcpy(token.nome, "ID");
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
            }
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
        }

        else 
        {
            strcpy(token.lexema, "Caractere nao identificado");
            strcpy(token.nome, "ERRO");
            index_input++;
        }

        registrar_token(arquivo_saida, token);
    }

    
}


#endif