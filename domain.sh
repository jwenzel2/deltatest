#!/bin/bash
#Script Name: domain.sh
#Author: Jeremiah Wenzel
#Date Of Last Revision: 7/21/21
#variables used mdir cdir
#create four directories

echo "enter a domain to lookup"
read domain
echo "running whois on " $domain
whois $domain
echo "running dig on " $domain
dig $domain
echo "running host on " $domain
host $domain
echo "running nslookup on " $domain
nslookup $domain