// #ifndef AST_HEADER
// #define AST_HEADER
#include<stdio.h> 
#include<stdlib.h>

struct node
{
    int dataINT;
    char *dataSTR;
    char* type;
    struct node *left; 
    struct node *right; 
};

/* newNode() allocates a new node with the given data and NULL left and  
right pointers. */
struct node* newNodeINT(int data, struct node *left, struct node *right)
{
    // Allocate memory for new node
    struct node* node = (struct node*)malloc(sizeof(struct node));

    // Assign data to this node 
    node->dataINT = data;
    node->type = "int";
    // Initialize left and right children as NULL 
    node->left = left;
    node->right = right;
    return(node);
}


struct node* newNodeSTR(char *data, struct node *left, struct node *right)
{
    // Allocate memory for new node
    struct node* node = (struct node*)malloc(sizeof(struct node)); 
    
    // Assign data to this node 
    node->dataSTR = data;
    node->type = "str";
    // Initialize left and right children as NULL 
    node->left = left; 
    node->right = right; 
    return(node); 
}

struct node* assemblerGen(struct node* node) 
{ 
    struct node *a;
    struct node *b;

    if ((node->left == NULL) || (node->right == NULL)){
        printf("MOV %d EAX\n", node->dataINT);
        return node;
    }
  
    // first recur on left subtree 
    a = assemblerGen(node->left);
    printf("MOV EAX AUX\n");
    // then recur on right subtree 
    b = assemblerGen(node->right); 
    // printf("MOV %d AUX\n", b->dataINT);


    if (node->dataSTR == "+"){
        // printf("MOV %d EAX\n", a->dataINT);
        printf("ADD AUX EAX\n");
        // printf("MOV EAX AUX\n");
        node->dataINT = a->dataINT + b->dataINT;
        return node;
    }
    else{
        // printf("MOV %d EAX\n", a->dataINT);
        printf("MUL AUX EAX\n");
        // printf("MOV EAX AUX\n");
        node->dataINT = a->dataINT * b->dataINT;
        return node;
    }
}

struct node* evalPostorder(struct node* node)
{ 
    struct node *a;
    struct node *b;

    if ((node->left == NULL) || (node->right == NULL)){
        // printf("%d ", node->dataINT);
        return node;
    }
  
    // first recur on left subtree 
    a = evalPostorder(node->left); 
  
    // then recur on right subtree 
    b = evalPostorder(node->right); 
  
    if (node->dataSTR == "+"){
        node->dataINT = a->dataINT + b->dataINT;
        return node;
    }
    else{
        node->dataINT = a->dataINT * b->dataINT;
        return node;
    }
    
    // printf("%d ", b->dataINT);
    return b;
    // // now deal with the node
    // if (node->type == "int"){
    //     printf("%d ", node->dataINT); 
    //     // printf("%d ", 0); 
    // }
    // else{
    //     printf("%s ", node->data);
    //     // printf("%d ", 1); 
    // }   
}
// #endif