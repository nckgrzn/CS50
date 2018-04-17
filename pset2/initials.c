// include libraries
#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
  // ask user for input
  string name = get_string();
  // safety check for acceptable user input 
  if (name != NULL)
  {
      // print capitalized first letter of first name in user input
          printf("%c", toupper(name[0]));
      // traverse through characters in user input in search of spaces
      for(int count = 0, length = strlen(name); count < length; count++)
      {
          if (name[count] == ' ')
          {
              count++;
              // display capitalized first letter of 
              printf("%c", toupper(name[count]));
          }
      }
       
  }
  printf("\n");
}