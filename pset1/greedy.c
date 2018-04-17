// include libraries
#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
  // initialize variables
  float dollars;
  int count = 0;
  // ask user for input
  do
  {
    printf("O hai! How much change is owed?\n");
    dollars = get_float();
  }
  while (dollars < 0);
  // round and convert dollar value to cent value
  int cents = roundf(dollars*100);
  // determine how many quarters can be used in current/remaining cent value
  while (cents >= 25)
  {
      cents = cents - 25;
      count++;
  }
  // determine how many dimes can be used in current/remaining cent value
  while (cents >= 10)
  {
      cents = cents - 10;
      count++;
  }
  // determine how many nickels can be used in current/remaining cent value
  while (cents >= 5)
  {
      cents = cents - 5;
      count++;
  }
  // determine how many pennies can be used in current/remaining cent value
  while (cents >= 1)
  {
      cents = cents - 1;
      count++;
  }
  // output the number of coins used
  printf("%i\n", count);
}