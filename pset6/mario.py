# import modules
import cs50

# ask user for input
while True:
    print("Height: ", end = "")
    height = cs50.get_int()
    if not height < 0 and not height > 23:
        break

# print half-pyramid
for row in range(height):
    print(" " * (height - row - 1), end = "")
    print("#" * (2 + row), end = "")
    print()