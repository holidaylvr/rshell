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
1. Can only run RSHELL within itself 5 times before encountering "execvp:fork" error.

2. Combining commands with only a single `&` produces slightly different results compared to bash.

3.Cannot handle single `|`. RSHELL is supposed to perform slightly second cmd but RSHELL performs both.

4.Cannot handle multiple combinations of `&&`. (ie: ls &&&& ls performs both ls commands in my shell). Parsing error with strtok_r()

5.Cannot handle `#` (or any combination) after a cmd. RSHELL treats this simply as the cmd. Another parsing error with strtok_r()

6.


