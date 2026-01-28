#include <string.h>
#include <stdlib.h>
#include "linked_list.h"

struct string_node {
    size_t size;
    char * value;
    struct string_node * next;
};

struct linked_list_of_strings {
    size_t length;
    struct string_node * head;
    struct string_node * tail;
};

struct linked_list_of_strings * create_linked_list_of_strings() {
    struct linked_list_of_strings * new = malloc(sizeof(struct linked_list_of_strings));
    new->head = NULL;
    new->tail = NULL;
    new->length = 0;
    return new;
}

struct string_node * create_string_node(char * string, size_t size) {
    struct string_node * new = malloc(sizeof(struct string_node));
    new->value = malloc((size + 1) * sizeof(char));
    new->size = size;
    strcpy(new->value, string);
    new->next = NULL;
    return new;
}

void delete_linked_list_of_str(struct linked_list_of_strings * list) {
    struct string_node * node = list->head;
    while(node != NULL) {
        struct string_node * next = node->next; 
        free(node->value);
        free(node);
        node = next;
    }
    free(list);
}

void add_to_linked_list_of_str(struct linked_list_of_strings * list, char * string) {
    if(list == NULL) 
        return;

    struct string_node * node = create_string_node(string, strlen(string));

    if(list->head == NULL) {
        list->head = node;
        list->tail = node;
    }
    else {
        struct string_node * last_node = list->tail;
        last_node->next = node;
        list->tail = node;
    }
    
    list->length += 1;

    return;
}

char ** get_array_from_list_of_str(struct linked_list_of_strings * list) {
    struct string_node * node = list->head;

    char ** string_array = malloc(sizeof(char*) * (list->length));

    for(size_t i = 0; i < list->length && node != NULL; i++) {
        string_array[i]  = malloc(sizeof(char) * (node->size + 1));
        strcpy(string_array[i], node->value);
        node = node->next;
    }

    return string_array;
}

size_t get_list_of_str_length(struct linked_list_of_strings * list) {
    return list->length;
}