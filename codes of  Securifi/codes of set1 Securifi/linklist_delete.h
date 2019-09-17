#define MAIN_H
struct Node 
{ 
  int data; 
  struct Node *next; 
};
void push(struct Node** head_ref, int new_data);
void deleteNode(struct Node **head_ref, int key);
void printList(struct Node *node);
