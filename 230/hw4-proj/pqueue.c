/********************************************************************

 The pqueue module implements a priority queue for the huffman coding
 algorithm. It is a very simply implementation of a priority queue.
 In particular, it simply maintains an array of TreeNode objects that
 gets sorted each time a TreeNode is enqueued.

 *******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "pqueue.h"
#include "tree.h"

// The maximum size of the priority queue. It should never exceed
// this size as it is the maximum size of a character. In fact, we
// do not even need to have 255 entries as we are only dealing with
// ASCII encoded characters.
#define MAXSIZE 255

/**
 * The PriorityQueue represents the priority queue. It holds an
 * array of TreeNode's and an int indicating the next empty entry in
 * the queue.
 */
struct PriorityQueue {
  TreeNode *queue[MAXSIZE];
  int       next;
};

/**
 * This function is used by the C library qsort routine. It is
 * similar to a Comparator object in the Java sense, however,
 * C simply uses a comparator function. It compares two TreeNode
 * objects to determine when one is less than the other. The
 * huffman coding algorithm requires characters to be sorted by
 * their frequencies in ascending order.
 */
static int comparator(const void* x, const void* y) {
  TreeNode* n1 = * (TreeNode**) x;
  TreeNode* n2 = * (TreeNode**) y;
  int a = n1->freq.v;
  int b = n2->freq.v;
  return (a < b) ? -1 : (a > b);
}

/**
 * A utility function to sort a PriorityQueue.
 */
static void sort(PriorityQueue *pq) {
  qsort(pq->queue, pq->next, sizeof(TreeNode*), comparator);
}

/**
 * Creates a new PriorityQueue object.
 */
PriorityQueue *pqueue_new() {
  PriorityQueue *pq = (PriorityQueue *) malloc(sizeof(PriorityQueue));
  for (int i = 0; i < MAXSIZE; i++) {
    pq->queue[i] = NULL;
  }
  pq->next = -1;
  return pq;
}

/**
 * Returns 1 if the priority queue is empty; 0 otherwise.
 */
static int is_empty(PriorityQueue *pq) {
  return pq->next == -1;
}

/**
 * Deallocates a priority queue.
 */
void pqueue_free(PriorityQueue *pq) {
  for (int i = 0; i < MAXSIZE; i++) {
    if (pq->queue[i] != NULL)
      free(pq->queue[i]);
  }
  free(pq);
}

/**
 * Enqueues the given TreeNode object `n` in the
 * PriorityQueue `pq`.
 */
void pqueue_enqueue(PriorityQueue *pq, TreeNode* n) {
  // TODO:
  // First, increment the next field of the priority queue to point to
  // the next slot. Next, check if next is greater than or equal to
  // the MAXSIZE. If it is print an error message and call exit with
  // an argument of 1. Otherwise, assign the tree node to the next
  // location in the queue and call sort.
  //
	pq->next++;
	if(pq->next >= MAXSIZE){
		printf("Error! Maximum size of the priority queue has been exeeded\n");
		exit(1);
	}
	else{
		pq->queue[pq->next] = n ;
		sort(pq);
	}
}

/**
 * Dequeues the TreeNode object in the PriorityQueue.
 * Returns NULL if the PriorityQueue is empty.
 */
TreeNode* pqueue_dequeue(PriorityQueue *pq) {
  // TODO:
  // First, check to see if the priority queue is empty. If it is you
  // should return NULL. Next, assign the tree node in the front of
  // the queue to a temporary variable T and move each of the
  // subsequent elements in the queue (1 - next) over by one slot to
  // the left. You need to do this to fill in the hole left by the
  // tree node we just dequeued. Next, make the last entry (the one
  // that next how points to NULL) and then decrement next. Finally,
  // return the tree node you assigned to the temporary T.
  return NULL;
}

/**
 * Prints the priority queue.
 */
void pqueue_print(PriorityQueue *pq) {
  for (int i = 0; i < pq->next; i++) {
    printf("%c", pq->queue[i]->freq.c);
    if (i+1 < pq->next)
      printf(", ");
  }
  printf("\n");
}

/**
 * Returns the size of the priority queue.
 */
int pqueue_size(PriorityQueue *pq) {
  return pq->next+1;
}
