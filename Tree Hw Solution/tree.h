//
//  tree.h
//  tree
//
//  Created by William McCarthy on 134//20.
//  Copyright © 2020 William McCarthy. All rights reserved.
//

#ifndef tree_h
#define tree_h

//-------------------------------------------------
typedef struct inode inode;
struct inode {
  int line_num;
  inode *nextline;
};

//-------------------------------------------------
typedef struct tnode tnode;
struct tnode {
  const char* word;
  int count;
  inode* line_node;
  tnode* left;
  tnode* right;
};

//----------------------------------------------
typedef struct tree tree;
struct tree {
  tnode* root;
  size_t size;
};


//-------------------------------------------------
inode* inode_create(int linenumber);
tnode* tnode_create(const char* word, int linenumber);
tree* tree_create(void);

void inode_delete(inode* p);
void tnode_delete(tnode* p);


void tree_delete(tree* t);
void tree_clear(tree* t);

bool tree_empty(tree* t);
size_t tree_size(tree* t);

tnode* tree_add(tree* t, char* word, int linenumber);

void tree_print(tree* t, int n);    // INORDER-printing

void tree_print_preorder(tree* t);
void tree_print_postorder(tree* t);
void tree_print_levelorder(tree* t);

void tree_test(tree* t, int n);
tree* tree_from_file(int argc, const char* argv[]);

#endif /* tree_h */
