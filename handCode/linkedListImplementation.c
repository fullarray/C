typedef struct node{
	int val;
	struct node * next;
} node_t;

node_t * head = NULL;
head = malloc(sizeof(node_t));
if(head == NULL){
	return 1;
}
head->val = 1;
head->next = malloc(sizeof(node_t));
head->next->val = 2;
head->next->next = NULL;


//iterate linkedlist
void print_list(node_t * head){
	node_t * current = head;
	
	while(current != NULL){
		printf("%d\n", current->val);
		current = current->next;
	}
}

void push(node_t * head, int val){
	node_t * current = head;
	while(current->next != NULL){
		current = current->next;
	}
	
	current->next = malloc(sizeof(node_t));
	current->next->val = val;
	current->next->next = NULL:
}

//adding at the beginning of the list
void push(node_t ** head, int val){
	node_t * new_node;
	new_node = malloc(sizeof(node_t));
	
	new_node->val = val;
	new_node->next = *head;
	*head = new_node;
}

//remove first node
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

//remove last node
int remove_last(node_t * head){
	int retval = 0;
	if(head->next == NULL){
		retval = head->val;
		free(head);
		return retval;
	}
	node_t * current = head;
	while(current->next->next != NULL){
		current = current->next;
	}
	
	retval = current->next->val;
	free(current->next);
	current->next = NULL;
	return retval;
}

//remove specific item
int remove_by_index(node_t ** head, int n){
	int i = 0;
	int retval = -1;
	node_t * current = *head;
	node_t * temp_node = NULL;
	if(n == 0){
		return pop(head);
	}
	for(int i =0; i< n-1; i++){
		if(current->next == NULL){
			return -1;
		}
		current = current->next;
	}
	temp_node = current->next;
	retval = temp_node->val;
	current->next = temp_node->next;
	free(temp_node);
	return retval;
}