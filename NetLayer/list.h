/*
 * list.h
 *
 *  Created on: Aug 22, 2019
 *      Author: wanji-hxy
 */

#ifndef INCLUDE_LIST_H_
#define INCLUDE_LIST_H_



#ifndef NULL
#define NULL   (void *)0
#endif
typedef unsigned int u32;

//the struct of the list
struct list_head {
    struct list_head *next, *prev;
};


/*************************************************
  Init the list
*************************************************/
#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct list_head *list)
{
    list->next = list;
    list->prev = list;
}


/*************************************************
  add list node
*************************************************/
//add the list bettwen prev list and next list
static inline void __list_add(struct list_head *new,
                  struct list_head *prev,
                  struct list_head *next)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}
//add the new list before the head list
static inline void list_add(struct list_head *new, struct list_head *head)
{
    __list_add(new, head, head->next);
}
//add new list to the head list tail
static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
    __list_add(new, head->prev, head);
}



/*************************************************
  delete list node
*************************************************/
//delete list node bettween prev and next
static inline void __list_del(struct list_head * prev, struct list_head * next)
{
    next->prev = prev;
    prev->next = next;
}
//delete the entry node list
static inline void list_del(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    entry->next = NULL;
    entry->prev = NULL;
}
//delete the entry node list and init
static inline void list_del_init(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    INIT_LIST_HEAD(entry);
}



/*************************************************
  replace list node
*************************************************/
//replace old node  to new node
static inline void list_replace(struct list_head *old,
                struct list_head *new)
{
    new->next = old->next;
    new->next->prev = new;
    new->prev = old->prev;
    new->prev->next = new;
}
//replace old to new list node and init old
static inline void list_replace_init(struct list_head *old,
                    struct list_head *new)
{
    list_replace(old, new);
    INIT_LIST_HEAD(old);
}


/*************************************************
  move list node
*************************************************/
//move list to the front of head node list
static inline void list_move(struct list_head *list, struct list_head *head)
{
    __list_del(list->prev, list->next);
    list_add(list, head);
}
//move list to the read of head list node
static inline void list_move_tail(struct list_head *list,
                  struct list_head *head)
{
    __list_del(list->prev, list->next);
    list_add_tail(list, head);
}




/*************************************************
  list node check opertion
*************************************************/
//check the list node wether the last node of head or not
static inline int list_is_last(const struct list_head *list,
                const struct list_head *head)
{
    return list->next == head;
}
//check head list node is empty or not
static inline int list_empty(const struct list_head *head)
{
    return head->next == head;
}
//check list is empty or not in the thread
static inline int list_empty_careful(const struct list_head *head)
{
    struct list_head *next = head->next;
    return (next == head) && (next == head->prev);
}
//check whether there is only one node or not in the head list node
static inline int list_is_singular(const struct list_head *head)
{
    return !list_empty(head) && (head->next == head->prev);
}



/*************************************************
  list cut operation
*************************************************/
//it was called by list_cut_position
static inline void __list_cut_position(struct list_head *list,
        struct list_head *head, struct list_head *entry)
{
    struct list_head *new_first = entry->next;
    list->next = head->next;
    list->next->prev = list;
    list->prev = entry;
    entry->next = list;
    head->next = new_first;
    new_first->prev = head;
}
//cut the head list node to two lists and list is the origin new cut list, the entry is the new cut list
static inline void list_cut_position(struct list_head *list,
        struct list_head *head, struct list_head *entry)
{
    if (list_empty(head))
        return;
    if (list_is_singular(head) &&
        (head->next != entry && head != entry))
        return;
    if (entry == head)
        INIT_LIST_HEAD(list);
    else
        __list_cut_position(list, head, entry);
}



/*************************************************
  list spilice operation
*************************************************/
//list spilice call by the list_splice
static inline void __list_splice(const struct list_head *list,
                 struct list_head *prev,
                 struct list_head *next)
{
    struct list_head *first = list->next;
    struct list_head *last = list->prev;

    first->prev = prev;
    prev->next = first;

    last->next = next;
    next->prev = last;
}
//splice the list to the tail of the head list
static inline void list_splice(const struct list_head *list,
                struct list_head *head)
{
    if (!list_empty(list))
        __list_splice(list, head, head->next);
}
//splice the list to the front of the head list
static inline void list_splice_tail(struct list_head *list,
                struct list_head *head)
{
    if (!list_empty(list))
        __list_splice(list, head->prev, head);
}
//splice the list to the tail of the head and init list
static inline void list_splice_init(struct list_head *list,
                    struct list_head *head)
{
    if (!list_empty(list)) {
        __list_splice(list, head, head->next);
        INIT_LIST_HEAD(list);
    }
}
//splice the list to the front of the head list and init list
static inline void list_splice_tail_init(struct list_head *list,
                     struct list_head *head)
{
    if (!list_empty(list)) {
        __list_splice(list, head->prev, head);
        INIT_LIST_HEAD(list);
    }
}



/*************************************************
  traverse the list
*************************************************/
#define offsetof(TYPE, MEMBER) ((u32) &((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({            \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - offsetof(type,member) );})

//get the ptr strcuture
#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)
//get the first of the list node
#define list_first_entry(ptr, type, member) \
    list_entry((ptr)->next, type, member)
//traverse the list
#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)
//reverse traverse the list
#define list_for_each_reverse(pos, head) \
    for (pos = (head)->prev; pos != (head); pos = pos->prev)
//traverse the list structure
#define list_for_each_entry(pos, head, member)                \
    for (pos = list_entry((head)->next, typeof(*pos), member);    \
         &pos->member != (head);     \
         pos = list_entry(pos->member.next, typeof(*pos), member))
//reverse traverse the list structure
#define list_for_each_entry_reverse(pos, head, member)            \
    for (pos = list_entry((head)->prev, typeof(*pos), member);    \
         &pos->member != (head);     \
         pos = list_entry(pos->member.prev, typeof(*pos), member))
//traverse the list begin at the pos node, no include pos node
#define list_for_each_entry_continue(pos, head, member)         \
    for (pos = list_entry(pos->member.next, typeof(*pos), member);    \
         &pos->member != (head);    \
         pos = list_entry(pos->member.next, typeof(*pos), member))
//reserve traverse the list begin at the pos node, no include pos node
#define list_for_each_entry_continue_reverse(pos, head, member)        \
    for (pos = list_entry(pos->member.prev, typeof(*pos), member);    \
         &pos->member != (head);    \
         pos = list_entry(pos->member.prev, typeof(*pos), member))
//traverse the list begin at the pos node, include pos node
#define list_for_each_entry_from(pos, head, member)             \
    for (; &pos->member != (head); pos = list_entry(pos->member.next, typeof(*pos), member))




#endif /* INCLUDE_LIST_H_ */
