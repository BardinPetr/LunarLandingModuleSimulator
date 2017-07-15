from multiprocessing import Process, Pipe, Queue
from subprocess import Popen, PIPE
from joystick import Joystick
import multiprocessing
import threading
import socket
import signal
import time
import json

millis = lambda: int(round(time.time() * 1000))

class Killer:
    kill_now = False
    def __init__(self):
        signal.signal(signal.SIGINT, self.exit_gracefully)
        signal.signal(signal.SIGTERM, self.exit_gracefully)

    def exit_gracefully(self, signum, frame):
        self.kill_now = True

class JServer:
    def __init__(self, dict):
        print ('Starting server')

        self.s = None
        self.dict = dict
        try:
            self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.s.bind(('127.0.0.1', 59667))
            self.s.listen(1)
            #self.ts = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            #self.ts.bind(('127.0.0.1', 59666))
            #self.ts.listen(1)
        except:
            print('Failed to start Server!')
            self.__init__(dict)

        self.joy = Joystick()
        self.tm = millis()
        self.r = True
        self.conn = None
        self.d = False

        self.runt = threading.Thread(target=self.run)
        self.runt.start()

        self.pingt = threading.Thread(target=self.timeout)
        self.pingt.start()

        print ('Server started')

    def timeout(self):
        while self.r:

            if (millis() - self.tm) > 5000 and self.conn != None and not self.d:
                self.d = True
                try:
                    self.conn.close()
                    del self.conn
                    self.conn = None
                    self.d = False
                except:
                    print(''), ''
                print('CLIENT TIMEOUT. DISCONNECTING')

    def accept(self):
        self.d = True
        try:
            self.conn, self.addr = self.s.accept()
            print('Connected')
            self.tm = millis()
            self.d = False
        except:
            print('Failed to connect to client!')

    def run(self):
        while self.r:
            if self.d:
                continue

            if self.conn == None and not self.d:
                self.accept()
            else:
                if self.conn == None:
                    continue
                try:
                    #self.conn.send(b'1111')
                    req = self.conn.recv(1000).decode("utf-8")
                    self.tm = millis()
                    if req == 'getBtns':
                        self.conn.send('$'.join(map(str, self.joy.getButtons())).encode('utf-8'))
                    elif req == 'getAxis':
                        self.conn.send('$'.join(map(str, self.joy.getAxis())).encode('utf-8'))
                    elif req == 'cExit':
                        self.conn.close()
                        self.conn = None
                    elif req == 'exit':
                        self.r = False
                        break
                    elif req.find('telem') != -1 and req.find('@') != -1:
                        try:
                            #self.ts.send(req.encode('utf-8'))
                            if req.split('@')[0] in self.dict:
                                fff = req.split('@')[1]
                                fff = json.loads(fff)
                                self.dict[req.split('@')[0]](fff)
                        except:
                            print(''), ''
                    elif req.find('@') != -1:
                        if req.split('@')[0] in self.dict:
                            fff = req.split('@')[1]
                            fff = json.loads(fff)
                            self.dict[req.split('@')[0]](fff)
                    else:
                        print("NOT RECOGNIZED! {}".format(req))
                        self.conn.close()
                        self.conn = None
                except Exception as errr:
                    print('GET DATA ERROR {}'.format(errr))

    def send(self, cmd):
        if not self.conn:
            return
        try:
            self.conn.send(cmd.encode('utf-8'))
        except:
            print('SEND CMD ERROR')

    def isCliennt(self):
        return self.conn != None

    def parseButtons(self, e):
        return {"joy": e[:4], "b1": e[5], "b2": e[4]}

    def __delete__(self, instance):
        try:
            instance.s.close()
            del instance.s
            del instance.joy
            self.s.close()
            del self.s
            del self.joy
        except:
            print (''), ''