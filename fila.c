#include "fila.h"

#include <stdlib.h>
#include <string.h>

typedef struct No No;
struct No {
    No *ant;
    No *prox;
    char dado[];
};

struct fila {
    size_t tam_dado;
    int n;
    No *ini;
    No *fim;
    No *atual;
    bool avancando;  
};

// cria uma fila vazia que suporta dados do tamanho fornecido (em bytes)
Fila f_cria(int tam_do_dado)
{
    Fila self = malloc(sizeof(struct fila));
    if (self == NULL) return NULL;
    self->n = 0;
    self->ini = NULL;
    self->fim = NULL;
    self->tam_dado = tam_do_dado;
    self->atual = NULL;
    self->avancando = true;
    return self;
}

// libera a memória ocupada pela fila
void f_destrói(Fila self)
{
    No *p = self->ini;

    while (p!= NULL) {
        No *prox = p->prox;
        free(p);
        p = prox;
    }
    free(self);
}

// diz se a fila está vazia
bool f_tá_vazia(Fila self)
{
    return self->ini == NULL;
}

// remove o dado no início da fila e, se pdado não for NULL, copia o dado
//   removido para *pdado
void f_remove(Fila self, void *pdado)
{
    if (self->ini == NULL) {
      return;
    }
    No *p = self->ini;
    if (pdado != NULL) {
        memcpy (pdado, p->dado, self->tam_dado);
    }
    self->ini = p->prox;
    if (self->ini == NULL) {
        self->fim = NULL;
    } else {
        self->ini->ant = NULL;
    }
    self->n--;
    free(p);
}

// insere o dado apontado por pdado no final da fila
void f_insere(Fila self, void *pdado)
{
    No *novo = malloc(sizeof(No) + self->tam_dado);
    if (novo == NULL) {
      return;
    }
    memcpy (novo->dado, pdado, self->tam_dado);

    novo->prox = NULL;
    novo ->ant = self->fim;

    if (self->fim == NULL) {
        self->ini = novo;
    } else {
        self->fim->prox = novo;
    }
    self->fim = novo;
    self->n++;
}

void f_inicia_percurso(Fila self, int pos_inicial)
{
    int pos = pos_inicial;

    self->avancando = (pos_inicial >= 0);

    if (pos < 0) {
        pos = pos + self->n;
    }

    if (pos < 0 || pos >= self->n) {
        self->atual = NULL;
        return;
    }
    No *p = self->ini;
    for (int i = 0; i < pos; i++) {
        p = p->prox;
    }
    self->atual = p;
}

bool f_próximo(Fila self, void *pdado)
{
    No *p = self->atual;

    if (p == NULL) {
        return false;
    }
    if (pdado != NULL) {
        memcpy (pdado, p->dado, self->tam_dado);
    }
    if (self->avancando) {
        self->atual =p->prox;
    } else  {
        self->atual = p->ant;
    }
    return true;
}
