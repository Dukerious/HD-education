// Binary Search Tree ADT implementation ... 

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "BSTree.h"

#define data(tree)  ((tree)->data)
#define left(tree)  ((tree)->left)
#define right(tree) ((tree)->right)
void printroot(Tree t, int depth);


typedef struct Node *Tree;
typedef struct Node {
   int  data;
   Tree left, right;
} Node;


// make a new node containing data
Tree newNode(Item it) {
   Tree new = malloc(sizeof(Node));
   assert(new != NULL);
   data(new) = it;
   left(new) = right(new) = NULL;
   return new;
}

// create a new empty Tree
Tree newTree() {
   return NULL;
}

// free memory associated with Tree
void freeTree(Tree t) {
   if (t != NULL) {
      freeTree(left(t));
      freeTree(right(t));
      free(t);
   }
}

// display Tree sideways
void showTreeR(Tree t, int depth) {
   if(t == NULL) {
      return;
   }
   showTreeR(t -> right, depth + 1);
   printroot(t, depth);
   showTreeR(t -> left, depth + 1);
}
void printroot(Tree t, int depth) {
   for(int i = 0; i < depth; i++) {
      printf("\t");
   }
   printf("%d\n", t -> data);
}
void showTree(Tree t) {
   showTreeR(t, 0);
}
// compute height of Tree
int TreeHeight(Tree t) {
   if(t == NULL) {
      return -1;
   }
   int right = TreeHeight(t -> right);
   int left = TreeHeight(t -> left);
   return 1 + ((right < left) ? left : right);
}

// count #nodes in Tree
int TreeNumNodes(Tree t) {
   // base case 
   if (t == NULL) {
      return 0;
   }
   //normal case
   return 1 + TreeNumNodes(t -> left) + TreeNumNodes(t -> right);
}
int TreeNumOdds(Tree t) {
   if (t == NULL) {
      return 0;
   }
   return TreeNumOdds(t -> left) + TreeNumOdds(t -> right) + ((t -> data & 1) == 1 ? 1 : 0);
}
// check whether a key is in a Tree
bool TreeSearch(Tree t, Item it) {
  if ( t == NULL) {
      return false;
  }
  if (t -> data < it) {
   return TreeSearch(t -> right, it);
  }
  if (t -> data > it) {
   return TreeSearch(t -> left, it);
  }
  return true;
}

// insert a new item into a Tree
Tree TreeInsert(Tree t, Item it) {
   if (t == NULL) {
      return newNode(it);
   }
   if (t -> data < it) {
      t -> right = TreeInsert(t->right, it);
   }
   if (t -> data > it) {
      t -> left = TreeInsert(t -> left,it);
   }
   return t;
}

Tree joinTrees(Tree t1, Tree t2) {
  if (t1 == NULL) return t2;
  else if (t2 == NULL) return t1;

   Tree leftmax = t1;
   // prev == previous one for the leftmax
   Tree prev = NULL;
   while(leftmax -> right != NULL) {
      prev = leftmax;
      leftmax = leftmax -> right;
   }

   prev -> right = leftmax -> left;
   leftmax -> left = t1;
   leftmax -> right = t2;


}

/////////////////////////////
Tree TreeDelete(Tree t, Item it) {
   if (t == NULL) {
      return t;
   }
   if (t -> data < it) {
      t -> right = TreeDelete(t -> right, it);
   }
   else if (t -> data > it) {
      t -> left = TreeDelete(t -> left, it);
   }
   else {
      Tree found = t;
      t = joinTrees(t -> left, t -< right);
      free(found)
   }
   return t;
}
///////////////////////////////////
Tree rotateRight(Tree n1) {
   if(n1 == NULL || n1 -> left == NULL) {
      return n1;
   }
   Tree root = n1 -> left;
   n1 -> left = root -> right;
   root -> right = n1;
   return root;
}

Tree rotateLeft(Tree n2) {
   if(n2 == NULL || n2 -> right == NULL) {
      return n2;
   }
   Tree root = n2 -> right;
   n2 -> right = root -> left;
   root -> left = n2;
   return root;
}

Tree insertAtRoot(Tree t, Item it) {
  if (t == NULL) {
   return newNode(it);
  }
   if (t -> data < it) {
      t -> right = insertAtRoot(t->right, it);
      t = rotateLeft(t);
   }
   if (t -> data > it) {
      t -> left = insertAtRoot(t -> left, it);
      t = rotateRight(t);
   }
   return t;

}

Tree partition(Tree t, int i) {
  if(t == NULL) return t;
  assert(i >= 0 && i < TreeNumNodes(t));
  int num = TreeNumNodes( t -> left);
  if ( i < num) {
   t -> left = partition( t -> left, i);
   t = rotateRight(t);
  }
  if ( i > num) {
   t -> right = partition(t -> right, i - num - 1);
  }
  return t;
}

Tree rebalance(Tree t) {
  int n = TreeNumNodes(t);
  if (n >= 3) {
   t = partition(t, n / 2);
  }
  t -> left = rebalance(t -> left);
  t -> right = rebalance(t -> right);
}


Tree InsertAVL(Tree t, int val) {
   if (t == NULL) {
      return newNode(val);
   }
   if (t -> data < val) {
      t -> left = InsertAVL(t-> right, val);
   }
   else if ( t-> data >val) {
      t -> right = InsertAVL(t-> left, val);
   }
   int l = TreeHeight(t -> left);
   int r = TreeHeight(t -> right);
   if(l - r > 1) {
      if(t -> left -> data < val) {
         t -> left = rotateLeft(t->left);
      }
      t = rotateRight(t);
   }
   else if (r - l > 1) {
       if(t -> left -> data < val) {
         t -> left = rotateRight(t->right);
      }
      t = rotateLeft(t);
   }

   return t;
}