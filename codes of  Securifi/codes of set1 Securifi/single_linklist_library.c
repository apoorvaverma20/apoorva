#include <stdio.h> 
#include <stdlib.h>
#include"single_linklist_library.h" 


int main() 
{ 
struct Node* head = NULL; 

// list becomes 6->NULL 
append(&head, 6); 

//list becomes 7->6->NULL 
push(&head, 7); 

// list becomes 1->7->6->NULL 
push(&head, 1); 

// list becomes 1->7->6->4->NULL 
append(&head, 4); 

//list becomes 1->7->8->6->4->NULL 
insertAfter(head->next, 8); 

printf("\n Created Linked list is: "); 
printList(head); 
printf("\n");
return 0; 
} 

void push(struct Node** head_ref, int new_data)
{ 
	struct Node* new_node = (struct Node*) malloc(sizeof(struct Node)); 
	new_node->data = new_data; 
	new_node->next = (*head_ref); 
	(*head_ref) = new_node; 
} 
void insertAfter(struct Node* prev_node, int new_data) 
{ 
	if (prev_node == NULL) 
	{ 
	printf("the given previous node cannot be NULL"); 
	return; 
	} 
	struct Node* new_node =(struct Node*) malloc(sizeof(struct Node)); 
	new_node->data = new_data; 
	new_node->next = prev_node->next; 
	prev_node->next = new_node; 
} 

void append(struct Node** head_ref, int new_data) 
{ 
	struct Node* new_node = (struct Node*) malloc(sizeof(struct Node)); 

	struct Node *last = *head_ref;
	new_node->data = new_data; 
	new_node->next = NULL; 
	if (*head_ref == NULL) 
	{ 
	*head_ref = new_node; 
	return; 
	} 
	while (last->next != NULL) 
		last = last->next; 
	last->next = new_node; 
	return; 
} 

void printList(struct Node *node) 
{ 
while (node != NULL) 
{ 
	printf(" %d ", node->data); 
	node = node->next; 
} 
} 


