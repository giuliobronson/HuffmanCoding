#include <stdio.h>
#include <stdlib.h>
#define MAX_CHAR 128

// Struct que representa um nó da árvore de Huffman
typedef struct node_tree { 
  int freq;
  char ch;
  struct node_tree *l;
  struct node_tree *r;
} tree;

// Struct que representa um nó da fila de prioridade
typedef struct node_queue { 
  struct node_tree node;
  struct node_queue *nxt;
} priority_queue;

priority_queue *init = NULL;
tree *root = NULL;
tree v[MAX_CHAR]; 

/**
 * Função que insere nó na fila de prioridade
 * 
 * @param pq ponteiro para o início da fila de prioridade 
 * @param node ponteiro para o nó adicionado
 * @returns booleano indicando o sucesso da inserção 
 */
int push(priority_queue **pq, tree *node) {
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

/**
 * Função que retira o primeiro o elemento do topo da fila de prioridade
 * 
 * @param pq ponteiro para o início da fila de prioridade
 * @returns nó que foi retirado 
 */
priority_queue *pop(priority_queue **pq) { 
  priority_queue *aux = *pq;
  if(!aux) return NULL; // Fila vazia
  *pq = (*pq)->nxt;
  return aux;
}

/**
 * Função que constroe novo nó na árvore de Huffman
 * 
 * @param t1 primeiro nó
 * @param t2 segundo nó
 * @returns nó pai dos dois nós passados como parâmetro
 */
tree *join(tree *t1, tree *t2) {
  tree *root = (tree*)malloc(sizeof(tree));
  root->freq = t1->freq + t2->freq;
  root->l = t1;
  root->r = t2;
  return root;
}

/**
 * Codificação a partir da árvore de Huffman
 * 
 * @param root ponteiro para a raiz da árvora de Huffman
 * @param v matriz com a nova codificação de cada caractere e número de bits
 * @param bin binário com a nova codificação
 */
void huffman_encoding(tree *root, int (*v)[2], int bin) { 
  if(root) { 
    static int n = 0;
    n++;
    if(root->ch) {
      v[(int)(root->ch)][0] = bin;
      v[(int)(root->ch)][1] = n - 1;
    }
    huffman_encoding(root->l, v, bin << 1);
    huffman_encoding(root->r, v, bin << 1 | 1);
    n--;
  }
}

/**
 * Função que associa a cada caractere de um arquivo a nova codificação 
 * de Huffman
 * 
 * @param f ponteiro para o arquivo original
 * @param fc ponteiro para o arquivo codificado
 * @param cod matriz com os códigos de cada caractere obtido a partir da ávore de 
 *            Huffman e o número de bits
 */
void compressing(FILE *f, FILE *fc, int (*cod)[2]) {
  int b = 0; int len = 0;
  while(1) {
    char ch = getc(f);
    if(feof(f)) break;
    int c = cod[(int)(ch)][0];
    int n = cod[(int)(ch)][1];
    b = b << n | c; len += n;
    b &= 0xffff; // Evita o overflow
    if(len >= 7) {
      len -= 7;
      putc((b >> len) & 0x7f, fc);
    }
  }
  len = 7 - len;
  putc((b << len) & 0x7f, fc);
}

/**
 * Função que calcula o número de bytes do arquivo 
 * 
 * @param f ponteiro para o arquivo
 * @returns tamanho do arquivo 
 */
unsigned long fsize(FILE *f) {
  fseek(f, 0L, SEEK_END);
  unsigned long sz = ftell(f);
  return sz;
}

int main() {
  // Calcula as frequências de cada símbolo
  FILE *f = fopen("text.txt", "r");
  while(1) {
    char ch = getc(f);
    if(feof(f)) break;
    int c = (int)(ch);
    if(!v[c].freq) v[c].ch = ch;
    v[c].freq++;
  }

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
  int cod[MAX_CHAR][2];
  huffman_encoding(root, cod, 0); 

  // Compressão do arquivo
  FILE *fc = fopen("text_cmp.txt", "w"); rewind(f);
  compressing(f, fc, cod);

  printf("%ld %ld\n", fsize(f), fsize(fc));

  return 0;
}