#include "extract.h"
#include "association.h"
#include "transform.h"

struct basic_association_info {
    char * antecedent;
    char * consequent;
    double support;
    double trust;
};

struct basic_association_info * create_association_info(char * antecedent, char * consequent,double support, double trust) {
    struct basic_association_info * info = malloc(sizeof(struct basic_association_info));
    info->antecedent = antecedent;
    info->consequent = consequent;
    info->support =support;
    info->trust = trust;
    return info;
}

void delete_basic_association_info(struct basic_association_info * info) {
    free(info);
}

struct basic_association_info ** make_infos(struct rules_collection * coll, struct csv * csv) {
    struct csv_page * page = get_csv_data(csv);
    struct csv_schema * schema = get_csv_schema(csv);
    struct basic_association_info ** info_collection = malloc(sizeof(struct basic_association_info *) * coll->length);
    for(int i = 0; i < coll->length; i++) {
        struct association_rule * rule = coll->rules[i];
        struct basic_association_info * info = create_association_info(
            schema->columns[rule->antecedent->values[0] + 1],
            schema->columns[rule->consequent + 1],
            ((double) rule->consequent_count / page->size),
            rule->antecedent_count != 0 ? ((double) rule->consequent_count / rule->antecedent_count) : 0
        );
        info_collection[i] = info;
    }
    return info_collection;
}

void write_on_file(struct basic_association_info ** infos, int quantity, double min_support) {
    FILE * fp = fopen("result.txt", "w");
    for(int i = 0; i < quantity; i++) {
        if(infos[i]->support >= min_support) {
            fprintf(fp, "(%s => %s) support: %.4f trust: %.4f\n", infos[i]->antecedent, infos[i]->consequent, infos[i]->support, infos[i]->trust);
        }
    }
    fclose(fp);
}


void print_rules_coll(struct rules_collection * coll, struct csv_schema * schema) {
    for(int i = 0; i < coll->length; i++) {
    struct association_rule * rule = coll->rules[i];
    printf("Association rule | antecedent : %s | consequent: %s | a_count : %d | c_count : %d \n",
        schema->columns[rule->antecedent->values[0] + 1], schema->columns[rule->consequent + 1], rule->antecedent_count, rule->consequent_count);
    }
}

int main() {
    struct csv * csv = extract_from_file("data.csv", 10);

    struct csv_page * page = get_csv_data(csv);
    struct csv_schema * schema = get_csv_schema(csv);

    struct int_matrix * matrix = transform(page, schema->quantity_of_columns, 1);

    struct rules_collection * coll = iterate_matrix(matrix);

    struct basic_association_info ** info_coll = make_infos(coll, csv);

    write_on_file(info_coll, coll->length, 0.2);

    for(int i = 0; i < coll->length; i++) {
        delete_basic_association_info(info_coll[i]);
    }
    delete_rules_collection(coll);
    delete_int_matrix(matrix);
    delete_csv(csv);
}