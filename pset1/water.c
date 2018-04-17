// include libraries
#include <stdio.h>
#include <cs50.h>

int main(void)
{
  // initialize variables
  int minutes;
  // ask user for input
  do
  {
    printf("Minutes: ");
    minutes = get_int();
  }
  while (minutes <= 0);
  // output user input in terms of bottles of water
  printf("Bottles: %i\n", minutes*12);
}