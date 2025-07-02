$Begin = Get-Date -Date '7/12/2021 00:00:00'
$End = Get-Date -Date '7/19/2021 00:00:00'
write-host "here are all Logs between 7/19/2021 and 7/12/2021 sending to file"
Get-EventLog -LogName System -After $Begin -Before $End >> week.txt
write-host "here are all the ErrorsLogs between 7/19/2021 and 7/12/2021 Sending to file"
Get-EventLog -LogName System -EntryType Warning -After $Begin -Before $End >> weekerror.txt
write-host "here are all the logs from id 16 between 7/19/2021 and 7/12/2021"
Get-EventLog -LogName System -After $Begin -Before $End Where-Object {$_.EventID -eq 16} |
Select-Object -Property Source, EventID, InstanceId, Message
write-host "here are 20 most recent logs"
Get-EventLog -LogName System -Newest 20
write-host "here are 500 most recent logs"
Get-EventLog -LogName System -Newest 500