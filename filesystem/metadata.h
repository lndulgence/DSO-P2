
/*
 *
 * Operating System Design / Diseño de Sistemas Operativos
 * (c) ARCOS.INF.UC3M.ES
 *
 * @file 	metadata.h
 * @brief 	Definition of the structures and data types of the file system.
 * @date	Last revision 01/04/2020
 *
 */
#include <stdint.h>
#include <stdbool.h> 
#include <string.h>
#define MAX_FILES 48 // NF1
#define MAX_NAME_LENGHT 32  // NF2
#define BLOCK_SIZE 2048 // NF4
#define MAX_FILE_SIZE 10240  // NF7
#define TOTAL_BLOCKS MAX_FILES*(MAX_FILE_SIZE/BLOCK_SIZE)
#define bitmap_getbit(bitmap_, i_) (bitmap_[i_ >> 3] & (1 << (i_ & 0x07)))

static inline void bitmap_setbit(char *bitmap_, int i_, int val_) {
  if (val_)
    bitmap_[(i_ >> 3)] |= (1 << (i_ & 0x07));
  else
    bitmap_[(i_ >> 3)] &= ~(1 << (i_ & 0x07));
}

typedef struct inode {
  char *name; 
  uint8_t block_numbers[5]; //blocks the file spans
  uint16_t size; // tamaño del fichero en bytes
  int pointer; // puntero del fichero
} inode;

typedef struct SuperBlock {
  uint16_t magic_number; //número que permite al sistema de montado ver que se trata del superbloque de este sistema de archivos
  long disk_size; // el tamaño del disco
  bool block_allocation_map [TOTAL_BLOCKS]; //mapa que muestra qué bloques hay disponibles.
  struct inode inodes[MAX_FILES]; // array de inodos
} Superblock;

