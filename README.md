RSHELL
==========

Licensing Information: READ LICENSE
---
Project source can be downloaded from https://github.com/holidaylvr/rshell.git
----

Author & Contributor List
-----------
Paul Starkey

All other known bugs and fixes can be sent to holidaylvr@ucr.edu

Reported bugs/fixes will be submitted to correction upon validation.

File List
---------
```
.:

Makefile

LICENSE

README.md

./src

./tests
```
```
/src:

main.cpp

```
```
./tests:

exec.script
```

About Program
-------------
In this assignment, I attempted to make a simple command shell comprising of all the commands avaiable to me in the /usr/bin folder. This program is able to execute not only the commands availabe in the folder, but also the flags associated to the commands.

The way the program works is the user is prompted a line that consists of the users login info, their host name, and the '$' symbol, which is immediately followed by the area for user input. The user then enters the command, and the command is executed and printed to the terminal. The process is intended to continue as such until the user inputs 'exit', which then causes the program to exit.

For example, if the user enters 'ls', the program will print to the terminal all the contents in the current directory. If the user then enters 'ls -a', the program will print all the files in the current directory including the hidden files.


How to run file
---------------
Once you have cloned the directory to your local machine, follow the directions below:
--------------------------------------------------------------------------------------
1. `cd` into `rshell` directory

2. Call `make`

3. `cd` into `bin`

4. Call `rshell`

5. Can do steps 3 & 4 by calling `bin/rshell`

RShell will now open up instead of BASH and you can execute almost commands as included in the regular terminal.


Program reviewed by the University of California, Riverside.
------------------------------------------------------------
Basic Overview of Command Shells: [HERE](http://linuxgazette.net/111/ramankutty.html)

This program is developed to write a simple command shell called "RShell."

- It will offer similarities such as the normal BASH Command shell.

- Execute the command of ls to a file (with given instructions above)

- Can call all commands of BASH with execvp function

- Have special built in command "exit" to exit RSHELL and return to your local SHELL


Bugs
---
1. Can only run RSHELL within itself 4 times before encountering "execvp:fork" error. Believe this issue has to due with my inadvertent fork() call in my main. I believe its hogging resources. 

2. Combining commands with only a single `&` produces slightly different results compared to bash.

3.Cannot handle single `|`. RSHELL is supposed to perform slightly second cmd but RSHELL performs both.

4.Cannot handle multiple combinations of `&&`. (ie: ls &&&& ls performs both ls commands in my shell). Parsing error with strtok_r()

5.Cannot handle `#` (or any combination) after a cmd. RSHELL treats this simply as the cmd. Another parsing error with strtok_r()

6.Cannot properly start a command string with `&&` or `||`. My shell parses too vigorously and dismisses these invalid commands. Treats such cases as if the connectors do not exist.

7.Cannot properly handle echo-ing into a file. (ie: `echo test > example` in my shell simply echoes the result.

8.RSHELL cannot handle the command cd. We were not supposed to implement this in the assignment though.

9.Up arrow to bring up previous commands does not work. 

 
