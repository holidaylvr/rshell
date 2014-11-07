#include <stdlib.h> //for exit
#include <string>
#include <string.h>
#include <algorithm>
#include <vector>
#include <iostream>
#include <dirent.h> //use for readdir
#include <sys/stat.h> //use for stat
#include <sys/types.h> //use for stat
#include <unistd.h> //use for stat
#include <stdio.h> //for perror
#include <errno.h> //perror


using namespace std;

bool compFunct(dirent* a, dirent* b)
    {
        int i=0, j=0;
        if(a->d_name[i] == '.')
        {

            while(a->d_name[i] == '.')
            {
                i++;  
            }
        }
        if(b->d_name[j] == '.')
        {
            while(b->d_name[j] == '.')
            {
                j++;
            }
    
        }
        return toupper(a->d_name[i]) < toupper(b->d_name[j]); 
    }

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
        //cerr << "made it " << endl;
        if(argc >= 2) //this means user entered ls and some args
        {
            const char* dirName=0;
            const char* dot= ".";
            int aFlag=0, lFlag=0, rFlag=0;
            vector<const char*> pathHold;
            vector<const char*> flagHold;
            //These checks will determine if user passes in path name or not
            if(argc == 2) //user only enters ls
            {
                //dirName=dot;
                pathHold.push_back(".");
                cerr << "only ls provided" << endl;
            }
            /*else if(argv[2][0] == '-') //|| argv[2][0] != '/')
            { //this implies that no path passed into ls. just "ls -flags"
                dirName = dot;
                cerr << "no path provided" << endl;
            }*/
            else
            {
                for(int i=2; i < argc; i++)
                {
                    if(argv[i][0] == '/')
                    {
                        pathHold.push_back(argv[i]);
                    }
                    if(argv[2][0] == '-')
                    {
                        pathHold.push_back(dot);
                    }
                    if(argv[i][0] == '-')
                    {
                        flagHold.push_back(argv[i]);
                 //       cerr << "Pushed flag " << flagHold.at(0) << endl;

                        for(int j=0; j < flagHold.size();  j++)
                        {
                            //for(int k=0; flagHold.at(i)[k] != "\0"; k++)
                            //{
                                const char* ach =0;
                                const char* lch;
                                const char* rch;
                                ach = strrchr(flagHold.at(j), 'a');
                                lch = strrchr(flagHold.at(j), 'l');
                                rch = strrchr(flagHold.at(j), 'R');
                                if(ach != 0)
                                {
                                        aFlag = 1;
                                        cerr << "a flag set " << endl;
                                }
                                if(lch != 0)
                                {
                                    lFlag =1;
                                    cerr << "l flag set" << endl; 
                                }
                                if(rch!=0)
                                {
                                    rFlag=1;
                                    cerr << "r flag set" << endl;

                                }
                            
                        }
                        flagHold.clear();
                    }
                    else
                            pathHold.push_back(argv[i]);
                    
                }
            }
            
            //THIS SHOULD BE IN LOOP... B/c IF USER ENTERS MULTIPLE PATHS
            for(int i=0; i < pathHold.size(); i++)
            {
                DIR *dirp = opendir(pathHold.at(i));
                if(dirp == NULL)
                {
                    perror("opendir");
                    exit(1);
                }
                dirent *direntp;
                vector<dirent*> dirHold;
                while((direntp = readdir(dirp))) //put dir names into container for later sorting ease
                {
                    if(NULL == direntp)
                    {
                        perror("readdir");
                        exit(1);
                    }
                    //cout << direntp->d_name << endl;
                    dirHold.push_back(direntp);
                }
                //cout << dirHold.at(1); 
                //cerr << "a flag " << aFlag << endl; 
                sort(dirHold.begin(), dirHold.end(), compFunct);
                for(int i = 0; i < dirHold.size(); i++)
                {
                    if(dirHold.at(i)->d_name[0] == '.') //&& -a flag
                    {
                        //cerr << "do I even go here " << endl;
                        //cerr << "a flag again" << aFlag << endl;
                        if(aFlag != 0 ) //hidden files
                        {

                            //cerr << "came here for: " << dirHold.at(i)->d_name << endl;
                            if(lFlag !=0) //list file properties
                            {
                                struct stat statbuf;
                                stat(dirHold.at(i)->d_name, &statbuf);
                                if(S_ISDIR(statbuf.st_mode))
                                {
                                    cout << 'd';
                                }
                                cout << dirHold.at(i)->d_name << '/' << endl;
                            }
                             //   cout << dirHold.at(i)->d_name << '/' << endl; 
                        }
                    }
                    else if(dirHold.at(i)->d_name[0] != '.')
                    {
                        if(lFlag !=0) //list all attributes of -l flag 
                        {
                            struct stat statbuf;
                            if( -1 == stat(dirHold.at(i)->d_name, &statbuf)) {
                                    perror("stat");
                                    exit (1);
                            }
                            //first attributes are the permissions for user, group, & other 
                            if(S_ISDIR(statbuf.st_mode)) { //dir flag
                                cout << 'd';
                            }
                            else if(S_ISLNK(statbuf.st_mode)) { //symbolic link flag
                                cout << 'l';
                            }else {
                                cout << '-';
                            }
                            if(S_IRUSR & statbuf.st_mode) { //user perm start here
                                cout << 'r';
                            }else
                                cout << '-';
                            if(S_IWUSR & statbuf.st_mode) {
                                cout << 'w';
                            }else
                                    cout << '-';
                            if(S_IXUSR & statbuf.st_mode) {
                                cout << 'x';
                            }else
                                cout << '-';
                            if(S_IRGRP & statbuf.st_mode) { //group perm start here
                                cout << 'r';
                            } else 
                                cout << '-';
                            if(S_IRGRP & statbuf.st_mode) {
                                cout << 'w';
                            }
                            else
                                cout << '-';
                            if(S_IXGRP & statbuf.st_mode) {
                                cout << 'x';
                            }else
                                cout << '-';
                            if(S_IROTH & statbuf.st_mode) { //other perm start here
                                cout << 'r';
                            } else 
                                cout << '-';
                            if(S_IROTH & statbuf.st_mode) {
                                cout << 'w';
                            }
                            else
                                cout << '-';
                            if(S_IXOTH & statbuf.st_mode) {
                                cout << 'x';
                            }else
                                cout << '-';

                            

                        }
                        cout << dirHold.at(i)->d_name << endl;
                    }
                }

                closedir(dirp);
            }
            

         }   
        
    }
    return 0;
}







