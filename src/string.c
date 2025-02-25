/*
 * Essential String Features
✅ Concatenation – Append one string to another dynamically.
✅ Splitting – Split a string by a delimiter (like strtok, but safer).
✅ Trimming – Remove leading/trailing whitespace or specific characters.
✅ Substring Extraction – Extract part of a string by index/length.
✅ Case Manipulation – Convert to lowercase, uppercase, or capitalize words.
✅ Searching – Find substrings, first/last occurrence of a character.
✅ Replacing – Replace occurrences of a substring with another.
✅ Reversing – Reverse a string in place.
✅ Dynamic Allocation – Ensure all operations work with dynamically allocated memory, avoiding buffer overflows.
* 

#include <stdio.h>
#include <stdlib.h>
#include "../includes/df_string.h"
#include <string.h>

{
  char* chars;
  int length;
  int capacity;
}

String* String_Create(char* initial_value) {
  String *string = malloc(sizeof(String));
  if (!string) {
    perror("String memory allocation failed.");
    exit(EXIT_FAILURE);
  }

  int length = strlen(initial_value);

  string->chars = malloc((length + 1) * sizeof(char));
  if (!string->chars) {
    perror("chars memory allocation failed.");
    exit(EXIT_FAILURE);
  }

  strcpy(string->chars, initial_value);
  string->length = length;
  string->capacity = length + 1;

  return string;
}

// THIS IS WRONG, need to finish
void String_Concat(String* dest, const char* src) {
  if (!src) return;

  int new_length = dest->length + strlen(src);

  if (new_length + 1 > dest->capacity) {
    int new_capacity = (new_length + 1 > dest->capacity * 2) ? new_length + 1 : dest->capacity * 2;
    
    char *resized_string = realloc(dest->chars, new_capacity * sizeof(int));
    if(!resized_string) {
      perror("Reallocation of String->capacity failed");
      return;
    }

    dest->chars = resized_string;
    dest->capacity = new_capacity;
    return;
  }
}

*/
