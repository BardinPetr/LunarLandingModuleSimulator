from copterUtils import CopterUtils
from JServer import JServer
from video import Video
from RC import RC
import time

#video = Video(False, False)
#video.newTelemetry({'alt': 41, 'roll': 24, 'pitch':16, 'yaw': 56, 'tg': 40})
#server = JServer({'telem': lambda x: video.newTelemetry(x)})

rc = RC(31)
server = JServer({})
cu = CopterUtils(rc, server)

#while not server.isCliennt():
#    print 'waiting'

thr = 1000
tConst = 1
while 1:
    #if not server.isCliennt():
    #    break
    #else:
        b = server.parseButtons(server.joy.getButtons())
        axis = server.joy.getAxis()
        if b["b1"] and b["joy"][0] and not cu.armed:
            cu.arm()
        if cu.armed:
            if b["b1"] and b["joy"][2]:
                cu.disarm()
            else:
                #rc.SendRC(1, 1500, False)
                #rc.SendRC(2, 1500, False)
                #rc.SendRC(3, 1200, False)
                #rc.SendRC(4, 1500, True)
                #time.sleep(0.2  )
                #continue
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

                z = rc.translate(axis[2], -1.0, 1.0, 1000.0, 2000.0)
                print '{0},{1},{2},{3}'.format(j, i, thr, z)
                rc.SendRC(1, j, False)
                rc.SendRC(2, i, False)
                rc.SendRC(3, thr, False)
                rc.SendRC(4, z, True)
                time.sleep(0.06)