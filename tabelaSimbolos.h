#ifndef TABELASIMBOLOS_H
#define TABELASIMBOLOS_H

// nome: tipo token
// lexema: conteúdo do token

typedef struct Token
{
    char nome[45], lexema[45];
    int linha, coluna;
} Token;

typedef struct No 
{
    char simbolo[45];
    struct No * next;
} No;

typedef struct tabelaSimbolos
{
    int size;
    No * head, * tail;
    No * iterator;

} TabelaSimbolos;

No * montarNo(const char * str);
void deletarNo(No * no);

int push(TabelaSimbolos * tabela, const char * str);
void pop(TabelaSimbolos * tabela);
int procurarSimboloRec(No * no, const char * str, int * index);
int procurarSimbolo(TabelaSimbolos * tabela, const char * str);
int obterToken(TabelaSimbolos * tabela, char * str);
int proximo(TabelaSimbolos * tabela);
char * atual(TabelaSimbolos * tabela);

void deletarTabela(TabelaSimbolos * tabela);

#endif