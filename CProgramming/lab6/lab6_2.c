#include <stdio.h>
#include "lab6.h"
#include <stdlib.h>

int main() {
    int data, num = 10, index, choice, choice2 = 0, value, steps, stack_data;
    struct Node *node1;
    struct List *list1;
    struct Stack *root = NULL;
    struct Queue *queue=NULL;
    printf("Enter the value for node : \n");
    while (i < num) {
        scanf("%d", &data);
        if (i == 0)
            list1 = front(data);
        else
            list_insert_sll(list1, data);
        i++;
    }
    print_list_sll(list1);

    do {
        printf("\n**************MENU**************\n");
        printf("1. Insertion \n");
        printf("2. Deletion\n");
        printf("3. Search Forward\n");
        printf("4. Search Backward\n");
        printf("5. Exit\n");

        printf("Enter your choice : \n");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("\nInsertion :\n");
                printf("Enter the value and location of the value in the linked list : \n");
                scanf("%d%d", &data, &index);
                node1 = insert(list1, index, data);
                if (node1 == NULL) {
                    printf("ERROR!!!\n");
                    printf("The index is greater than the total indexes in linked list\n");
                } else {
                    printf("Value inserted successfully\n");
                    print_list_sll(list1);
                }
                break;
            case 2 :
                printf("Enter the location that you would like to delete : \n");
                scanf("%d", &index);
                index = remove_node(list1, index);
                if (index == -1) {
                    printf("ERROR!!!\n");
                    printf("The index is greater than the total indexes in linked list\n");
                } else {
                    printf("Value deleted successfully\n");
                    print_list_sll(list1);
                }
                break;
            case 3:
                printf("Enter the value to be searched\n");
                scanf("%d", &value);
                steps = search_forward(list1, value);
                if (steps == -1)
                    printf("Value not found in the list\n");
                else
                    printf("The total steps taken : %d\n", steps);
                break;
            case 4:
                printf("Enter the value to be searched\n");
                scanf("%d", &value);
                steps = search_backward(list1, value);
                if (steps == -1)
                    printf("Value not found in the list\n");
                else
                    printf("The total steps taken : %d\n", steps);
                break;
            case 5:
                printf("Thank you!!!\n");
                break;
            default:
                printf("Wrong choice!!!\n");
                break;
        }
    } while (choice != 5);

    do {
        printf("\nStacks and Queues : \n");
        printf("1. Stack Push \n");
        printf("2. Stack Pop\n");
        printf("3. Enqueue\n");
        printf("4. Dequeue\n");
        printf("5. Exit\n");
        printf("Enter your choice : \n");
        scanf("%d", &choice2);

        switch (choice2) {
            case 1:
                printf("Enter 5 values in the stack :\n");
                for (int j = 0; j < 5; ++j) {
                    scanf("%d", &stack_data);
                    push(&root, stack_data);
                }
                break;
            case 2:
                for (int k = 0; k < 5; ++k) {
                    printf("%d popped from stack\n", pop(&root));
                }
                break;
            case 3:
                printf("Queue\n");
                printf("Enter the elements in the queue : \n");
                for (int l = 0; l < 5; ++l) {
                    scanf("%d", &data);
                    enqueue(queue,data);
                }
                break;
            case 4:
                printf("De-queuing \n");
                for (int m = 0; m < 5; ++m) {
                    dequeue(queue);
                }
                break;
            default:
                break;
        }

    } while (choice2 != 5);
}

struct Node *create_node_sll(int data, struct Node *prev) {
    struct Node *node2;
    node2 = malloc(sizeof(struct Node));
    node2->info = data;
    node2->next = NULL;
    node2->prev = prev;
    return node2;
}

/*
void free_memory_sll(struct List *list2) {
    struct Node *current;
    current = list2->first;
    while (current->next != NULL)
        current = current->next;
    free(list2);
    list2 = NULL;
}
 */

struct List *front(int data) {
    struct List *list2;
    struct Node *node2;
    node2 = create_node_sll(data, NULL);
    list2 = malloc(sizeof(struct List));
    list2->first = node2;
    return list2;
}

void print_list_sll(struct List *list2) {
    struct Node *current;
    current = list2->first;
    printf("\nLinked list : \t");
    while (current != NULL) {
        printf("%d\t", current->info);
        current = current->next;
    }
    printf("\n");
}

void list_insert_sll(struct List *list3, int data) {
    struct Node *current;
    current = list3->first;
    if (current == NULL) {
        list3->first = create_node_sll(data, NULL);
    } else {
        while (current->next != NULL)
            current = current->next;
        current->next = create_node_sll(data, current);
    }
}

struct Node *insert(struct List *list, int index, int value) {
    struct Node *current, *new_node,*temp;
    int current_index = 0;
    current = list->first;
    if (index > 10)
        return NULL;
    else if(index==0){
        new_node = create_node_sll(value, NULL);
        new_node->next =current;
        current->prev=new_node;
        new_node->prev=NULL;
        list->first=new_node;
        return new_node;
    }
    else {
        new_node = create_node_sll(value, NULL);
        while (current->next != NULL && (current_index < (index - 1))) {
            current = current->next;
            current_index++;
        }
        new_node->next = current->next;
        temp=current->next;
        temp->prev=new_node;
        new_node->prev=current;
        current->next = new_node;

        return new_node;
    }

}

int remove_node(struct List *list, int index) {
    struct Node *current, *d_node,*temp;
    int current_index = 0;
    current = list->first;
    if (index > 10)
        return -1;
    else if (index == 0) {
        d_node = current;
        current = d_node->next;
        current->prev=NULL;
        list->first = current;
        free(d_node);
        return 1;
    } else {
        while (current->next != NULL && (current_index < (index - 1))) {
            current = current->next;
            current_index++;
        }
        d_node = current->next;
        current->next = d_node->next;
        temp=d_node->next;
        temp->prev=current;
        free(d_node);
        return 1;
    }
}

int search_forward(struct List *list, int value) {
    struct Node *current;
    int index = 0, steps = 1;
    current = list->first;
    while (current->next != NULL) {
        if (current->info == value)
            return steps;
        current = current->next;
        index++;
        steps++;
    }
    if (current->info == value)
        return steps;
    return -1;
}

int search_backward(struct List *list, int value) {
    struct Node *current;
    int index = 9, steps = 1;
    current = list->first;
    while (current->next != NULL) {
        current = current->next;
    }
    while (current->prev != NULL) {
        if (current->info == value)
            return steps;
        current = current->prev;
        index--;
        steps++;
    }
    if (current->info == value)
        return steps;
    return -1;
}

//struct Stack* newNode(int data)
//{
//    struct Stack* stack =malloc(sizeof(struct Stack));
//    stack->data = data;
//    stack->next = NULL;
//    return stack;
//}

int empty(struct Stack *root) {
    return !root;
}

void push(struct Stack **root, int data) {
    struct Stack *stack = malloc(sizeof(struct Stack));
    stack->data = data;
    stack->next = *root;
    *root = stack;
    printf("%d is pushed to stack\n", data);
}

int pop(struct Stack **root) {
    if (empty(*root))
        return -1;
    struct Stack *temp = *root;
    *root = (*root)->next;
    int popped = temp->data;
    free(temp);

    return popped;
}

void enqueue(struct Queue *queue, int value) {

    if (rear_queue == NULL) {
        rear_queue = malloc(sizeof(struct Queue));
        rear_queue->next = NULL;
        rear_queue->data = value;
        front_queue = rear_queue;
    } else {
        temp_queue = malloc(sizeof(struct Queue));
        rear_queue->next = temp_queue;
        temp_queue->data = value;
        temp_queue->next = NULL;

        rear_queue = temp_queue;
    }
    counter++;
}

int dequeue(struct Queue *queue) {
    front1_queue = front_queue;

    if (front1_queue == NULL) {
        printf("\n Empty queue Error!!!!");
        return -1;
    } else if (front1_queue->next != NULL) {
        front1_queue = front1_queue->next;
        printf("\n Dequeued value : %d", front_queue->data);
        free(front_queue);
        front_queue = front1_queue;
    } else {
        printf("\n Dequeued value : %d", front_queue->data);
        free(front_queue);
        front_queue = NULL;
        rear_queue = NULL;
    }
    counter--;
    return 0;
}