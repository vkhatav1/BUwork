#include <stdio.h>
#include "lab6.h"
#include <stdlib.h>

int main() {
    int data,num;
    struct car *car2,*current,*last_car;
    struct train *train2;
    train2 = &train1;
    //list1 = create_list(node);
    printf("Enter the number of cars in the train :\n");
    scanf("%d",&num);
    printf("Enter the values to the train :\n");
    while (i<num) {
        scanf("%d", &data);
        list_insert(train2, data);
    }
    print_list(train2);
    current=train2->first;
    while(current->next!=NULL)
        current=current->next;
    last_car=current;
    for (int j = 0; j < num; ++j) {
        free_memory(train2);
    }
    print_list(train2);

    return 0;
}

struct car *create_node(int a) {
    struct car *car3;
    car3 =malloc(sizeof(struct car));
    car3->cargo_weight = a;
    car3->next = NULL;
    i++;
    return car3;
}

void print_list(struct train *train3) {
    struct car *current;
    current = train3->first;
    if(current==NULL)
    {
        printf("Empty Train!!\n");
    }
    while (current != NULL) {
        printf("Weight : %d\t", current->cargo_weight);
        printf("Address : %p\n",current);
        current = current->next;
    }
}

void list_insert(struct train *train3, int data) {
    struct car *current;
    current = train3->first;
    if (current == NULL)
        train3->first = create_node(data);
    else {
        while (current->next != NULL)
            current = current->next;
        current->next= create_node(data);
    }
}

void free_memory(struct train *train3)
{
    struct car *current,*temp;
    current=train3->first;
    temp=current->next;
    train3->first=temp;
    printf("Car weight being freed is %d\n",current->cargo_weight);
    free(current);
    current=NULL;
}