#include "tabelaTokens.h"
#include <stdlib.h>
#include <string.h>

NoToken * montarNoToken(Token newToken)
{
    NoToken * p = (NoToken *) malloc(sizeof(NoToken));

    if (p)
    {
        p->token = newToken;
        p->next = NULL;
    }

    return p;
}

void deletarNoToken(NoToken * no)
{
    free(no);
}

// A função push a seguir deve retornar 1 caso seja possível alocar dinamicamente um elemento na * tabela de simbolos.
// Caso contrário, deve retornar 0.

int pushToken(TabelaTokens * tabela, Token token)
{
    NoToken * p = montarNoToken(token);

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

void popToken(TabelaTokens * tabela)
{
    if (tabela->head)
    {
        NoToken * temp = tabela->head;
        tabela->head = tabela->head->next;
        deletarNoToken(temp);
        tabela->size--;
    }
}

void deletarTabelaToken(TabelaTokens * tabela)
{
    while (tabela->head)
    {
        NoToken * p = tabela->head->next;
        deletarNoToken(tabela->head);
        tabela->head = p;
    }
}

int proximoToken(TabelaTokens * tabela)
{
    if (tabela->iterator)
    {
        tabela->iterator = tabela->iterator->next;
        return 1;
    }

    return 0;
}

// A função a seguir retorna um token com o lexema definido como -1 
// caso não seja possível retornar algo da tabela

Token atualToken(TabelaTokens * tabela)
{
    Token res;
    strcpy(res.lexema, "-1");
    strcpy(res.nome, "-1");

    if (tabela->iterator)
        res = tabela->iterator->token;

    return res;
}