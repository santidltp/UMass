#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "wset.h"

/**
 * The WordNode struct is used to represent a node in a linked
 * list of words. It is defined in this file because it is
 * used only within this file. It is used only as part of the
 * implementation of a word set.
 */
typedef struct WordNode WordNode;
struct WordNode {
  int       freq;   // The frequency of the the word.
  char*     word;   // The word itself.
  WordNode* next;   // The next word node in the list.
};

/**
 * A WordSet struct is used to represent a "set". We are
 * implementing a set as a linked list of WordNodes.
 */
struct WordSet {
  int       size;   // The number of elements in the set.
  WordNode* head;   // The starting node in the set.
};

/**
 * The node_free function is used to free a WordNode object.
 */
static void node_free(WordNode* n) {
  if (n != NULL) {
    free(n->word);
    free(n);
  }
}

/**
 * The node_free_all function is used to free a list of 
 * WordNode objects.
 */
static void node_free_all(WordNode* n) {
  if (n != NULL) {
    node_free_all(n->next);
    node_free(n);
  }
}

/**
 * The node_new function is used to create a new WordNode object.
 */
static WordNode* node_new(char* word) {
  WordNode* n = (WordNode*) malloc(sizeof(WordNode));
  n->freq = 1;
  int length = strlen(word);
  n->word = (char*) malloc(length+1);
  memcpy(n->word, word, length+1);
  n->next = NULL;
  return n;
}

/**
 * The wset_new function is used to create a new WordSet object.
 */
WordSet* wset_new() {
  // TODO: Allocate a new WordSet object using `malloc`. Make sure you
  // initialize it properly to avoid possible errors or confusion.
  WordSet* word_set = (WordSet*) malloc(sizeof(WordSet));
  word_set->size = 0;
  word_set->head = NULL;/////////////////*****************************************************
  return word_set;
}

/**
 * The wset_free function is used to free a WordSet object.
 */
void wset_free(WordSet* wset) {
  // TODO: Deallocate a WordSet object using `free`.
  free(wset);//I want to break free
}

/**
 * Returns the number of distinct words in the set.
 */
int wset_size(WordSet* wset) {
  return wset->size;
}

/**
 * The wset_add function is used to add a new WordNode object to the
 * set if it does not exist. If it does not exist we create a new
 * WordNode and add it to the set - making sure to update its
 * frequency to 1. If it does exist we increment its frequency. The
 * function returns the frequency of the word.
 */
int wset_add(WordSet* wset, char* word) {
  // TODO: The goal of this function is to add the word `word` to the
  // WordSet. The set implementation is simply a linked list of
  // WordNode's. If a WordNode object does not exist for the given
  // word you must create a new WordNode object, initialize its
  // frequency to 1, and add it to the set. 
  //
  // If the word is in the set already then you simply update its
  // frequency. This function returns the frequency of the word.
  //
  // HINTS: Make sure to check for special cases such as an empty
  // set. You should use the `strncmp` function to compare words in
  // the set. Use the `man` command to learn more about this
  // function.
  // printf("%i\n",wset->size);

  // printf("%s\t\t\t\t\n\n\n",word);
  WordNode* nody=NULL;
  nody= wset->head;
 if(wset->head==NULL){
    wset->head = node_new(word);
    wset->size++;
    return 1;
  }/*
wset_print(wset);
wset_print(nody);
*/
  while(nody != NULL){
     
    //If the word is already there, increment the 
    //frequency.
        // printf("%s\t\t%s\n\n\n\n\n",wset->head->word,word);
    if(strcmp(nody->word,word)==0){

      nody->freq++;
     return nody->freq;
    }
    else{
      nody = nody->next;
    }
 
      

  }
    nody= node_new(word);
    nody->next = wset->head;
    wset->head = nody;
    wset->size++; 
  return nody->freq;
}

/**
 * The wset_fast_add function adds a new word to the set if it does
 * not exist. In addition if the word does exist in the set it will
 * move the WordNode object to the front of the linked list. This will
 * improve performance of adding words to the set and incrementing
 * their frequency because the most frequently accessed nodes will
 * percolate to the front of the list. Thus, this will result in
 * shorter search times. The function returns the frequency of the
 * word.
 */
int wset_fast_add(WordSet* wset, char* word) {
  // TODO: Implement a fast add function. This function will also add
  // words to the set if they do not exist or update the word's
  // frequency if it is already in the set. In addition, if the word
  // already exists in the set you must move the associated WordNode
  // object to the front of the list. This will result in frequently
  // seen words residing at the beginning of the list - which will
  // reduce the amount of time it takes to find them.
  //
  // This function is used by the wordfreq-fast.c program.
  //
  // HINTS: Make sure to check for special cases such as an empty
  // set. You should use the `strncmp` function to compare words in
  // the set. Use the `man` command to learn more about this
  // function.


  
 
  WordNode* tail =NULL;
  WordNode* temp =NULL;
  WordNode* head =NULL;
  tail = wset->head;
  head = wset->head;
  int frequency=0;
  
  
  //if its empty.
 if(head==NULL){
    head= node_new(word);
    wset->head=head;
    wset->size++;
    return 1;
  }
  //if is already in the first one.
  if(strcmp(head->word,word)==0){
    head->freq++;
    frequency=head->freq;
    return frequency;
  }

  while(tail->next != NULL) tail=tail->next;//tail definition

//add another word
  while(head->next != NULL){
    //If the word is already there, increment the 
    //frequency.
    if(strcmp(head->next->word,word)==0){
      head->next->freq++;
      temp= head->next;
      head->next = head->next->next;
     
       temp->next=wset->head; 
       wset->head=temp;
       wset->size++;

     return head->freq;
    }
    else{
      head = head->next;
    }
  }

    head= node_new(word);
    head->next = wset->head;
    wset->head = head;
    wset->size++;
  return head->freq;
 
}

/**
 * The wset_remove function is used to remove the given word
 * from a WordSet object. It returns the frequency of the word it
 * removed or -1 if the word was not found.
 */
int wset_remove(WordSet* wset, char* word) {
  // TODO: This function must remove the word from the set. You should
  // traverse the list until you find the word and then remove it. Do
  // not forget to free the WordNode object or you will have a memory
  // leak! Make sure you adjust the size of the set after you remove
  // the word. Pay careful attention to edge cases such as an empty
  // set and when the word you are removing is the first element in
  // the linked list of WordNode objects.



  
if (wset->head == NULL)return -1;


WordNode* head=NULL;
WordNode* tail=NULL;
WordNode* removeNode=NULL;
head=wset->head;
tail=wset->head;
int frequency=0;

 
//first case-scenario
     


 if(strcmp(head->word,word)==0) {
          removeNode = head;
            frequency=head->freq;
            wset->head = head->next;
      wset->size--;
      free(removeNode);
      return frequency;
    }


//remove last
    while(tail->next != NULL)tail=tail->next;//defininsg tail
   if(tail!=NULL)
    if(strcmp(tail->word,word)==0){
      removeNode=tail;         
      WordNode* previousToTail = head;
      while (previousToTail->next != tail)
         previousToTail = previousToTail->next;
      tail = previousToTail;
      tail->next = NULL;
       wset->size--;
       frequency=removeNode->freq;
      free(removeNode);
      return frequency;
    }
    //delete in between

    while(head->next != tail){
 
        if(strcmp(head->next->word,word)==0){
          //delte
          //subtract
          //return
 
          
           removeNode=head->next;
           head->next=removeNode->next;
           wset->size--;
           frequency=removeNode->freq;
           free(removeNode);
           return frequency;
        }
        //go to the next
        head=head->next;

    }
  return -1;
}

/**
 * The wset_exists function is used to test the existence of
 * the given word in a WordSet object. It returns 1 if the word exists
 * in the set; 0 otherwise.
 */
int wset_exists(WordSet* wset, char* word) {
  // TODO: iterate through the linked list until you find the word. If
  // you do not find the word then you return 0.

WordNode* head = NULL;
head=wset->head;
//wset_print(nody);

 //printf("%s\t\t\t",wset->head);
  while(head != NULL){
    //If the word is already there, increment the 
    //frequency.
    
    if(strcmp(head->word,word)==0){
      return 1; //found it
    }
    if(head->next != NULL)
      head = head->next;
      else break;
  }

  return 0;

}

/**
 * The wset_freq function returns the frequency of the given word
 * in the WordSet object. If the word does not exist it returns 0.
 */
int wset_freq(WordSet* wset, char* word) {
  // TODO: iterate through the linked list until you find the word -
  // return its frequency. If you do not find the word then return 0.
  WordSet* head=NULL;
  head=wset;
    while(head->head != NULL){
    //If the word is already there, increment the 
    //frequency.
    if(strcmp(head->head->word,word)==0){
     return head->head->freq;
    }
 if(head->head->next != NULL)
  head->head = head->head->next;
     else break;
  }
  wset_print(wset);
  return 0;
}

/**
 * The wset_print function prints the words and frequencies in
 * the WordSet object.
 */
void wset_print(WordSet* wset) {
  int longest = 0;
  for (WordNode* p = wset->head; p != NULL; p = p->next) {
    int len = strlen(p->word);
    if (longest < len)
      longest = len;
  }
  for (WordNode* p = wset->head; p != NULL; p = p->next) {
    printf("%-.*s : %d\n", longest, p->word, p->freq);
  }
}
