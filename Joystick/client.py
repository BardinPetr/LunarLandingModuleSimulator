import sys, time
sys.path.append(r"c:\Users\Petr\Desktop\LunarLandingModuleSimulator\Joystick")

from JClient import JClient

cl = JClient()

print('Started')

while 1:
    axis = cl.getAxis()
    btns = cl.getBtns()
    print("{0}\t{1}".format(axis, btns))
    time.sleep(0.1)
del cl