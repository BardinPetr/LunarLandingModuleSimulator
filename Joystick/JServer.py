from multiprocessing import Process, Pipe, Queue
from subprocess import Popen, PIPE
from joystick import Joystick
import server_telem
import multiprocessing
import threading
import socket
import signal
import time
import json
import sys

millis = lambda: int(round(time.time() * 1000))

class JServer:
    def __init__(self, dict, rc):
        print ('Starting server')

        self.s = None
        self.dict = dict
        self.rc = rc

        try:
            self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.s.bind(('127.0.0.1', 59667))
            self.s.listen(1)
        except:
            print('Failed to start Server!')
            self.__init__(dict, rc)

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
                    pass
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
                    req = self.conn.recv(2000).decode("utf-8")
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
                    elif req.find('telemx') != -1 and req.find('@') != -1 and not req.find(':') != -1:
                        try:
                            req = str(req[7:])
                            req += '@' + '@'.join(map(str, self.rc._send)) + "@666"
                            server_telem.telem_broadcast(server_telem.tconn, req.encode('utf-8'))
                            print req
                        except:
                            pass
                    elif req.find('telem') != -1 and req.find('@') != -1:
                        try:
                            if req.split('@')[0] in self.dict:
                                fff = req.split('@')[1]
                                fff = json.loads(fff)
                                self.dict[req.split('@')[0]](fff)
                        except:
                            pass
                    elif req.find('@') != -1:
                        if req.split('@')[0] in self.dict:
                            fff = req.split('@')[1]
                            fff = json.loads(fff)
                            self.dict[req.split('@')[0]](fff)
                    else:
                        print("NOT RECOGNIZED! {}".format(req))
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
            server_telem.run = False
            server_telem.s.close()
            server_telem.athr.join()
        except:
            pass