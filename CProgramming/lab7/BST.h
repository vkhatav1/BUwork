struct Tree{
    struct Node *root;
};
struct Tree tree;

struct Node{
    struct Node *left;
    struct Node *right;
    int data;
    struct Node *parent;
};
struct Node node;

int flag=0;
struct Tree * createTree();
struct Node * createNode(int data);
void insertNode(struct Node *node1,int data);
void insert(struct Tree * bst, int data);
struct Node * searchNode(struct Node *, int data);
struct Node * search(struct Tree *bst, int data);
void print_tree(struct Node *);
void free_tree(struct Node *node);



