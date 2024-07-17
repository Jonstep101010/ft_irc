## nc/netcat (using localhost)
### launch server
```shell
# ./ircserv <port> <password>
./ircserv 8989 test
```
### connect
```shell
# nc -C <ip> <port>
# `-C` tells nc to send CRLF (`\r\n`) line terminations
nc -C localhost 8989
```
### authenticate
```shell
# PASS provides our password
PASS test
```
### register
```shell
# we provide our chosen nickname
NICK netcat
# we tell the server our username `ncname`
USER netcat 8 * :ncname
# we should receive a welcome message now
# :0.0.0.0 001 netcat :Welcome to the IRC server!
```
### respond to PING
timeout for getting disconnected is 60 seconds
```shell
# ping from server for timeout
PING netcat
# respond with `PONG `
PONG 
```

## irssi client: DCC
```shell
# user one sends a PRIVMSG offering up the file to user two
Alice: `/dcc SEND Bob filename`
# user two accepts the file from user one
Bob: `/dcc GET Alice`
```