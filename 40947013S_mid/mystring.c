#include "mystring.h"

char *mystrtok_r(char *str, const char *delim, char **saveptr)
{
    char *endptr;
    if(str == NULL) str = *saveptr;
    if(*str == 0)
    {
        *saveptr = str; return NULL;
    }
    str = str + strspn (str, delim);
    if (*str == 0)
    {
        *saveptr = str; return NULL;
    }  
    endptr = str + strcspn (str, delim);
    if (*endptr == 0)
    {
        *saveptr = endptr; return str;
    }

    *endptr = 0;
    *saveptr = endptr+1;
    return str;
}