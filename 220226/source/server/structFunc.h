#ifndef STRUCTFUNC_H
#define STRUCTFUNC_H

singlyLinkedList *createSinglyLinkedList();
void addNewUser(singlyLinkedList *list, unsigned int number, unsigned int time, const char *name);
void addNewUserBack(singlyLinkedList *list, unsigned int number, unsigned int time, const char *name);
void printSinglyLinkedList(singlyLinkedList *list);

doublyLinkedList *createDoublyLinkedList();
void addNewMessage(doublyLinkedList *list, unsigned int number, unsigned int time, const char *name, const char *text);
void addNewMessageBack(doublyLinkedList *list, unsigned int number, unsigned int time, const char *name, const char *text);
void printDoublyLinkedList(doublyLinkedList *list);

#endif
