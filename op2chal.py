#pingedpython
#created By: Jeremiah Wenzel
#created on 3/26/2022
import subprocess
#get network address    
network = input("enter network portion of your ip range end with a period: EX 172.16.1.: ")

#get start jpst
hstart = input("enter first host number 0-255: ")
hend = input("enter last host number 0-255: ")

#convert to int
hstartnum = int(hstart)
hendnum = int(hend)

# Configure subprocess to hide the console window
info = subprocess.STARTUPINFO()
#info.dwFlags |= subprocess.STARTF_USESHOWWINDOW
#info.wShowWindow = subprocess.SW_HIDE
offcnt = 0
oncnt = 0
# ping the range of hosts
while hstartnum != hendnum + 1:
    current = network + str(hstartnum)

    output = subprocess.Popen(['ping', '-n', '1', '-w', '500', current ], stdout=subprocess.PIPE, startupinfo=info).communicate()[0]
    if "Destination host unreachable" in output.decode('utf-8'):
        offcnt += 1
        print(current + " is offline")
    elif "Request timed out" in output.decode('utf-8'):
        print(current + " is offline")
        offcnt += 1
    else:
        oncnt += 1 
        print(current + " is online")
    hstartnum += 1

#print out how many hosts are online   
offcntstr = str(offcnt)
oncntstr = str(oncnt)
print(offcntstr + " hosts offline")
print(oncntstr + " was online")