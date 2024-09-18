#include "analisador.h"

int main()
{
    FILE * arquivo_entrada, * arquivo_saida;
    char input[45];
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
    while (!feof(arquivo_entrada))
    {
        if (fscanf(arquivo_entrada, "%s", input) != -1)
        {
            printf("%s\n", input);
            analisador_lexicografico(input, arquivo_saida, linha, coluna++);
        }

    }

    fclose(arquivo_entrada);
    fclose(arquivo_saida);
    return 0;
}
