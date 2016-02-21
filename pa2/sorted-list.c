/*
 * sorted-list.c
 */

#include <stdlib.h>
#include "sorted-list.h"

/*
 * Your list is used to store data items of an unknown type, which you need to sort.
 * Since the type is opaque to you, you do not know how to directly compare the data.
 *
 * You can presume though that a user will supply your code with a comparator function
 * that knows how to compare the data being sorted, but your code must do the rest
 * of the bookkeeping in a generic manner.
 *
 * The comparator function will take pointers to two data items and will return -1 if the 1st 
 * is smaller, 0 if the two are equal, and 1 if the 2nd is smaller.
 *
 * The user will also supply a destruct function will take a pointer to a single data item
 *	and knows how to deallocate it. If the data item does not require deallocation, the user's
 *  destruct function will do nothing.
 *
 * Note that you are not expected to implement any comparator or destruct functions.
*  Your code will have appropriate comparator function and a destruct functions added to it.
 */


//-=-=-=-You must implement all the functions and definitions below-=-=-=-

//=====0: SortedList=====================================
//===0.1: List Definition, List Create/Destroy

/*
 * Sorted list type that will hold all the data to be sorted.
 */
void NodeDestroy(SortedListPtr list, NodePtr node){
	if(node == NULL)
		return;
	if(list != NULL)
		list->df(node->data);
	free(node);
}

/*
 * SLCreate creates a new, empty, 'SortedList'.
 *
 * SLCreate's parameters will be a comparator (cf) and destructor (df) function.
 *   Both the comparator and destructor functions will be defined by the user as per
 *     the prototypes above.
 *   Both functions must be stored in the SortedList struct.
 * 
 * SLCreate must return NULL if it does not succeed, and a non-NULL SortedListPtr
 *   on success.
 */
SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df) {
	SortedListPtr ptr = (SortedListPtr) malloc(sizeof(struct SortedList));
	ptr -> cf = cf;
	ptr -> df = df;
	ptr -> head = NULL;
	return ptr;
}

/*
 * SLDestroy destroys a SortedList, freeing all dynamically-allocated memory.
 */
void SLDestroy(SortedListPtr list) {
	while (list -> head != NULL && SLRemove(list, list -> head -> data) != 0) {}
}

//===0.2: List Insert/Remove

/*
 * SLInsert inserts a given data item 'newObj' into a SortedList while maintaining the
 *   order and uniqueness of list items.
 * 
 * SLInsert should return 1 if 'newObj' is not equal to any other items in the list and
 *   was successfully inserted.
 * SLInsert should return 0 if 'newObj' is equal to an item already in the list or it was
 *   not successfully inserted
 *
 * Data item equality should be tested with the user's comparator function *
 */
int SLInsert(SortedListPtr list, void *newObj) {
	if (list -> head == NULL) {
		list -> head = (NodePtr) malloc(sizeof(struct Node));
		list -> head -> data = newObj;
		list -> head -> numRef = 1;
		list -> head -> next = NULL;
		return 1;
	}

	NodePtr temp = list -> head;

	if (list->cf(newObj, temp -> data) > 0) {
		list -> head = (NodePtr) malloc(sizeof(struct Node));
		list -> head -> data = newObj;
		list -> head -> next = temp;
		list -> head -> numRef = 1;
		return 1;
	}

	while (temp -> next != NULL) {
		if (list -> cf(temp -> data, newObj) == 0) {
			return 0;
		} 

		if ((list -> cf(temp -> data, newObj) > 0) && (list -> cf(newObj, temp->next->data) > 0)) {
			NodePtr temp2 = temp -> next;
			temp -> next = (NodePtr) malloc(sizeof(struct Node));
			temp -> next -> data = newObj;
			temp -> next -> next = temp2;
			temp -> next -> numRef = 1;
		} else {
			temp = temp -> next;
		}
	}

	//Could be the last element
	if (list -> cf(temp -> data, newObj) == 0) {
		return 0;
	}

	if (list -> cf(temp -> data, newObj) > 0) {
		temp -> next = (NodePtr) malloc(sizeof(struct Node));
		temp -> next -> data = newObj;
		temp -> next -> next = NULL;
		temp -> next -> numRef = 1;
		return 1;
	}
	return 0;
}

/*
 * SLRemove should remove 'newObj' from the SortedList in a manner that
 *   maintains list order.
 *
 * SLRemove must not modify the data item pointed to by 'newObj'.
 *
 * SLRemove should return 1 on success, and 0 on failure.
 */
int SLRemove(SortedListPtr list, void *newObj) {
	NodePtr temp = list -> head;
	NodePtr toBeDeleted = NULL;

	if (list -> cf(temp -> data, newObj) == 0) {
		toBeDeleted = list -> head;
		list -> head = list -> head -> next;
		if (toBeDeleted -> numRef <= 1) {
			NodeDestroy(list, list->head);
		} else {
			toBeDeleted -> numRef--;
		}
		return 1;
	}

	NodePtr nextToBeDeleted = NULL;
	while (temp -> next != NULL) {
		if (list -> cf(temp -> next -> data, newObj)) {
			nextToBeDeleted = temp;
			toBeDeleted = temp -> next;
			break;
		}
		temp = temp -> next;
	}

	if (nextToBeDeleted == NULL) {
		return 0;
	}

	nextToBeDeleted -> next = nextToBeDeleted -> next -> next;
	if (toBeDeleted -> numRef <= 1) {
		NodeDestroy(list, toBeDeleted);
	} else {
		toBeDeleted -> numRef--;
	}
	return 1;
}

/*
 * SLCreateIterator creates a SortedListIterator for the SortedList pointed to by 'list'.
 *
 * SLCreateIterator should return a non-NULL SortedListIteratorPtr on success.
 * SLCreateIterator should return a NULL SortedListIteratorPtr if it could not
 *  construct a SortedListIterator, or if the SortedListPtr parameter 'list' is NULL.
 */
SortedListIteratorPtr SLCreateIterator(SortedListPtr list) {
	if(list == NULL)
		return NULL;
	SortedListIteratorPtr temp = (SortedListIteratorPtr) malloc(sizeof(struct SortedListIterator));
	temp->list = list;
	temp->current = list->head;
	if(temp->current != NULL)
		temp->current->numRef++;
	return temp;
}

/*
 * SLDestroyIterator destroys a SortedListIterator pointed to by parameter 'iter'.
 *
 * SLDestroyIterator should destroy the SortedListIterator, but should NOT
 *  affect the list used to create the SortedListIterator in any way.
 */
void SLDestroyIterator(SortedListIteratorPtr iter) {
	if(iter == NULL)
		return;
	if(iter->current != NULL)
	{
		if(iter->current->numRef == 1)
			NodeDestroy(iter->list, iter->current);
	}
	free(iter);
}

//===1.2: SortedList Iterator Get/Next Operations

/*
 * SLNextItem returns a pointer to the data associated with the
 *  next item in the SortedListIterator's list
 *
 * SLNextItem should return a NULL if all of iter's elements have
 *  been iterated through.
 *
 * NB: Be sure to check the length of the list that SortedListIterator holds
 *         before attempting to access and return an item from it.
 *         If an item is removed from the list, making it shorter, be careful not
 *         to try to read and return an item off the end of the list.
 */
void * SLNextItem(SortedListIteratorPtr iter) {
	if(iter == NULL)
		return NULL;
	if(iter->current == NULL)
		return NULL;
	NodePtr temp = iter->current->next;
	iter->current->numRef--;
	if(iter->current->numRef == 0)
		NodeDestroy(iter->list, iter->current);
	iter->current = temp;
	if(iter->current == NULL)
		return NULL;
	return iter->current->data;
}

/*
 * SLGetItem should return a pointer to the current data item
 *   The current data item is the item that was most recently returned by SLNextItem
 *   SLGetItem should not alter the data item that SortedListIterator currently refers to
 *
 * SLGetItem should return a NULL pointer if the SortedListIterator has been advanced
 *  all the way through its list.
 *
 * NB: Be sure to check the length of the list that SortedListIterator holds
 *         before attempting to access and return an item from it.
 *         If an items are removed from the list, making it shorter, be careful not
 *         to try to read and return an item off the end of the list.
 */
void * SLGetItem( SortedListIteratorPtr iter ) {
	if(iter == NULL)
		return NULL;
	if(iter->current == NULL)
		return NULL;
	return iter->current->data;
}

