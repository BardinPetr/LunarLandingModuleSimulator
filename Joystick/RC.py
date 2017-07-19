import serial
import time
import threading
import socket
import signal
import time
import json

class RC:
    def __init__(self, port):
        self.chan = [None, b'1500', b'1500', b'1000', b'1500', b'1000', b'0', b'0', b'0', b'0']
        self.ser = serial.Serial("COM" + str(port), 57600)
        time.sleep(2)
        self.SendRC(1, 1500, True)

        self.r = True

        self.pingt = threading.Thread(target=self.run)
        self.pingt.start()

        self.gimbal = [0.0, 0.0, 0.0]

    def run(self):
        while self.r:
            line = self.ser.readline()
            self.gimbal = line.split('@')

    def SendRC(self, id, val, writenow = False):
        self.chan[id] = str(val)

        if writenow:
            e = ("!" + '@'.join(self.chan[1:]) + '\r').encode('utf-8')
            print e
            self.ser.write(e)

    def SendGimbal(self, id, val, writenow = False):
        self.SendRC(id + 6, val, writenow)

    def SendSrv(self, val, writenow = False):
        self.SendRC(9, val, writenow)

    def SendRC_s(self, id, val, writenow = False):
        val = self.translate(val, -1000, 1000, 1000, 2000)
        self.SendRC(id, val, writenow)

    def __delete__(self, instance):
        try:
            instance.ser.close()
            self.ser.close()
        except:
            print 'RC exit error'

    def translate(self, value, leftMin, leftMax, rightMin, rightMax):
        leftSpan = leftMax - leftMin
        rightSpan = rightMax - rightMin
        valueScaled = float(value - leftMin) / float(leftSpan)
        return int(rightMin + (valueScaled * rightSpan))