#include <stdlib.h> //for exit
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
        if(argc >= 2) //this means user entered ls and some args
        {
            
            const char *dirName = "."; //must be for curr dur name
            DIR *dirp = opendir(dirName);
            if(dirp == NULL)
            {
                    perror("opendir");
                    exit(1);
            }
            dirent *direntp;
            vector<dirent*> dirHold;
            vector<dirent> dirHold2;
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
            
            sort(dirHold.begin(), dirHold.end(), compFunct);
            for(int i = 0; i < dirHold.size(); i++)
            {
                if(dirHold.at(i)->d_name[0] == '.')
                {
                   cout << dirHold.at(i)->d_name << '/' << endl; 
                }
                else
                    cout << dirHold.at(i)->d_name << endl;
            }

            closedir(dirp);

         }   
        
    }
    return 0;
}







