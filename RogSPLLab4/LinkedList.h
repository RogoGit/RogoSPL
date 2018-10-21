//
// Created by dell on 19.10.18.
//

#ifndef ROGSPLLAB4_LINKEDLIST_H
#define ROGSPLLAB4_LINKEDLIST_H

#define true 1
#define false 0

typedef int bool;

struct linked_list {
    int element;
    struct linked_list* next_el;
    struct linked_list* prev_el;
};

struct linked_list* create_linked_list(int element);

void linked_list_free(struct linked_list** link_list);

void front_add (struct linked_list** link_list, int element);

void back_add (struct linked_list** link_list, int element);

struct linked_list* list_node_at(struct linked_list* link_list, unsigned int index);

int list_length(struct linked_list const* link_list);

int list_get(struct linked_list* link_list, unsigned int index);

int list_sum(struct linked_list const* link_list);

//void print_this_list(struct linked_list const* link_list);

bool deserialize(struct linked_list** list, const char* filename);

bool load(struct linked_list** list, const char* filename);

bool save(struct linked_list* list, const char* filename);

bool serialize(struct linked_list* list, const char* filename);

#endif //ROGSPLLAB4_LINKEDLIST_H
