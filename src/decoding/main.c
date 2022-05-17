#include <stdio.h>

unsigned long fsize(char *file) { // Retorna o valor em bytes do tamanho do arquivo
  FILE *f = fopen(file, "r");
  if(fseek(f, 0, SEEK_END)) 
    return -1;
  unsigned long sz = ftell(f);
  fclose(f);
  return sz;
}

void decoding() { // Recupera o texto original codificado por Huffman

}

int main() {
  return 0;
}