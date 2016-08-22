CC = g++

LIBS = -lopencv_imgproc -lopencv_highgui -lopencv_objdetect -lopencv_core -lopencv_videoio 

motionTracking:
		$(CC) -I/usr/include/ motionTracking.cpp -o motionTracking.out $(LIBS)

motionTracking_modified:
		$(CC) -I/usr/include/ motionTracking_modified.cpp -o motionTracking_modified.out $(LIBS)

tutorial_multitracker:
		$(CC) -I/usr/include/ tutorial_multitracker.cpp -o tutorial_multitracker.out $(LIBS) -lopencv_tracking

count_things:
		$(CC) -I/usr/include/ count_things.cpp -o count_things.out $(LIBS) -lopencv_tracking

