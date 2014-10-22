/********************************************************************

 The table module manages the mapping from characters to their
 huffman encoding. It requires a properly constructed huffman
 tree to be built before it is used. The primary functions that
 are exported by this module include:

 EncodeTable* table_build(TreeNode* root);
   - Returns an encoding table from a properly constructed huffman
     tree.

 void table_free(EncodeTable* etab);
   - Frees an encoding table.

 char* table_bit_encode(EncodeTable* etab, unsigned char c);
   - Returns a bit encoding of the given character `c` with the
     provided encoding table. The format of the array of characters
     that are returned is a sequence of 1's and 0's terminated by
     -1. Each 1 and 0 in the encoding string represents a path to
     a character in the tree. We use this encoding to write bits
     to the compressed file.

 *******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "table.h"

#define CHAR_START      0
#define CHAR_END        127
#define NUMBER_OF_CHARS (CHAR_END - CHAR_START)

/**
 * A BitArray represents the encoding of a particular character.
 * It is used by this module to map a character `c` to an array
 * of characters which will be either a 1 or a 0. The `bits`
 * string is terminated by a -1. The length `len` is the length
 * of the bit array (excluding the -1) and the max is the maximum
 * length of the bit array.
 */
typedef struct BitArray BitArray;
struct BitArray {
  char bits[NUMBER_OF_CHARS];
  int  len;
  int  max;
};

/**
 * The `bit_array_new` function returns a new bit array.
 */
static BitArray* bit_array_new() {
  BitArray *b = (BitArray*) malloc(sizeof(BitArray));
  b->max  = NUMBER_OF_CHARS;
  b->len  = 0;
  return b;
}

/**
 * The `bit_array_new_from` function returns a new bit
 * array from the given bit array. In other words, it
 * creates a copy of the given bit array. It is the
 * responsibility of the caller to manage the deallocation.
 */
static BitArray* bit_array_new_from(BitArray* b) {
  BitArray *nb = (BitArray*) malloc(sizeof(BitArray));
  nb->len  = b->len;
  nb->max  = b->max;
  memcpy(nb->bits, b->bits, b->len);
  return nb;
}

/**
 * The `bit_array_free` function deallocates the
 * bit array object.
 */
static void bit_array_free(BitArray* b) {
  free(b);
}

/**
 * Appends a bit to the provided bit array object.
 */
static void bit_array_append_bit(BitArray* b, char bit) {
  assert (b->len < b->max);
  b->bits[b->len] = bit;
  b->len++;
}

/**
 * Returns 1 if the bit array is full; 0 otherwise.
 */
static int bit_array_full(BitArray* b) {
  return b->len >= b->max;
}

/**
 * An EncodeTable holds a mapping from characters to bit arrays.
 * It is used during the encoding process to retrieve the bit
 * representation of a character for the huffman coding.
 */
struct EncodeTable {
  BitArray* table[NUMBER_OF_CHARS+1];
};

/**
 * This function recursively constructs the encoding table. It visits
 * each node in the tree generating the encoding. If the tree node is
 * an INTERNAL node we assign a 1 bit to the left child of the tree and
 * a 0 bit to the right child of a tree. If the tree is a LEAF node we
 * add the mapping to the encode table. We create new bit arrays as we
 * descend into the tree.
 */
static void rec_gen_table(EncodeTable* etab, TreeNode* node, BitArray* b) {
  // If the bit arrray is full there is something wrong so we fail.
  if (bit_array_full(b)) {
    fprintf(stderr, "Exceeded the size of the bitarray");
    exit(1);
  }

  // If we have an internal node we recursively descend into the tree.
  if (node->type == INTERNAL) {
    TreeNode* left  = node->left;
    TreeNode* right = node->right;

    // If the left child is not NULL we create a new bit array from the
    // old one, append a 1 bit, and recursively follow the left branch.
    if (left != NULL) {
      BitArray *lb = bit_array_new_from(b);
      bit_array_append_bit(lb, 1);
      rec_gen_table(etab, left, lb);
    }

    // If the right child is not NULL we create a new bit array from the
    // old one, append a 0 bit, and recursively follow the right branch.
    if (right != NULL) {
      BitArray *rb = bit_array_new_from(b);
      bit_array_append_bit(rb, 0);
      rec_gen_table(etab, right, rb);
    }

    // We need to make sure to deallocate the intermediate bit array.
    bit_array_free(b);
  }
  // Leaf Node Case:
  else {
    // We finally reached a leaf node so we add its mapping into the
    // encode table.
    char ch = node->freq.c;
    etab->table[ch%NUMBER_OF_CHARS] = b;
  }
}

/**
 * This function returns the huffman encoding for a given character
 * `c` in the provided encoding table. The format of the returned
 * string is a sequence of 1's and 0's representing the encoding.
 * The string is terminated by -1.
 *
 * It is the responsibility of the caller to deallocate the array
 * of characters that are returned.
 */
char* table_bit_encode(EncodeTable* etab, unsigned char c) {
  // If we receive a character that is out of range we skip it.
  if (c < CHAR_START || c > CHAR_END) {
    printf("Invalid ASCII character: '%c'. Skipping...\n", c);
    return NULL;
  }

  // Otherwise, we retrieve the encoding for the character:
  BitArray *b = etab->table[c%NUMBER_OF_CHARS];

  // Make a copy of the encoding:
  char     *e = (char *) malloc(b->len+1);
  memcpy(e, b->bits, b->len);

  // Terminate with a -1:
  *(e + b->len) = -1;

  // Note that it is the responsibility of the caller to deallocate
  // the array of characters that we return.
  return e;
}

/**
 * This function is useful for debugging. It prints out the mapping
 * from characters to their huffman encoding.
 */
void table_print(EncodeTable* etab) {
  for (int i = CHAR_START; i < CHAR_END; i++) {
    unsigned char      c = (unsigned char) i;
    BitArray *b = etab->table[c%NUMBER_OF_CHARS];
    if (b != NULL) {
      printf("%d '%c' = ", c, c);
      for (int i = 0; i < b->len; i++) {
	       printf("%d", b->bits[i]);
      }
      printf("\n");
    }
  }
}

/**
 * Returns an encoding table given the huffman tree.
 */
EncodeTable* table_build(TreeNode* root) {
  // Allocate a new encoding table:
  EncodeTable* etab = (EncodeTable*) malloc(sizeof(EncodeTable));

  // Initialize each entry to NULL:
  for (int i = 0; i < NUMBER_OF_CHARS; i++) {
    etab->table[i] = NULL;
  }

  // Recursively construct the encoding table:
  rec_gen_table(etab, root, bit_array_new());

  // Return the constructed table:
  return etab;
}

/**
 * Frees the encoding table.
 */
void table_free(EncodeTable* etab) {
  // First, free each of the bit arrays in the table.
  for (int i = 0; i < NUMBER_OF_CHARS; i++) {
    bit_array_free(etab->table[i]);
  }
  free(etab);
}
