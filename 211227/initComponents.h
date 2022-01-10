#ifndef INITCOMPONENTS_H
#define INITCOMPONENTS_H

#define MAXLENOPERATION 32

typedef struct menu_s {
    unsigned int numMenu;
    char operation[MAXLENOPERATION];
    struct menu_s *ptrNextOp;
} menu_t;

int getSizeStruct(menu_t *head);
menu_t *addNewElement(menu_t *head, const unsigned int numOp, const char *Op);
menu_t *getLast(menu_t *head);
menu_t *getOperation(menu_t *head, unsigned int *numOp);
menu_t *addNewElementEnd(menu_t *head, const unsigned int numOp, const char *Op);
char *cutNameLib(const char *name, const unsigned int ptr);
char *changeNameLib(const char *name, const unsigned int ptr);
int findNameLib(const char *filename);
int isPlugin(const unsigned char type, const char *name);
menu_t *findPlugins(menu_t *head);
menu_t *componentsInit();

#endif //INITCOMPONENTS_H
