
#include <bits/stdc++.h> 
using namespace std; 

class Node 
{ 
	public: 
	int data; 
	Node *next; 
}; 
void push(Node** head_ref, int new_data) 
{ 
	Node* new_node = new Node(); //allocate node
	new_node->data = new_data; 
	new_node->next = (*head_ref); 
	(*head_ref) = new_node; 
} 
void insertAfter(Node* prev_node, int new_data) 
{ 
	if (prev_node == NULL) 
	{ 
		cout<<"the given previous node cannot be NULL"; 
		return; 
	} 

	/* 2. allocate new node */
	Node* new_node = new Node(); 
	new_node->data = new_data; 
	new_node->next = prev_node->next; 
	prev_node->next = new_node; 
} 


void append(Node** head_ref, int new_data) 
{ 
	Node* new_node = new Node(); 

	Node *last = *head_ref; 
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
void printList(Node *node) 
{ 
	while (node != NULL) 
	{ 
		cout<<" "<<node->data; 
		node = node->next; 
	} 
} 

int main() 
{ 
	Node* head = NULL;  //starting with empty node 
	append(&head, 96); //inserting 96 as head
	push(&head, 17);   //17->96
	push(&head, 51);   //51->17->96
	append(&head, 14); //51->17->96->14
	insertAfter(head->next, 8); 
	cout<<"Created Linked list is: "; 
	printList(head); 
	
	return 0; 
} 


