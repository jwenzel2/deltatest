#!/bin/bash
#Script Name: iparray.sh
#Author: Jeremiah Wenzel
#Date Of Last Revision: 7/12/21
#variables used mdir cdir
#create four directories
mkdir dir1 dir2 dir3 dir4
cdir=$(pwd)
#declare array and point each element to a directory
mydir=($cdir/dir1 $cdir/dir2 $cdir/dir3 $cdir/dir4)
a=0
#create a file in each directory and announce it
while [ $a -ne 4 ]
do
    echo "creating myfile in "${mydir[$a]}
    touch ${mydir[$a]}/myfile
    ((a=a+1))
done
