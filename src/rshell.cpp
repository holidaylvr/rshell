#include<iostream>
#include <vector>
#include<unistd.h> //for execvp
#include<stdio.h> //for perror
#include<errno.h> //perror 
#include<sys/types.h> //for wait
#include<sys/wait.h> //for wait
#include<string>
#include <string.h>
#include <stdlib.h>

using namespace std;

int main ()
{
    //initial forking which I now realize was an error
    //leaving b/c program already developed
    int pid = fork();
    if(pid == -1) 
    {
        perror("fork outside:");
        exit(1); 
    }
    else if (pid == 0)
    {
        while (1)
        {   //infinite loop to run until `exit` command
            char* login = getlogin();
            //gets user info/id && prints to terminal
            if ( 0 ==  getlogin())
            {
                perror("getLogin()");
            }
            char user[256];
            if (-1 == gethostname(user, sizeof(user)))
            {
                perror("gethostname");
            }
            int findComment;
            string parse="";
            //line outputs user info before prompt
            cout << login << user << "$ ";
            getline(cin, parse);
            findComment = parse.find("#");
            int findOr;
            int findAnd;
            findOr = parse.find("||");
            findAnd = parse.find("&&");
            int orTrip=0;
            int andTrip = 0;
            //if find #, replace with null so that parsing treats it as end of array
            //source of one of my bugs
            if(findComment != -1)
            {
                parse.at(findComment) = 0;
            }
            if(findOr != -1) //signal if user input has ||
            {  
                orTrip = 1;
            }
            if(findAnd != -1) //signal if user input has && 
            {
                andTrip = 1;
            }
            char del[] = ";||&&"; //delimiter to signal diff cmd
            char *token, *token2;
            char *savptr1, *savptr2;
                           
            char* replace=0; 
            replace = new char[parse.size()+2];
            strcpy(replace, parse.c_str());
            
            //iterate the parsed token
            token = strtok_r(replace, del, &savptr1);
            while (token != NULL)
            {
                string fakeTok = token;
                char* replace2=0;
                replace2 = new char[fakeTok.size()+1];

                char delim2[] = " "; //to parse further.. take out spaces
                strcpy(replace2, fakeTok.c_str());
               
                token2 = strtok_r(replace2, delim2, &savptr2);
                vector<string> hold;
                //put parsed data into vector for ease of access
                while(token2 != NULL)
                {
                    hold.push_back(token2);
                    token2 = strtok_r(NULL, delim2, &savptr2);
                }

                //loop to get parsed data into array for execvp
                char **argv = new char*[hold.size()+1];
                for(unsigned i=0; i<hold.size();i++)
                {
                   if(hold.at(i) == "exit")
                   {
                        exit(1);
                   }
                   argv[i] = new char[hold.at(i).size()+1];
                   strcpy(argv[i], hold.at(i).c_str());
                   argv[hold.size()] = 0;
                }

                //fork to actually execute cmds 
                int pid2 = fork();
                int status = 0;
                if(pid2 == -1)
                {
                    perror("fork");
                    exit (1);
                }
                else if (pid2 == 0)
                {
                    if (execvp(argv[0],argv) != -1 && orTrip == 1)
                    {
                        exit (1);
                    }
                    else if( -1 == execvp(argv[0], argv))
                    {
                        perror("execvp");
                        if(andTrip == 1)
                        {
                            exit(1);
                        }
                        else if(orTrip == 1)
                        {
                            exit(1);
                        }
                        exit(1);
                    }
                }
                else 
                {   
                    if(-1 == wait(&status))
                    {
                        perror("wait()");
                    }
                    else
                    {  
                        if(status > 0 && andTrip == 1) //break out if && failed
                        {
                            break;

                        }
                        if(WEXITSTATUS(status)==0 && orTrip==1) //break if cmd succeeds and || cmd 
                        {
                            break;
                        }
                        else
                        {
                            //inc token
                            token = strtok_r(NULL, del, &savptr1);
                        }
                    }
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
    }
    return 0;
}
