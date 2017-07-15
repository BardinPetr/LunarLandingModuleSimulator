import sys, time
sys.path.append(r"c:\Users\Petr\Desktop\LunarLandingModuleSimulator\Joystick")
sys.path.append(r"c:\Users\Petr\Anaconda2\Lib")
sys.path.append(r"c:\Users\Petr\Anaconda2\Lib\site-packages")

import clr
clr.AddReference("MAVLink")

from MAVLink import mavlink_command_long_t
from System import Byte
import MissionPlanner
import MAVLink

from JClient import JClient

####################################
def translate(value, leftMin, leftMax, rightMin, rightMax):
    leftSpan = leftMax - leftMin
    rightSpan = rightMax - rightMin
    valueScaled = float(value - leftMin) / float(leftSpan)
    return rightMin + (valueScaled * rightSpan)
'''
class CopterUtils:
    def __init__(self, rc, wGPS = False):
        self.rc = rc
        for chan in range(1, 9):
            if chan == 3:
                continue
            self.rc.SendRC(chan, 1500, False)
        self.rc.SendRC(3, 1000, True)

        while cs.lat == 0 and wGPS:
            print('Waiting for GPS')
            Script.Sleep(1000)

        self.armed = False

    def arm(self):
        #Script.ChangeMode("ALTHOLD")
        self.setThr(1000)
        print('Arming')
        MAV.doARM(True)
        #self.setThr()
        print('Armed')
        self.armed = True

    def disarm(self):
        self.setThr(1000)
        Script.ChangeMode("STABILIZE")
        print('Disarming')
        MAV.doARM(False)
        print('Disarmed')
        self.armed = False

    def delay(self, w):
        Script.Sleep(w)

    def land(self):
        print('Landing')
        Script.ChangeMode("LAND")

    def takeoff(self, l=10):
        Script.ChangeMode("ALTHOLD")
        self.setThr(1600)
        self.delay(4000)
        self.setThr()

    def RCcal(self):
        for chan in range(1, 15):
            for i in range(1000, 2001, 100):
                print("{0}.{1}".format(chan, i))
                self.rc.SendRC(chan, i, True)
                self.delay(100)

    def setThr(self, e=1500):
        self.rc.SendRC(3, e, True)

    def parseButtons(self, e):
        return {"joy":e[:4], "b1": e[5], "b2": e[4]}

'''
####################################

print ('Script started')

#cu = CopterUtils(False)
cl = JClient({b"arm": lambda: MAV.doARM(True), b"disarm": lambda: MAV.doARM(False)})

#while 1:
#    cu.RCcal()
#    cu.delay(50)

mode = 1
thr = 1000
tConst = 10
print('Started')

'''
    2
0       1
    3 
'''

while 1:
    '''
    st = time.time()
    axis = cl.getAxis()
    btns = cl.getBtns()

    if len(axis) != 3 or len(btns) != 6:
        print('SERVER ERROR')
        continue
    b = cu.parseButtons(btns)

    #x = translate(axis[0], -1.0, 1.0, 1000.0, 2000.0)
    #y = translate(axis[1], -1.0, 1.0, 1000.0, 2000.0)
    #z = translate(axis[2], -1.0, 1.0, 1000.0, 2000.0)

    #self.rc.SendRC(1, x, False)
    #self.rc.SendRC(2, y, True)
    #self.rc.SendRC(4, z, False)

    #f = translate(0.0 if axis[1] >= 0.0 else axis[1], -1.0, 0.0, 2000.0, 1300.0)
    #self.rc.SendRC(3, f, True)

    if b["b1"] and b["joy"][0] and not cu.armed:
        cu.arm()
    if cu.armed:
        if b["b1"] and b["joy"][2]:
            cu.disarm()

        if mode == 1:
            i = 0
            j = 0
            if b["joy"][2]:
                i = 1600
            elif b["joy"][3]:
                i = 1400
            else:
                i = 1500

            if b["joy"][1]:
                j = 1600
            elif b["joy"][0]:
                j = 1400
            else:
                j = 1500

            if b["b2"]:
                thr -= (0 if (thr - tConst) < 1000 else tConst)
            elif b["b1"]:
                thr += (0 if (thr + tConst) > 2000 else tConst)

            z = translate(axis[2], -1.0, 1.0, 1000.0, 2000.0)

            rc.SendRC(1, j, False)
            rc.SendRC(2, i, False)
            rc.SendRC(3, thr, False)
            rc.SendRC(4, z, True)
        elif mode == 2:
            if b["b1"]:
                cu.setThr(1700)
            elif b["b2"]:
                cu.setThr(1300)
            else:
                cu.setThr()
    '''
    cl.sendTelemetry({'alt': cs.alt, 'roll': -cs.roll, 'pitch': cs.pitch, 'yaw': cs.yaw, 'tg': 40})
    Script.Sleep(20)

del cl
