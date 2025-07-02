#!/bin/bash
#Script Name: process.sh
#Author: Jeremiah Wenzel
#Date Of Last Revision: 7/14/21
#Variables used: file id
file=$(pwd)/testfile
#if file doesnt exist create it then spam with with numbers
if [ -f $file ] 
then 
rm testfile
id=1
    while [ $id -ne 667 ]
    do
    echo "test"
        touch $file
        echo "$id" >> $file
        ((id=$id + 1))
    done
else   
    id=1
    while [ $id -ne 667 ]
    do
    echo "test"
        touch $file
        echo "$id" >> $file
        ((id=$id + 1))
    done
    fi