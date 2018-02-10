/*	huffman.c
 *	Andrew Nguyen
 *	Lab 5
 *	Friday Lab, COEN 12
 *
 *	This file contains a huffman code that assigns 
 *	variable-length codes to input characters based 
 *	on its frequency. Utilizes priority queue of trees.
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include "pqueue.h"
#include "pack.h"

static int nodecmp(struct node *n1, struct node *n2);

int main(int argc, char *argv[]){
	
	int occurs[256] = {0};	/* array of characters in file */
	int i;			/* index */
	int c;			/* counts */

	//call file
	FILE *fp = fopen(argv[1], "r");

	if (fp == NULL)
		return 0;
	
	//step 1: Count the number of occurences of each character in file
	while ((c = getc(fp))){
		if (c == EOF)
			break;
		occurs[c]++;
	}

	//initialize priority queue
	PQ *pq = createQueue(nodecmp);
	struct node *leaves[257];
	struct node *np;
	
	//step 2: create binary tree with just a leaf for each character
	for (i = 0; i < 256; i++){
		if (occurs[i] !=  0){
			np = malloc(sizeof(struct node));
			np->parent = NULL;
			np->count = occurs[i];
			addEntry(pq, np);
			leaves[i] = np;
			leaves[i]->count = np->count;
		}
	}
	
	//continued: create a tree with zero count for the end of file marker
	np = malloc(sizeof(struct node));
	np->parent = NULL;
	np->count = 0;
	addEntry(pq, np);
	leaves[256] = np;
	leaves[256]->parent = NULL;
	leaves[256]->count = 0;
	
	//part 3: remove two lowest trees and creates a new tree with both as children
	//the new tree is inserted back into priority queue 
 	
	//part 4: continues until huffman tree remains
	while(numEntries(pq) > 1){
		struct node *min1;
		struct node *min2;
		struct node *np;
		
		np = malloc(sizeof(struct node));
		min1 = removeEntry(pq);
		min2 = removeEntry(pq);
		np->parent = NULL;
		np->count = min1->count + min2->count;
		min1->parent = np;
		min2->parent = np;
		addEntry(pq, np);
	}

	int hop = 0;
	int bit[257] = {0};

	//step 5: counts the number of bits for each character by starting at the leaf and moving towards the root.  
	for (i = 0; i < 257; i++){
		struct node *locn;
		locn = leaves[i];
		if (occurs[i] > 0){
			while(locn != NULL){
				hop++;
				locn = locn->parent;
			}
			bit[i] = hop;
		}
		hop = 0;
		free(locn);
	}	
	
	//prints the total bits per character
	for (i = 0; i < 257; i++){
		if (leaves[i] != NULL){
			if (isprint(i) == 0)
				printf("'%03o': %d x %d bits = %d bits\n", i, leaves[i]->count, bit[i], (leaves[i]->count)*bit[i]);
			if (isprint(i) == 1)
				printf("'%c': %d x %d bits = %d bits\n", i, leaves[i]->count, bit[i], (leaves[i]->count)*bit[i]);
		}
	}
	fclose(fp);

	//call pack function 
	pack(argv[1], argv[2], leaves);
	return 0;
}

// given pointers to two nodes, compare their counts
static int nodecmp(struct node *n1, struct node *n2){
	return (n1->count - n2->count);
}

