#!/bin/bash

if (test "$1" = "build"); then
	rm -f taibu || exit 1
	gcc taibu.c $(pkg-config --cflags --libs gtkwidget2 libfile vte jansson) -o taibu || exit 1
	
elif (test "$1" = "install"); then
	cp -f taibu /bin || exit 1
	mkdir -p /usr/share/doc/taibu || exit 1
	cp -f readme.txt /usr/share/doc/taibu || exit 1
	
	rm -f taibu
	
elif (test "$1" = "remove"); then
	rm -rf /bin/taibu /usr/share/doc/taibu || exit 1
	
fi
