import time

class CopterUtils:
    def __init__(self, rc, server):
        self.rc = rc
        self.server = server
        for chan in range(1, 5):
            if chan == 3:
                continue
            self.rc.SendRC(chan, 1500, False)
        self.rc.SendRC(3, 1000, True)

        self.armed = False

    def arm(self, mode = "STABILIZE"):
        #self.server.send(mode.lower())
        print('Arming')

        self.rc.SendRC(1, 1500, False)
        self.rc.SendRC(2, 1500, False)
        self.rc.SendRC(3, 1000, False)
        self.rc.SendRC(4, 2000, True)

        time.sleep(6)

        self.rc.SendRC(1, 1500, False)
        self.rc.SendRC(2, 1500, False)
        self.rc.SendRC(3, 1000, False)
        self.rc.SendRC(4, 1500, True)

        #self.server.send("arm")
        print('Armed')
        self.armed = True

    def disarm(self):
        #self.setThr(1000)
        #self.server.send("stabilize")
        print('Disarming')

        self.rc.SendRC(1, 1500, False)
        self.rc.SendRC(2, 1500, False)
        self.rc.SendRC(3, 970, False)
        self.rc.SendRC(4, 2000, True)

        time.sleep(10)

        self.rc.SendRC(1, 1500, False)
        self.rc.SendRC(2, 1500, False)
        self.rc.SendRC(3, 970, False)
        self.rc.SendRC(4, 1500, True)

        #self.server.send("disarm")
        print('Disarmed')
        self.armed = False

    def RCcal(self):
        for chan in range(1, 15):
            for i in range(1000, 2001, 100):
                print("{0}.{1}".format(chan, i))
                self.rc.SendRC(chan, i, True)
                time.sleep(50)

    def setThr(self, e=1500):
        self.rc.SendRC(3, e, False)