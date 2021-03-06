#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <tchar.h>

typedef enum color { red = 1, black } color;

struct treeNode {
	struct treeNode *left;
	struct treeNode *right;
	struct treeNode *parent;

	int data;
	color col;
};

typedef struct treeNode TreeNode;
typedef TreeNode *TreeNodePtr;


TreeNodePtr node(int data);
TreeNodePtr nilnode();
TreeNodePtr nil;

TreeNodePtr parentNode(TreeNodePtr nodePtr);
TreeNodePtr uncleNode(TreeNodePtr nodePtr);
TreeNodePtr siblingNode(TreeNodePtr nodePtr);
TreeNodePtr grandNode(TreeNodePtr nodePtr);

TreeNodePtr tree_minimum(TreeNodePtr x);
TreeNodePtr tree_maximum(TreeNodePtr x);
void transplant(TreeNodePtr *rootPtr, TreeNodePtr u, TreeNodePtr v);

void left_rotate(TreeNodePtr *rootPtr, TreeNodePtr x);
void right_rotate(TreeNodePtr *rootPtr, TreeNodePtr y);

void rb_insert(TreeNodePtr *rootPtr, TreeNodePtr z);
void rb_insert_fixup(TreeNodePtr *rootPtr, TreeNodePtr z);
bool rb_delete(TreeNodePtr *rootPtr, TreeNodePtr z);

void rb_delete_fixup(TreeNodePtr *rootPtr, TreeNodePtr x);

int tree_total(TreeNodePtr rootPtr);
int tree_black_total(TreeNodePtr rootPtr);
int tree_black_height(TreeNodePtr rootPtr);

color get_color(TreeNodePtr nodePtr);
int get_data(TreeNodePtr nodePtr);

void inOrder(TreeNodePtr rootPtr);
void bst_print(TreeNodePtr rootPtr, int level);

void setnil();

void printLevelOrder(TreeNodePtr rootPtr);
void printGivenLevel(TreeNodePtr rootPtr, int level);
int height(TreeNodePtr node);

TreeNodePtr node_exist(TreeNodePtr rootPtr, int value);

TreeNodePtr successor(TreeNodePtr rootPtr,TreeNodePtr target);
TreeNodePtr predecessor(TreeNodePtr rootPtr,TreeNodePtr target);

void copy_tree(TreeNodePtr rootPtr, TreeNodePtr *copyPtr);
bool is_exist(TreeNodePtr rootPtr, TreeNodePtr x);

int main(int argc, char* argv[])
{
	srand(time(NULL));
	TreeNodePtr rootPtr = NULL;
	TreeNodePtr ptr = NULL;
	nil = nilnode();
	setnil();
	FILE *fp_input = fopen("input.txt", "r");
	FILE *fp_output = fopen("output.txt", "w");
	FILE *fp_search = fopen("search.txt", "r");

	if (fp_input == NULL || fp_output == NULL || fp_search == NULL)
	{
		printf("file open error\n");
		return -1;
	}

	int data;

	while (fscanf(fp_input, "%d", &data) != EOF)
	{
		if (data == 0)
			break;
		else if (data > 0)
		{
			rb_insert(&rootPtr, node(data));
		}
		else
		{
			rb_delete(&rootPtr, node(abs(data)));
		}
	}

	while (fscanf(fp_search, "%d", &data) != EOF)
	{
		char temp1[100] = { 0 };
		char temp2[100] = { 0 };
		char temp3[100] = { 0 };
		if (data == 0)
		{
			fprintf(fp_output,"0");
			break;
		}
		fprintf(fp_output,"%s %s %s\n", 
			predecessor(rootPtr,node_exist(rootPtr, data))->data !=0? itoa(predecessor(rootPtr, node_exist(rootPtr, data))->data,temp1,10):"NIL",
			is_exist(rootPtr, node(data))?itoa(data,temp2,10):"NIL",
			successor(rootPtr,node_exist(rootPtr, data))->data!=0?itoa(successor(rootPtr, node_exist(rootPtr, data))->data,temp3,10):"NIL");
	}

	fclose(fp_input);
	fclose(fp_output);
	fclose(fp_search);

	return;
}
void printLevelOrder(TreeNodePtr rootPtr)
{
	int h = height(rootPtr);
	int i;
	for (i = 1; i <= h; i++)
		printGivenLevel(rootPtr, i);
}
void printGivenLevel(TreeNodePtr rootPtr, int level)
{
	if (rootPtr == nil || rootPtr == NULL)
		return;
	if (level == 1)
		printf("%d[%s]\n", rootPtr->data, rootPtr->col == red ? "R" : "B");
	else if (level > 1)
	{
		printGivenLevel(rootPtr->left, level - 1);
		printGivenLevel(rootPtr->right, level - 1);
	}
}
int height(TreeNodePtr node)
{
	if (node == nil)
		return 0;
	else
	{
		int lheight = height(node->left);
		int rheight = height(node->right);

		if (lheight > rheight)
			return(lheight + 1);
		else return(rheight + 1);
	}
}
bool is_exist(TreeNodePtr rootPtr, TreeNodePtr x)
{
	TreeNodePtr tempPtr = rootPtr;
	while (tempPtr != nil && x->data != tempPtr->data)
	{
		if (tempPtr->data > x->data)
			tempPtr = tempPtr->left;
		else
			tempPtr = tempPtr->right;
	}

	return tempPtr != nil;
}


TreeNodePtr node_exist(TreeNodePtr rootPtr, int value)
{
	TreeNodePtr findPtr = rootPtr;
	while (findPtr != nil && findPtr->data != value)
	{
		if (findPtr->data > value)
			findPtr = findPtr->left;
		else
			findPtr = findPtr->right;
	}
	if (findPtr == nil)
		return node(value);
	else
		return findPtr;
}

TreeNodePtr successor(TreeNodePtr rootPtr, TreeNodePtr target)
{
	if (target->parent == nil && target->col == red)
	{
		TreeNodePtr copyPtr;
		copy_tree(rootPtr, &copyPtr);

		rb_insert(&copyPtr, target);

		TreeNodePtr tempPtr = successor(copyPtr, target);

		return node_exist(rootPtr, tempPtr->data);
	}

	if (tree_maximum(rootPtr) == target)
		return nil;

	if (target->right != nil)
		return tree_minimum(target->right);

	TreeNodePtr temp = target->parent;

	while (temp != nil && target == temp->right)
	{
		target = temp;
		temp = temp->parent;
	}
	return temp;
}

TreeNodePtr predecessor(TreeNodePtr rootPtr, TreeNodePtr target)
{
	if (target->parent == nil && target->col == red)
	{
		TreeNodePtr copyPtr;
		copy_tree(rootPtr, &copyPtr);

		rb_insert(&copyPtr, target);

		TreeNodePtr tempPtr = predecessor(copyPtr, target);

		return node_exist(rootPtr, tempPtr->data);
	}

	if (tree_minimum(rootPtr) == target)
		return nil;
	
	if (target->left != nil)
		return tree_maximum(target->left);



	TreeNodePtr temp = target->parent;

	while (temp != nil && target == temp->left)
	{
		target = temp;
		temp = temp->parent;
	}
	return temp;
}

void copy_tree(TreeNodePtr rootPtr, TreeNodePtr *copyPtr)
{
	(*copyPtr) = node(rootPtr->data);
	(*copyPtr)->col = rootPtr->col;
	(*copyPtr)->parent = rootPtr->parent;
	if (rootPtr != nil)
	{
		(*copyPtr)->left = node(rootPtr->left);
		(*copyPtr)->left->col = rootPtr->left->col;
		(*copyPtr)->right = node(rootPtr->right);
		(*copyPtr)->right->col = rootPtr->right->col;
		copy_tree(rootPtr->left, &((*copyPtr)->left));
		copy_tree(rootPtr->right, &((*copyPtr)->right));
	}

}

void bst_print(TreeNodePtr rootPtr, int level)
{
	if (rootPtr != rootPtr->right && rootPtr->right != NULL)
		bst_print(rootPtr->right, level + 1);
	for (int i = 0; i < level; i++)
		printf("	");
	printf("%d[%s]\n", rootPtr->data, rootPtr->col == red ? "R" : "B");
	if (rootPtr != rootPtr->left&& rootPtr->left != NULL)
		bst_print(rootPtr->left, level + 1);
}
int tree_total(TreeNodePtr rootPtr)
{
	int count = 0;
	if (rootPtr != NULL && rootPtr != nil)
	{
		count += tree_total(rootPtr->left);
		count++;
		count += tree_total(rootPtr->right);
	}
	return count;
}
int tree_black_total(TreeNodePtr rootPtr)
{
	int count = 0;
	if (rootPtr != NULL && rootPtr != nil)
	{
		count += tree_black_total(rootPtr->left);
		if (get_color(rootPtr) == black)count++;
		count += tree_black_total(rootPtr->right);
	}
	return count;
}
int tree_black_height(TreeNodePtr rootPtr)
{
	TreeNodePtr x = rootPtr;
	int count = 0;
	int i;
	while (x != nil)
	{
		i = rand() % 2;
		x = i ? x->left : x->right;
		if (x->col == black)
			count++;
	}
	return count;
}
void inOrder(TreeNodePtr rootPtr)
{
	if (rootPtr != NULL && rootPtr != nil)
	{
		inOrder(rootPtr->left);
		printf("%d %s\n", get_data(rootPtr), get_color(rootPtr) == red ? "R" : "B");
		inOrder(rootPtr->right);
	}
}
TreeNodePtr node(int value)
{
	if (value == 0)
		return nil;
	TreeNodePtr Ptr = NULL;
	TreeNodePtr *nodePtr = &Ptr;
	*nodePtr = malloc(sizeof(TreeNode));
	if (*nodePtr != NULL) {
		(*nodePtr)->data = value;
		(*nodePtr)->col = red;
		(*nodePtr)->parent = nil;
		(*nodePtr)->left = nil;
		(*nodePtr)->right = nil;
	}

	return (*nodePtr);
}
TreeNodePtr nilnode()
{
	TreeNodePtr Ptr = NULL;
	TreeNodePtr *nodePtr = &Ptr;
	*nodePtr = malloc(sizeof(TreeNode));
	if (*nodePtr != NULL) {
		(*nodePtr)->data = NULL;
		(*nodePtr)->col = black;
		(*nodePtr)->parent = nil;
		(*nodePtr)->left = nil;
		(*nodePtr)->right = nil;
	}
	return (*nodePtr);
}
void left_rotate(TreeNodePtr *rootPtr, TreeNodePtr x)
{
	TreeNodePtr y = x->right;
	x->right = y->left;
	if (y->left != nil)
		y->left->parent = x;
	y->parent = parentNode(x);
	if (parentNode(x) == NULL || parentNode(x) == nil)
		(*rootPtr) = y;
	else if (x == x->parent->left)
		parentNode(x)->left = y;
	else
		parentNode(x)->right = y;
	y->left = x;
	x->parent = y;
}
void right_rotate(TreeNodePtr *rootPtr, TreeNodePtr y)
{
	TreeNodePtr x = y->left;
	y->left = x->right;
	if (x->right != nil)
		x->right->parent = y;
	x->parent = parentNode(y);
	if (parentNode(y) == NULL || parentNode(y) == nil)
		(*rootPtr) = x;
	else if (y == parentNode(y)->left)
		parentNode(y)->left = x;
	else
		parentNode(y)->right = x;
	x->right = y;
	y->parent = x;
}
color get_color(TreeNodePtr nodePtr)
{
	if (nodePtr != NULL)
		return nodePtr->col;
	else
		return 0;
}
int get_data(TreeNodePtr nodePtr)
{
	if (nodePtr != NULL&& nodePtr != nil)
		return nodePtr->data;
	else
		return NULL;
}
TreeNodePtr parentNode(TreeNodePtr nodePtr)
{
	if (nodePtr != NULL)
		return nodePtr->parent;
	else return nil;
}
TreeNodePtr siblingNode(TreeNodePtr nodePtr)
{
	if (nodePtr == NULL)
		return nil;
	else if (nodePtr->parent != NULL && nodePtr->parent != nil)
	{
		if (parentNode(nodePtr)->left == nodePtr) return parentNode(nodePtr)->right;
		else return parentNode(nodePtr)->left;
	}
	else
	{
		return nil;
	}
}
TreeNodePtr uncleNode(TreeNodePtr nodePtr)
{
	if (nodePtr != NULL)	return siblingNode(nodePtr->parent);
	else return nil;
}
TreeNodePtr grandNode(TreeNodePtr nodePtr)
{
	if (nodePtr != NULL)	return nodePtr->parent->parent;
	else return nil;
}
void rb_insert(TreeNodePtr *rootPtr, TreeNodePtr z)
{
	TreeNodePtr y = nil;
	TreeNodePtr x = (*rootPtr);

	while (x != nil && x != NULL)
	{
		y = x;
		if (get_data(z) < get_data(x))
			x = x->left;
		else
			x = x->right;
	}
	z->parent = y;
	if (y == nil)
		(*rootPtr) = z;
	else if (get_data(z) < get_data(y))
		y->left = z;
	else
		y->right = z;
	z->col = red;
	rb_insert_fixup(rootPtr, z);
}
void rb_insert_fixup(TreeNodePtr *rootPtr, TreeNodePtr z)
{

	TreeNodePtr y = nil;
	while (get_color(parentNode(z)) == red)
	{
		if (parentNode(z) == grandNode(z)->left)
		{
			y = grandNode(z)->right;
			if (y != NULL && get_color(y) == red)
			{
				parentNode(z)->col = black;
				y->col = black;
				grandNode(z)->col = red;
				z = grandNode(z);
			}
			else
			{
				if (z == parentNode(z)->right)
				{
					z = parentNode(z);
					left_rotate(rootPtr, z);
				}
				parentNode(z)->col = black;
				grandNode(z)->col = red;
				right_rotate(rootPtr, grandNode(z));
			}
		}
		else
		{
			y = grandNode(z)->left;
			if (y != NULL && get_color(y) == red)
			{
				parentNode(z)->col = black;
				y->col = black;
				grandNode(z)->col = red;
				z = grandNode(z);
			}
			else
			{
				if (z == parentNode(z)->left)
				{
					z = parentNode(z);
					right_rotate(rootPtr, z);
				}
				parentNode(z)->col = black;
				grandNode(z)->col = red;
				left_rotate(rootPtr, grandNode(z));
			}
		}
	}
	(*rootPtr)->col = black;
}
void transplant(TreeNodePtr *rootPtr, TreeNodePtr u, TreeNodePtr v)
{
	if (parentNode(u) == nil)
		(*rootPtr) = v;
	else if (u == parentNode(u)->left)
		parentNode(u)->left = v;
	else
		parentNode(u)->right = v;

	v->parent = parentNode(u);
}
TreeNodePtr tree_minimum(TreeNodePtr x)
{
	TreeNodePtr currentPtr = x;

	while (currentPtr->left != nil)
		currentPtr = currentPtr->left;
	return currentPtr;
}
TreeNodePtr tree_maximum(TreeNodePtr x)
{
	TreeNodePtr currentPtr = x;

	while (currentPtr->right != nil)
		currentPtr = currentPtr->right;
	return currentPtr;
}
bool rb_delete(TreeNodePtr *rootPtr, TreeNodePtr z)
{
	//	setnil();
	TreeNodePtr x = nil;
	TreeNodePtr delNode = (*rootPtr);

	while (get_data(z) != get_data(delNode))
	{
		if (delNode == nil || delNode == NULL)
		{
			return false;
		}
		if (get_data(delNode) > get_data(z))
		{
			delNode = delNode->left;
		}
		else
		{
			delNode = delNode->right;
		}
	}
	TreeNodePtr y = delNode;
	color y_origin = get_color(y);

	if (delNode->left == nil)
	{
		x = delNode->right;
		transplant(rootPtr, delNode, delNode->right);
	}
	else if (delNode->right == nil)
	{
		x = delNode->left;
		transplant(rootPtr, delNode, delNode->left);
	}
	else
	{
		y = tree_minimum(delNode->right);
		y_origin = get_color(y);
		x = y->right;

		if (parentNode(y) == delNode)
			x->parent = y;
		else
		{
			transplant(rootPtr, y, y->right);
			y->right = delNode->right;
			y->right->parent = y;
		}

		transplant(rootPtr, delNode, y);
		y->left = delNode->left;
		y->left->parent = y;
		y->col = get_color(delNode);

	}
	if (y_origin == black)
		rb_delete_fixup(rootPtr, x);

	free(delNode);
	return true;
}
void rb_delete_fixup(TreeNodePtr *rootPtr, TreeNodePtr x)
{

	TreeNodePtr w = nil;
	while (x != (*rootPtr) && get_color(x) == black)
	{
		if (x == parentNode(x)->left)
		{
			w = parentNode(x)->right;
			if (get_color(w) == red)
			{
				w->col = black;
				parentNode(x)->col = red;
				left_rotate(rootPtr, parentNode(x));
				w = parentNode(x)->right;
			}
			if (get_color(w->left) == black&&get_color(w->right) == black)
			{
				w->col = red;
				x = parentNode(x);
			}
			else
			{
				if (get_color(w->right) == black)
				{
					w->left->col = black;
					w->col = red;
					right_rotate(rootPtr, w);
					w = parentNode(x)->right;
				}

				w->col = get_color(parentNode(x));
				parentNode(x)->col = black;
				w->right->col = black;
				left_rotate(rootPtr, parentNode(x));
				x = (*rootPtr);
			}
		}
		else
		{
			w = parentNode(x)->left;
			if (get_color(w) == red)
			{
				w->col = black;
				parentNode(x)->col = red;
				right_rotate(rootPtr, parentNode(x));
				w = parentNode(x)->left;
			}
			if (get_color(w->right) == black&&get_color(w->left) == black)
			{
				w->col = red;
				x = parentNode(x);
			}
			else
			{
				if (get_color(w->left) == black)
				{
					w->right->col = black;
					w->col = red;
					left_rotate(rootPtr, w);
					w = parentNode(x)->left;
				}
				w->col = get_color(parentNode(x));
				parentNode(x)->col = black;
				w->left->col = black;
				right_rotate(rootPtr, parentNode(x));
				x = (*rootPtr);
			}
		}
	}
	x->col = black;
}
void setnil()
{
	nil->col = black;
	nil->left = nil;
	nil->right = nil;
	nil->parent = nil;
	nil->data = NULL;
}