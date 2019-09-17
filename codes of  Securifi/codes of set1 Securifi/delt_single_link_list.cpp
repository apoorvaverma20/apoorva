
#include <bits/stdc++.h> 
using namespace std; 

class Node 
{ 
	public: 
	int data; 
	Node* next; 
}; 


void deleteList(Node** head_ref) 
{ 

Node* current = *head_ref; 
Node* next; 

while (current != NULL) 
{ 
	next = current->next; 
	free(current); 
	current = next; 
} 
	
*head_ref = NULL; 
} 

/*void push(Node** head_ref, int new_data) 
{ 

	Node* new_node = new Node(); 
	new_node->data = new_data; 
	new_node->next = (*head_ref); 
	(*head_ref) = new_node; 
} */
void test(Node* head_ref, int new_data) 
{ 

	Node* new_node = new Node(); 
	new_node->data = new_data; 
	new_node->next = (head_ref); 
	(head_ref) = new_node; 
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
	Node* head = NULL; 
	/*
	//1->12->1->4->1
	push(&head, 1); 
	push(&head, 4); 
	push(&head, 1); 
	push(&head, 12); 
	push(&head, 1); 
	printList(head);
	cout << "\nDeleting linked list"; 
	//deleteList(&head); */
        test(head, 1); 
        printList(head);
	test(head, 4);
          printList(head); 
	test(head, 1); 
        printList(head);
	test(head, 12); 
       printList(head);
	test(head, 1); 
	printList(head);
        cout << "\nDeleting linked list";

        
        
	
	
	cout << "\nLinked list deleted"; 
	
} 

