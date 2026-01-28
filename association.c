#include <stdlib.h>
#include "association.h"
#include "transform.h"

#pragma region INT_ARRAY

void delete_int_array(struct int_array * array) {
    free(array->values);
    free(array);
}

struct int_array * create_int_array(int length) {
    struct int_array * new = malloc(sizeof(struct int_array));
    new->values = malloc(sizeof(int) * length);
    new->length = length;
    new->quantity = 0;
    return new;
}

int add_to_int_array(struct int_array * array, int value) {
    if(array->quantity < array->length) {
        array->values[array->quantity] = value;
        array->quantity++;
        return 1;
    }
    return 0;
}

#pragma endregion

struct association_rule * create_association_rule(struct int_array * antecedent, int consequent) {
    struct association_rule * rule = malloc(sizeof(struct association_rule));
    rule->antecedent = antecedent;
    rule->consequent = consequent;
    rule->antecedent_count = 0;
    rule->consequent_count = 0;
    return rule;
}

void delete_association_rule(struct association_rule * ass) {
    delete_int_array(ass->antecedent);
    free(ass);
}

struct rules_collection * create_rules_collection(struct association_rule ** rules, int length) {
    struct rules_collection * coll = malloc(sizeof(struct rules_collection));
    coll->rules = rules;
    coll->length = length;
    return coll;
}

struct rules_collection * generate_associations(int qtt_columns) {
    int length = (qtt_columns * (qtt_columns - 1));
    struct association_rule ** rules = malloc(sizeof(struct association_rule *) * (length));
    int position = 0;
    for(int j = 0; j < qtt_columns; j++) {
        for(int i = 0; i < qtt_columns; i++) {
            if(j != i) {
                struct int_array * arr = create_int_array(1);
                add_to_int_array(arr, j);
                rules[position] = create_association_rule(arr, i);
                position++;
            }
        }
    }

    return create_rules_collection(rules, length);
}

void increment_associations_count(struct association_rule ** rules, struct int_array * arr, int association_count) {
    for(int i = 0; i < association_count; i++) {
        struct association_rule * rule = rules[i];
        int quantity = rule->antecedent->quantity;
        
        int finded = 1;
        int k = 0;
        while(k < quantity && finded) {
            int find = 0;
            for(int j = 0; j < arr->quantity && !find; j++) {
                if(rule->antecedent->values[k] == arr->values[j]) {
                    find = 1;
                }
            }

            if(!find)
               finded = 0;
            else k++;
        }

        if(!finded) {
            continue;
        }

        rule->antecedent_count++;
        
        int stop = 0;
        for(int j = 0; j < arr->quantity && !stop; j++) {
            if(arr->values[j] == rule->consequent) {
                stop = 1;
                rule->consequent_count++;
            }
        }
    }
}

struct rules_collection * iterate_matrix(struct int_matrix * matrix) {
    struct rules_collection * rules_coll = generate_associations(matrix->yCount);
    for(int i = 0; i < matrix->xCount; i++) {
        struct int_array * arr = create_int_array(matrix->yCount);
        for(int j = 0; j < matrix->yCount; j++) {
            if(matrix->values[i][j]) {
                add_to_int_array(arr, j);
            }
        }
        increment_associations_count(rules_coll->rules, arr, matrix->xCount);
        delete_int_array(arr);
    }
    return rules_coll;
}

void delete_rules_collection(struct rules_collection * coll) {
    for(int i = 0; i < coll->length; i++) {
        delete_association_rule(coll->rules[i]);
    }
}