#ifndef BST_H
#define BST_H

struct BST_Node{
	struct BST_Node *left_child;
	struct BST_Node *right_child;
	int key;
	void *data;
};

// Returns BST_Node of the node who key matches the search_key
// Returns NULL if search_key is not found in the BST
struct BST_Node *bst_search( struct BST_Node *root, int search_key );

// Adds the given <key, data> pair to the BST
// Returns BST_SUCCESS upon success, BST_DUP_KEY if duplicate key is being sent
// Note that the ADDRESS of the root (instead of root) should be sent as first paraemter
// For example:
// BST_Node *root;
// int ststus = bst_add_node (&root, somekey, somedata);

int bst_add_node( struct BST_Node **root, int key, void *data );

// Prints the keys of the BST
void bst_print( struct BST_Node *root );

// Releases the entire BST by recursively calling free()
void bst_free( struct BST_Node *root );

// Frees not only the nodes but also the data in the nodes
void bst_destroy( struct BST_Node *root );

#define BST_SUCCESS 0
#define BST_DUP_KEY 1
#define BST_NULL 2

#endif
