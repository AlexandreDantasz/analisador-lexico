#ifndef TABELASIMBOLOS_H
#define TABELASIMBOLOS_H

typedef struct No 
{
    char simbolo[45];
    struct No * next;
} No;

typedef struct tabelaSimbolos
{
    int size;
    No * head, * tail;

} TabelaSimbolos;

No * montarNo(const char * str);
void deletarNo(No * no);

int push(TabelaSimbolos * tabela, const char * str);
void pop(TabelaSimbolos * tabela);
int procurarSimboloRec(No * no, const char * str, int * index);
int procurarSimbolo(TabelaSimbolos * tabela, const char * str);

void deletarTabela(TabelaSimbolos * tabela);

#endif