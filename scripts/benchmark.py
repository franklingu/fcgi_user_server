#!/usr/bin/python
import httplib
import urllib
import sys

def login(username, pswd):
	params = urllib.urlencode({'username': username, 'pswd': pswd})
	headers = {"Content-type": "application/x-www-form-urlencoded", "Accept": "text/plain"}
	conn = httplib.HTTPConnection("localhost")
	conn.request("POST", "/login", params, headers)
	r = conn.getresponse()
	if r.status != 200:
		print("login error")

if __name__ == '__main__':
	for _ in xrange(1,100):
		login(sys.argv[1], sys.argv[2])
