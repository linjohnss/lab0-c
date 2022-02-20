#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    // malloc return NULL if allocate failed.
    if (head)
        INIT_LIST_HEAD(head);

    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (!l)
        return;

    // traverse all entries and free the entry and its value
    element_t *entry, *safe;
    list_for_each_entry_safe (entry, safe, l, list) {
        free(entry->value);
        free(entry);
    }
    free(l);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *ele = malloc(sizeof(element_t));
    if (!ele)
        return false;

    // allocate string space
    size_t length = strlen(s);
    ele->value = malloc(length + 1);
    if (!ele->value) {
        free(ele);
        return false;
    }

    // copy the string into space
    strncpy(ele->value, s, length);
    ele->value[length] = '\0';

    // add new node in begining of head
    list_add(&ele->list, head);

    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *ele = malloc(sizeof(element_t));
    if (!ele)
        return false;

    // allocate string space
    size_t length = strlen(s);
    ele->value = malloc(length + 1);
    if (!ele->value) {
        free(ele);
        return false;
    }

    // copy the string into space
    strncpy(ele->value, s, length);
    ele->value[length] = '\0';

    // add new node in begining of head
    list_add_tail(&ele->list, head);

    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return target element.
 * Return NULL if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 *
 * NOTE: "remove" is different from "delete"
 * The space used by the list element and the string should not be freed.
 * The only thing "remove" need to do is unlink it.
 *
 * REF:
 * https://english.stackexchange.com/questions/52508/difference-between-delete-and-remove
 */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    element_t *ptr = list_first_entry(head, element_t, list);
    list_del_init(&ptr->list);
    size_t length = strlen(ptr->value);
    if (sp) {
        length = (length > bufsize - 1) ? bufsize - 1 : length;
        strncpy(sp, ptr->value, length);
        sp[length] = '\0';
    }
    return ptr;
}

/*
 * Attempt to remove element from tail of queue.
 * Other attribute is as same as q_remove_head.
 */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    element_t *ptr = list_last_entry(head, element_t, list);
    list_del_init(&ptr->list);
    size_t length = strlen(ptr->value);
    if (sp) {
        length = (length > bufsize - 1) ? bufsize - 1 : length;
        strncpy(sp, ptr->value, length);
        sp[length] = '\0';
    }
    return ptr;
}

/*
 * WARN: This is for external usage, don't modify it
 * Attempt to release element.
 */
void q_release_element(element_t *e)
{
    free(e->value);
    free(e);
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *li;

    list_for_each (li, head)
        len++;
    return len;
}

/*
 * Delete the middle node in list.
 * The middle node of a linked list of size n is the
 * ⌊n / 2⌋th node from the start using 0-based indexing.
 * If there're six element, the third member should be return.
 * Return NULL if list is NULL or empty.
 */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || list_empty(head))
        return NULL;
    element_t *ptr;
    struct list_head *rear, *front;
    rear = head->prev;
    front = head->next;
    while (front != rear && front->next != rear) {
        front = front->next;
        rear = rear->prev;
    }
    list_del_init(front);
    ptr = list_entry(front, element_t, list);
    q_release_element(ptr);
    return true;
}

/*
 * Delete all nodes that have duplicate string,
 * leaving only distinct strings from the original list.
 * Return true if successful.
 * Return false if list is NULL.
 *
 * Note: this function always be called after sorting, in other words,
 * list is guaranteed to be sorted in ascending order.
 */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/*
 * Attempt to swap every two adjacent nodes.
 */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *right, *left;
    left = head->next;
    right = left->next;
    while (left != head && right != head) {
        list_move(left, right);
        left = left->next;
        right = left->next;
    }
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(struct list_head *head)
{
    if (!head)
        return;
    else if (list_empty(head))
        return;

    struct list_head *node, *safe;
    list_for_each_safe (node, safe, head) {
        node->next = node->prev;
        node->prev = safe;
    }
    head->next = head->prev;
    head->prev = safe;
}

struct list_head *mergeTwoList(struct list_head *L1, struct list_head *L2)
{
    struct list_head *head = NULL, **ptr = &head, **node = NULL;

    for (element_t *ele1, *ele2; L1 && L2; *node = (*node)->next) {
        ele1 = list_entry(L1, element_t, list);
        ele2 = list_entry(L2, element_t, list);
        node = (strcmp(ele1->value, ele2->value) <= 0) ? &L1 : &L2;
        *ptr = *node;
        ptr = &(*ptr)->next;
    }
    *ptr = (struct list_head *) ((__UINTPTR_TYPE__) L1 | (__UINTPTR_TYPE__) L2);
    //*ptr = L1 ? L1 : L2;
    return head;
}

struct list_head *mergesort_list(struct list_head *head)
{
    if (!head || !head->next)
        return head;
    struct list_head *slow = head;
    for (struct list_head *fast = head->next; fast && fast->next;
         fast = fast->next->next)
        slow = slow->next;
    struct list_head *mid = slow->next;
    slow->next = NULL;
    struct list_head *left = mergesort_list(head), *right = mergesort_list(mid);
    return mergeTwoList(left, right);
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    head->prev->next = NULL;

    head->next = mergesort_list(head->next);
    struct list_head *ptr;
    for (ptr = head; ptr->next; ptr = ptr->next)
        ptr->next->prev = ptr;
    ptr->next = head;
    head->prev = ptr;
}
