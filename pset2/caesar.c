// include libraries
#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
  // verify that user has input only two command-line arguments
  if (argc == 2)
  {
      // ask user for input
      printf("plaintext: ");
      string text = get_string();
      // safety check for acceptable input
      if (text != NULL)
      {
          printf("ciphertext: ");
          // traverse through characters in input
          for(int count = 0, length = strlen(text); count < length; count++)
          {
              // check for alphabetic characters
              if (isalpha(text[count]))
              {
                  // check for upper-case letters
                  if (isupper(text[count]))
                  {
                      // encrypt upper-case letters
                      int encrypted = text[count] + atoi(argv[1]) % 26;
                      // check for encrypting past 'Z'
                      if (encrypted >= 91)
                      {
                        // wrap around upper-case alphabet
                        encrypted = 65 + (encrypted % 91);
                        printf("%c", encrypted);
                      }
                      else
                      printf("%c", encrypted);
                  }
                  else
                  // check for lower-case letters
                  if (islower(text[count]))
                  {
                      // encrypt lower-case letters
                      int encrypted = text[count] + atoi(argv[1]) % 26;
                      // check for encrypting past 'z'
                      if (encrypted >= 123)
                      {
                        // wrap around lower-case alphabet
                        encrypted = 97 + (encrypted % 123);
                        printf("%c", encrypted);
                      }
                      else
                      printf("%c", encrypted);
                  }
              }
              else
              // display non-alphabetic characters w/o encryption
              printf("%c", text[count]);
              
          }
          printf("\n");
          return 0;
      }
  }
  else
  // display error message
  printf("Please enter two command-line arguments only.\n");
  return 1;
}