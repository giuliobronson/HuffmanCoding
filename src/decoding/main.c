#include <stdio.h>

typedef struct node_tree { 
  char ch;
  int freq;
  struct node_tree *l;
  struct node_tree *r;
} tree;

tree *orgn = NULL;

/**
 * Recupera o texto original codificado por Huffman
 * 
 * @param root ponteiro para o início da árvore de Huffman
 * @param f ponteiro para arquivo codificado
 * @param fd ponteiro para arquivo decodificado
 */
void decoding(tree *root, FILE *f, FILE *fd) { 
  static int pos = 0;
  static char ch;
  if(!root) return;
  if(!root->l && !root->l) {
    putc(root->ch, fd);
    root = orgn;
  }
  if(!pos) ch = getc(f);
  if(feof(f)) return;
  int b = (int)(ch) >> (6 - pos++) & 0x1; pos = pos % 7; 
  if(!b) decoding(root->l, f, fd);
  else decoding(root->r, f, fd);
}

int main() {
  // Recuperação da árvore
  
  // Decodificação
  FILE *f = fopen("text_cmp.txt", "r");
  FILE *fd = fopen("text_dec.txt", "w");
  decoding(orgn, f, fd);

  fclose(f); fclose(fd);

  return 0;
}