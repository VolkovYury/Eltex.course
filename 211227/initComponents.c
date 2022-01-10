#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "initComponents.h"

/**
 * В функции формируется односвязный список, который содержит все возможные подключённые плагины.
 * @return Указатель на первый элемент односвязного списка с операциями, которые удалось "подключить"
 */
menu_t *componentsInit()
{
    menu_t *operations = NULL;
    printf("Component initialization...\n");

    operations = findPlugins(operations);
    printf("Initialized %d components\n", getSizeStruct(operations));

    return operations;
}

/**
 * Функция для вычисления размера односвязного списка
 * @param head Указатель на первый элемент односвязного списка
 * @return Количество элементов, содержащееся в списке, в виде целого числа.
 */
int getSizeStruct(menu_t *head)
{
    int size = 0;
    for (; head != NULL; head = head->ptrNextOp) {
        ++size;
    }
    return size;
}

/**
 * Функция добавляет элемент в начало односвязного списка
 * @param head Указатель на первый элемент односвязного списка
 * @param numOp Значение поля numMenu в заполняемой структуре
 * @param Op Значение поля operation в заполняемой структуре
 * @return Указатель на добавленный элемент односвязного списка
 */
menu_t *addNewElement(menu_t *head, const unsigned int numOp, const char *Op)
{
    menu_t *tmp = (menu_t *) malloc(sizeof(menu_t));
    tmp->numMenu = numOp;
    strcpy(tmp->operation, Op);
    tmp->ptrNextOp = head;

    return tmp;
}

/**
 * Функция возвращает указатель на последний элемент односвязного списка
 * @param head Указатель на первый элемент односвязного списка
 * @return Указатель на последний элемент односвязного списка
 */
menu_t *getLast(menu_t *head)
{
    if (head == NULL)
        return NULL;
    while (head->ptrNextOp)
        head = head->ptrNextOp;
    return head;
}

/**
 * Функция возвращает указатель на элемент односвязного списка
 * @param head Указатель на первый элемент односвязного списка
 * @param numOp Указатель на номер операции
 * @return Указатель на искомый элемент односвязного списка
 */
menu_t *getOperation(menu_t *head, unsigned int *numOp)
{
    if (head == NULL)
        return NULL;
    while (head->ptrNextOp && head->numMenu != *numOp)
        head = head->ptrNextOp;
    return head;
}

/**
 * Функция добавляет элемент в конец односвязного списка
 * @param head Указатель на первый элемент односвязного списка
 * @param numOp Значение поля numMenu в заполняемой структуре
 * @param Op Значение поля operation в заполняемой структуре
 * @return Указатель на первый элемент односвязного списка
 */
menu_t *addNewElementEnd(menu_t *head, const unsigned int numOp, const char *Op)
{
    if (!head) {
        head = addNewElement(head, numOp, Op);
        return head;
    }

    menu_t *prev = getLast(head);
    menu_t *tmp = (menu_t *) malloc(sizeof(menu_t));
    tmp->numMenu = numOp;
    strcpy(tmp->operation, Op);
    tmp->ptrNextOp = NULL;

    prev->ptrNextOp = tmp;

    return head;
}

/**
 * Функция "обрезает" полученную строку после элемента, на который указывает значение ptr
 * @param name Строка для обрезки
 * @param ptr "Указатель" на последний символ
 * @return Указатель на строку. После использования строки НЕОБХОДИМО удалить её из кучи
 */
char *cutNameLib(const char *name, const unsigned int ptr)
{
    int size = ptr + 1 + 1; // 1 - переход от "указателя" к размеру массива, 1 - место для '\0'
    char *newName = (char *) malloc(sizeof(char)*size);
    int i = 0;
    for (; i <=ptr; i++) {
        newName[i] = name[i];
    }

    newName[i] = '\0';
    return newName;
}

/**
 * Функция добавляет "_init" после элемента с индексом ptr
 * @param name Строка для вставки в конец
 * @param ptr "Указатель" на последний символ
 * @return Указатель на строку. После использования строки НЕОБХОДИМО удалить её из кучи
 */
#define STRSIZE 6
char *changeNameLib(const char *name, const unsigned int ptr)
{
    const char endFunc[STRSIZE] = "_init\0";
    int size = (ptr + 1) + STRSIZE; // 1 - переход от "указателя" к номеру последнего элемента строки
    char *newName = (char *) malloc(sizeof(char)*size);
    int i = 0;

    for (; i <= ptr; i++)
        newName[i] = name[i];

    for (int k = 0; endFunc[k] != '\0'; k++, i++)
        newName[i] = endFunc[k];

    newName[i] = '\0';
    return newName;
}
#undef STRSIZE

/**
 * Функция осуществляет проверку на то, является файл динамической библиотекой указанного формата или нет
 * @param filename Строка для анализа
 * @return Возвращает 0, если переданная строка не соответствует маске "*.so*". Иначе возвращает номер элемента массива,
 * за которым следует разыскиваемый фрагмент
 */
#define STRSIZE 4
int findNameLib(const char *filename)
{
    char libFormat[STRSIZE] = ".so\0";
    unsigned int k = 0;

    for (int i = 0; filename[i] != '\0'; i++) {
        if (filename[i] == libFormat[k])
            k++;
        else
            k = 0;

        if (k == STRSIZE-1)
            return (i-STRSIZE+1);
    }

    return 0;
}
#undef STRSIZE

/**
 * Функция содержит ряд проверок, в результате которой выяснится является ли анализируемый файл подходящим плагином
 * @param type Тип анализируемого файла (man readdir)
 * @param name Строка для анализа. Является именем файла с расширением
 * @return 1, если файл является поддерживаемым плагином. 0 - плагин не поддерживается.
 */
int isPlugin(const unsigned char type, const char *name)
{
    // Соответствие типу файла
    if (type != DT_REG) // DT_REG - обычный файл
        return 0;

    // Соответствие формату библиотеки
    int ptr = findNameLib(name);
    if (!ptr)
        return 0;

    // Возможность подключить найденную динамическую библиотеку
    const char libPath[11] = "./plugins/\0";
    char path[80];
    sprintf(path, "%s%s", libPath, name);
    void *lib;
    lib = dlopen(path, RTLD_LAZY);
    if (!lib)
        return 0;

    // Наличие функции-маркера, что данная библиотека - необходимый плагин.
    char newNameFunc[40];
    strcpy(newNameFunc, changeNameLib(name, ptr));
    int (*libInit) (void);
    libInit = dlsym (lib, newNameFunc);
    if (!libInit) {
        dlclose(lib);
        return 0;
    }

    dlclose(lib);
    return 1;
}

/**
 * Функция осуществляет поиск плагинов в указанной директории и добавляет их в односвязный список
 * @param head Указатель на первый элемент односвязного списка
 * @return Указатель на первый элемент односвязного списка
 */
menu_t *findPlugins(menu_t *head)
{
    DIR *dir;
    struct dirent *de;
    int counter = 0;

    dir = opendir("./plugins/");
    while (dir) {
        de = readdir(dir);
        if (!de)
            break;
        if (isPlugin(de->d_type, de->d_name)) {
            char nameOp[80];
            strcpy(nameOp, cutNameLib(de->d_name, findNameLib(de->d_name)));

            head = addNewElementEnd(head, ++counter, nameOp);
        }

    }
    closedir(dir);
    return head;
}
