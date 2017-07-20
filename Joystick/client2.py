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

print ('Script started')
cl = JClient({b"arm": lambda: MAV.doARM(True), b"disarm": lambda: MAV.doARM(False), b"stabilize": lambda: Script.ChangeMode("STABILIZE")})
print('Started')

'''
    2
0       1
    3 
'''
gimbal = [0.0, 0.0, 0.0]
while 1:
    sTime = time.time()
    cl.sendTelemetry({'roll': -cs.roll,
                      'pitch': cs.pitch,
                      'yaw': cs.yaw,
                      'lat': cs.lat,
                      'lng': cs.lng,
                      'alt': cs.alt,
                      'gpsst': cs.gpsstatus,
                      'gpssatcnt': cs.satcount,
                      'gspeed': cs.groundspeed,
                      'vspeed': cs.verticalspeed,
                      'ax': cs.ax,
                      'ay': cs.ay,
                      'az': cs.az,
                      'gx': cs.gx,
                      'gy': cs.gy,
                      'gz': cs.gz,
                      'mx': cs.mx,
                      'my': cs.my,
                      'mz': cs.mz,
                      'mode': cs.mode,
                      'batt': cs.battery_voltage,
                      'battrem': cs.battery_remaining,
                      'battcur': cs.current,
                      'armed': cs.armed,
                      'reserved': 666 })
    cl.sendTelemetryx([ round(-cs.roll, 2),round(cs.pitch, 2),round(cs.yaw, 2),cs.lat,cs.lng,cs.alt,cs.gpsstatus,cs.satcount,round(cs.groundspeed, 2),round(cs.verticalspeed, 2),round(cs.ax, 2),round(cs.ay, 2),round(cs.az, 2),round(cs.gx, 2),round(cs.gy, 2),round(cs.gz, 2),cs.mx,cs.my,cs.mz,cs.mode,cs.battery_voltage,cs.battery_remaining,cs.current,cs.armed])
    #print('time: {}'.format(time.time() - sTime))
    Script.Sleep(20)

del cl
