#include "tree.h"
#include <stdlib.h>

struct atc_list* atc_list_append(struct atc_list *list, struct tree_common *value) {
    struct atc_list* new_value = malloc(sizeof(struct atc_list));
    new_value->value = value;
    new_value->next = list;
    return new_value;
}