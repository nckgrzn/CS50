# import modules
import cs50

# ask user for input
while True:
    print("O hai! How much change is owed?")
    dollars = cs50.get_float()
    if not dollars < 0:
        break

# round and convert dollar value to cent value
cents = round(dollars * 100)

# initialize count
count = 0

# determine how many quarters can be used in current cent value
while (cents >= 25):
      cents -= 25
      count += 1

# determine how many dimes can be used in current cent value
while (cents >= 10):
  cents -= 10
  count += 1

# determine how many nickels can be used in current/remaining cent value
while (cents >= 5):
  cents -= 5
  count += 1

# determine how many pennies can be used in current/remaining cent value
while (cents >= 1):
  cents -= 1
  count += 1

# output the number of coins used
print(count)