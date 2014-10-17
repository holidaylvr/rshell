#include<iostream>
#include<unistd.h> //for execvp
#include<stdio.h> //for perror
#include<errno.h> //perror 
#include<sys/types.h> //for wait
#include<sys/wait.h> //for wait
#include<boost/tokenizer.hpp> //Tokenizer class for parsing
#include<string>
#include<boost/token_iterator.hpp>
#include <string.h>

using namespace std;
using namespace boost;

int main ()
{
    //get infinite while loop running that exits on `exit`
    //run the loop inside of parent
    //once recieve valid input, pass to child and deal with execvp
    
    int pid = fork();
    if(pid == -1) //throw an error if fork returns -1
    {
        perror("fork");
        exit(1); 
    }
    else if (pid == 0)
    {
        cout << "hey" << endl;
        char *argv[3];
        argv[0] = new char[3];
        strcpy(argv[0], "ls");
        //rgv[1] = new char[3];
        argv[1] = "-l";
        //argv[2] = new char[3];
        argv[2] = "-a";
        execvp("ls",argv);
        cout << "Child process" << endl;
        while (1)
        {
            //infinite loop to run until exit command
            string parse="";
            cout << "$ ";
            getline(cin, parse);
            tokenizer<> tok(parse);
            for(tokenizer<>::iterator beg=tok.begin(); beg != tok.end(); ++beg)
            {
                //where eventual parsing will go down
                if(*beg == "exit") //if usser wants to quit, enter `exit`
                {
                    exit(1);
                }
                else if(*beg == "#")
                {
                    cout << "hey" << endl;
                    break;
                }
            }
        }

    }
    else if (pid > 0)
    {   
        if( -1 == wait(0))
        {
            perror("wait");
        }
        cout << "Parent process" << endl;
    }
    


    return 0;
}
