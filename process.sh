#!/bin/bash
#Script Name: process.sh
#Author: Jeremiah Wenzel
#Date Of Last Revision: 7/13/21
#variables me id
#
me=$(whoami)
#greet user
echo "Greetings here are your running tasks running as user: "$me
#list runing processes running as user
ps -U $me -u $me u
id=9

#prompt for processes to be killed untill 0 is entered to stop
while [ $id -ne 0 ]
do
    echo "what pid do you want me to kill? (0 to quit): "
    read id
    if [ $id -ne 0 ]
    then
        kill $id
        echo "killing pid: "$id
    fi
done