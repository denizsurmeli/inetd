# inetd
simplified inetd service 

# Part 1: Mandatory Part
- Run `make mandatory` in the directory.
- `server` and `client` executables will be compiled.
- Usage:
    - `./server` for starting the server
    - `./client <id>` for running the client, with `<id>` being the necessary argument.
# Part 2: Bonus Part
- Run `make bonus` in the directory.
- `inetd`,`inetd_client`, `service` executables will be there. `service` is the executable for `square` and `cube` services.
- NOTE: When you run the `inetd` service, make sure that you run the service in the same directory since `service` program will be in that directory.
- Usage:
    - `./inetd` for starting the server
    - `./inetd_client <port>` for running the client, with `<port>` being the necessary argument., `5010` for the square, `5020` for the cube service.
Run `make clean` for removing all the compiled binaries. 
