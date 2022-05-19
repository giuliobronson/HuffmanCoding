#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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

int push(priority_queue **pq, tree *node) { // Função que insere nó na priority_queue
  priority_queue *aux = *pq, *prv = NULL;
  aux = *pq;
  while(aux) {
    if(node->freq < aux->node.freq) break;
    prv = aux;
    aux = aux->nxt;
  }
  priority_queue *new = (priority_queue*)malloc(sizeof(priority_queue));
  new->node = *node;
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

void huffman_coding(tree *root, int *v, int bin) { // Codificação a partir da árvore de Huffman
  if(root) { 
    if(root->ch) v[(int)(root->ch)] = bin;
    huffman_coding(root->l, v, bin << 1);
    huffman_coding(root->r, v, bin << 1 | 1);
  }
}

int count_bits(int n) {
  unsigned int count = 0;
  while (n) {
    count++;
    n >>= 1;
  }
  return count;
}

int compressing(FILE *f, int *cod) {
  int bin = 0b0;
  while(1) {
    char ch = getc(f);
    if(feof(f)) break;
    int c = cod[(int)(ch)];
    int n = count_bits(c);
    bin = (bin << n) | c;
  }
  return bin;
}

int main() {
  // Calcula as frequências de cada símbolo
  FILE *f = fopen("text.txt", "r");
  int bin = 0b0;
  while(1) {
    char ch = getc(f);
    if(feof(f)) break;
    int c = (int)(ch);
    bin = bin << 8 | c;
    if(!v[c].freq) v[c].ch = ch;
    v[c].freq++;
  }
  rewind(f);

  // Inicialização da priority_queue
  for(int i = 0; i < MAX_CHAR; i++) {
    if(v[i].freq) push(&init, &v[i]);
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
  int cod[MAX_CHAR];
  huffman_coding(root, cod, 0); 

  // Compressão do arquivo
  FILE *fc = fopen("text_compressed.txt", "w");
  int bin_cmp = compressing(f, cod);

  printf("%d\n", bin);
  printf("%d\n", bin_cmp);
  
  fclose(f);

  return 0;
}