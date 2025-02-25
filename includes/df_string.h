#ifndef STRING_H
#define STRING_H

typedef struct df_string DfString;

DfString* DfString_Create(char* initial_value);

void DfString_Concat(DfString* dest, const char* src);

void DfString_Concat_String(DfString* dest, DfString* src);

#endif
