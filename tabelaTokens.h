#ifndef TABELATOKENS_H
#define TABELASTOKENS_H
#include "tabelaSimbolos.h" // Para struct Token

typedef struct NoToken
{
    Token token;
    struct NoToken * next;
} NoToken;

typedef struct tabelaTokens
{
    int size;
    NoToken * head, * tail;
    NoToken * iterator;

} TabelaTokens;

NoToken * montarNoToken(Token newToken);

void deletarNoToken(NoToken * no);

int pushToken(TabelaTokens * tabela, Token token);

void popToken(TabelaTokens * tabela);

void deletarTabelaToken(TabelaTokens * tabela);

int proximoToken(TabelaTokens * tabela);

Token atualToken(TabelaTokens * tabela);

#endif