#cookiethief.py
#
#Created By: Jeremiah Wenzel
#Created On: 5/14/2022

#deal with imports
import requests

#get input
URL = input("Enter The Url: ")

#lets get some requests

response = requests.get(URL)
response.cookies
for cookie in response.cookies:
    print('cookie domain = ' + cookie.domain)
    print('cookie name = ' + cookie.name)
    print('cookie value = ' + cookie.value)