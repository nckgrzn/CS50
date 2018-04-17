# import modules
import cs50
import sys

# verify that user has input only two command-line arguments
if len(sys.argv) == 2:

    # ask user for input
    print("plaintext: ", end = "")
    text = cs50.get_string()

    # safety check for acceptable input
    if not text == None:
        print("ciphertext: ", end = "")

        # traverse through characters in input
        for i in range(len(text)):

            # check for alphabetical characters
            if text[i].isalpha():

                # check for upper-case letters
                if text[i].isupper():

                    # encrypt upper-case letters
                    encrypted = ord(text[i]) + int(sys.argv[1]) % 26

                    # check for encrypting past 'Z'
                    if encrypted >= 91:

                        # wrap around upper-case alphabet
                        encrypted = 65 + (int(encrypted) % 91)
                        print(chr(encrypted), end = "")
                    else:
                        print(chr(encrypted), end = "")

                # check for upper-case letters
                elif text[i].islower():

                    # encrypt lower-case letters
                    encrypted = ord(text[i]) + int(sys.argv[1]) % 26

                    # check for encrypting past 'z'
                    if encrypted >= 123:

                        # wrap around upper-case alphabet
                        encrypted = 97 + (int(encrypted) % 123)
                        print(chr(encrypted), end = "")
                    else:
                        print(chr(encrypted), end = "")
            else:

                # display non-alphabetic characters w/o encryption
                print(text[i], end = "")
        print()
else:

    # display error message
    print("Please enter two command-line arguments only.")
