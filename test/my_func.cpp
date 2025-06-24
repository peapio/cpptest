
#include "my_func.h"
#include "../dbg.h"
#include <assert.h>


char* MyStrCpy(char* src, char* dst)
{
    if(!dst || !src) return NULL;

    char * res = dst;
    while(*src != '\0')
    {
        *dst = *src;
        dst++;
        src++;
    }
    *dst = '\0';
    return res;
}

char* MyStrCat(char* src, char* dst)
{
    char * start = dst;

    while(*dst != '\0')
    {
        dst++;
    }

    while(*src != '\0')
    {
        *dst = *src;
        dst++;
        src++;
    }
    *dst = '\0';

    return start;

}

int MyStrCmp(char* s1, char* s2)
{
    if(!s1 || !s2) return NULL;
    
    while(*s1 == *s2 && *s1!= '\0' && *s2 != '\0')
    {
        s1++;
        s2++;
    }

    return *s1 - *s2;

}