//  Albert Paez
//  tree.c
//  tree
//
//  Created by William McCarthy on 134//20.
//  Copyright © 2020 William McCarthy. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "tree.h"
#include "utils.h"

//================================================
//Exercise 6-3
inode* inode_create(int linenumber) {
  inode* p = (inode*)malloc(sizeof(inode));
  p->line_num = linenumber;
  p->nextline = NULL;
  return p;
}

//================================================
//Exercise 6-3
void inode_delete(inode* p) {
  free((void*)p->line_num);
  free(p);
}

//================================================
//Exercise 6-3
void inode_append(inode* p, int linenumber) {
  p->nextline = inode_create(linenumber);
  
}
//================================================
tnode* tnode_create(const char* word, int linenumber) {
  tnode* p = (tnode*)malloc(sizeof(tnode));
  p->word = strdup(word);    // copy of word allocated on heap
  p->count = 1;
  p->line_node = inode_create(linenumber);
  p->left = NULL;
  p->right = NULL;

  
  return p;
}

//====================================================================
void tnode_delete(tnode* p) {
    free((void*)p->word);
    free(p);
}

//====================================================================
tree* tree_create(void) {
  tree* p = (tree*)malloc(sizeof(tree));
  p->root = NULL;
  p->size = 0;
  
  return p;
}

//====================================================================
static void tree_deletenodes(tree* t, tnode* p) {
  if (p == NULL) { return; }
  
  tree_deletenodes(t, p->left);
  tree_deletenodes(t, p->right);
  tnode_delete(p);
  t->size--;
}

//====================================================================
void tree_clear(tree* t) {
  tree_delete(t);
  t->root = NULL;
  t->size = 0;
}

//====================================================================
void tree_delete(tree* t) { tree_deletenodes(t, t->root); }

//====================================================================
bool tree_empty(tree* t)  { return t->size == 0; }

//====================================================================
size_t tree_size(tree* t) { return t->size; }

//====================================================================
static tnode* tree_addnode(tree* t, tnode** p, const char* word, int linenumber) {
  int compare;
  
  if (*p == NULL) {
    *p = tnode_create(word, linenumber);
  } else if ((compare = strcmp(word, (*p)->word)) == 0) {
    (*p)->count++;
    inode_append((*p)->line_node, linenumber);
  } else if (compare < 0) { tree_addnode(t, &(*p)->left, word,linenumber);
  } else {
    tree_addnode(t, &(*p)->right, word, linenumber);
  }
  

  return *p;
}

//====================================================================

static char* str_process(char* s, char* t) {
  char* p = s;
  char ignore[] = "\'\".,;;?!()/’";
  while (*t != '\0') {
    if (strchr(ignore, *t) == NULL || (*t == '\'' && (p != s || p != s + strlen(s) - 1))) {
      *p++ = tolower(*t);
    }
    ++t;
  }
  *p++ = '\0';
  return s;
}

//====================================================================
tnode* tree_add(tree* t, char* word, int linenumber) {
  char buf[100];
  
  if (word == NULL) { return NULL; }
  str_process(buf, word);
  
  tnode* p = tree_addnode(t, &(t->root), buf, linenumber);
  t->size++;

  return p;
}



//====================================================================
//Exercise 6-3
static void tree_printme(tree* t, tnode* p) {
  if (p->count > 1) { printf("%5d -- ", p->count); }
  else {
    printf("         ");
  }
  printf("%-18s", p->word);
  inode* plines = p->line_node;
  while (plines != NULL) { 
    printf("%d", plines->line_num);
    if (plines->nextline != NULL) {
      printf(", ");
    }
     plines = plines->nextline;
  }
 
  printf("\n");
}
//====================================================================
//Exercise 6-2
static void tree_printalpha(tree*t, tnode* p, int n) {
  int i;
  static char prev[BUFSIZ];
  static bool firsttime = true;
  // const char* noisewords[BUFSIZ] = {"the at is i and then"};
  if (firsttime) {
    memset(prev, 0, sizeof(prev));
    strcpy(prev, p->word);
    firsttime = false;
  }
  if (strlen(p->word) > 2) {
    if (strncmp(prev, p->word, n) != 0) { printf("\n");}
    strcpy(prev, p->word);
    printf("%s(%d) ", p->word, p->count);
    
  }
}
//====================================================================
//Exercise 6-2
static void tree_printnodes_n(tree* t, tnode* p, int n) { 
  if (p == NULL) { return; }

  tree_printnodes_n(t, p->left, n);
  tree_printalpha(t,p, n);
  tree_printnodes_n(t, p->right, n);
}
//===================================================================
//Exercise 6-4
int compare_tnode(const void *elem1, const void *elem2) {
    return (((tnode*) elem2)->count - ((tnode*) elem1)->count);
}
//===================================================================
//Exercise 6-4
int count_tree_elements(tnode *p) {
    int elements = 1;
    if (p == NULL) { return 0; }

    elements += count_tree_elements(p->left);
    elements += count_tree_elements(p->right);
    return elements;
}
//===================================================================
//Exercise 6-4
int copy_tree_to_array(tnode tarray[], int count, tnode *p) {
    if (p == NULL) { return count; }

    tarray[count++] = *p;
    if (p->left != NULL)
        count = copy_tree_to_array(tarray, count, p->left);
    if (p->right != NULL)
        count = copy_tree_to_array(tarray, count, p->right);

    return count;
}
//===================================================================
//Exercise 6-4
static void tree_printnodes_freq(tree* t, tnode* p) {
  int i, tsize = count_tree_elements(p);
  tnode tnode_array[tsize];
  
  copy_tree_to_array(tnode_array, 0, p);
  qsort(tnode_array, tsize, sizeof(tnode), compare_tnode);
  for (i = 0; i < tsize; ++i)
      printf("%5d -- %-18s\n", tnode_array[i].count, tnode_array[i].word);


}
//====================================================================
static void tree_printnodes(tree* t, tnode* p) {
  if (p == NULL) { return; }

  tree_printnodes(t, p->left);
  tree_printme(t,p);
  tree_printnodes(t, p->right);
}

//====================================================================
static void tree_printnodes_preorder(tree* t, tnode* p) {
  if (p == NULL) { return; }
  
  tree_printme(t, p);
  tree_printnodes(t, p->left);
  tree_printnodes(t, p->right);
}

//====================================================================
static void tree_printnodes_postorder(tree* t, tnode* p) {
  if (p == NULL) { return; }
  
  tree_printnodes_postorder(t, p->left);
  tree_printnodes_postorder(t, p->right);
  tree_printme(t, p);
}

//====================================================================
static void tree_printnodes_reverseorder(tree* t, tnode* p) {
  if (p == NULL) { return; }
  
  tree_printnodes_reverseorder(t, p->right);
  tree_printme(t, p);
  tree_printnodes_reverseorder(t, p->left);
}

//====================================================================
//void tree_print_levelorder(tree* t);

//====================================================================
void tree_print(tree* t, int n)       { tree_printnodes_n(t, t->root, n);          printf("\n"); }

//====================================================================
void tree_print_inorder(tree* t)      { tree_printnodes(t, t->root);               printf("\n"); }
//====================================================================
void tree_print_preorder(tree* t)     { tree_printnodes_preorder(t, t->root);      printf("\n"); }

//====================================================================
void tree_print_postorder(tree* t)    { tree_printnodes_postorder(t, t->root);     printf("\n"); }

//====================================================================
void tree_print_reverseorder(tree* t) { tree_printnodes_reverseorder(t, t->root);  printf("\n"); }

//====================================================================
//Exericse 6-4
void tree_print_freqorder(tree* t)    {tree_printnodes_freq(t, t->root);           printf("\n"); }

void tree_test(tree* t, int n) {
  printf("=============== TREE TEST =================================\n");
  printf("\n\nprinting grouped variable order...========================================\n");
  tree_print(t, n);
  printf("end of printing grouped variable order...=====================================\n\n");

  printf("\n\nprinting in order...================================\n");
  tree_print_inorder(t);
  printf("end of printing in order...=============================\n\n");

  printf("\n\nprinting frequency order...========================================\n");
  tree_print_freqorder(t);
  printf("end of printing frequency order...=====================================\n\n");

  printf("tree size is: %zu\n", tree_size(t));
  printf("clearing tree...\n");
  tree_clear(t);
  printf("after clearing tree, size is: %zu\n", tree_size(t));
  tree_print(t, n);
  
  printf("=============== END OF TREE TEST ==========================\n");
}

//====================================================================
tree* tree_from_file(int argc, const char* argv[]) {
  if (argc < 2) { return NULL; }

  FILE* fin;
  const char* filename = argv[1];
  if ((fin = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "Could not open file: '%s'\n", filename);
    exit(1);
  }

  char buf[BUFSIZ];
  char delims[] = " \n";
  int size = 0, linenumber = 1;
  memset(buf, 0, sizeof(buf));

  tree* t = tree_create();
  while (fgets(buf, BUFSIZ, fin)) {
    char* word = strtok(buf, delims);
    tree_add(t, word, linenumber);

    while ((word = strtok(NULL, delims)) != NULL) { 
      tree_add(t, word, linenumber);
      
    }
    linenumber++;
  }
  printf("%d words added...\n", size);
  fclose(fin);

  return t;
}


//====================================================================
int main(int argc, const char* argv[]) {
  tree* t = tree_from_file(argc, argv);
  if (argv[2] == 0) { argv[2] = "3"; }
  if (t != NULL) { tree_test(t, atoi(argv[2]));  tree_delete(t);  return 0; }
  
  char buf[BUFSIZ];
  char delims[] = " .,;?!\'\"()\n";
  int size = 0;
  int linenumber = 1;
  memset(buf, 0, sizeof(buf));
  

  t = tree_create();
  while (fgets(buf, BUFSIZ, stdin)) {
    char* word = strtok(buf, delims);
    tree_add(t, word, linenumber);

    while ((word = strtok(NULL, delims)) != NULL) {
      tree_add(t, word, linenumber);
    }
  }
  printf("%d words added...\n", size);

  tree_test(t,atoi(argv[2]));
  tree_delete(t);
  
  return 0;
}

