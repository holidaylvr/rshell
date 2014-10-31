#include <stdlib.h> //for exit
#include <iostream>
#include <dirent.h> //use for readdir
#include <sys/stat.h> //use for stat
#include <sys/types.h> //use for stat
#include <unistd.h> //use for stat
#include <stdio.h> //for perror
#include <errno.h> //perror



using namespace std;

int main(int argc, char** argv)
{
    string lsID = "ls";
    //argv[0] = ./a.out ... just a note
    //if they user enters invalid command, throw error
    if(argv[1] != lsID)
    {
        cout << "bash: command not found" << endl;
        exit(1);
    }
    else
    {
        if(argc ==2) //this means user just entered ls alone
        {
          
        }
    }
    return 0;
}
