#include <angsys.h>

uint ang_strlen(char const* str)
{
    int c = 0;
    if(str == NULL)
        return 0;
    while(str[c] != 0)
        c++;
    return c;
}

int ang_strcmp(char const* str1, char const*  str2)
{
    int c = 0;
    if(str1 == str2)
        return 0;
    else if(str1 == NULL)
        return -1;
    else if(str2 == NULL)
        return 1;
    LOOP:
    if(str1[c] < str2[c])
        return -1;
    else if(str1[c] > str2[c])
        return 1;
    else if(str1 == NULL)
        return 0;
    goto LOOP;
}


char* ang_strcpy(char* dest, char const*  src)
{
    int c = 0;
    if(dest == NULL)
        return NULL;
     
    while(src[c] != 0)
        dest[c] = src[c++];
    dest[c] = 0;
    return dest;
}

char* ang_strcat(char* dest, char const*  src)
{
    int i, c = 0;
    if(dest == NULL)
        return NULL;
    i = ang_strlen(dest); 
    while(src[c] != 0)
        dest[i + c] = src[c++];
    dest[i + c] = 0;
    return dest;
}

char* ang_itoa(ang_int32_t value, int base, char* src)
{
    
    return src;
}

char* ang_utoa(ang_uint32_t value, int base, char* src)
{
   
    return src;
}

char* ang_ftoa(double value, int pres, char* src)
{
    
    return src;
}