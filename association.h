#include "transform.h"
#ifndef ASSOCIATION_HEADER
#define ASSOCIATION_HEADER
struct int_array {
    int quantity;
    int * values;
    int length;
};
struct association_rule {
    struct int_array * antecedent;
    int consequent;
    int antecedent_count;
    int consequent_count;
};

struct rules_collection {
    struct association_rule ** rules;
    int length;
};

void delete_rules_collection(struct rules_collection * coll);
struct rules_collection * iterate_matrix(struct int_matrix * matrix);
#endif