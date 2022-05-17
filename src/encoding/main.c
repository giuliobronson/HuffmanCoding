#include <stdio.h>

unsigned long fsize(char *file) { // Retorna o valor em bytes do tamanho do arquivo
  FILE *f = fopen(file, "r");
  if(fseek(f, 0, SEEK_END)) 
    return -1;
  unsigned long sz = ftell(f);
  fclose(f);
  return sz;
}

void compressing() { // Aplica o algoritimo de Huffman para comprimir o arquivo de texto
  
}

int main() {
  char *file = "text.txt";
  printf("%ld\n", fsize(file));
  return 0;
}