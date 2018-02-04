#include <stdio.h>
#include "BST.h"
#include <stdlib.h>

int main() {

    int data[9], i = 0,value=0,choice=0;
    struct Tree *bst;
    struct Node *node1,*temp;
    node1=&node;
    FILE *fptr = fopen("data.txt", "r");
    if (fptr != NULL) {

        while (!feof(fptr)) {
            fscanf(fptr, "%d", &data[i]);
            i++;
        }
        fclose(fptr);

    } else {
        printf("\n File not found!!!\nEXITING..!!!");
        exit(0);
    }
    bst=createTree();

    for (int k = 0; k < 9; ++k) {
        insert(bst,data[k]);
    }
    do{
        printf("\n************MENU************\n");
        printf("1. Insertion \n");
        printf("2. Search \n");
        printf("3. Display \n");
        printf("4. Exit \n");
        printf("Enter your choice : \n");
        scanf("%d",&choice);
        switch(choice){
            case 1:

                printf("Enter the value for insertion : \n");
                scanf("%d",&value);
                insertNode(bst->root,value);
                break;
            case 2:
                printf("Enter the value for search : \n");
                scanf("%d",&value);
                node1=search(bst,value);
                if(flag==0)
                        printf("Value not found, Please try again!!!\n");
                else{
                    printf("Node Value :%d\n",node1->data);
                    temp=node1->left;
                    if(temp==NULL)
                        printf("Left Node : %d\n",0);
                    else
                        printf("Left Node : %d\n",temp->data);
                    temp=node1->right;
                    if(temp==NULL)
                        printf("Right Node : %d\n",0);
                    else
                        printf("Right Node : %d\n",temp->data);
                    temp=node1->parent;
                    if(temp==NULL)
                        printf("Parent Node : %d\n",0);
                    else
                        printf("Parent Node : %d\n",temp->data);
                }
                break;
            case 3:
                printf("\n");
                node1=bst->root;
                print_tree(node1);
                break;
            case 4:
                free_tree(bst->root);
                printf("\nQuiting the program!!!\n");
                break;
            default:
                printf("\nWrong choice, Try again!!!!\n");
                break;
        }

    }while (choice!=4);


    return 0;
}

struct Tree * createTree(){
    struct Tree *tree1;
    tree1=&tree;
    tree1->root=NULL;
    return tree1;
}

struct Node * createNode(int data){
    struct Node *node1=malloc(sizeof(struct Node));
    node1->data=data;
    node1->left=NULL;
    node1->right=NULL;
    node1->parent=NULL;
    return node1;
}

void insertNode(struct Node *node1, int data){
    struct Node *node2;
    if(data==node1->data){
        printf("Value entered already exists!!!\n");
    }
    else{
        if(data < node1->data){
            if(node1->left == NULL){
                node2=createNode(data);
                node2->parent=node1;
                node1->left=node2;
            }
            else
                insertNode(node1->left,data);
        }
        else if(data > node1->data){
            if(node1->right == NULL){
                node2=createNode(data);
                node2->parent=node1;
                node1->right=node2;
            }
            else
                insertNode(node1->right,data);
        }
    }

}

void insert(struct Tree *bst, int data){
    if(bst->root==NULL){
        struct Node *node1=malloc(sizeof(struct Node));
        node1->data=data;
        node1->left=NULL;
        node1->right=NULL;
        node1->parent=NULL;
        bst->root=node1;
    }
    else
        insertNode(bst->root,data);
}

struct Node * search(struct Tree *bst, int data){
    struct Node *node2;
    node2=bst->root;

    if(node2==NULL){
        printf("Tree not found!!!\n");
        return NULL;
    }
    else if(node2->data==data){
        flag=1;
        printf("Value found successfully!!!\n");
    }
    else
        node2=searchNode(node2,data);
    return node2;
}

struct Node * searchNode(struct Node *node2, int data){

    if(node2==NULL){
        return NULL;
    }
    else{
        if(node2->data==data){
            printf("Value found successfully!!!\n");
            flag=1;
            return node2;
        }
        else{
            if(node2->data < data)
                return searchNode(node2->right,data);
            else
                return searchNode(node2->left,data);
        }
    }

}

void print_tree(struct Node *root)
{
    if (root!= NULL)
    {
        print_tree(root->left);
        printf("%d \t", root->data);
        print_tree(root->right);
    }
}

void free_tree(struct Node *node)
{
    if (node == NULL)
        return;


    free_tree(node->left);

    free_tree(node->right);

    printf("%d ", node->data);
    free(node);
    node=NULL;
}