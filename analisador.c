#include "analisador.h"

void apagar_string(char * input, int * index)
{
    strcpy(input, "");
    *index = 0;
}

int main()
{
    FILE * arquivo_entrada, * arquivo_saida;
    char input[45] = "", character;
    int index = 0;
    arquivo_entrada = fopen("input.txt", "rt");
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

    char verificador_linhas;
    int linha = 1, coluna = 1;
    while ((character = fgetc(arquivo_entrada)) != EOF)
    {

        if (character == '\n')
        {
            input[index] = '\0';
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

    fclose(arquivo_entrada);
    fclose(arquivo_saida);
    return 0;
}
