from copterUtils import CopterUtils
from JServer import JServer
from video import Video
from RC import RC
import time
import signal
import time
import json
import sys

#video = Video(False, False)
#video.newTelemetry({'alt': 41, 'roll': 24, 'pitch':16, 'yaw': 56, 'tg': 40})
#server = JServer({'telem': lambda x: video.newTelemetry(x)})

def newTelem(x):
    cu.armed = x['armed']

rc = RC(36)
server = JServer({'telem': newTelem}, rc)
cu = CopterUtils(rc, server)

def signal_term_handler(signal, frame):
    print 'got {}'.format(signal)
    del server
    del cu
    del rc
    sys.exit(0)
signal.signal(signal.SIGTERM, signal_term_handler)
signal.signal(signal.SIGINT, signal_term_handler)
signal.signal(signal.SIGBREAK, signal_term_handler)

while not server.isCliennt():
    print '', ''

thr = 1000
tConst = 10
mode = 2
while 1:
    #if not server.isCliennt():
    #    break
    #else:
        b = server.parseButtons(server.joy.getButtons())
        axis = server.joy.getAxis()
        if b["b1"] and b["joy"][0] and not cu.armed:
            cu.arm()
            thr = 1000
        if cu.armed:
            if b["b1"] and b["joy"][2]:
                cu.disarm()
                thr = 1000
            if b["b1"] and b["joy"][1]:
                mode = (1 if mode == 2 else 2)
                time.sleep(0.25)
            else:
                if mode == 1:
                    x = 0
                    y = 0
                    if b["joy"][2]:
                        x = 1600
                    elif b["joy"][3]:
                        x = 1400
                    else:
                        x = 1500

                    if b["joy"][1]:
                        y = 1600
                    elif b["joy"][0]:
                        y = 1400
                    else:
                        y = 1500

                    if b["b2"]:
                        thr -= (0 if (thr - tConst) < 1000 else tConst)
                    elif b["b1"]:
                        thr += (0 if (thr + tConst) > 2000 else tConst)
                    z = rc.translate(axis[2], -1.0, 1.0, 1000.0, 2000.0)
                elif mode == 2:
                    x = rc.translate(axis[0], -1.0, 1.0, 1000.0, 2000.0)
                    y = rc.translate(axis[1], -1.0, 1.0, 1000.0, 2000.0)
                    z = rc.translate(axis[2], -1.0, 1.0, 1000.0, 2000.0)
                    if b["b2"]:
                        thr -= (0 if (thr - tConst) < 1000 else tConst)
                    elif b["b1"]:
                        thr += (0 if (thr + tConst) > 2000 else tConst)
                else:
                    roll = (10 if axis[0] > 0.1 else (-10 if axis[0] < -0.1 else 0))
                    pitch = (10 if axis[1] > 0.1 else (-10 if axis[1] < -0.1 else 0))
                    yaw = (10 if axis[2] > 0.1 else (-10 if axis[2] < -0.1 else 0))

                    x = 0
                    y = 0
                    z = 0
                    thr = 0

                    rc.SendGimbal(0, roll)
                    rc.SendGimbal(1, pitch)
                    rc.SendGimbal(2, yaw)

                #  '{0},{1},{2},{3}'.format(j, i, thr, z)
                rc.SendRC(1, x)
                rc.SendRC(2, y)
                rc.SendRC(3, thr)
                rc.SendRC(4, z, True)
                time.sleep(0.06)