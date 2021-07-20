#show processes in order of cpu usage
Get-Process | sort cpu -Descending
#show processes in order of PID
Get-Process | sort id -Descending
#show first 5 processes with highest ws(k)
Get-Process | sort ws -Descending | select -First 5
#launch edge and point to a website
Start-Process "C:\Program Files (x86)\Microsoft\Edge\Application\msedge.exe " "https://owasp.org/www-project-top-ten/"
#do it again 10 times
$b = 1
while($b -ne 10)
{
    Start-Process "C:\Program Files (x86)\Internet Explorer\iexplore.exe" "https://owasp.org/www-project-top-ten/" 
    $b++
}

while ($b -ne 1)
{
    Stop-Process -name iexplore
    $b--
}
