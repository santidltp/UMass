/********************************************************************
 
 The decoder module is the main API for the huffman decoding
 functionality.

*******************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "tree.h"
#include "bits-io.h"
#include "decoder.h"

/**
 * The Decoder structure is used to maintain all the information
 * required to decode an input file using the huffman coding
 * algorithm.
 */
struct Decoder {
  FILE       *outfp;
  BitsIOFile *bfile;
  TreeNode   *tree;
};

/**
 * Returns a pointer to an Decoder object or NULL if there is an
 * error.
 */
Decoder *decoder_new(char *infile, char *outfile) {
  assert (infile != NULL && outfile != NULL);

  // Open the file with bits-io library:
  BitsIOFile *bfile = bits_io_open(infile, "r");
  if (bfile == NULL)
    return NULL;

  // Open the output file:
  FILE *outfp = fopen(outfile, "w");
  if (outfp == NULL)
    return NULL;

  // Create the Decoder object:
  Decoder *decoder = (Decoder *) malloc(sizeof(Decoder));
  decoder->bfile   = bfile;
  decoder->outfp = outfp;

  // Read the tree:
  TreeNode *tree = bits_io_read_tree(bfile);
  if (tree == NULL) {
    bits_io_close(decoder->bfile);
    return NULL;
  }
  decoder->tree = tree;

  return decoder;
}

/**
 * Deallocates an Decoder object. Returns -1 if there is an error.
 */
int decoder_free(Decoder *decoder) {
  assert(decoder != NULL);
  int status = 0;
  status = bits_io_close(decoder->bfile);
  tree_free(decoder->tree);
  fclose(decoder->outfp);
  free(decoder);
  return status;
}

/**
 * A utility function to decode a single character.
 */
static int decode_one(Decoder *decoder) {
  assert(decoder != NULL);

  BitsIOFile *bfile = decoder->bfile;
  TreeNode   *tree  = decoder->tree;

  TreeNode *p = tree;
  while (p->type != LEAF) {
    int bit = bits_io_read_bit(bfile);
    if (bit == -1)
      return -1;
    else if (bit == 1)
      p = p->left;
    else
      p = p->right;

    if (p == NULL)
      return -1;
  }
  return p->freq.c;  
}

/**
 * Decodes the input file to the output file.
 *
 * Returns the number of bytes decoded.
 */
int decoder_decode(Decoder *decoder) {
  assert(decoder != NULL);

  // Read characters:
  int ch;
  while ((ch = decode_one(decoder)) != -1) {
    fprintf(decoder->outfp, "%c", ch);
  }

  // Return the number of bytes written:
  return bits_io_num_bytes(decoder->bfile);
}
