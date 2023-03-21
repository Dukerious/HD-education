#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "bBST.h"
#include "List.h"
////////////////////////////////////////////////////////////////////////
/* IMPORTANT:
   Do NOT modify the code above this line.
*/
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// Auxiliary function prototypes

// Write Auxiliary function prototypes here, and declare them as static
static void doTreeFree(Node root);
static bool DoTreeSearch(Node root, int key);
static Node newNode(int key);
static Node rotateRight(Node n1);
static Node rotateLeft(Node n2);
static Node DoTreeInsert(Node root, int key, bool *success);
static Node DoTreeDelete(Node root, int key, bool *success);
static int UpdateHeight(Node root);
static int TreeHeight(Node root);
static Node TreeReblance(Node root);
static Node joinTrees(Node t1, Node t2);
static void DoTreeToList(Node root, List retval);
static int DoTreeKthSmallest(Node root, int k);
static int SumNode(Node root);
static int DoTreeLCA(Node root, int a, int b);
static int DoTreeFloor(Node root, int key);
static bool FindSmaller(Node root, int key);
static int DoTreeCeiling(Node root, int key);
static bool FindBigger(Node root, int key);
static void DoTreeSearchBetween(Node root, int ceiling, int floor, List retval);

// static void printTree(Node root);
////////////////////////////////////////////////////////////////////////
static void doTreeFree(Node root) {
    if(root == NULL) return;
    doTreeFree(root -> left);
    doTreeFree(root -> right);
    free(root);
}

static bool DoTreeSearch(Node root, int key) {
    if(root == NULL) return false;

    if(root -> key == key) return true;
    return (root -> key > key) ? DoTreeSearch(root -> left, key) : DoTreeSearch(root -> right, key);
}

static Node newNode(int key) {
   Node new = malloc(sizeof(*new));
   if (new == NULL) {
		fprintf(stderr, "Insufficient memory!\n");
		exit(EXIT_FAILURE);
	}
   new -> left = new -> right = NULL;
   new -> key = key;
   new -> height = 0;
   return new;
}

static Node rotateRight(Node n1) {
    if(n1 == NULL || n1 -> left == NULL) {
        return n1;
    }
    Node root = n1 -> left;
    n1 -> left = root -> right;
    root -> right = n1;

    n1 -> height = UpdateHeight(n1);
    root -> height = UpdateHeight(root);
    return root;
}

static Node rotateLeft(Node n2) {
    if(n2 == NULL || n2 -> right == NULL) {
        return n2;
    }
    Node root = n2 -> right;
    n2 -> right = root -> left;
    root -> left = n2;

    n2 -> height = UpdateHeight(n2);
    root -> height = UpdateHeight(root);
    return root;
}

static Node DoTreeInsert(Node root, int key, bool *success) {
    if(root == NULL) {
        *success = true;
        return newNode(key);
    }

    if(root -> key < key) {
      root -> right = DoTreeInsert(root -> right, key, success);
    }
    else if (root -> key > key) {
      root -> left = DoTreeInsert(root -> left, key, success);
    } else return root;

    root -> height = UpdateHeight(root);

    int l = TreeHeight(root -> left);
    int r = TreeHeight(root -> right);

    if(l - r > 1) {
        if(root -> left -> key < key) {
        root -> left = rotateLeft(root -> left);
        }
        root = rotateRight(root);
    }

    else if (r - l > 1) {
        if (root -> right -> key > key) {
            root -> right = rotateRight(root->right);
        }
        root = rotateLeft(root);
    }

    return root;
}

static Node DoTreeDelete(Node root, int key, bool *success) {
    if(root == NULL) {
        *success = false;
        return root;
    }

    if(root -> key < key) {
      root -> right = DoTreeDelete(root -> right, key, success);
    }
    else if (root -> key > key) {
      root -> left = DoTreeDelete(root -> left, key, success);
    }
    else {
        Node found = root;
        root = joinTrees(root -> left, root -> right);
        free(found);
    }
    return root;
}

static Node joinTrees(Node t1, Node t2) {
   if(t1 == NULL) {
      return t2;
   }
   else if(t2 == NULL) {
      return t1;
   }
   if(t1 -> right == NULL) {
      t1 -> right = t2;
      return t1;
   }
   Node LeftMax = t1;
   Node prev = NULL;
   while (LeftMax -> right != NULL) {
      prev = LeftMax;
      LeftMax = LeftMax -> right;
   }
   prev -> right = LeftMax -> left;

   LeftMax -> left = t1;
   LeftMax -> right = t2;
   return LeftMax;
}

static Node TreeReblance(Node root) {
    if (root == NULL) return root;
    root -> left = TreeReblance(root -> left);
    root -> right = TreeReblance(root -> right);

    root -> height = UpdateHeight(root);
    int l = TreeHeight(root -> left);
    int r = TreeHeight(root -> right);
    if(l - r > 1) {
        if(root -> left -> right != NULL) {
        root -> left = rotateLeft(root -> left);
        }
        root = rotateRight(root);
    }

    else if (r - l > 1) {
        if (root -> right -> left != NULL) {
            root -> right = rotateRight(root->right);
        }
        root = rotateLeft(root);
    }
    return root;
}

static int UpdateHeight(Node root) {
    if (root == NULL) return -1;

    int right = TreeHeight(root -> right);
    int left = TreeHeight(root -> left);
    return 1 + ((right < left) ? left : right);
}

static int TreeHeight(Node root) {
   if(root == NULL) return -1;
   return root -> height;
}

static void DoTreeToList(Node root, List retval) {
    if(root == NULL) return;
    DoTreeToList(root -> left, retval);
    ListAppend(retval, root -> key);
    DoTreeToList(root -> right, retval);
}

static int DoTreeKthSmallest(Node root, int k) {
    if(root == NULL) return UNDEFINED;
    int index = SumNode(root -> left) + 1;
	if(k < index) {
		return DoTreeKthSmallest(root -> left, k);
	}
	else if(k > index) {
		return DoTreeKthSmallest(root -> right, k - index);
	}
    return root -> key;
}

static int SumNode(Node root) {
	if(root == NULL) return 0;
	return 1 + SumNode(root -> left) + SumNode(root -> right);
}

static int DoTreeLCA(Node root, int a, int b) {
    if(root == NULL) return UNDEFINED;
    int key = root -> key;
    if(root -> key == a || root -> key == b) return key;
    if(root -> key > a && root -> key < b) return key;
    if(root -> key > a && root -> key > b) return DoTreeLCA(root -> left, a, b);
    return DoTreeLCA(root -> right, a, b);
}

static int DoTreeFloor(Node root, int key) {
    if (root == NULL) return UNDEFINED;
    int rootkey = root -> key;
    if (rootkey == key) return rootkey;
    if(rootkey < key) {
        if(FindSmaller(root -> right, key))
            return DoTreeFloor(root -> right, key);
        return rootkey;
    }
    return DoTreeFloor(root -> left, key);
}

static bool FindSmaller(Node root, int key) {
    if(root == NULL) return false;
    if(root -> key <= key) return true;
    return FindSmaller(root -> left, key);
}

static int DoTreeCeiling(Node root, int key) {
    if (root == NULL) return UNDEFINED;
    int rootkey = root -> key;
    if (rootkey == key) return rootkey;
    if(rootkey > key) {
        if(FindBigger(root -> left, key))
            return DoTreeCeiling(root -> left, key);
        return rootkey;
    }
    return DoTreeCeiling(root -> right, key);
}

static bool FindBigger(Node root, int key) {
    if(root == NULL) return false;
    if(root -> key >= key) return true;
    return FindBigger(root -> right, key);
}

// static void printTree(Node root) {
//     if(root == NULL) return;
//     printf("%d\n", root -> key);
//     // printf("and the hight is %d\n", root->height);
//     printTree(root -> left);
//     printTree(root -> right);
// }

////////////////////////////////////////////////////////////////////////

/**
 * Creates a new empty tree.
 * The time complexity of this function must be O(1).
 */
Tree TreeNew(void) {
    Tree newtree = malloc(sizeof(*newtree));
    if (newtree == NULL) {
		fprintf(stderr, "Insufficient memory!\n");
		exit(EXIT_FAILURE);
	}
    newtree -> root = NULL;
    return newtree;
}

////////////////////////////////////////////////////////////////////////

/**
 * Frees all memory allocated for the given tree.
 * The time complexity of this function must be O(n).
 */
void TreeFree(Tree t) {
    if (t == NULL) return;
    Node root = t -> root;
    doTreeFree(root);
    free(t);
    return;
}
////////////////////////////////////////////////////////////////////////

/**
 * Searches the tree for a given key and returns true if the key is in
 * the tree or false otherwise.
 * The time complexity of this function must be O(log n).
 */
bool TreeSearch(Tree t, int key) {
    if(t == NULL) return false;
    return DoTreeSearch(t -> root, key);
}


////////////////////////////////////////////////////////////////////////

/**
 * Inserts the given key into the tree.
 * You must use the proper AVL insertion algorithm.
 * Returns true if the key was inserted successfully, or false if the
 * key was already present in the tree.
 * The time complexity of this function must be O(log n).
 */
bool TreeInsert(Tree t, int key) {
    if(t == NULL) return false;

    bool success = false;
    t -> root = DoTreeInsert(t -> root, key, &success);
    return success;
}

////////////////////////////////////////////////////////////////////////

/**
 * Deletes the given key from the tree if it is present.
 * You must use the proper AVL deletion algorithm.
 * Returns true if the key was deleted successfully, or false if the key
 * was not present in the tree.
 * The time complexity of this function must be O(log n).
 */
bool TreeDelete(Tree t, int key) {
    if(t == NULL) return false;

    bool success = true;
    t -> root = DoTreeDelete(t -> root, key, &success);
    t -> root = TreeReblance(t -> root);
    return success;
}

////////////////////////////////////////////////////////////////////////

/**
 * Creates a list containing all the keys in the given tree in ascending
 * order.
 * The time complexity of this function must be O(n).
 */
List TreeToList(Tree t) {
    List retval = ListNew();
    if(t == NULL) return retval;
    DoTreeToList(t -> root, retval);
    return retval;
}

////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////

/**
 * Returns the k-th smallest key in the tree.
 * Assumes that k is between 1 and the number of nodes in the tree.
 * k = 1 will return the smallest value in the tree.
 * The time complexity of this function must be O(log n + k).
 */
int TreeKthSmallest(Tree t, int k) {
    if (t == NULL) return UNDEFINED;
    return DoTreeKthSmallest(t -> root, k);
}


////////////////////////////////////////////////////////////////////////

/**
 * Returns the k-th largest key in the tree.
 * Assumes that k is between 1 and the number of nodes in the tree.
 * k = 1 will return the largest value in the tree.
 * The time complexity of this function must be O(log n + k).
 */
int TreeKthLargest(Tree t, int k) {
    if (t == NULL) return UNDEFINED;
    int SumRoot = SumNode(t -> root);
    k = SumRoot - k + 1;
    return DoTreeKthSmallest(t -> root, k);
}

////////////////////////////////////////////////////////////////////////

/**
 * Returns the least common ancestor of two keys, a and b.
 * Returns UNDEFINED if either a or b are not present in the tree.
 * a may be larger than b, or equal to each other.
 * Either a or b may be the LCA in some cases.
 * The time complexity of this function must be O(log n).
 */
int TreeLCA(Tree t, int a, int b) {
    if (t == NULL) return UNDEFINED;
    if (!TreeSearch(t, a) || !TreeSearch(t, b)) return UNDEFINED;
    if(a > b) return DoTreeLCA(t -> root, b, a);
    return DoTreeLCA(t -> root, a, b);
}

////////////////////////////////////////////////////////////////////////

/**
 * Returns the largest key less than or equal to the given value.
 * Returns UNDEFINED if there is no such key.
 * The time complexity of this function must be O(log n).
 */
int TreeFloor(Tree t, int key) {
    if (t == NULL) return UNDEFINED;
    return DoTreeFloor(t -> root, key);
}

////////////////////////////////////////////////////////////////////////

/**
 * Returns the smallest key greater than or equal to the given value.
 * Returns UNDEFINED if there is no such key.
 * The time complexity of this function must be O(log n).
 */
int TreeCeiling(Tree t, int key) {
    if (t == NULL) return UNDEFINED;
    return DoTreeCeiling(t -> root, key);
}

////////////////////////////////////////////////////////////////////////

/**
 * Searches for all keys between the two given keys (inclusive) and
 * returns the keys in a list in ascending order.
 * Assumes that lower is less than or equal to upper.
 * The time complexity of this function must be O(log n + m), where m is
 * the length of the returned list.
 */
List TreeSearchBetween(Tree t, int lower, int upper) {
    List retval = ListNew();
    if(t == NULL) return retval;
    int ceiling = TreeCeiling(t, lower);
    int floor = TreeFloor(t, upper);
    DoTreeSearchBetween(t -> root, ceiling, floor, retval);
    return retval;
}

static void DoTreeSearchBetween(Node root, int ceiling, int floor, List retval) {
    if(root == NULL) return;
    int rootkey = root -> key;
    DoTreeSearchBetween(root -> left, ceiling, floor, retval);
    if(ceiling <= rootkey && rootkey <= floor) ListAppend(retval, rootkey);
    DoTreeSearchBetween(root -> right, ceiling, floor, retval);
}

////////////////////////////////////////////////////////////////////////

