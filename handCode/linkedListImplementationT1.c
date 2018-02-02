#include <stdio.h>
#include <stdlib.h>

typedef struct node{
	int val;
	struct node * next;
} node_t;

void print_list(node_t * head){
	node_t * current = head;
	while(current != NULL){
		printf("%d\n", current->val);
		current = current->next;
	}
}

int pop(node_t ** head){
	int retval = -1;
	node_t * next_node = NULL;
	if(*head == NULL){
		return -1;
	}
	
	next_node = (*head)->next;
	retval = (*head)->val;
	free(*head);
	*head = next_node;
	return retval;
}


int remove_by_value(node_t ** head, int val){
	//To be implemented
}

int main(){
	node_t * testList = malloc(sizeof(node_t));
	testList->val = 1;
	testList->next = malloc(sizeof(node_t));
	testList->next->val = 2;
	testList->next->next = malloc(sizeof(node_t));
	remove_by_value(&testList, 2);
	print_list(&testList);
}

