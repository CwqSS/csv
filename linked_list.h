#ifndef LINKED_LIST_STRING
#define LINKED_LIST_STRING

struct linked_list_of_strings;
struct linked_list_of_strings * create_linked_list_of_strings() ;
void delete_linked_list_of_str(struct linked_list_of_strings * list);
void add_to_linked_list_of_str(struct linked_list_of_strings * list, char * string);
char ** get_array_from_list_of_str(struct linked_list_of_strings * list);
size_t get_list_of_str_length(struct linked_list_of_strings * list);


#endif