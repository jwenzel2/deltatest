#passchecker.py
#For Challenge16
#
#Created By: Jeremiah Wenzel
#Created On: 4/11/2022

#deal with imports
import os

#get password to be checked
pwstring = input("Enter password you want to check: ")

#defensive or offensive
question = input("Your Selections \
1. Offensive \
2. Defensive \
q. Quit \
Choose 1 or 2 or q: ")

if question == '1':
    if os.path.exists('rockyou.txt'):
        lines = 0
        with open('rockyou.txt', 'r', encoding='ANSI') as fp:
            for line in fp:
                word = line.rstrip('\n')
                if pwstring == word:
                    flag = 1
            fp.close()
        if flag == 0:
            print("Password Not In Wordlist!")
            quit()
        if flag == 1:
            print("Password Found In Wordlist!")
            quit()
    else:
        print("try again when you actually have the rockyou.txt wordlist in same directory as this script!")        
if question == '2' or 2:
    path = input("Enter path of Wordlist: ")
    if 'rockyou.txt' in path:
        wordlist = open(path, 'r', encoding='ANSI')
    else:
        wordlist = open(path, 'r')
    index = 0;
    flag = 0;
    for line in wordlist:
        index += 1
        if pwstring in line:
            flag = 1
    if flag == 0:
        print("Password Not In Wordlist!")
    if flag == 1:
        print("Password Found In Wordlist!")
        quit()
if question == 'q' or question == 'Q':
    quit()
