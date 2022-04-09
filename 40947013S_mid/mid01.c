#include "mystring.h"

int main() 
{ 
    char str[] = "Geeks for Geek"; 
    char* token; 
    char* rest = str; 
  
    while ((token = mystrtok_r(rest, " ", &rest))) 
        printf("%s\n", token); 
  
    return 0; 
}
