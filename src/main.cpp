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
        //testing of mine for execvp... DELETE in final build
        /*    
        char *argv[3];
        argv[0] = new char[3];
        strcpy(argv[0], "ls");
        //rgv[1] = new char[3];
        argv[1] = "-l";
        execvp("ls",argv);
        */
        cout << "Child process" << endl;
        while (1)
        {
            //infinite loop to run until exit command

            string parse="";
            cout << "$ ";
            getline(cin, parse);
            char del[2] = ";"; //delimiter to signal diff cmd
            char* token=0; //point to null initially
                           //token will be parser
            char* replace=0; //point to null
            replace = new char[parse.size()+1];
            memcpy(replace, parse.c_str(), parse.size()+1);
            
            token = strtok(replace, del);
            while (token != NULL)
            {
                //printf( " %s\n", token);
                cout << "token: " <<  token << endl;
                
                string fakeTok = token;
                cout << "faketok: " << fakeTok << endl;

                char* replace2=0;
                replace2 = new char[fakeTok.size()+1];

                char delim2[2] = " "; //to parse further.. take out spaces
                memcpy(replace2, fakeTok.c_str(), fakeTok.size()+1);
               
                char* tok2=0; 
                tok2 = strtok(replace2, delim2);
                cout << token << endl;
                while(tok2 != NULL)
                {
                    cout << tok2 << endl;
                    tok2 = strtok(NULL, delim2);
                }
                token = strtok(NULL, del);//inc token to next grouping
                //execvp(token[0], token);
            }

                //I need these two conditions. REMINDER
                //if(*beg == "exit") //if usser wants to quit, enter `exit`
                //else if(*beg == "#")
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
