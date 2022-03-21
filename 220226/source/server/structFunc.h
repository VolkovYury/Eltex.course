#ifndef STRUCTFUNC_H
#define STRUCTFUNC_H

void printSinglyLinkedList(singlyLinkedList *list);
void printDoublyLinkedList(doublyLinkedList *list);

singlyLinkedList *createSinglyLinkedList();
void addNewUser(singlyLinkedList *list, unsigned int number, unsigned int time, const char *name);
void addNewUserBack(singlyLinkedList *list, unsigned int number, unsigned int time, const char *name);

doublyLinkedList *createDoublyLinkedList();
void addNewMessage(doublyLinkedList *list, unsigned int number, unsigned int time, const char *name, const char *text);
void addNewMessageBack(doublyLinkedList *list, unsigned int number, unsigned int time, const char *name, const char *text);


#endif
