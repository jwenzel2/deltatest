#challenge6.py 
#This script encrypts a file and or decrypts it or encrypts a string
#
#Created By: Jeremiah Wenzel
#Created On: 4/1/2022

#take care of imports
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

#ask user for a selection
print("Welcome User\n \
Please choose between options 1-4 or q to quit\n \
1. Encrypt a file\n \
2. Decrypt a file\n \
3. Encrypt a string\n \
4. Decrypt a string\n  \
q to quit\n")

#set condition for loop
sucess = False

selection = input("Please choose between 1-4 or q to quit: ")

if selection == '1':
    file = input("Enter the path of the file to be encrypted: ")
    #attempt to encrypt a file and throw an exception on failure
    buffer_size = 65536
    key = open('encrypt.key', 'rb')
    curkey = key.read()
    
    input_file = open(file, 'rb')
    output_file = file, '.enc'
    data = open(file, 'rb').read()
    cipher = AES.new(curkey, AES.MODE_CFB)
            
    buffer = open(file, 'rb').read(buffer_size)
    output_file = file + '.enc'
    file_out = open(output_file, 'wb')
    file_out.write(cipher.iv)
    while len(buffer) > 0:
        ciphered_bytes = cipher.encrypt(data)
        file_out.write(ciphered_bytes)
        buffer = input_file.read(buffer_size)
    file_out.close()
    print("Encrypted ", file)



elif selection == '2':
    file = input("Enter the path to the encrypted file to be decrypted: ")
    #attempt to decrypt a file and throw and exception on failure
    buffer_size = 65536 
    input_file = open(file, 'rb')
    output = input("file to decrypt as")
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

# Close the input and output files
    input_file.close()
    output_file.close()


elif selection == '3':
    string = input("Enter a string to be encrypted: ")

elif selection == '4':
    string = input("Enter an encrypted string to be decrypted: ")

elif selection == 'q':
    print("goodbye!")
    quit()