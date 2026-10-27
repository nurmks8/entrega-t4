#include "grafo.h"
#include <stdlib.h>
#include <string.h>

struct aresta {
  int origem;
  int destino;
  void *dado;
};

struct grafo {
  int tam_no;
  int tam_aresta;
  int tipo_consulta;
  int no_consulta;
  int proxima_aresta;

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
    self->tipo_consulta = 0;
    self->no_consulta = -1;
    self->proxima_aresta = 0;
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
    for (j = i + 1; j < self->n_arestas; j++) {
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
    self->arestas,
    (self->n_arestas + 1) * sizeof(struct aresta)
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
{
  self->tipo_consulta = 1;
  self->no_consulta = nó_origem;
  self->proxima_aresta = 0; 
}
void g_arestas_que_chegam(Grafo self, int nó_destino)
{
  self->tipo_consulta = 2;
  self->no_consulta = nó_destino;
  self->proxima_aresta = 0;
}
void *g_próxima_aresta(Grafo self, int *p_nó_vizinho)
{
  if (self == NULL || p_nó_vizinho == NULL) {
    return NULL;
  }
  int i;
  for (i = self->proxima_aresta; i < self->n_arestas; i++) {
    if (self->tipo_consulta == 1 &&
        self->arestas[i].origem == self->no_consulta) {
      *p_nó_vizinho = self->arestas[i].destino;
      self->proxima_aresta = i + 1;
      return self->arestas[i].dado;
    }
    if (self->tipo_consulta == 2 &&
        self->arestas[i].destino == self->no_consulta) {
      *p_nó_vizinho = self->arestas[i].origem;
      self->proxima_aresta = i + 1;
      return self->arestas[i].dado;
    }
  }
  return NULL;
}
bool g_tem_ciclo_recursivo(Grafo self, int atual, bool *visitado, bool *em_caminho);
bool g_tem_ciclo(Grafo self)
{
  if (self == NULL) {
    return false;
  }

  if (self->n_nos == 0) {
    return false;
  }
  bool visitado[self->n_nos];
  bool em_caminho[self->n_nos];

  for (int i = 0; i < self->n_nos; i++) {
    visitado[i] = false;
    em_caminho[i] = false;
  }
  for (int i = 0; i < self->n_nos; i++) {
    if (!visitado[i]) {
      if (g_tem_ciclo_recursivo(self, i, visitado, em_caminho)) {
        return true;
      }
    }
  }
  return false;
}
bool g_tem_ciclo_recursivo(Grafo self, int atual, bool *visitado, bool *em_caminho)
{
  visitado[atual] = true;
  em_caminho[atual] = true;
  
  int vizinho;
  void *dado_aresta;

  g_arestas_que_partem(self, atual);

  while ((dado_aresta = g_próxima_aresta(self, &vizinho)) != NULL) {
    if (!visitado[vizinho]) {
      if (g_tem_ciclo_recursivo(self, vizinho, visitado, em_caminho)) {
        return true;
      }
    } else if (em_caminho[vizinho]) {
      return true;
    }
  }
  em_caminho[atual] = false;
  return false;
}

Fila g_ordem_topológica(Grafo self)
{
  if (self == NULL) {
    return NULL;
  }
  int n = g_num_nós(self);

  Fila ordem = f_cria(sizeof(int));
  Fila disponiveis = f_cria(sizeof(int));

  if (ordem == NULL || disponiveis == NULL) {
    f_destrói(ordem);
    f_destrói(disponiveis);
    return NULL;
  }

  if (n == 0) {
    f_destrói(disponiveis);
    return ordem;
  }

  int grau_entrada[n];
  for (int i = 0; i < n; i++) {
    grau_entrada[i] = 0;
  
  int vizinho;
  void *dado_aresta;

  g_arestas_que_chegam(self, i);
 
   while ((dado_aresta = g_próxima_aresta(self, &vizinho)) != NULL) {
    grau_entrada[i]++;
   }
  }
  for (int i = 0; i < n; i++) {
    if (grau_entrada[i] == 0) {
      f_insere(disponiveis, &i);
    }
  }
  int processados = 0;
  int atual;
  int vizinho;
  void *dado_aresta;

  while (!f_tá_vazia(disponiveis)) {
    f_remove(disponiveis, &atual);
    f_insere(ordem, &atual);
    processados++;

    g_arestas_que_partem(self, atual);
    while ((dado_aresta = g_próxima_aresta(self, &vizinho)) != NULL) {
      grau_entrada[vizinho]--;
      if (grau_entrada[vizinho] == 0) {
        f_insere(disponiveis, &vizinho);
      }
    }
  }
  f_destrói(disponiveis);

  if (processados != n) {
    f_destrói(ordem);
    return NULL;
  }
  return ordem;
}