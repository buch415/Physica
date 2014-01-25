#!/bin/bash

clear

echo Building...
g++ -w src/*.cpp src/PugiXML/*.cpp src/TinyJS/*.cpp -obin/test -L/usr/local/lib -lSDL2main -lSDL2 -lSDL2_image -lSDL2_gfx -lSDL2_ttf

if [ $? = 0 ]
then
	echo Build successful.
	echo Now running...
	
	cd ./bin
	./test
	cd ..
	
	echo $?
	
	echo LOG
	
	cat ./bin/output.log
fi
