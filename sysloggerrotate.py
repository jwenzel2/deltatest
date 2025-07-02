# syslogger
##
# Created By: Jeremiah Wenzel#
# Created On: 4/30/2022#
#
import logging
import time
from logging.handlers import TimedRotatingFileHandler

logging.basicConfig(format='%(message)s %(asctime)s', filename='python.log', filemode='a', level=logging.INFO)


def create_timed_rotating_log(path):
    """"""
    logger = logging.getLogger("Rotating Log")
    logger.setLevel(logging.INFO)

    handler = TimedRotatingFileHandler(path,
                                       when="d",
                                       interval=1,
                                       backupCount=5)
    logger.addHandler(handler)

# ----------------------------------------------------------------------
if __name__ == "__main__":
    log_file = "python.log"
    create_timed_rotating_log(log_file)
passwd = 'Password'
passwdhash = hash(passwd)

password = input("Please enter the password: ")

hashed = hash(password)

if hashed == passwdhash:
    print("Login Sucess")
    logging.info("Sucessfull Login At:")
else:
    print("Login Failed Your Attempt has Been Logged")
    logging.warning('Incorrect Password Attempt At:')
   
    