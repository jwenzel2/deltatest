#!/bin/bash
#Script Name: ip.sh
#Author: Jeremiah Wenzel
#Date Of Last Revision: 7/8/21
#variables used jack
#fetch network config and store in file
jack=$(ip a)

#display the output
echo $jack

#now lets send the same command to a file
ip a >> afile