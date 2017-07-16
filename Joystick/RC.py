import serial
import time

class RC:
    def __init__(self, port):
        self.chan = [None, b'1500', b'1500', b'1000', b'1500', b'1000']
        self.ser = serial.Serial("COM" + str(port), 57600)
        time.sleep(2)
        self.SendRC(1, 1500, True)

    def SendRC(self, id, val, writenow = False):
        self.chan[id] = str(val)

        if writenow:
            e = ("!" + '@'.join(self.chan[1:]) + '\r').encode('utf-8')
            print e
            self.ser.write(e)

    def SendRC_s(self, id, val, writenow = False):
        val = self.translate(val, -1000, 1000, 1000, 2000)
        self.SendRC(id, val, writenow)

    def __delete__(self, instance):
        instance.ser.close()

    def translate(self, value, leftMin, leftMax, rightMin, rightMax):
        leftSpan = leftMax - leftMin
        rightSpan = rightMax - rightMin
        valueScaled = float(value - leftMin) / float(leftSpan)
        return int(rightMin + (valueScaled * rightSpan))