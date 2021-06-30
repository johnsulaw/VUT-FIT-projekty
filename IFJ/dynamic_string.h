///Project: IFJ 20
///File: dynamic_string.h
///Author(s): Jan Šulavík (xsulav01)
///Date: 1. 10. 2020

#ifndef IFJ20_DYNAMIC_STRING_H
#define IFJ20_DYNAMIC_STRING_H

#define INITIAL_LENGTH 50 // initial length of dynamic string

/* struct defining dynamic string */
typedef struct{
    char* value;
    int length;
    int allocated_length;
}MyString;


int my_string_init(MyString* string);
void my_string_free(MyString* string);
int my_strncat(MyString* dst, char c);
int my_strcat(MyString* string, char* str);
void my_strclear(MyString* string);

#endif //IFJ20_DYNAMIC_STRING_H
