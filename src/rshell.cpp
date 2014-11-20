#include<iostream>
#include <sys/stat.h>
#include <vector>
#include<unistd.h> //for execvp
#include<stdio.h> //for perror
#include<errno.h> //perror 
#include<sys/types.h> //for wait
#include<sys/wait.h> //for wait
#include<string>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

using namespace std;

int new_proc(int in, int out, char ** cmd)
{
    cout << "in new proc " << endl;
    pid_t pid;

    if( (pid = fork()) == -1 )
    {
            perror("fork:21");
            exit(1);
    }
    else if(pid == 0) //child
    {
        if(in != 0) //
        {
            dup2(in, 0);
            close(in); //close it. Copied to STDOUT
        }
        else if(in != 1) //
        {
            dup2(out, 1); //we will write to pipe
            close(out);//close it. Copied it to STDIN
        }
        if( -1 == execvp(cmd[0], cmd))
        {
                perror("execvp:37");
                exit(1);
        }
        //else //means in == 0 so first process
             //------------------------------------------------------------------> ! ! ! ! ! ! !  ! ! ! ! ! ! ! ! 
          //      execvp(cmd[0], cmd);
    }
    return pid; //IDK?
}

int fork_pipe(int n, vector<string> arg_list, vector<int> redir_flags)
{
    cout << "HERE " << endl;
    int i;
    pid_t pid;
    int in, fd[2];
    in = 0; //first process should read from STDIN... unless other redir flag set
            //determine redir flag below! ! ! ! ! ! ! -------------------------------->  ! ! ! ! ! ! !  ! !
        char *token_temp, *savptr_temp, *replace_temp;
        char del2[] = "< >";
        vector<string> single_cmd;
        replace_temp = new char[arg_list.at(0).size()+1];

        char **argv = new char*[arg_list.at(0).size()+1];
        strcpy(replace_temp, arg_list.at(i).c_str());
        token_temp = strtok_r(replace_temp, del2, &savptr_temp);

//-----------------------------------------------------------------------------
    cout << "58 " <<  n << endl;    
    for(int i=0; i < n-1; i++) //all but last processes created here
    {
        cout << "loop " << endl;
        strcpy(replace_temp, arg_list.at(i).c_str());    
        token_temp = strtok_r(replace_temp, del2, &savptr_temp);

        while(token_temp != 0)
        {
            single_cmd.push_back(token_temp);
            //cout << "74 " << token_temp << endl;
            token_temp = strtok_r(NULL, del2, &savptr_temp);
        }
        for(int j=0; j < single_cmd.size(); j++)
        {
            argv[j] = new char[single_cmd.at(i=j).size()+1];
            strcpy(argv[j], single_cmd.at(j).c_str());
            cout << "81 " << argv[j] << endl;
            argv[single_cmd.size()] = 0;
        }
        single_cmd.clear();//clear up vector

        //cout << "88" << argv[0] << endl;
        pipe(fd); 
        //fd[1]write end of the pipe we carry in from prev iteration....
        
        //need to change STDIN if input redir
        if(redir_flags.at(i) == 0)
        {
            if( -1 == (in = open(argv[0], O_RDONLY | O_CREAT, 0777)))
            {
                perror("open:94");
                exit(1);
            }
            cout << in  << "100 "<< endl;
            
        }
        else if(redir_flags.at(i) == 1)
        {
            if(-1 == (fd[1] = open(argv[0], O_RDWR | O_CREAT, 0777)))
            {
                perror("open:105 ");
                exit(1);
            }
        }
        else if(redir_flags.at(i) == 2)
        {
            cout << "out2 115 " << endl;
            if(-1 == (fd[1] = open(argv[0], O_RDWR | O_APPEND, 0777)))
            {
                perror("open:117 ");
                exit(1);
            }
        }
        cout << "in 123 " << in << endl;
        new_proc(in, fd[1], argv);

        //dont need to write to pipe, child does that
        close(fd[1]);
        cout << "returned 128" << endl;
        cout << "i " << i << endl;
        //keep read end of pipe, next child will read from here
        in = fd[0];

     }
//-----------------------------------------------------------------------------
        cout << "134 " <<  token_temp << endl;
        /*
        strcpy(replace_temp, arg_list.at(n-1).c_str());    
        token_temp = strtok_r(replace_temp, del2, &savptr_temp);*/
        while(token_temp != 0)
        {
            single_cmd.push_back(token_temp);
            cerr << "142 " << token_temp << endl;
            token_temp = strtok_r(NULL, del2, &savptr_temp);
        }
        cerr << "144 " << endl;
        
        argv = new char*[single_cmd.size()+1]; 
        for(int j=0; j < single_cmd.size(); j++)
        {
            argv[j] = new char[single_cmd.at(j).size()+1];
            strcpy(argv[j], single_cmd.at(j).c_str());
            cout << "149 " << argv[j] << endl;
            cout << "command size 149 " << single_cmd.size() << endl;
            cout << "j " << j << endl;
            argv[arg_list.size()] = 0;
        }
        
     //last stage of pipe, set STDIN to be read end of prev pipe and output 
        //to the original fd 1
        if(in != 0)
            dup2(in, 0);

        //case where only one cmd "cat < test.cpp"
        if(redir_flags.at(0) == 0 && redir_flags.size() == 1)
        {
            cout << "input redir 161 " << endl;
            if(n==1)
            {
                if(-1 == (in = open(argv[1], O_RDONLY | O_CREAT, 1777)))
                {
                    perror("open 167");
                    exit(1);
                
                }
                close(0);
                dup2(in,0);
            }
            else if(-1 == (in = open(argv[0], O_RDONLY | O_CREAT, 1777)))
            {
                close(0);
                if(-1 == dup2(in,0))
                {
                    perror("dupe 168 ");
                    exit(1);
                }
            }
            close(0);
            dup2(in,0);
        }
        else if(redir_flags.at(i) == 1 && redir_flags.size() == 1)
        {
            cout << "out 170 " << endl;
            if(-1 == (fd[1] = open(argv[0], O_RDWR | O_CREAT, 1777)))
            {
                perror("open:175 ");
                exit(1);
            }
            //argv[arg_list.size()-1] = 0;
        }
        else if(redir_flags.at(i) == 2 && redir_flags.size() ==1)
        {
            cout << "out2 178 " << endl;
            if(-1 == (fd[1] = open(argv[0], O_RDWR | O_APPEND, 1777)))
            {
                perror("open:182 ");
                exit(1);
            }
        }
        else if(in != 0)
        //input and output to original fd
        {
            dup2(in,0);
        }

        //execute last stage with current proc
        cout << "execute betch " << endl;
        return execvp(argv[0], argv);
        //CHANGE TO APPROPRIATE ARRAY AFTER PARSING



}

int main ()
{
        while (1)
        {   //infinite loop to run until `exit` command
            char* login = getlogin();
            //gets user info/id && prints to terminal
            if ( 0 ==  login)
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
            //all these variable will allow me to see what is contained in user input
            int findAnd, findPipe, findInRedir, findOutRedir;
            findAnd = parse.find("&&");
            findPipe = parse.find("|");
            findInRedir = parse.find("<");
            findOutRedir = parse.find(">");
            int orTrip=0,andTrip=0, pipeTrip=0, inRedirTrip=0, outRedirTrip=0;
            int outRedirTrip2=0;
            //if find #, replace with null so that parsing treats it as end of array
            //source of one of my bugs
            if(findComment != -1)
            {
                parse.at(findComment) = 0;
                cout << "found comment: " << endl;
            }
            if(findAnd != -1) //signal if user input has && 
            {
                andTrip = 1;
                cout << "found `&&`: " << endl;

            }
            if(findPipe !=-1 )//need to check if pipe or `||`
            {
                if(parse[findPipe + 1] == '|')
                {
                    cout << "found ||" << endl;
                    orTrip = 1;
                }
                else
                {
                    pipeTrip = 1;
                    cout << "found pipe: " << findPipe << endl;
                }
            }
            if(findInRedir != -1)
            {
                inRedirTrip = 1;
                cout << "found input redirection: " << endl;
            }
            if(findOutRedir != -1)
            {
                if(parse[findOutRedir + 1] == '>')
                {
                    outRedirTrip2 =1;
                    cout << "found output redirection2" << endl;
                }
                else
                {
                    outRedirTrip = 1;
                    cout << "found output redirection: " << endl;
                }
            }
            char del[] = ";|&"; //delimiter to signal diff cmd
            
            char del2[] = "<| >";
            char *token, *token2, *token3;
            char *savptr1, *savptr2, *savptr3;
                           
            char* replace=0; 
            char* replace2=0;
            replace = new char[parse.size()+2];
            replace2 = new char[parse.size()+2];
            strcpy(replace, parse.c_str());
            //iterate the parsed token
            string token4;
            vector<string> tokens;
            vector<int> redirect_indicator;
            token = strtok_r(replace, del, &savptr1);
            
            strcpy(replace2, replace);
            
            /*token3 = strtok_r(replace2, del2, &savptr3);
            while(token3 != NULL)
            {
                cout << "180: " << token3 << endl;
                token3 = strtok_r(NULL, del2, &savptr3);
            }*/
            
//-----------------PUT CMD STRINGS  INTO VECTOR FOR EASE-----------------------
            while(token != NULL)
            {
                token4  = token;
                //cout << "215 " <<  token << endl;

                //will push indicators into vector for reference in pipe_fork
                if(token4.find("<")!=-1){
                    redirect_indicator.push_back(0);
                    //cout << "pushed 0 " << endl;
                }
                else if(token4.find('>')!=-1)
                {
                    if(token4[token4.find('>')+1] == '>')
                    {
                        redirect_indicator.push_back(2);
                        //cout << "pushed 2: " << endl;
                    }
                    else{
                        redirect_indicator.push_back(1);
                        //cout << "pushed 1 " << endl;
                    }
                }
                else{
                    redirect_indicator.push_back(-1);
                    //cout << "pushed -1 " << endl;
                }

                tokens.push_back(token);
                
                
                token = strtok_r(NULL, del, &savptr1);
            }
            /*for(int i=0; i < redirect_indicator.size(); i++)
            {
                cout << "indicator: " << redirect_indicator.at(i) << endl;
            }*/
            char ** cmd  = new char*[tokens.size()];

//-------------------PUT CMD STRINGS INTO ARRAY FOR LATER----------------------
            //cout << "testing tokens: " << endl;
            for(int i = 0; i < tokens.size(); i++ )
            {   
                
                cmd[i] = new char[tokens.at(i).size()+1];
                strcpy(cmd[i], tokens.at(i).c_str());
                cout << "366 " << cmd[i] << endl;
            }
            //cout << "cmd: " << tokens.size() << endl;
            //exit(1);
            if(orTrip !=0 || andTrip !=0)
            {
                    while (token != NULL)
                    {
                            cout << "pased token first round: " << endl;
                            cout << token << endl;
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
                            else if (pid2 == 0) //child
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
                            else //parent
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
//-----------this will handle i/o redirection and piping-----------------------
            else if (orTrip != 0)
            {
                while(token != 0)
                {
                //cout << "token: " << token <<  endl;
                //below is for parsing for input redirection....
                string strToken = token;
                char * tokHold = 0;
                char * tok2 = 0;
                int inRedirFlag=0, outRedFlag=0, outRedFlag2=0;
                tokHold = new char[strToken.size()+2];
                strcpy(tokHold, strToken.c_str());
//----------------------CHECK curr token for redirection ----------------------                
                if(strToken.find('<') != -1)
                {
                    inRedirFlag = 1;
                    //cout << strToken << endl;
                }
                else if(strToken.find(">") != -1)
                {
                    if(strToken[strToken.find(">") + 1] == '>')
                    {
                        outRedFlag2 = 1;
                    }
                    else
                        outRedFlag = 1;
                }
                

//-------------------------------PIPE--------------------------------------                
                    char del2[] = "< >";
                    vector<string> hold2;
                    tok2 = strtok_r(tokHold, del2, &savptr2);
                    while(tok2 != NULL)
                    {
                        //cout << tok2 << endl;
                        hold2.push_back(tok2);
                        tok2 = strtok_r(NULL, del2, &savptr2);
                    }
//-------------------loop to get parsed data into array for execvp-------------
                    char **argv = new char*[hold2.size()+1];
                    for(unsigned i=0; i<hold2.size();i++)
                    {
                        if(hold2.at(i) == "exit")
                        {
                            exit(1);
                        }
                        argv[i] = new char[hold2.at(i).size()+1];
                        strcpy(argv[i], hold2.at(i).c_str());
                        argv[hold2.size()] = 0;
                    }

                    //input now parsed into appropriate form (ie: [cat, make])
                    cout << "inRedirFlag: " << inRedirFlag << endl;
                    if(inRedirFlag !=0 )
                    {
                        int fd2 = open(argv[1], O_RDONLY | O_CREAT, 0777);
                        cout << fd2 << endl;
                        if(fd2 == -1)
                        {
                            perror("open:229");
                            exit(1);
                        }
                        int oldstdout = dup(1);
                        if(oldstdout == -1)
                        {
                            perror("dup:253");
                            exit(1);
                        }
                        if(-1 == close(0))
                        {       
                            perror("close:258");
                            exit(1);
                        }   
                        if(-1 == dup(fd2))
                        {
                            perror("dup:263");
                            exit(1);

                        }
                        
                    }

                //cout << "pipeTrip;269: " << pipeTrip << endl;
                int fd[2];
                if(pipe(fd)==-1)
                {
                    perror("pipe:273");
                    exit(1);
                }

                int pid = fork();
                if(pid == -1)
                {
                    perror("fork:281");
                    exit(1);
                }
                else if(pid == 0) //child
                {   
                    //execvp should be in here. Child writing to pipe maybe
                    //ERROR CHECK
                    if(pipeTrip != 0) //WRITE to pipe
                    {
                        close(fd[0]); //close pipe-read b/c not used
                        close(STDOUT_FILENO); //close stdout
                        dup(fd[1]); //copy pipe write to lowest available 
                                    //position
                        execvp(argv[0], argv);
                    }
                    else if(inRedirFlag != 0) //execvp normally
                    {
                        cout << "here normal" << endl; 
                        if(-1 == execvp(argv[0], argv))
                        {
                            perror("execvp:300");
                            exit(1);
                        }
                    }
                    else
                        if(-1 == execvp(argv[0], argv))
                        {
                            perror("execvp:307");
                            exit(1);
                        }

                }
                else if(pid > 0) //parent 
                {               //READ from pipe
                    wait(0); 
                    //need another fork to execute right side of pipe
                    cout << "token:" << token << endl;
                    token = strtok_r(NULL, del, &savptr1);
                    cout << "token after inc:" << token << endl;
                    inRedirFlag=0;
                    cout << "Redirflag after" << inRedirFlag << endl;

                }
                


                //token = strtok_r(NULL, del, &savptr1);
            }
            //EXIT prog after entire string parsed!!
            exit(1);
            }
//---------------------REVISED RSHELL -----------------------------------------
            else
            {
                fork_pipe(tokens.size(), tokens, redirect_indicator);



            }
            
        }
        return 0;
}





















