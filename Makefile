webserver:
	gcc src/webserver.c src/thread.c src/base64.c src/util.c src/response.c src/uri.c src/mime.c src/net.c src/config.c src/server.c src/buffer.c -o bin/webserver