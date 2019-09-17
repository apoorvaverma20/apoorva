
#include <stdio.h> 
#include <stdlib.h> 
#include"linklist_delete.h"

int main() 
{ 
	struct Node* head = NULL; 
	push(&head, 7); 
	push(&head, 1); 
	push(&head, 3); 
	push(&head, 2); 
	puts("Created Linked List: "); 
	printList(head); 
	deleteNode(&head, 1); 
	puts("\nLinked List after Deletion of 1: "); 
	printList(head); 
	return 0; 
}
 


