// include libraries
#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
  // verify that user has input only two command-line arguments
  if (argc != 2)
  {
  // display error message if more or less than two command-line arguments
  printf("Please enter two command-line arguments only.\n");
  return 1;
  }
  // get length of key
  int arglen = strlen(argv[1]);
  // traverse through characters in key
  for(int count = 0; count < arglen; count++)
  {
  	// verify that key is in alphabetical characters only
  	if (isalpha(argv[1][count]))
  	{
    	// equalize alpha-key values to 0+
    	if (isupper(argv[1][count]))
    	{
    		argv[1][count] = argv[1][count] - 'A';
    	}
    	else
    	{
    	  if (islower(argv[1][count]))
      	{
      		argv[1][count] = argv[1][count] - 'a';
      	}
    	}
  	}
  	else
  	{
    	// display error message if any non-alphabetical characters were used in key
    	printf("Please enter alphabetical characters only.\n");
    	return 1;
  	}
  }
  // ask user for input
  printf("plaintext: ");
  string text = get_string();
  // safety check for acceptable input
  if (text != NULL)
  {
      printf("ciphertext: ");
      // traverse through characters in input
      for(int txtcount = 0, txtlen = strlen(text), keycount = 0; txtcount < txtlen; txtcount++)
      {
          // check for alphabetic characters
          if (isalpha(text[txtcount]))
          {
              // check for upper-case letters
              if (isupper(text[txtcount]))
              {
                  // encrypt upper-case letters
                  int encrypted = text[txtcount] + argv[1][keycount];
                  keycount++;
                  // check for encrypting past 'Z'
                  if (encrypted >= 91)
                  {
                    // wrap around upper-case alphabet
                    encrypted = 65 + (encrypted % 91);
                    printf("%c", encrypted);
                  }
                  else
                  {
                    printf("%c", encrypted);
                  }
                  // check for incrementing past key legnth
                  if (keycount >= arglen)
                  {
                    // wrap around key
                    keycount = 0;
                  }
              }
              else
              {
                // check for lower-case letters
                if (islower(text[txtcount]))
                {
                    // encrypt lower-case letters
                    int encrypted = text[txtcount] + argv[1][keycount];
                    keycount++;
                    // check for encrypting past 'z'
                    if (encrypted >= 123)
                    {
                      // wrap around lower-case alphabet
                      encrypted = 97 + (encrypted % 123);
                      printf("%c", encrypted);
                    }
                    else
                    {
                      printf("%c", encrypted);
                    }
                    // check for incrementing past key legnth
                    if (keycount >= arglen)
                    {
                    // wrap around key
                    keycount = 0;
                    }
                }
              }
          }
          else
          {
            // display non-alphabetic characters w/o encryption
            printf("%c", text[txtcount]);
          }
      }
      printf("\n");
      return 0;
  }
}