//
// Created by dell on 19.10.18.
//
#include <stdio.h>
#include <stdlib.h>
#include <printf.h>
#include "LinkedList.h"

struct linked_list* create_linked_list(int element) {

    struct linked_list *new_element_list = (struct linked_list*)malloc(sizeof(struct linked_list));
    new_element_list->element = element;
    new_element_list->prev_el = NULL;
    new_element_list->next_el = NULL;
    return new_element_list;

}

void linked_list_free(struct linked_list** link_list) {

    while (*link_list != NULL) {
        struct linked_list* last = (*link_list)->prev_el;
        free(*link_list);
        (*link_list) = last;
    }

}

void front_add (struct linked_list** link_list, int element) {

    if (*link_list == NULL){ *link_list = create_linked_list(element); return; }

    struct linked_list *new_element = (struct linked_list*)malloc(sizeof(struct linked_list));
    new_element->element = element;
    new_element->prev_el = NULL;
    struct linked_list* tmp_list = *link_list;
    while(tmp_list->prev_el != NULL){
        tmp_list = tmp_list->prev_el;
    }
    new_element->next_el = tmp_list;
    tmp_list->prev_el = new_element;
}

void back_add (struct linked_list** link_list, int element) {

    if (*link_list == NULL){ *link_list = create_linked_list(element); return; }

    struct linked_list *new_element = (struct linked_list*)malloc(sizeof(struct linked_list));
    new_element->element = element;
    new_element->next_el = NULL;
    new_element->prev_el = *link_list;
    (*link_list)->next_el = new_element;
    *link_list = new_element;
}

struct linked_list* list_node_at(struct linked_list* link_list, unsigned int index) {

    unsigned int i = 0;

    if (link_list == NULL) { return NULL; }

    while (link_list->prev_el != NULL){ link_list = link_list->prev_el; }

    while (i != index){
        i++;
        link_list = link_list->next_el;
        if (link_list == NULL) { return NULL; }
    }

    return link_list;
}

int list_length(struct linked_list const* link_list){
    int leng = 0;
    while (link_list != NULL) {
        struct linked_list* last = link_list->prev_el;
        link_list = last;
        leng++;
    }
    return leng;
}

int list_get(struct linked_list* link_list, unsigned int index){
    struct linked_list* ret_val = list_node_at(link_list, index);
    return ret_val == NULL ? 0 : ret_val->element;
  /* if (ret_val == NULL) { return 0; } else {
       ret_val->element;
       return ret_val;
   }*/
}

int list_sum(struct linked_list const* link_list){
    int sum = 0;
    while (link_list != NULL){
        sum = sum + link_list->element;
        link_list = link_list->prev_el;
    }
    return sum;
}

/*void print_this_list(struct linked_list const* link_list) {

    printf("\n Количество элементов: %d\n",list_length(link_list));
    printf("Сумма элементов: %d\n", list_sum(link_list));
    if (link_list == NULL){
        printf("Список пуст\n");
        return;
    }
    while (link_list->prev_el != NULL){
        link_list = link_list-> prev_el;
    }
    printf("Список: ");
    while (link_list != NULL) {
        printf("%d ", link_list->element);
        link_list = link_list->next_el;
    }
    printf("\n");
}*/

bool serialize(struct linked_list* link_list, const char* filename){
    FILE* output_file = fopen(filename,"wt");
    if (output_file == NULL) {
        return false;
    }
    size_t sz = (size_t) list_length(link_list);
    int i;
    int* buf = (int*)malloc(sz* sizeof(int));
    for (i = 0; i < sz; ++i) {
        buf[i] = link_list->element;
        link_list = link_list->prev_el;
    }
    fwrite(buf, sz, sizeof(int), output_file);
    fclose(output_file);
    return true;
}

bool deserialize(struct linked_list** list, const char* filename){
    FILE* input_file = fopen(filename, "rt");
    if (input_file == NULL) {
        return false;
    }
    fseek(input_file, 0, SEEK_END);
    long sz = ftell(input_file);
    int i;
    rewind(input_file);

    int* buf = (int*)malloc(sz);
    fread(buf, sz/sizeof(int), sizeof(int), input_file);

    for(i = 0; i < sz/ sizeof(int); ++i){
        front_add(list, *(buf + i));
    }
    fclose(input_file);
    return true;
}

bool load(struct linked_list** list, const char* filename){
    int e;
    FILE* input_file = fopen(filename, "r");
    if (input_file == NULL) {
        return false;
    }
    while ( fscanf(input_file, "%d", &e) != EOF) {
        back_add(list, e);
    }
    fclose(input_file);
    return true;
}

bool save(struct linked_list* list, const char* filename){
    FILE* output_file = fopen(filename, "wt");
    if (output_file == NULL){
        return false;
    }
    while (list != NULL) {
        fprintf(output_file, "%d ", list->element);
        list = list->prev_el;
    }
    fclose(output_file);
    return true;
}



