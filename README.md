### DISCLAIMER
This project is based heavily on examples from a textbook called [Beginning Linux Programming](http://www.wrox.com/WileyCDA/WroxTitle/Beginning-Linux-Programming-4th-Edition.productCd-0470147628.html). Chapter 15 covers sockets programming in C.

### What am I looking at?
This is a basic TCP file server written in C. The server process serves a raw text file to the client process, which reads the data and displays it. I wrote this an exercise to gain a better understanding of sockets programming in C.

### What do I do with it?
Run `make` to compile. Then start the server process in the background with `./server_process`, and run the client process. Run `killall server_process` when you are finished. You should get something similar to the following. 

```
$ ./server_process &
[1] 6826
[SERVER] waiting...
$ ./client_process 
[SERVER] adding client on fd 4
[CLIENT] connection established with server, choose from one of the following pages:
> home
> index
[SERVER] done greeting client 4
home
[SERVER] client requested home on fd 4
[SERVER] serving client home on fd 4
[CLIENT] response from server:
###   ###  #########  ###      ###  ########
###   ###  ##     ##  #####  #####  ##
#########  ##     ##  ### #### ###  ########
###   ###  ##     ##  ###  ##  ###  ##
###   ###  #########  ###      ###  ########
[SERVER] removed client on fd 4
```

### Can I talk to the server with curl or a web browser?
Not really. Because the server does not handle HTTP requests, the client will not receive a response.
