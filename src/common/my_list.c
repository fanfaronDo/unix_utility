#include "my_list.h"

void list_init(node* head, char* value) {
  head->next = NULL;
  head->string = value;
}
void add_elem(node* head, char* string, int count) {
  if (count == 0)
    list_init(head, string);
  else {
    node* current = head;
    while (current->next != NULL) current = current->next;
    current->next = malloc(sizeof(node));
    current->next->string = string;
    current->next->next = NULL;
  }
}
void deep_free_list(node* head) {
  while (head->next != NULL) {
    node* current = head;
    head = current->next;
    free(current->string);
    free(current);
  }
  free(head->string);
  free(head);
}
void free_list(node* head) {
  while (head->next != NULL) {
    node* current = head;
    head = current->next;
    free(current);
  }
  free(head);
}

void print_list(node* head) {
  node* current = head;
  while (current != NULL) {
    printf("%s\n", current->string);
    current = current->next;
  }
}
