import threading
import socket
import signal
import time
import json

def accept(tc, s, run):
    while run:
        print 'telem waiting'
        try:
            conn, addr = s.accept()
            tc += [conn]
            print('Video Connected')
        except:
            print('Failed to connect to client!')

def telem_broadcast(tc, d):
    iiii = 0
    for s in tc:
        try:
            s.send(d.encode('utf-8'))
            iiii+=1
        except:
            tc.pop(iiii)

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(('127.0.0.1', 59666))
s.listen(100)
tconn = []
run = True
athr = threading.Thread(target=accept, args=(tconn, s, run))
athr.start()