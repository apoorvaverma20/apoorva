#include <stdio.h> 
#include"linklist_delete.h"
#include <stdlib.h> 

void push(struct Node** head_ref, int new_data) 
{ 
	struct Node* new_node = (struct Node*) malloc(sizeof(struct Node)); 
	new_node->data = new_data; 
	new_node->next = (*head_ref); 
	(*head_ref) = new_node; 
} 
void deleteNode(struct Node **head_ref, int key) 
{ 
	struct Node* temp = *head_ref, *prev; 
	if (temp != NULL && temp->data == key) //deleting 1st element
	{ 
		*head_ref = temp->next;
		free(temp);	
		return; 
	} 
	while (temp != NULL && temp->data != key) //deleting middle element
	{ 
		prev = temp; 
		temp = temp->next; 
	} 
	if (temp == NULL) return; // If key was not present in linked list  
	prev->next = temp->next; 
	free(temp); //free memory
} 
void printList(struct Node *node) 
{ 
	while (node != NULL) 
	{ 
		printf(" %d ", node->data); 
		node = node->next; 
	} 
} 

