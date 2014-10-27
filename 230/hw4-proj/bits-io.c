/********************************************************************

 The bits-io module is used to encode bits into an output file. The
 bits-io module buffers output/input bits into an internal byte-sized
 buffer using a special format to know when it is supposed to write or
 read a full byte to the output or input file. Here are the details
 for both writing and reading bits to a file:

 WRITING BITS

   Because we are using the stdio function fputc to write a byte to an
   output file we are required by its interface to deal with bytes and
   not bits when writing. That is, it is not possible to write a
   single bit to an output file in Unix - so, we must buffer the bits
   that we ultimately want to write to the output file into a
   byte. When the byte is full we can then write it to the output
   file.

   To do this correctly, we need to invent a byte format such that we
   know when a byte is "full". When a byte is full we can then write
   it to the output file and start with an empty byte. The byte format
   we chose for this assignment begins with the following format:

     B = 11111110

   This bit representation can easily be specified in C using the
   bitwise complement operator on the value 1: ~1.

   When we write a bit (say 1) to this byte B we write the bit into
   the least significant bit with the result of the bits shifted left
   by 1. This will result in:

     B = 11111101
                ^
                |---------- bit 1 we just "wrote"

   If we were to then write the bit 0 to B it would result in the
   following byte:

     B = 11111010
                ^
                |---------- bit 0 we just "wrote"
  
   Using this format we use the left-most 0 bit as a delimiter between
   bits not written and bits written:

     B = [bits not written]0[bits written]

   In this example, we have the following:

     B = 11111 0 10
          NW   D W

   Where NW are the not written bits, D is the delimiter 0, and W are
   the written bits. We would keep writing bits until the byte B is
   full. A full byte would look like this (where the bits a-g
   represents the bits written):

     B = 0abcdefg

   If the most significant bit is 0 we know the byte is full and we are
   now prepared to write the byte to the output file using fputc. After
   we write the byte to the output file we would then reset the byte to
   its original value ~1.

   There is one special case, however, when we do not have any more
   bits to write and we have not filled our byte buffer. In this case,
   we simply write the remaining byte to the output file. This is not
   significant for writing bytes, however, it is important to remember
   when we read bits back in from a compressed input file that the
   last byte we read in may not be completely full.

 READING BITS

   Because we are using the stdio function fgetc to read bytes from an
   input file we are required by its interface to deal with bytes and
   not bits when reading. In the general case (a byte that was full)
   we read in a byte from the compressed input file with the following
   format:

   B = 0abcdefg

   We know that the most significant bit will be 0 for a full byte. We
   can simply shift the byte left by 1 to remove the most significant
   bit and begin to read meaninful bits after that. So, if we shift
   left by one we get:

   B = abcdefg0

   Now, we can read the first meaningful bit with a shift right by 7
   to get bit 'a' and shift left by 1 to remove the bit we just read
   in our buffer to get:

   B = bcdefg00

   Each successive bit we read would perform the same operations until
   we have read in all 7 bits. We know we have read all the bits when
   we have finally read in bit 7. We use a counter to remember which
   bit we read next.

   The last byte written is a special case when we read it back. We
   know by using the format discussed above that all 1's before the
   delimiter bit 0 represent bits that have not been used
   (written). So, for the last byte we need to shift left all the 1
   bits, followed by shifting left the 0 delimiter. We would then be
   left with the bits that are meaninful. We maintain a counter to
   know how many of the remaining bits are meaninful.

 *******************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "bits-io.h"
#include "tree.h"

/**
 * This structure is used to maintain the writing/reading of a
 * compressed file.
 */
struct BitsIOFile {
  FILE *fp;            // The output/input file
  char *mode;          // The mode "w" for write and "r" for read
  unsigned char byte;  // The byte buffer to hold the bits we are
		       // reading/writing
  int nbits;           // The number of bits written/read so far
  int nbytes;          // The number of bytes written/read so far
};

/**
 * Returns the number of bytes written/read.
 */
int bits_io_num_bytes(BitsIOFile *bfile) {
  return bfile->nbytes;
}

/**
 * Opens a new BitsIOFile. Returns NULL if there is a failure.
 *
 * The `name` is the name of the file.
 * The `mode` is "w" for write and "r" for read.
 */
BitsIOFile* bits_io_open(const char *name, char *mode) {
  // Open the output file:
  FILE *fp = fopen(name, mode);

  if (fp == NULL) {
    return NULL;
  }
  else {
    BitsIOFile *bfile = (BitsIOFile*) malloc(sizeof(BitsIOFile));
    bfile->fp     = fp;
    bfile->mode   = mode;
    bfile->byte   = ~1;     // Initialize the byte with 11111110:
    bfile->nbits  = -1;     // Set the number of bits written/read to -1
    bfile->nbytes = 0;
    return bfile;
  }
}

/**
 * Close the BitsIOFile. Returns EOF if there was an error.
 */
int bits_io_close(BitsIOFile *bfile) {
  assert(bfile != NULL);

  // Write the current (last) byte if we are in "w" mode:
  if (strncmp(bfile->mode, "w", 1) == 0 && bfile->nbits != 0) {
    int byte = fputc(bfile->byte, bfile->fp);

    // Check to see if there was a problem:
    if (byte == EOF) {
      // We will follow the fputc return value convention
      // which is to return EOF:
      return EOF;
    }
  }

  int byte = bfile->byte;
  fflush(bfile->fp);
  fclose(bfile->fp);
  free(bfile);

  return byte;
}

/**
 * Read a bit from the BitsIOFile.
 */
int bits_io_read_bit(BitsIOFile *bfile) {
  assert(bfile != NULL);
  // TODO:
  // First, we need to check to see if this is the first time we are
  // reading from the BitsIOFile. If it is you need to read a byte in
  // using fgetc and store the byte in `bfile->byte`. Make sure to
  // initialize the number of bits read to 0 and the number of bytes
  // read to 0.
  // 
  	if(bfile->nbits == -1 && bfile->nbytes == 0){
  		bfile->byte = fgetc(bfile->fp);
  		bfile->nbits   = 0;
  		bfile->nbytes  = 0;
  	}
  //
  // Next, you need to check to see if you have read in all
  // 7 bits from the byte. If you just read in the first byte, this
  // will not matter. Otherwise, you need to use fgetc to read in the
  // next byte, reset the number of bits read to 0, and increment the
  // number of bytes read. If fgetc returns EOF you will need to
  // return EOF. 
  //	
  	if(bfile->nbits == 7) {
  	  bfile->byte = fgetc(bfile->fp);
  	  bfile->nbits = 0;
  	  bfile->nbytes++;
    
	    if (bfile->byte == EOF) {
		    //will return EOF if is EOF.
	      return EOF;
	    }   
	}
  //
  // Next, you need to actually read a bit from the byte buffer. You
  // will first need to check for the special case of reading the last
  // byte that may not be full. That is, if the number of bits read is
  // 0 then you need to loop while the most significant bit is 1 (this
  // will require a right shift of 7). The body of the loop will
  // discard the most significant 1 bit with a left shift of 1 (hint:
  // you need to make sure that when you shift left by 1 your store
  // the result back into the byte buffer). (another hint: make sure
  // you update the number of bits read - even though we are
  // discarding them - ponder why you need to do this). After this
  // loop completes you will be ready to start reading meaningful
  // bits.
  //
	if(bfile->nbits == 0){
		while((bfile->byte >> 7) == 1){
			bfile->byte = (bfile->byte << 1) ;
			bfile->nbits++;
		}
	}
  //
  // Lastly, shift the byte buffer left by 1 to shift out the previous
  // bit read (or the 0 bit in the case we are dealing with a new byte
  // or the last byte). Then shift right by 7 to retrieve the most
  // significant bit. Update the number of bits read and return the
  // bit.
	int bit;
	bfile->byte = (bfile->byte << 1) ;
	bit = bfile->byte >> 7;
	bfile->nbits++;
  return bit;
}

/**
 * Writes the given bit (1 or 0) to the BitsIOFile.
 */
int bits_io_write_bit(BitsIOFile *bfile, unsigned char bit) {
  assert(bfile != NULL);

  // Write the bit into the byte:
  bfile->byte = (bfile->byte << 1) | bit;

  // Check if the byte is full and write if it is. A byte is
  // full if its left-most bit is 0:
  if ((bfile->byte >> 7) == 0) {
    int byte = fputc(bfile->byte, bfile->fp);

    // Check to see if there was a problem:
    if (byte == EOF) {
      // We will follow the fputc return value convention
      // which is to return EOF:
      return EOF;
    }
    // Reset the byte:
    bfile->byte = ~1;
    // Update the number of bytes written:
    bfile->nbytes += 1;
  }
  return bit;
}

/**
 * Writes the huffman tree to the BitsIOFile.
 *
 * We need to write the tree to the file so that we can use it when we
 * decode the compressed file.
 */
int bits_io_write_tree(BitsIOFile *bfile, TreeNode *tree) {
  // If the mode is not for writing we return -1.
  if (strncmp(bfile->mode, "w", 1) != 0)
    return -1;

  tree_serialize(tree, bfile->fp);
  return tree_size(tree);
}

/**
 * Reads the huffman tree from the BitsIOFile.
 *
 * We need to do this first so we have a tree that will be used to
 * decode the rest of the input.
 */
TreeNode* bits_io_read_tree(BitsIOFile *bfile) {
  // If the mode is not for writing we return -1.
  if (strncmp(bfile->mode, "r", 1) != 0)
    return NULL;
  TreeNode *tree = tree_deserialize(bfile->fp);
  return tree;
}
