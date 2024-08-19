#include "tree.h"
#include <stdlib.h>

struct atc_list* atc_list_append(struct atc_list *list, struct tree_common *value) {
    struct atc_list* new_value = malloc(sizeof(struct atc_list));
    new_value->value = value;
    new_value->next = list;
    return new_value;
}


struct atc_list* atc_list_reverse(struct atc_list *list) {
    struct atc_list *reversed = NULL;
    for (struct atc_list *it = list; it; ) {
        struct atc_list *it_next = it->next;
        it->next = reversed;
        reversed = it;
        it = it_next;
    }
    return reversed;
}