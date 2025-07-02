#sshbrute.ssh
#
#Created By: Jeremiah Wenzel
#Created ON: 4/12/2022

import os
import zipfile 
count = 0
zippath = input("Please Enter Zipfile path: ")
my_zip_file = zipfile.ZipFile(zippath)
if os.path.exists('rockyou.txt'):
    with open('rockyou.txt' , 'rb') as passwords:
        password_list = passwords.readlines()
        my_zip_file = zipfile.ZipFile(zippath)
        for index, password in enumerate(password_list):
            try:
                my_zip_file.extractall(path="sucess" , pwd=password.strip())
                print("Password Found:" , password.decode().strip())
                print("All Files has been Extracted inside the sucess Folder")
                break
            except:
                print("The Password: ", password.decode().strip(), "Was incorrect!")
else:
    print("try again when you actually have the rockyou.txt wordlist in same directory as this script!")   