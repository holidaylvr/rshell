#include<iostream>
#include <vector>
#include<unistd.h> //for execvp
#include<stdio.h> //for perror
#include<errno.h> //perror 
#include<sys/types.h> //for wait
#include<sys/wait.h> //for wait
#include<string>
#include <string.h>

using namespace std;

int main ()
{
    int pid = fork();
    if(pid == -1) 
    {
        perror("fork outside:");
        exit(1); 
    }
    else if (pid == 0)
    {
        cout << "Child process" << endl;
        while (1)
        {
            //infinite loop to run until exit command
            char* login = getlogin();
            if ( 0 == getlogin())
            {
                perror("getLogin()");
            }

            char user[256];
            if (-1 == gethostname(user, sizeof(user)))
            {
                perror("gethostname");
            }
            string parse="";
            cout << login << user <<   "$ ";
            getline(cin, parse);
            char del[] = ";"; //delimiter to signal diff cmd
            char *token, *token2;
            char *savptr1, *savptr2;
                           
            char* replace=0; 
            replace = new char[parse.size()+1];
            memcpy(replace, parse.c_str(), parse.size()+1);
            
            token = strtok_r(replace, del, &savptr1);
            if (token == "exit" ) 
             {
                cout << "die" << endl;
             }
                    
            while (token != NULL)
            {
                //cout << "token: " <<  token << endl;
                string fakeTok = token;
                char* replace2=0;
                replace2 = new char[fakeTok.size()+1];

                char delim2[] = " "; //to parse further.. take out spaces
                memcpy(replace2, fakeTok.c_str(), fakeTok.size()+1);
               
                token2 = strtok_r(replace2, delim2, &savptr2);
                vector<string> hold;
                while(token2 != NULL)
                {
                    hold.push_back(token2);
                    //cout << "loop: " << token2 << endl;
                    token2 = strtok_r(NULL, delim2, &savptr2);
                }
                //loop to get parsed data into array for execvp
                char *argv[hold.size()+1];
                
                for(unsigned i=0; i<hold.size();i++)
                {
                   if(hold.at(i) == "exit")
                   {
                        exit(1);
                   }
                   argv[i] = new char[hold.at(i).size()+1];
                   strcpy(argv[i], hold.at(i).c_str());
                }
                
                int pid2 = fork();
                if(pid2 == -1)
                {
                    perror("fork inner");
                    exit (1);
                }
                else if (pid2 == 0)
                {
                    cout << "HERE" << endl;
                    cout << argv[0] << endl;
                    execvp(argv[0], argv);
                    perror("execvp");
                    exit(1);
                }
                else 
                {   
                    if(-1 == wait(0))
                    {
                        perror("wait()");
                    }
                    else
                    {
                        token = strtok_r(NULL, del, &savptr1);//inc token to next grouping
                    }
                }
            }
         }
     }    
    else if (pid > 0)
    {   
        int status2 = 2;
        if( -1 == wait(&status2))
        {
            if(status2 > 2) { exit(1);}
            perror("wait");
        }
        cout << "Parent process" << endl;
    }
    return 0;

    //BUGS 
    //CANNOT enter bogus commands... shell freaks out
    //CANNOT handle connectors yet
    //outputs error messages even when cmd is successful
    //CANNOT exit on exit
    //CANNOT handle comments yet
}
