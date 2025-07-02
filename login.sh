#!/bin/bash
#Script Name: loginlog.sh
#Author: Jeremiah Wenzel
#Date Of Last Revision: 7/9/21
#variables used user
#ask for user to lookup log history
echo "enter user to lookup: "
#get the requestd user
read user

#send user to function to lookup said user and display login logs
user_lookup (){
name=$user
#display log of requestd user
command="last $name"
$command
}

user_lookup $user