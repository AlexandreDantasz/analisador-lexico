#include "analisador.h"


typedef enum {
    TOKEN_ERROR,
    TOKEN_EOF
} TokenType;

typedef struct TokenErro {
    TokenType type;
    char value[MAX_TOKEN_SIZE];
    int line;
    int column;
} TokenErro;

// Função get_next_token para leitura de tokens de input.pas
TokenErro get_next_token(FILE *file, int *line, int *column) {
    TokenErro token;
    token.type = TOKEN_EOF;
    token.value[0] = '\0';

    char ch;
    while ((ch = fgetc(file)) != EOF) {
        (*column)++;
        if (ch == '\n') {
            (*line)++;
            *column = 0;
            continue;
        }

        if (isspace(ch)) {
            continue;
        }

        // Se encontrar um caractere inválido
        if (!isalnum(ch) && !strchr("+-*/=", ch)) {
            token.type = TOKEN_ERROR;
            token.value[0] = ch;
            token.value[1] = '\0';
            return token;
        }

        // Placeholder para token válido
        token.type = TOKEN_EOF;
        return token;
    }

    token.type = TOKEN_EOF;
    return token;
}

void report_error(TokenErro token, int line, int column) {
    printf("Error: Invalid token '%s' at line %d, column %d\n", token.value, line, column);
}

// Função principal para processamento do arquivo input.pas


void apagar_string(char * input, int * index)
{
    strcpy(input, "");
    *index = 0;
}

int main()
{
     FILE *file = fopen("input.pas", "r");
    if (!file) {
        fprintf(stderr, "Could not open file.\n");
        return 1;
    }

    int line = 1, column = 0;
    TokenErro token;

    do {
        token = get_next_token(file, &line, &column);
        if (token.type == TOKEN_ERROR) {
            report_error(token, line, column);
        }
    } while (token.type != TOKEN_EOF);

    fclose(file);

    FILE * arquivo_entrada, * arquivo_saida;
    char input[45] = "", character;
    int index = 0;
    arquivo_entrada = fopen("input.pas", "rt");
    arquivo_saida = fopen("output.lex", "w");

    if (!arquivo_entrada)
    {
        puts("Problemas na abertura do arquivo de entrada");
        return -1;
    }

    if (!arquivo_saida)
    {
        puts("Problemas na abertura do arquivo de saida");
        return -1;
    }

    int linha = 1, coluna = 0;

    /*
        O código a seguir apenas fará a concatenação da string "input" com a variável "caractere" 
        até o momento que a variável "caractere" representar um caractere delimitador (espaço em branco ou \n).
        Quando isso acontecer, o analisador léxico deverá analisar a string "input" e registrar os tokens de acordo
        com a análise feita. 
    */

    while ((character = fgetc(arquivo_entrada)) != EOF)
    {

        if (character == '\n')
        {
            input[index] = '\0'; // isso representa o final da string, ou seja, a concatenação acabou.
            analisador_lexicografico(input, arquivo_saida, linha, coluna);
            apagar_string(input, &index);
            linha++;
            coluna = 0;
        }
        else
        {
            if (character == ' ')
            {
                input[index] = '\0';
                analisador_lexicografico(input, arquivo_saida, linha, coluna);
                coluna = coluna + strlen(input) + 1; 
                apagar_string(input, &index);
            }
            else 
            {
                if (strlen(input) == 45)
                {
                    puts("ERRO LÉXICO: QUANTIDADE DE CARACTERES MÁXIMA EXCEDIDO POR IDENTIFICADOR");
                    printf("Linha: %i - coluna: %i\n", linha, coluna + 1);
                    return -1;
                }

                input[index++] = character;
            }
        }
    }


    // Essa parte do código analisa lexicamente o último caractere inserido no input.

    if (character != '\0' && character != ' ')
    {
        input[index] = '\0';
        analisador_lexicografico(input, arquivo_saida, linha, coluna);
    }
    

    fclose(arquivo_entrada);
    fclose(arquivo_saida);

    /*
        Essa parte do código utiliza a função que será usada no analisador sintático
    */

    char str[45];
    obterToken(&tabela, str);
    printf("%s\n", str);

    obterToken(&tabela, str);
    printf("%s", str);

    deletarTabela(&tabela);

    return 0;
}