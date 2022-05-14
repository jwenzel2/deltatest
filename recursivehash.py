# scriopt to generate a list of hashes for cychallenge32.py
#
# Created By: Jeremiah Wenzel
# Created On: 5/8/2022

import os
import hashlib

userpath = input("Input path to directory to be scanned: ")
targetDir = os.listdir(userpath)


def hash_file(filename):
   """"This function returns the SHA-1 hash
   of the file passed into it"""

   # make a hash object
   h = hashlib.sha1()

   # open file for reading in binary mode
   with open(filename,'rb') as file:

       # loop till the end of the file
       chunk = 0
       while chunk != b'':
           # read only 1024 bytes at a time
           chunk = file.read(1024)
           h.update(chunk)

   # return the hex representation of digest
   return h.hexdigest()

for index in range(len(targetDir)):
    print("Scanning: ", targetDir[index])
    filePath = os.path.join(userpath, targetDir[index])



    if os.path.exists('signatures'):
        hashtable = open('signatures', 'a')
    else:
        hashtable = open('signatures', 'a')

    hashed = hash_file(filePath)
    print(filePath, "has a hash of:", hashed)
    hashtable.write(hashed)
    hashtable.write('\n')
    index += 1