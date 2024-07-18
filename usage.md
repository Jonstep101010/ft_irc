## launch server
```shell
# ./ircserv <port> <password>
./ircserv 9191 test
```
## nc/netcat (using localhost)
### connect
```shell
# nc -C <ip> <port>
# `-C` tells nc to send CRLF (`\r\n`) line terminations
nc -C localhost 9191
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
# respond to server
PONG
```
### send a message
`PRIVMSG <nick/#channel> :<message>`
```shell
# send a message to the client with the nickname bob
PRIVMSG bob :Hello, World!
# send a message to the channel `potato-lovers`
PRIVMSG #potato-lovers :Hello, World!
```
## IRSSI client
### connect to server
#### optional: using config
```shell
# located at `~/.irssi/config`
# copy config after launching irssi the first time (to create ~/.irssi directory)
cp ./config ~/.irssi/config
```
#### connect with launch arguments
```shell
# irssi -c <ip> -p <port> -w <password>
irssi -c localhost -p 9191 -w test
```
### file transfer using DCC
```shell
# user one sends a PRIVMSG offering up the file to user two
/dcc SEND Bob filename
# user two accepts the file from user one
/dcc GET Alice
```
### channels only: bot usage
```shell
# get the commands available
!help
```
## Commands example
| Feature                | Command in `nc`                  | Command in `irssi`              |
|------------------------|----------------------------------|---------------------------------|
| Join/Create channel    | `JOIN #<channel>`                | `/join #<channel>`              |
| Set channel invite only| `MODE #<channel> +i`             | `/mode #<channel> +i`           |
| Kick user from channel | `KICK #<channel> <user_nick>`    | `/kick #<channel> <user_nick>`  |