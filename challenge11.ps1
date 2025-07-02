#setup filesharing
Set-NetFirewallRule -DisplayGroup "File And Printer Sharing" -Enabled True -Profile Any
#enable localized pings
New-NetFirewallRune -DisplayName "ICMPv4" -Direcion Inbount -Protocol ICMPv4 -IcmpType 8 -RemoteAddress <localsubnet> -Action Allow
#enable remote desktop
Set-ItemProperty -Path 'HKLM:\System\CurrentControlSet\Control\Terminal Server' -name "fDenyTSConnections" -value 0
Enable-NetFirewallRule -DisplayGroup "Remote Desktop"

Enable-WindowsOptionalFeature -Online -FeatureName Microsoft-Hyper-V -All

Set-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\Services\LanmanServer\Parameters" SMB1 -Type DWORD -Value 0 -Force