#ifndef PARSEFUNCTIONS_H
#define PARSEFUNCTIONS_H

char *scanString();
char **parseString(char *string);
void addArg(unsigned const char *flag, char *arg, unsigned int *argPtr, char **args, unsigned int *argsIndex);
unsigned int findPairQuotes(char *string, unsigned const int *strPtr);

#endif
