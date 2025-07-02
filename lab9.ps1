#get user name
$Fname = Read-Host -Propmpt 'Enter Full Name:'
$Uname = Read-Host -Prompt 'Enter User Name:'
$Pwd = Read-Host -AssecureString -Prompt 'Enter Password:'
#Create Local User
New-LocalUser -Name $Uname -Password $Pwd -FullName $Fname -Description "user of computer"
Add-LocalGroupMember -Group "Users" -Member "d.schrute"

#download software
Invoke-WebRequest -Uri "https://cdn06.foxitsoftware.com/product/phantomPDF/desktop/win/11.0.0/FoxitEditor110_enu_Setup_Website.exe" -OutFile FoxitEditor110_enu_Setup_Website.exe
Invoke-WebRequest -Uri "Invoke-WebRequest -Uri "https://download.documentfoundation.org/libreoffice/stable/7.1.4/win/x86_64/LibreOffice_7.1.4_Win_x64.msi" -OutFile LibreOffice_7.1.4_Win_x64.msi" -OutFile LibreOffice_7.1.4_Win_x64.msi
Invoke-WebRequest -Uri "https://download.mozilla.org/?product=thunderbird-78.12.0-SSL&os=win64&lang=en-US" -OutFile thunderbird.exe

#install software
./FoxitEditor110_enu_Setup_Website.exe
./LibreOffice_7.1.4_Win_x64.msi
./thunderbird.exe
#setup rdp
Set-ItemProperty -Path 'HKLM:\System\CurrentControlSet\Control\Terminal Server' -Name "fDenyTSConnections" -value 0
Enable-NetFirewallRule -DisplayGroup "Remote Desktop"
Set-ItemProperty -Path 'HKLM:\SYSTEM\CurrentControlSet\Control\Terminal Server\WinStations\RDP-Tcp\' -Name “UserAuthentication” -Value 1
Add-LocalGroupMember -Group "Remote Desktop Users" -Member $Uname
