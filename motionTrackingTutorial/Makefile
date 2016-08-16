CC = g++

LIBS = -lopencv_imgproc -lopencv_highgui -lopencv_objdetect -lopencv_core -lopencv_videoio 

motionTracking:
		$(CC) -I/usr/include/ motionTracking.cpp -o motionTracking.out $(LIBS)

tutorial_multitracker:
		$(CC) -I/usr/include/ tutorial_multitracker.cpp -o tutorial_multitracker.out $(LIBS) -lopencv_tracking
