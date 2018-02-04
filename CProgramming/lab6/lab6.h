struct car
{
    int cargo_weight;
    struct car *next;
};
struct car car1;

struct train
{
    struct car *first;
};
struct train train1;

struct car* create_node(int);
void print_list(struct train *);
void list_insert(struct train *train3,int data);
void free_memory(struct train *);
int i=0;



struct Node
{
    int info;
    struct Node *next;
    struct Node *prev;
};

struct List
{
    struct Node *first;
};

struct Stack
{
    int data;
    struct Stack* next;
};

struct Queue
{
    int data;
    struct Queue *next;
};

struct Queue *front_queue=NULL,*rear_queue=NULL,*temp_queue=NULL,*front1_queue=NULL;

struct Node* create_node_sll(int,struct Node*);
void print_list_sll(struct List *);
void list_insert_sll(struct List *list3,int data);
struct List* front(int data);
void free_memory_sll(struct List *);
struct Node * insert(struct List *list, int index,int value);
int remove_node(struct List * list, int index);
int search_forward(struct List * list,int value);
int search_backward(struct List *list,int value);

struct Stack* newNode(int data);
int empty(struct Stack *root);
void push(struct Stack** root, int data);
int pop(struct Stack** root);

void enqueue(struct Queue * queue, int value);
int dequeue(struct Queue * queue);

int counter=0;




