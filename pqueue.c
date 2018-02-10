/*	Andrew Nguyen
 *	COEN 12 Friday Lab
 *	Lab 5
 *
 *	Description: This file contains a priority queue 
 *	ADT using a binary heap implemented by an array.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "pqueue.h"
#define START_LENGTH 10
#define p(x) (((x)-1)/2)	/* parent */
#define l(x) (((x)*2)+1)	/* left */
#define r(x) (((x)*2)+2)	/* right */

struct pqueue{
	int count;		/* number of entries in array */
	int length;		/* length of allocated aray */
	void **data;		/* allocated array of entries */
	int(*compare)();	/* comparison function */
};
typedef struct pqueue PQ;

//	createQueue
//	O(n)
//	return a pointer to a new priority queue using *compare
PQ *createQueue(int (*compare)()){
	assert(compare != NULL);
	PQ *pq;			/* declare pointer */
	pq = malloc(sizeof(PQ));/* allocate memory for pointer */
	assert (pq != NULL);

	//initialize pointer
	pq->count = 0;		
	pq->length = START_LENGTH;
	pq->compare = compare;
	pq->data = malloc(sizeof(void*)*START_LENGTH);                                   
	assert(pq->data != NULL);
	return pq;
}

//	destroyQueue
//	O(n)
//	dealocate memory associated with the priority queue pointed to by pq
void destroyQueue(PQ *pq){
	assert(pq != NULL);

	free(pq->data);			/* free data */
	free(pq);			/* free pointer */
}

//	numEntries
//	O(1)
//	return the number of entries in the piority queue pointed to by pq
int numEntries(PQ *pq){
	assert(pq != NULL);

	return(pq->count);		/* return count */
}

//	addEntry
//	O(logn)
//	add entry to the priority queue pointed to by pq
void addEntry(PQ *pq, void *entry){
	assert(pq != NULL && entry != NULL);

	//if priority queue is full reallocate new memory
	if (pq->count == pq->length){
		pq->data = realloc(pq->data, sizeof(void*)*2*pq->length);
		pq->length = 2*pq->length;
	}
	
	pq->data[pq->count] = entry;	/* initializes entry */
	int x = pq->count;		/* initializes integer */
	void *temp;			/* declares temporary pointer */

	//place the new element at the end of the binary heap
	while (pq->compare(entry, pq->data[p(x)]) < 0){
		temp = pq->data[p(x)];
		pq->data[p(x)] = entry;
		pq->data[x] = temp;	
		x = p(x);
	}

	pq->count++;			/* reheap up */
}

//	removeEntry
//	O(logn)
//	remove and return the smallest entry from the priority queue pointed to by pq
void *removeEntry(PQ *pq){
	assert(pq != NULL);

	//recalls root data
	void *min = pq->data[0];
	pq->data[0] = pq->data[pq->count - 1];
	pq->count--;			/* reheap down */

	void *temp = pq->data[0];	/* initializes temporary pointer */
	void *swap;			/* declares temporary swap pointer */
	int parent = 0;			/* initializes integer */

	//while left parent is less than the count
	while (l(parent) < pq->count){
		//if right parent is less than count
		if (r(parent) < pq->count){

			//if the left parent is less than the right parent
			if (pq->compare(pq->data[l(parent)], pq->data[r(parent)]) < 0){
				//if left parent is less than the root
				if(pq->compare(pq->data[l(parent)], pq->data[parent]) < 0){
					swap = pq->data[l(parent)];
					pq->data[parent] = swap;
					pq->data[l(parent)] = temp;
					parent = l(parent);
				}
				else{
					break;
				}
			}
			
			//if right parent is less than the left parent
			else{
				//if right parent is less than the root
				if(pq->compare(pq->data[r(parent)], pq->data[parent]) < 0){
					swap = pq->data[r(parent)];
					pq->data[parent] = swap;
					pq->data[r(parent)] = temp;
					parent = r(parent);
				}
				else{
					break;
				}
			}
		}
		
		// only a left child
		else{ 
			//if left parent is less than the root
			if (pq->compare(pq->data[l(parent)], pq->data[parent]) < 0){
				swap = pq->data[l(parent)];
				pq->data[parent] = swap;
				pq->data[l(parent)] = temp;
				parent = l(parent);
			}
			else{
				break;
			}
		}
	}
	
	//return the smallest entry from priority queue
	return min;
	 
}
