#ifndef MY_LIST_H
#define MY_LIST_H
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  char* string;
  struct node* next;

} node;

void list_init(node* head, char* value);
void add_elem(node* head, char* string, int count);
char* pop_elem(node* node);
void print_list(node* head);
void free_list(node* head);
void deep_free_list(node* head);

#endif