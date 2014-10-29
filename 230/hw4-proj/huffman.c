/********************************************************************
 
 This represents the main huffman coding algorithm. The algorithm
 operates in three phases:

 (1) Compute Frequencies 
       This phase works by reading each character from the input and
       updating its frequency in a table of Frequency objects (see
       tree.h).

 (2) TreeNode Creation
       After the frequency of each character in the input have been
       found we create a new TreeNode object for each character and
       add it to a priority queue. These initial TreeNode objects do
       not have child nodes. We figure out the children in part (3).

 (3) Huffman Tree Construction
      
       The third phase dequeues TreeNode objects from the priority
       queue until the priority queue contains only a single TreeNode
       object - this represents the root of the tree. With each
       iteration we dequeue the next two TreeNode objects L and R,
       create a new internal TreeNode X with its frequency being the
       addition of the two TreeNode objects we dequeued. We then
       assign the left child of X to be L and right child of X to be
       R. We then enqueue X into the priority queue.
       

*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "huffman.h"
#include "tree.h"
#include "pqueue.h"

// The start and end of the ASCII characters
#define CHAR_START      0
#define CHAR_END        127
#define NUMBER_OF_CHARS (CHAR_END - CHAR_START + 1)

/**
 * The Context object is used to pass information between each of the
 * huffman phases. It contains the table of frequencies of the
 * characters and a pointer to the priority queue.
 */
typedef struct Context Context;
struct Context {
  Frequency table[NUMBER_OF_CHARS];
  PriorityQueue *pq;
};

/**
 * (1) Compute Frequencies 
 *
 * This phase computes the frequencies of the characters given the
 * FILE pointer fp.
 */
static void compute_freq(FILE *fp, Context *ctx) {
  // TODO:
  // First, we need to initialize the frequency table in the Context
  // object. We can initialize its frequency to 0 and its character to
  // '\0'. Next, we iterate over the input to determine the
  // frequencies of each character. A useful function to grab a
  // character from a file is `getc`. You should look at its man page
  // for more details. You should make sure that the character you
  // read in is in the range of the characters we are considering. If
  // it is in the range you must update its entry in the frequency
  // table by incrementing its frequency and character. It is ok if we
  // assign the same character multiple times to the same Frequency
  // object in the table, as long as we assign it at least once.
  //
	for (int i = 0; i < NUMBER_OF_CHARS; ++i){
		ctx->table[i].v     = 0;
		ctx->table[i].c     = i + CHAR_START;
	}
	int c;

	//iterate trhought the file
	while((c = getc(fp)) != EOF){
		//is the char read within the range?
		if (c >= CHAR_START && c <= CHAR_END )
		{//if so...
			ctx->table[c - CHAR_START].v++;
		}
	}
}
/**
 * (1) TreeNode Creation
 *
 * This phase iterates over the frequency table we constructed in
 * phase (2) and creates a new TreeNode object for each character
 * found in the frequency table. Each new TreeNode object is enqueued
 * into the priority queue.
 */
static void create_tree_nodes(Context *ctx) {
  // TODO:
  // First, we need to iterate over the characters we are
  // considering. If the character was not encountered in the input we
  // skip it as we do not need to worry about encoding that
  // character. If the frequency of a character is greater than 0 we
  // create a new TreeNode object. The new TreeNode object must be
  // initialized properly. In particular, it should be a LEAF node, it
  // should receive the Frequency object in the frequency table, and
  // its left and right children should be NULL. We worry about
  // constructing the tree in phase (3). Lastly, enqueue the new
  // TreeNode in the priority queue.
  //
	for (int i = 0; i <= CHAR_END; ++i){
		if (ctx->table[i].v > 0){
			TreeNode *new_node = tree_new();
			new_node->freq.c = ctx->table[i].c;
			new_node->freq.v = ctx->table[i].v;
			pqueue_enqueue(ctx->pq,new_node);
		}
	}
}

/**
 * (3) Huffman Tree Construction
 *
 * This is the third and final phase that constructs the Huffman tree
 * from the priority queue populated in phase (2). This function
 * returns the final tree that can be used to encode/decode characters
 * and binary encoding respectively. 
 */
static TreeNode *build_tree(Context *ctx) {
  // TODO:
  // You must iterate while the size of the priority queue is greater
  // than 1. For each iteration you must dequeue the next two TreeNode
  // objects L and R. After you dequeue you need to compute the sum of
  // their frequencies F. Next, create a new TreeNode object N with
  // its type being INTERNAL, its frequency being F, and its left
  // child pointing to L and its right child pointing to R. Lastly,
  // enqueue N into the priority queue. After you break out of this
  // loop your priority queue will have a single TreeNode object which
  // represents the root of the tree. Dequeue the remaining TreeNode
  // and return it.
  //	
	//printf("%d\n",pqueue_size(ctx->pq) );
	while(pqueue_size(ctx->pq) > 1){
		TreeNode *LEFT;
		TreeNode *RIGHT;
		TreeNode *N = tree_new();
		int sum;
		N->type=INTERNAL;
		
		LEFT  = pqueue_dequeue(ctx->pq);
		RIGHT = pqueue_dequeue(ctx->pq);
		sum = LEFT->freq.v + RIGHT->freq.v;

		N->left = LEFT;
		N->right= RIGHT;
		N->freq.v = sum;
		//idk how to do this one...
		pqueue_enqueue(ctx->pq,N);
	}
	TreeNode *root = NULL;
	if (pqueue_size(ctx->pq) == 1)
		root = pqueue_dequeue(ctx->pq); 
	 
  return root;
}

/**
 * Returns a pointer to a TreeNode object or NULL if there is an
 * error. 
 * 
 * Given a filename this function will use the huffman coding
 * algorithm to construct a huffman tree that can be used to encode
 * characters from the input file.
 */
TreeNode* huffman_build_tree(char *filename) {
  // Define a Context object:
  Context ctx;

  // Create a priority queue:
  ctx.pq = pqueue_new();

  // If the priority queue is NULL there was a problem!
  if (ctx.pq == NULL)
    return NULL;

  // (1) Compute the frequencies:
  FILE *fp = fopen(filename, "r");
  if (fp == NULL)
    return NULL;
  compute_freq(fp, &ctx);
  fclose(fp);

  // (2) Create the tree nodes:
  create_tree_nodes(&ctx);

  // (3) Build huffman tree:
  TreeNode *root = build_tree(&ctx);

  // Free the priority queue - it is no longer needed.
  free(ctx.pq);

  return root;
}

/**
 * Returns the character for the given encoding string or -1 on
 * error.
 *
 * This function will traverse the given tree with the provided
 * encoding string (see table.c for the format of the encoding
 * string). For each "bit" in the encoding string we traverse left if
 * it is a 1 and right if it is a 0. When we reach -1 we should be at
 * a LEAF - we then return the character found at that leaf.
 */
int huffman_find(TreeNode *tree, char* encoding) {
  TreeNode *t = tree;
  for (char *ch = encoding; *ch != -1; ch = ch+1) {
    if (*ch == 1) {
      if (t->left == NULL)
	return -1;
      t = t->left;
    }
    else {
      if (t->right == NULL)
	return -1;
      t = t->right;
    }
  }
  assert (t->type == LEAF);
  return t->freq.c;
}
