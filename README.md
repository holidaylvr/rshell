RSHELL
==========

Project source can be downloaded from https://github.com/holidaylvr/rshell.git
----

Author & Contributor List
-----------
Paul Starkey

All other known bugs and fixs can be sent to pstar001@ucr.edu

Reported bugs/fixes will be submitted to correction upon validation.

File List
---------
```

Makefile

LICENSE

README.md

./src

./tests
```
```
/src:
cp.cpp
ls.cpp
main.cpp
rshell.cpp
Timer.h

```
```
./tests:
ls.script
exec.script
pipe.script
hw3.script
```

About Program
-------------
RSHELL
------
In this assignment, I attempted to make a simple command shell comprising of all the commands avaiable to me in the /usr/bin folder. This program is able to execute not only the commands availabe in the folder, but also the flags associated to the commands.

The way the program works is the user is prompted a line that consists of the users login info, their host name, and the '$' symbol, which is immediately followed by the area for user input. The user then enters the command, and the command is executed and printed to the terminal. The process is intended to continue as such until the user inputs 'exit', which then causes the program to exit.

For example, if the user enters 'ls', the program will print to the terminal all the contents in the current directory. If the user then enters 'ls -a', the program will print all the files in the current directory including the hidden files.

UPDATE 11/20/14:
RSHELL now has the ability to pipe and redirect commands! Just Like a real-life bash (with some limitations naturally). 

for example, if the user enters 'cat < file | tr a-z A-Z | tee new | tr A-Z a-z > new2' RSHELL will redirect the input from 'file' to cat, which will then be piped to 'tr' which will be piped to tee, which creates a new file, and so on. I am sure you know what that command does and do not need me to walk you through it.

UPDATE 12/2/2014
RSHELL has had the execvp call replaced by execv. This means that $PATH is now manually parsed and searched when finding the location of cmds. Also, the `cd` functionality has been implemented manually to use chdir instead of bash`s built in cd.

LS
------
For this assignment, I attempted to implement some functionality of the bash command `ls`. Namely the -l -a and -R. However, I was unable to implement the -R flag. It introduced too many bugs into my code and I decided a final build without it was best.

The way the program works is that the user compiles the program and passes to it either files, paths, flags, or a combination of the three.



How to run file
---------------
Once you have cloned the directory to your local machine, follow the directions below:
--------------------------------------------------------------------------------------
1. `cd` into `~/CS100/rshell` directory

2. make

3. bin/rshell 

4. repeat


RShell will now open up instead of BASH and you can execute almost commands as included in the regular terminal.

LS
-----
1. cd ~/CS100/rshell

2. make

3. bin/ls ls /[paths] -[flags]

Program reviewed by the University of California, Riverside.
------------------------------------------------------------
Basic Overview of Command Shells: [HERE](http://linuxgazette.net/111/ramankutty.html)

This program is developed to write a simple command shell called "RShell."

- It will offer similarities such as the normal BASH Command shell.

- Execute the command of ls to a file (with given instructions above)

- Can call all commands of BASH with execvp function

- Have special built in command "exit" to exit RSHELL and return to your local SHELL


Bugs...SO many bugs. Need an exterminator.
---
RSHELL
------
1. Can only run RSHELL within itself 4 times before encountering "execvp:fork" error. Believe this issue has to due with my inadvertent fork() call in my main. I believe its hogging resources. 

2. Combining commands with only a single `&` produces slightly different results compared to bash.

3. Cannot handle single `|`. RSHELL is supposed to perform slightly second cmd but RSHELL performs both.

4. Cannot handle multiple combinations of `&&`. (ie: ls &&&& ls performs both ls commands in my shell). Parsing error with strtok_r()

5. Cannot handle `#` (or any combination) after a cmd. RSHELL treats this simply as the cmd. Another parsing error with strtok_r()

6. Cannot properly start a command string with `&&` or `||`. My shell parses too vigorously and dismisses these invalid commands. Treats such cases as if the connectors do not exist.

7. Cannot properly handle echo-ing into a file. (ie: `echo test > example` in my shell simply echoes the result.

8. RSHELL cannot handle the command cd. We were not supposed to implement this in the assignment though.

9. Up arrow to bring up previous commands does not work. 

UPDATED 11/20/14

10. Program cannot loop while implementing the piping/redirecting funcionality. I found the source was the way I was handling (or rather not) my main fork. I attempted to fix this but but it simply reintroduced far too many bugs. I figured it would be better to have code that works well on a single process at a time rather than no proceses at a time.

11. Cannot handle 'cat < file > file2'

12. Sometimes randomly throughout code execution the program will create random files with pieces of names (ie: 'tr', 'a-z', 'echo'). I believe this bug is caused when a full command is invalid and it only executes partially.

13. The previous RSHELL implemenatation was effected by the new version.. Still unsure as to the severity. (ie: 'echo hey && fart' executes when it should not).

UPDATED 12/2/2014
14. RSHELL can now only go two level deep. Once in the second level, functionality deminishes for reasons unknown. As long as RSHELL is only a single level deep functionality remains full.

15. Cannot handle `~` being passed to cmd cd 

16. `echo $PATH` literally echoes $PATH.... Fails

17. All previous bugs still apply to this new version.

LS
-----
1. Outputs extra endl whitespace

2. Cannot handle the -R flag

3. Sorting does not work

4. Does not print in color like actual ls

5. Ls is a parameter to the function

