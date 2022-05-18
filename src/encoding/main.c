#include <stdio.h>
#include <stdlib.h>
#define MAX_CHAR 257

typedef struct node_tree { // Struct que representa um nó da árvore de Huffman
  char ch;
  int freq;
  struct node_tree *l;
  struct node_tree *r;
} tree;

typedef struct node_queue { // Struct que representa um nó da priority_queue
  struct node_tree node;
  struct node_queue *nxt;
} priority_queue;

priority_queue *init = NULL;
tree *root = NULL;
tree v[MAX_CHAR]; 

unsigned long fsize(char *file) { // Retorna o valor em bytes do tamanho do arquivo
  FILE *f = fopen(file, "r");
  if(fseek(f, 0, SEEK_END)) 
    return -1;
  unsigned long sz = ftell(f);
  fclose(f);
  return sz;
}

int push(priority_queue **pq, tree *node) { // Função que insere nó na priority_queue
  priority_queue *aux = *pq, *prv = NULL;
  while(aux) {
    if(node->ch == aux->node.ch) return 0; // Chave já inserida
    aux = aux->nxt;
  }
  aux = *pq;
  while(aux) {
    if(node->freq < aux->node.freq) break;
    prv = aux;
    aux = aux->nxt;
  }
  priority_queue *new = (priority_queue*)malloc(sizeof(priority_queue));
  new->node.ch = node->ch;
  new->node.freq = node->freq;
  new->nxt = aux;
  if(prv) prv->nxt = new; // Verifica se o elemento é inserido no início da fila
  else *pq = new; // Altera o início da fila 
  return 1;
}

priority_queue *pop(priority_queue **pq) { // Função que retira o primeiro o elemento do topo da fila
  priority_queue *aux = *pq;
  if(!aux) return NULL; // Fila vazia
  *pq = (*pq)->nxt;
  return aux;
}

tree *join(tree *t1, tree *t2) { // Função que constroe novo nó na árvore de Huffman
  tree *root = (tree*)malloc(sizeof(tree));
  root->freq = t1->freq + t2->freq;
  root->l = t1;
  root->r = t2;
  return root;
}

void compressing() { // Aplica o algoritimo de Huffman para comprimir o arquivo de texto
  
}

int main() {
  // Calcula as frequências de cada símbolo
  FILE *f = fopen("text.txt", "r");
  while(1) {
    char ch = getc(f);
    if(feof(f)) break;
    int i = (int)(ch - '0');
    if(i == 1) v[i].ch = ch;
    v[i].freq++;
  }
  unsigned long sz = fsize(f);
  fclose(f);

  // Inicialização da priority_queue
  for(int i = 0; i < MAX_CHAR; i++) {
    if(v[i].ch) push(&init, &v[i]);
  }

  // Construção da árvore de Huffman
  while(init) {
    priority_queue *pq1 = pop(&init); 
    priority_queue *pq2 = pop(&init); 
    if(pq1 && pq2) {
      root = join(&pq1->node, &pq2->node);
      push(&init, root);
    }
  }

  // Codificação de cada char a partir da árvore de Huffman

  // Compressão do arquivo

  return 0;
}