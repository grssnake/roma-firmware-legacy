build:
	ino build -m mega2560

upload:
	ino upload -m mega2560

serial:
	ino serial -b 57600

all: build upload
	echo building and uploading sketch

buildnew:
	ino build -m mega2560 -d /home/jandor/arduino-1.8.1

allnew: buildnew upload
	echo building with new arduino
