#!/bin/bash

gcc src/webserver.c src/thread.c src/base64.c src/util.c src/response.c src/uri.c src/mime.c src/net.c src/config.c src/server.c src/buffer.c -o bin/webserver

while [ -n "$1" ]; do
case "$1" in
    -e)  #!/bin/sh 
         ./bin/webserver;;
    -v) 
         valgrind   --leak-check=full --show-reachable=yes --track-origins=yes ./bin/webserver ;;
esac
shift
done

./bin/webserver