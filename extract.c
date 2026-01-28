#include "extract.h"
#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct csv {
    const char * filename;
    long int offset;
    size_t page_size;
    csv_schema * schema;
    csv_page * page;
    size_t page_number;
    int has_more_data;
};

typedef struct linked_list_of_strings ListOfString;

FILE * get_current_position_in_csv(struct csv * csvp) {
    FILE * fp = fopen(csvp->filename, "r");
    fseek(fp, csvp->offset, SEEK_SET);
    return fp;
}

csv_schema * create_csv_schema(char ** columns, int length) {
    csv_schema * new = malloc(sizeof(csv_schema));
    new->columns = columns;
    new->quantity_of_columns = length;
    return new;
}

csv_instance * create_csv_instance(char ** values) {
    csv_instance * instance = malloc(sizeof(csv_instance));
    instance->values = values;
    return instance;
}

csv_page * create_csv_page(size_t page_size) {
    csv_page * page = malloc(sizeof(csv_page));
    page->instances = malloc(sizeof(csv_page *) * page_size);
    page->size = 0;
    return page;
}

struct csv * create_csv(const char * filename, size_t page_size) {
    struct csv * new = malloc(sizeof(struct csv));
    new->filename = filename;
    new->schema = NULL;
    new->page = NULL;
    new->offset = 0;
    new->page_size = page_size;
    new->page_number = 0;
    new->has_more_data = 1;
    return new;
}

ListOfString * extract_line(FILE * fp, long int * bytes_advanced_history, int * has_more_data) {
    char separator = ',';
    char buffer[20];
    int c = 0;
    ListOfString * list = create_linked_list_of_strings();
    long int bytes_advanced = 0;
    while(c != '\n' && c != EOF) {
        c = fgetc(fp);
        int size = 0;
        while(c != separator && c != '\n' && c != '\r' && c != EOF) {
            buffer[size] = c;
            size++;
            c = fgetc(fp);
        }
        buffer[size] = '\0';

        add_to_linked_list_of_str(list, buffer);
        bytes_advanced += (size + 1) * sizeof(char);
    }

    if(c == EOF && has_more_data != NULL) {
        *has_more_data = 0;
    }

    if(bytes_advanced_history != NULL) {
        *bytes_advanced_history += bytes_advanced;
    }

    return list;
}

csv_schema * extract_header(struct csv * csv_pointer) {
    FILE * fp = get_current_position_in_csv(csv_pointer);
    ListOfString * list = extract_line(fp, &csv_pointer->offset, &csv_pointer->has_more_data);

    char ** columns = get_array_from_list_of_str(list);
    size_t length = get_list_of_str_length(list);

    csv_schema * schema = create_csv_schema(columns, length);

    csv_pointer->schema = schema;

    fclose(fp);
    delete_linked_list_of_str(list);

    return schema;
}

void extract_data(struct csv * csv_pointer) {
    FILE * fp = get_current_position_in_csv(csv_pointer);
    
    csv_page * page = create_csv_page(csv_pointer->page_size);
    for(size_t i = 0; i < csv_pointer->page_size; i++) {
        if(!csv_pointer->has_more_data) {
            continue;
        }

        ListOfString * list = extract_line(fp, &csv_pointer->offset, &csv_pointer->has_more_data);
        char ** values = get_array_from_list_of_str(list);

        page->instances[i] = create_csv_instance(values);
        page->size += 1;
        delete_linked_list_of_str(list);
    }
    fclose(fp);
    csv_pointer->page = page;
    csv_pointer->page_number += 1;
}

struct csv * extract_from_file(const char * filename, size_t page_size) {
    struct csv * csv_pointer = create_csv(filename, page_size);
    
    extract_header(csv_pointer);
    extract_data(csv_pointer);

    return csv_pointer;
}

#pragma region delete_linked_list_of_str

void delete_csv_schema(csv_schema * schema) {
    free(schema->columns);
    free(schema);
}

void delete_csv_instance(csv_instance * instance) {
    free(instance->values);
    free(instance);
}

void delete_csv_page(csv_page * page) {
    for(size_t i = 0; i < page->size; i++) {
        delete_csv_instance(page->instances[i]);
    }
    free(page->instances);
    free(page);
}

void delete_csv(struct csv * csv) {
    delete_csv_page(csv->page);
    delete_csv_schema(csv->schema);
    free(csv);
}

#pragma endregion


void print_csv_header(struct csv * csv) {
    for(int i = 0; i < csv->schema->quantity_of_columns; i++) {
        printf("%s\t ", csv->schema->columns[i]);
    }
    printf("\n");
}

void print_csv_instance(csv_instance * instance, int quantity) {
    for(int i = 0; i < quantity; i++) {
        printf("%s\t", instance->values[i]);
    }
    printf("\n");
}

void print_csv_data(struct csv * csv) {
    for(size_t i = 0; i < csv->page->size; i++) {
        print_csv_instance(csv->page->instances[i], csv->schema->quantity_of_columns);
    }
    printf("\n");
}

struct csv_page * get_csv_data(struct csv * csv) {
    return csv->page;
}

struct csv_schema * get_csv_schema(struct csv * csv) {
    return csv->schema;
}


