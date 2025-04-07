// Run with clang -Wall avl_tree.c -o avl
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Struct Node
typedef struct Node
{
    int key;
    struct Node *left;
    struct Node *right;
    int height;
} Node;

/*
* Create Node
*/
Node *createNode(int key)
{
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node) return NULL;
    node->key = key;
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}

/*
Get Height: Return height of the tree
*/ 
int getHeight(Node *node)
{
    return node ? node->height : 0;
}

/*
 * Calculate Balance Factor: Return the height diff between left tree and right tree
 * 0: Tree is balanced
 * Positive: Left subtree is heavy
 * Negative: Right subtree is heavy
 */
int getBalanceFactor(Node *node)
{
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

/*
 * Right Rotate: Right rotate the tree to make the tree balance
 */

Node *rightRotate(Node *node)
{
    if (node == NULL) return NULL;
    Node *leftNode = node->left;
    Node *rightOfLeftNode = leftNode->right;
    leftNode->right = node;
    node->left = rightOfLeftNode;
    node->height = 1 + (getHeight(node->left) > getHeight(node->right) ? getHeight(node->left) : getHeight(node->right));
    leftNode->height = 1 + (getHeight(leftNode->left) > getHeight(leftNode->right) ? getHeight(leftNode->left) : getHeight(leftNode->right));
    return leftNode;
}

/*
* Left Rotate: Right rotate the tree to make the tree balance
*/ 
Node *leftRotate(Node *node)
{
    if (node == NULL) return NULL;
    Node *rightNode = node->right;
    Node *leftOfRightNode = rightNode->left;
    rightNode->left = node;
    node->right = leftOfRightNode;
    node->height = 1 + (getHeight(node->left) > getHeight(node->right) ? getHeight(node->left) : getHeight(node->right));
    rightNode->height = 1 + (getHeight(rightNode->left) > getHeight(rightNode->right) ? getHeight(rightNode->left) : getHeight(rightNode->right));
    return rightNode;
}

/*
* Search: Search a for a particular value in the tree
*/
Node *search(Node *root, int key)
{
    if (!root || root->key == key)
        return root;
    return key < root->key ? search(root->left, key) : search(root->right, key);
}

/*
* Insertion: Inserting a new node to the tree
*/
Node *insert(Node *node, int key)
{
    // Insertion
    if (!node)
        return createNode(key);
    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else
        return node;

    // Get balance factor
    node->height = 1 + (getHeight(node->left) > getHeight(node->right) ? getHeight(node->left) : getHeight(node->right));
    int balance = getBalanceFactor(node);

    // Rotation
    if (balance > 1 && key < node->left->key) // RR 
        return rightRotate(node);
    if (balance < -1 && key > node->right->key) // LL
        return leftRotate(node);
    if (balance > 1 && key > node->left->key) // LR
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && key < node->right->key) // RL
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

/*
* Find Min Value: Find node with the minimum value in the tree
*/ 
Node *minValueNode(Node *node)
{
    if (node == NULL) return NULL;
    Node *current = node;
    while (current->left)
        current = current->left;
    return current;
}

// Deletion
Node *deleteNode(Node *root, int key)
{
    // Deletion
    if (!root) // base case
        return root;
    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else
    {
        if (!root->left || !root->right) // case node with 1 or 0 child
        {
            Node *temp = root->left ? root->left : root->right;
            if (!temp) // node with 0 child
            {
                temp = root;
                root = NULL;
            }
            else // node with 1 child
                *root = *temp;
            free(temp);
        }
        else // case node with 2 children: get inorder successor (smallest node) 
        {
            Node *temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = deleteNode(root->right, temp->key);
        }
    }
    
    if (!root) // Check if the tree is empty after the deletion 
        return root;

    // Rotation
    root->height = 1 + (getHeight(root->left) > getHeight(root->right) ? getHeight(root->left) : getHeight(root->right));
    int balance = getBalanceFactor(root);
    if (balance > 1 && getBalanceFactor(root->left) >= 0)
        return rightRotate(root);
    if (balance > 1 && getBalanceFactor(root->left) < 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && getBalanceFactor(root->right) <= 0)
        return leftRotate(root);
    if (balance < -1 && getBalanceFactor(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

// Free Node
void freeTree(Node *root)
{
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

// Inorder Traversal
void inOrderTraversal(Node *start)
{
    if (start == NULL) return;
    inOrderTraversal(start->left);
    printf("%d\n", start->key);
    inOrderTraversal(start->right);
}

// Print Tree Level
void printLevel(struct Node* node, int level) {
    if (node == NULL)
        return;
    if (level == 1){
        printf("%d  ", node->key);
    }
    else if (level > 1) {
        printLevel(node->left, level - 1);
        printLevel(node->right, level - 1);
    }
}
// Pretty Print AVL Tree
void prettyPrintTree(Node* root) {
    int height = getHeight(root);
    for (int i = 1; i <= height; i++) {
        printLevel(root, i);
        printf("\n");
    }
}

void printTreeInfo(Node* root){
    printf("\n");
    printf("===========================Tree Info===========================\n");
    printf("Height of AVL Tree is: %d\n", getHeight(root));
    printf("---------------------------------------------------------------\n");
    printf("Print AVL Tree by Level\n");
    prettyPrintTree(root);
    printf("---------------------------------------------------------------\n");
    printf("Print AVL Tree In-Order: \n");
    inOrderTraversal(root);
    printf("---------------------------------------------------------------\n");
    printf("Node with minimum value is: %d\n", minValueNode(root)->key);
    printf("===============================================================\n");
}

int main()
{
    Node *root = NULL;
    int nodeCount = 0;
    int numNodes;
    printf("Enter the number of nodes you would like to put in the tree: ");
    scanf("%d", &numNodes);
    printf("---------------------------------------------------------------\n");
    int capacity = numNodes + 5;
    int nodes[capacity];

    printf("Enter %d numbers\n", numNodes);
    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < numNodes; i++){
        printf("Enter number #%d: ", i + 1 );
        scanf("%d", &nodes[i]);
        printf("\n");
        root = insert(root, nodes[i]);
        nodeCount++;
    } 
    printTreeInfo(root);

    printf("Enter one more number: ");
    scanf("%d", &nodes[nodeCount]);
    printf("---------------------------------------------------------------\n");
    root = insert(root, nodes[nodeCount]);
    nodeCount++;
    printTreeInfo(root);

    printf("Enter one last number: ");
    scanf("%d", &nodes[nodeCount]);
    printf("---------------------------------------------------------------\n");
    root = insert(root, nodes[nodeCount]);
    nodeCount++;
    printTreeInfo(root);

    printf("The tree contains: ");
    for (int i = 0; i < nodeCount; i++){
        printf("%d ", nodes[i]);
    } 
    printf("\n");
    printf("---------------------------------------------------------------\n");

    int numToDelete;
    printf("Select one number from the above list to delete: ");
    scanf("%d", &numToDelete);
    printf("---------------------------------------------------------------\n");
    root = deleteNode(root, numToDelete);   
    nodeCount--;
    printTreeInfo(root);

    freeTree(root);
    return 0;
}