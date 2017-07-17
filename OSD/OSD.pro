TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    tclient.cpp

INCLUDEPATH += "D:\opencv3\build\install\include"

LIBS += "D:\opencv3\build\bin\libopencv_core320.dll"
LIBS += "D:\opencv3\build\bin\libopencv_highgui320.dll"
LIBS += "D:\opencv3\build\bin\libopencv_imgcodecs320.dll"
LIBS += "D:\opencv3\build\bin\libopencv_imgproc320.dll"
LIBS += "D:\opencv3\build\bin\libopencv_features2d320.dll"
LIBS += "D:\opencv3\build\bin\libopencv_calib3d320.dll"

LIBS += "D:\opencv3\build\bin\libopencv_video320.dll"
LIBS += "D:\opencv3\build\bin\libopencv_videoio320.dll"
LIBS += "D:\opencv3\build\bin\opencv_ffmpeg320.dll"

LIBS += -lws2_32

HEADERS += \
    tclient.h
