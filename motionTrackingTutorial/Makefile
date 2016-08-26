CC = g++

LIBS = -lopencv_imgproc -lopencv_highgui -lopencv_objdetect -lopencv_core -lopencv_videoio 

motionTracking: motionTracking.cpp 
		$(CC) -I/usr/include/ motionTracking.cpp -o motionTracking.out $(LIBS)

motionTracking_modified: motionTracking_modified.cpp
		$(CC) -I/usr/include/ motionTracking_modified.cpp -o motionTracking_modified.out $(LIBS)

tutorial_multitracker:tutorial_multitracker.cpp
		$(CC) -I/usr/include/ tutorial_multitracker.cpp -o tutorial_multitracker.out $(LIBS) -lopencv_tracking

count_things: count_things.cpp
		$(CC) -I/usr/include/ count_things.cpp -o count_things.out $(LIBS) -lopencv_tracking

using_blobs: using_blobs.cpp
		g++ `pkg-config opencv cvblob --cflags --libs` using_blobs.cpp -o using_blobs

