// include libraries
#include <stdio.h>
#include <cs50.h>

int main(void)
{
  // initialize variables
  int height;
  // ask user for input
  do
  {
    printf("Height: ");
    height = get_int();
  }
  while (height < 0 || height > 23);
  // print spaces and hashes in shape of half-pyramid, using user input for pyramid's height
  for(int row = 1; row <= height; row++)
  {
    for(int space = height - row; space > 0; space--)
    {
      printf(" ");
    }
    for(int hash = height - row; hash <= height; hash++)
    {
      printf("#");
    }
    printf("\n");
  }
}