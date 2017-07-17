from copterUtils import CopterUtils
from JServer import JServer
from video import Video
from RC import RC
import time

#video = Video(False, False)
#video.newTelemetry({'alt': 41, 'roll': 24, 'pitch':16, 'yaw': 56, 'tg': 40})
#server = JServer({'telem': lambda x: video.newTelemetry(x)})

def newTelem(x):
    cu.armed = x['armed']

rc = RC(1)
server = JServer({'telem': newTelem})
cu = CopterUtils(rc, server)

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
                else:
                    i = rc.translate(axis[0], -1.0, 1.0, 1000.0, 2000.0)
                    j = rc.translate(axis[1], -1.0, 1.0, 1000.0, 2000.0)

                if b["b2"]:
                    thr -= (0 if (thr - tConst) < 1000 else tConst)
                elif b["b1"]:
                    thr += (0 if (thr + tConst) > 2000 else tConst)

                z = rc.translate(axis[2], -1.0, 1.0, 1000.0, 2000.0)
                #  '{0},{1},{2},{3}'.format(j, i, thr, z)
                rc.SendRC(2, j, False)
                rc.SendRC(1, i, False)
                rc.SendRC(3, thr, False)
                rc.SendRC(4, z, True)
                time.sleep(0.06)