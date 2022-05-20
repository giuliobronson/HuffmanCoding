#include <stdio.h>
#include <stdlib.h>
#define MAX_CHAR 128

// Struct que representa um nó da árvore de Huffman
typedef struct node_tree { 
  char ch;
  int freq;
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
 * @param v array com a nova codificação de cada caractere
 * @param bin binário com a nova codificação
 */
void huffman_encoding(tree *root, int *v, int bin) { 
  if(root) { 
    if(root->ch) v[(int)(root->ch)] = bin;
    huffman_encoding(root->l, v, bin << 1);
    huffman_encoding(root->r, v, bin << 1 | 1);
  }
}

// Contador de bits de um número inteiro
int count_bits(int n) {
  int count = 0;
  do {
    count++;
    n >>= 1;
  } while(n);
  return count;
}

/**
 * Função que associa a cada caractere de um arquivo a nova codificação 
 * de Huffman
 * 
 * @param f ponteiro para o arquivo original
 * @param fc ponteiro para o arquivo codificado
 * @param cod array com os códigos de cada caractere obtido a partir da ávore de 
 *            Huffman
 */
void compressing(FILE *f, FILE *fc, int *cod) {
  char ch = getc(f); int b = cod[(int)(ch)];
  int len = count_bits(b);
  while(1) {
    ch = getc(f);
    if(feof(f)) break;
    int c = cod[(int)(ch)];
    int n = count_bits(c);
    b = b << n | c; len += n;
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
  int cod[MAX_CHAR];
  huffman_encoding(root, cod, 0); 

  // Compressão do arquivo
  FILE *fc = fopen("text_cmp.txt", "w"); rewind(f);
  compressing(f, fc, cod);

  printf("%ld %ld\n", fsize(f), fsize(fc));

  fclose(f); fclose(fc);

  return 0;
}