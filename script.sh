clear
gcc src/webserver.c src/thread.c src/util.c src/uri.c src/net.c src/config.c src/server.c src/buffer.c -o bin/webserver
bin/webserver