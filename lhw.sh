#!/bin/bash
#Script Name: lhw.sh
#Author: Jeremiah Wenzel
#Date Of Last Revision: 7/15/21
#Variables used: cprod cvend cid cbus cwid rdis rid rsize 
#ddesc dprod dvend dphys dbus dwid dclock dcap dconf dres
#ndesc nprod nvend nphys nbus nlog nver nser nsize ncap nwid
#nclock nconf nres
#get cpu info
cprod=$(lshw -class cpu | grep product)
cvend=$(lshw -class cpu | grep vendor)
cid=$(lshw -class cpu | grep physical)
cbus=$(lshw -class cpu | grep bus)
cwid=$(lshw -class cpu | grep width)
#get ram info
rdis=$(lshw -class memory | grep description)
rid=$(lshw -class memory | grep physical)
rsize=$(lshw -class memory | grep size)
#get Display adapter info
ddesc=$(lshw -class display | grep description)
dprod=$(lshw -class display | grep product)
dvend=$(lshw -class display | grep vendor)
dphys=$(lshw -class display | grep physical)
dbus=$(lshw -class display | grep bus)
dwid=$(lshw -class display | grep width)
dclock=$(lshw -class display | grep clock)
dcap=$(lshw -class display | grep capabilities)
dconf=$(lshw -class display | grep configuration)
dres=$(lshw -class display | grep resources)
#get Network Adaptor info
ndesc=$(lshw -class network | grep description)
nprod=$(lshw -class network | grep product)
nvend=$(lshw -class network | grep vendor)
nphys=$(lshw -class network | grep physical)
nbus=$(lshw -class network | grep bus)
nlog=$(lshw -class network | grep logical)
nver=$(lshw -class network | grep version)
nser=$(lshw -class network | grep serial)
nsize=$(lshw -class network | grep size)
ncap=$(lshw -class network | grep capabilities)
nwid=$(lshw -class network | grep width)
nclock=$(lshw -class network | grep clock)
nconf=$(lshw -class network | grep configuration)
nres=$(lshw -class network | grep resources)
#Display results
echo "CPU Information"
echo "------------------------------------"
echo -e $cprod'\n'$cvend'\n'$cid'\n'$cbus'\n'$cwid
echo "Ram Information"
echo "------------------------------------"
echo -e $rdis'\n'$rid'\n'$rsize
echo "Display Adapter Information"
echo "------------------------------------"
echo -e $ddesc'\n'$dprod'\n'$dvend'\n'$dphys'\n'$dbus'\n'$dwid'\n'$dclock'\'$dcap'\n'$dconf'\n'$dres
echo "Network Adaptor Information"
echo "------------------------------------"
echo -e $ndesc'\n'$nprod'\n'$nvend'\n'$nphys'\n'$nbus'\n'$nlog'\n'$nver'\n'$nser'\n'$nsize'\n'$ncap'\n'$nwid'\n'$nclock'\n'$nconf'\n'$nres