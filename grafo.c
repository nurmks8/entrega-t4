#include "grafo.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    int origem;
    int destino;
    void *dado;
};

struct aresta {
  int origem;
  int destino;
  void *dado;
};

struct grafo {
  int tam_no;
  int tam_aresta;

  void **nos;
  int n_nos;

  struct aresta *arestas;
  int n_arestas;
};

// cria um grafo vazio que suporta dados do tamanho fornecido (em bytes)
//   nos nós e nas arestas
Grafo g_cria(int tam_nó, int tam_aresta)
{
    if (tam_nó <= 0 || tam_aresta <= 0) {
        return NULL;
    }
    Grafo self = malloc(sizeof(struct grafo));
    
    if (self == NULL) {
        return  NULL;
    }
    self->tam_no = tam_nó;
    self->tam_aresta = tam_aresta;
    self->n_nos = 0;
    self->nos = NULL;
    self->n_arestas = 0;
    self->arestas = NULL;

    return self;
}

// libera a memória ocupada por um grafo
void g_destrói(Grafo self)
{
  if (self == NULL) {
        return;
  }
  int i;
  for (i = 0; i < self->n_nos; i++) {
    free(self->nos[i]);
  }
  free(self->nos);
  
  for (i = 0; i < self->n_arestas; i++) {
    free(self->arestas[i].dado);
  }
  free(self->arestas);
  free(self);
}

int g_insere_nó(Grafo self, void *p_dado)
{
  if (self == NULL || p_dado == NULL) {
    return -1;
  }
  void **novo;
  int id = self->n_nos;

  novo = realloc(self->nos, (self->n_nos + 1) * sizeof(void *));
  if ( novo == NULL) {
    return -1;
  }
  self->nos = novo;
  self->nos[id] = malloc(self->tam_no);
  if (self->nos[id] == NULL) {
    return -1;
  }
  memcpy(self->nos[id], p_dado, self->tam_no);
  self->n_nos++;
  return id;
}

void g_remove_nó(Grafo self, int nó)
{
  if (self == NULL || nó < 0 || nó >= self->n_nos) {
    return;
  }

  int i, j;
  for (i = 0; i < self->n_arestas; i++) {
     if (self->arestas [i].origem == nó ||
        self->arestas[i].destino == nó) {
        free(self->arestas[i].dado);

      for (j = i + 1; j < self->n_arestas; j++) {
        self->arestas[j - 1] = self->arestas[j];
      }
    self->n_arestas--;
    i--;
     }
   }
   for (i = 0; i < self->n_arestas; i++) {
      if (self->arestas [i].origem > nó) {
        self->arestas[i].origem--;
      }
      if (self->arestas[i].destino > nó) {
        self->arestas[i].destino--;
      }
   }
   free(self->nos[nó]);
   for (j = nó + 1; j < self->n_nos; j++) {
     self->nos[j - 1] = self->nos[j];
   }
    self->n_nos--;
}

void g_altera_valor_do_nó(Grafo self, int nó, void *p_dado)
{
  if (self == NULL || nó < 0 || nó >= self->n_nos || p_dado == NULL) {
    return;
  }
  memcpy(self->nos[nó], p_dado, self->tam_no);
}

void *g_valor_do_nó(Grafo self, int nó)
{
  if (self == NULL || nó < 0 || nó >= self->n_nos) {
    return NULL;
  }
    return self->nos[nó];
  
}


int g_num_nós(Grafo self) 
{
  if (self == NULL) {
    return -1;
  }
  return self->n_nos;
}

void g_altera_valor_da_aresta(Grafo self, int nó_origem, int nó_destino,
                              void *p_dado)
{
  if (self == NULL || nó_origem < 0 || nó_origem >= self->n_nos ||
      nó_destino < 0 || nó_destino >= self->n_nos) {
    return;
  }
  int i, j;
   for (i = 0; i < self->n_arestas; i++) {
     if (self->arestas[i].origem == nó_origem &&
        self->arestas[i].destino == nó_destino) {
        if (p_dado != NULL) {
            memcpy(self->arestas[i].dado, p_dado, self->tam_aresta);
            return;
        }
    free(self->arestas[i].dado);
    for (int j = i + 1; j < self->n_arestas; j++) {
        self->arestas[j - 1] = self->arestas[j];
    }
    self->n_arestas--;
    return;
    }
   }
   if (p_dado == NULL) {
    return;
   }
   void *novo_dado = malloc(self->tam_aresta);
   
   if (novo_dado == NULL) {
     return;
   }
   memcpy(novo_dado, p_dado, self->tam_aresta);

   struct aresta *novas = realloc(
    self->arestas,(self->n_arestas + 1) * sizeof(struct aresta)
   );

    if (novas == NULL) {
      free(novo_dado);
      return;
    }
    self->arestas = novas;
    
    self->arestas[self->n_arestas].origem = nó_origem;
    self->arestas[self->n_arestas].destino = nó_destino;
    self->arestas[self->n_arestas].dado = novo_dado;

    self->n_arestas++;
}

void *g_valor_da_aresta(Grafo self, int nó_origem, int nó_destino)
{
  if (self == NULL || nó_origem < 0 || nó_origem >= self->n_nos ||
      nó_destino < 0 || nó_destino >= self->n_nos) {
    return NULL;
  }
  int i;
  for (i = 0; i < self->n_arestas; i++) {
    if (self->arestas[i].origem == nó_origem &&
        self->arestas[i].destino == nó_destino) {
      return self->arestas[i].dado;
    }
  }
  return NULL;
}

void g_arestas_que_partem(Grafo self, int nó_origem)

void g_arestas_que_chegam(Grafo self, int nó_destino)

void *g_próxima_aresta(Grafo self, int *p_nó_vizinho)

bool g_tem_ciclo(Grafo self)


Fila g_ordem_topológica(Grafo self);