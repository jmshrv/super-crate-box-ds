#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H

typedef struct JamieNode
{
	void *data;
	struct JamieNode *next;
	struct JamieNode *prev;
} JamieNode;

typedef struct JamieLinkedList
{
	JamieNode *head;
	JamieNode *tail;
} JamieLinkedList;

JamieNode *initialise_node(void);
void free_node(JamieNode *);
JamieLinkedList *initialise_linked_list(void);
void free_linked_list(JamieLinkedList *);
void append_linked_list(JamieLinkedList *, void *);
void prepend_linked_list(JamieLinkedList *, void *);
void remove_head_linked_list(JamieLinkedList *);
void remove_tail_linked_list(JamieLinkedList *);
void print_linked_list(JamieLinkedList *, void (*)(void *));
void print_char(void *);
void print_int(void *);
void print_double(void *);
void print_string(void *);

#endif
