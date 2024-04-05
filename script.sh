#!/bin/bash

gcc src/webserver.c src/thread.c src/util.c src/uri.c src/net.c src/config.c src/server.c src/buffer.c -o bin/webserver

while [ -n "$1" ]; do
case "$1" in
    -o)  #!/bin/sh 
         ./bin/webserver;;
    -v) valgrind ./bin/webserver;;
esac
shift
done