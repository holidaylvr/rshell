#include <stdlib.h> //for exit
#include <iomanip> //for formatting
#include <time.h> //for time formatting
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
#include <pwd.h> // for getpwuid
#include <grp.h> //for getgrgid
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
                        //cerr << "I pushed: " << argv[i] << endl;
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
                    //else
                    //        pathHold.push_back(argv[i]);
//--> SHOULD REMEDY DOUBLE PRINT
                }
            }
            
            //THIS SHOULD BE IN LOOP... B/c IF USER ENTERS MULTIPLE PATHS
            cout << "path size: ";
            cout << pathHold.size() << endl;
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
//--> WE GO HERE TWICE??                cout << dirHold.size() << endl;
                char timeStr[200];
                time_t t;
                struct tm *tmp;
                for(int i=0, exFlag=0, dFlag=0; i < dirHold.size(); i++, exFlag=0, dFlag=0)
                {
                    if(dirHold.at(i)->d_name[0] == '.') //signals hidden files
                    {
                        if(aFlag != 0 ) //hidden files
                        {
                        struct stat statbuf;
                                
                        if(-1 == stat(dirHold.at(i)->d_name, &statbuf)) {
                            perror("stat");
                            exit(1);
                        }
                            if(S_ISDIR(statbuf.st_mode)){
                                    dFlag=1;
                            }
                            if(S_IXUSR & statbuf.st_mode || S_IXOTH & statbuf.st_mode){
                                    exFlag=1;
                            }

                            if(lFlag !=0) //list file properties
                            {
                                //I know super inefficiency in code... But It's barely working atm
                                //So I'm keeping it indefinitely
                                struct stat statbuf;
                                
                                if(-1 == stat(dirHold.at(i)->d_name, &statbuf)) {
                                        perror("stat");
                                        exit(1);
                                }
                                if(S_ISDIR(statbuf.st_mode)) { //dir flag
                                    cout << 'd';
                                    dFlag = 1;
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
                                    exFlag =1;
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
                                    cout << "x ";
                                }else
                                    cout << "- ";
                                int n = 80;
                                std::cout <<  std::left << statbuf.st_nlink << ' ';
                                struct passwd *getUID;
                                if((getUID = getpwuid(statbuf.st_uid)) == 0 ){
                                        perror("getpwuid");
                                        exit(1);
                                }
                                struct group* groupID;
                                if((groupID = getgrgid(statbuf.st_gid)) == 0){
                                        perror("getgrgid");
                                        exit(1);
                                }



                                cout << getUID->pw_name << ' ';
                                cout << groupID->gr_name << ' ';
                                std::cout.width(5); std::cout << std::right <<  statbuf.st_size << ' ';
                                tmp = localtime(&statbuf.st_mtime);
                                if(strftime(timeStr, 200 , "%b %d %k:%M ", tmp) ==0){
                                        perror("strftime");
                                        exit(1);
                                }
                                cout << timeStr ;
                                
                                //cout << dirHold.at(i)->d_name << '/' << endl;
                            }
                                //cout << "so I didn't go above here.." << endl;
                            cout << dirHold.at(i)->d_name << '/' << endl; 
                        }
                    }
                    else if(dirHold.at(i)->d_name[0] != '.') //normal dir
                    {
                        struct stat statbuf;
                        if( -1 == stat(dirHold.at(i)->d_name, &statbuf)) {
                                    perror("stat");
                                    exit (1);
                        }    
                        if(S_ISDIR(statbuf.st_mode)){
                               dFlag=1;
                        }
                        if(S_IXUSR & statbuf.st_mode || S_IXOTH & statbuf.st_mode){
                               exFlag=1;
                        }
                        
                        
                        if(lFlag !=0) //list all attributes of -l flag 
                        {
                            struct stat statbuf;
                            //int exFlag=0, dFlag=0;
                            if( -1 == stat(dirHold.at(i)->d_name, &statbuf)) {
                                    perror("stat");
                                    exit (1);
                            }
                            //first attributes are the permissions for user, group, & other 
                            if(S_ISDIR(statbuf.st_mode)) { //dir flag
                                cout << 'd';
                                dFlag = 1;
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
                                exFlag =1;
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
                                cout << "x ";
                            }else
                                cout << "- ";
                            int n = 80;
                            struct passwd* getUID;
                            if((getUID = getpwuid(statbuf.st_uid)) == 0){
                                    perror("getpwuid");
                                    exit(1);
                            }
                            struct group* groupID;
                            if((groupID = getgrgid(statbuf.st_gid)) == 0){
                                    perror("getgrgid");
                                    exit(1);
                            }
                            std::cout <<  std::left << statbuf.st_nlink << ' ';
                            cout << getUID->pw_name << ' ';
                            //cout << statbuf.st_uid << ' ';
                            cout << groupID->gr_name <<  ' ';
                            /*int numDig=0;
                            off_t size=statbuf.st_size;
                            while (size>0){
                                    ++numDig;
                                    size/=10;
                            }*/ //idea to find size for formatting.. flawed
                            
                            cout << setw(6); std::cout << std::right <<  statbuf.st_size << ' ';
                            char timeStr[200];
                            struct tm* tmp;
                            tmp = localtime(&statbuf.st_mtime);
                            if(strftime(timeStr, 200 , "%b %d %k:%M ", tmp)==0){
                                perror("strftime"); 
                                exit(1);
                            }
                            cout << timeStr;

                            
                                //cout << statbuf.st_mtime << ' ';


                            

                        }
                        

                        cout << dirHold.at(i)->d_name;
                        if(dFlag !=0) {
                            cout << '/' << endl;
                        }
                        else if (exFlag!=0) {
                            cout << '*' << endl;
                        }
                        else cout << endl;
                    }
                }

                closedir(dirp);
            }
            

         }   
        
    }
    return 0;
}







