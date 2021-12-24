//
#include <stdio.h>
#include <dlfcn.h>

int main()
{
    void *handle;
    int (*myfunc)(int);
    char *error;
    int value = 15;

    handle = dlopen("./libprinttest/libprinttest.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        return 1;
    }

    myfunc = dlsym(handle, "printTest");
    error = dlerror();
    if (error != NULL) {
        fprintf(stderr, "%s\n", error);
        return 1;
    }

    int result = (*myfunc)(value);
    printf("Result: 2*x = %d\n", result);
    dlclose(handle);

    return 0;
}

