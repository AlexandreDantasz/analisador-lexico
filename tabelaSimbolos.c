#include "tabelaSimbolos.h"
#include <stdlib.h>
#include <string.h>

No * montarNo(const char * str)
{
    No * p = (No *) malloc(sizeof(No));

    if (p)
    {
        strcpy(p->simbolo, str);
        p->next = NULL;
    }

    return p;
}

void deletarNo(No * no)
{
    free(no);
}

// A função push a seguir deve retornar 1 caso seja possível alocar dinamicamente um elemento na * tabela de simbolos.
// Caso contrário, deve retornar 0.

int push(TabelaSimbolos * tabela, const char * str)
{
    No * p = montarNo(str);

    if (p)
    {
        if (!tabela->head) {
            tabela->head = p;
            tabela->iterator = tabela->head;
        }
            
        else 
            tabela->tail->next = p;
        
        tabela->tail = p;
        tabela->size++;

        return 1;
    }

    return 0;
}

void pop(TabelaSimbolos * tabela)
{
    if (tabela->head)
    {
        No * temp = tabela->head;
        tabela->head = tabela->head->next;
        deletarNo(temp);
        tabela->size--;
    }
}

int procurarSimboloRec(No * no, const char * str, int * index)
{
    if (no)
    {
        if (!strcmp(str, no->simbolo))
            return 1;
        
        * index += 1;
        return procurarSimboloRec(no->next, str, index);
    }

    return 0;
}

// A função a seguir deve retornar o índice do elemento na * tabela de simbolos.
// Caso ele não seja econtrado, deve retornar -1

int procurarSimbolo(TabelaSimbolos * tabela, const char * str)
{
    int index = 0;

    if (procurarSimboloRec(tabela->head, str, &index))
        return index;

    return -1;
}

void deletarTabela(TabelaSimbolos * tabela)
{
    while (tabela->head)
    {
        No * p = tabela->head->next;
        deletarNo(tabela->head);
        tabela->head = p;
    }
}

int obterToken(TabelaSimbolos * tabela, char * str) {

    if (tabela->iterator) {
        strcpy(str, tabela->iterator->simbolo);
        tabela->iterator = tabela->iterator->next;
        return 1;
    }

    return 0;

}

int proximo(TabelaSimbolos * tabela)
{
    if (tabela->iterator)
    {
        tabela->iterator = tabela->iterator->next;
        return 1;
    }

    return 0;
}

char * atual(TabelaSimbolos * tabelaSimbolos)
{
    if (tabelaSimbolos->iterator)
    {
        return tabelaSimbolos->iterator->simbolo;
    }
    return NULL;
}