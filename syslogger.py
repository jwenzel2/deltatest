#syslogger
#
#Created By: Jeremiah Wenzel
#Created On: 4/30/2022

import logging

logging.basicConfig(format='%(message)s %(asctime)s', filename='python.log', filemode= 'a', level=logging.INFO)

passwd = 'Password'
passhash = hash(passwd)

password = input("Please enter the password: ")

hash = hash(password)

if hash == passhash:
    print("Login Sucess")
    logging.info("Sucessfull Login At:")
else:   
    logging.warning('Incorrect Password Attempt At:')
   
    