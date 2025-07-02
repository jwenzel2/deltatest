# pysigmaldetec
#
# Created By: Jeremiah Wenzel
# Created On: 5/8/2022

import os
import hashlib

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

userpath = input("Input path to directory to be scanned: ")
targetDir = os.listdir(userpath)
poscnt = 0
filecnt = 0


for index in range(len(targetDir)):
    num = 0
    filePath = os.path.join(userpath, targetDir[index])
    hashed = hash_file(filePath)
    hashedln = hashed + '\n'
    with open('signatures', 'r') as f:
        for line in f:
            #print(filePath, hashed, line)
            if (hashedln.__eq__(line)):
                print("positive")
                print(filePath, "Signature matched malicious signature base")
                print(filePath, hashed)
                poscnt += 1
    f.close()

    filecnt += 1
    index += 1