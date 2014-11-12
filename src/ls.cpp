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

bool compFunct(string a, string b)
{
    int i=0, j=0;
    if(a[i]=='.'){
        while(a[i]=='.'){
            i++;
        }
    }
    if(b[j]=='.'){
        while(b[j]=='.'){
            j++;
        }
    }

    return toupper(a[i]) < toupper(b[j]);
}

//calculates width of longest name
//depends on -a flag
int maxWidth(vector<string> dirName, int aFlag)
{
    int max;
    for(int i=0; i < dirName.size(); i++)
    {   
                        
        if(dirName.at(i)[0]=='.')
        {
            if(aFlag!=0) //we want to include w/-a
            {
                if(strlen(dirName.at(i).c_str()) > max)
                {   
                    max = strlen(dirName.at(i).c_str());
                }
            }
                              
        }
        else 
        {   
            if(strlen(dirName.at(i).c_str()) > max)
            {
                max = strlen(dirName.at(i).c_str());
            }
        }

    }
    return max + 2;
}

int main(int argc, char** argv)
{
    string lsID = "ls";
    //if they user enters invalid command, throw error
    if(argv[1] != lsID){
        cout << "bash: command not found" << endl;
        exit(1);
    }
    else {
        if(argc >= 2)  { //this means user entered ls and some args
            const char* dirName=0;
            const char* dot= ".";
            int aFlag=0, lFlag=0, rFlag=0;
            vector<string> pathHold;
            vector<const char*> flagHold;
            vector<string> fileHold;

            //These checks will determine if user passes in path name or not
            if(argc == 2) { //user only enters ls
                pathHold.push_back(".");
            }
            else {
                for(int i=2; i < argc; i++) {
                    if(argv[i][0] == '/') {
                        pathHold.push_back(argv[i]);
                    }
                    if(argv[2][0] == '-' && argc==3){
                        pathHold.push_back(dot);
                    }
                    if(argv[i][0] == '-') {
                        flagHold.push_back(argv[i]);

                        for(int j=0; j < flagHold.size();  j++)
                        {//will iterate through flag vector to determine 
                         //the flags entered
                            const char* ach =0;
                            const char* lch;
                            const char* rch;
                            ach = strrchr(flagHold.at(j), 'a');
                            lch = strrchr(flagHold.at(j), 'l');
                            rch = strrchr(flagHold.at(j), 'R');
                            if(ach != 0){
                                aFlag = 1;
                            }
                            if(lch != 0)
                            {
                                lFlag =1;
                            }
                            if(rch!=0)
                            {
                                rFlag=1;
                            }
                        }
                        flagHold.clear();
                    }
                    else if(argv[i][0] != '/' && argv[i][0] != '-')
                         fileHold.push_back(argv[i]);
                }
            }
            sort(fileHold.begin(), fileHold.end(), compFunct); //to sort files
            string pathBegin;
            string pathTemp;

            //--------------------------OUTPUT FILES--------------------------//
            string filePath=".";
            if(fileHold.size()!=0){
                DIR *dirp = opendir(filePath.c_str());
                if(dirp == NULL) {
                    perror("opendir: 137");
                    exit(1);
                }
                
                if(lFlag==0){
                    for(int i=0; i < fileHold.size();i++){
                         struct stat statbuf;
                         pathTemp = pathBegin + "./" +  fileHold.at(i);
                         if(-1 == stat(pathTemp.c_str(), &statbuf)) {
                             perror("stat:124");
                             continue; 
                         }
                        cout << fileHold.at(i) << "  " ;
                    }
                    cout  << endl;
                }
                else {
                char timeStr[200];
                time_t t;
                struct tm *tmp;
                for(int i=0, exFlag=0, dFlag=0; i < fileHold.size(); i++, exFlag=0, dFlag=0){
                    
                         struct stat statbuf;
                         pathTemp = pathBegin + "./" +  fileHold.at(i);
                         if(-1 == stat(pathTemp.c_str(), &statbuf)) {
                             perror("stat:124");
                             continue; 
                         }
                             if(S_ISDIR(statbuf.st_mode)){
                                dFlag=1;
                             }
                             if(S_IXUSR & statbuf.st_mode){
                                exFlag=1;
                             }

                             if(lFlag !=0) {//list file properties
                                 struct stat statbuf;
                                
                                 if(-1 == stat(pathTemp.c_str(), &statbuf)) {
                                     perror("stat: 187");
                                 }
                                 int n = 80;
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
                            }
                            cout << fileHold.at(i);
                            if(dFlag!=0){
                                 cout << "/";
                            }
                            else if(exFlag!=0){
                                 cout << "*";

                            }
                            
                            if(lFlag!=0){
                                cout << endl;
                            }
                            else
                                 cout << "  ";
                    }

                if(closedir(dirp)==-1){
                    perror("closedir:263");
                    exit(1);
                }}
                cout << endl;
            }

            //-----------OUTPUT PATHS NORMALLY----------------//
            for(int l=0; l < pathHold.size(); l++) 
            {
                if(pathHold.size() > 1){
                     cout << pathHold.at(l) << ':' <<  endl;
                }
                DIR *dirp = opendir(pathHold.at(l).c_str());
                if(dirp == NULL) {
                    perror("opendir:249");
                    exit(1);
                }
                pathTemp = pathBegin = pathHold.at(l); //Hold the path for use down in STAT()
                dirent *direntp;
                vector<string > dirHold;
                int counter=0;
                while((direntp = readdir(dirp))) 
                {//put dir names into container for later sorting ease
                    if(NULL == direntp){
                        perror("readdir:260");
                        exit(1);
                    }
                    dirHold.push_back(direntp->d_name);

                    counter++;
                }
                sort(dirHold.begin(), dirHold.end(),compFunct);
                char timeStr[200];
                time_t t;
                struct tm *tmp;
                int count=0;
                int bigName=0;
                for(int i=0; i<dirHold.size();i++)
                {
                    struct stat statbuf;
                    pathTemp = pathBegin + '/' + dirHold.at(i);
                    if(-1 == stat(pathTemp.c_str(), &statbuf)) {
                        perror("stat:308");
                        exit(1);
                    }
                    if(statbuf.st_size > bigName)
                    {
                       bigName = sizeof(statbuf.st_size);
                    }
                }

                
                if(lFlag!=0){
                    for(int i=0; i < dirHold.size(); i++){
                        struct stat statbuf;
                        pathTemp = pathBegin + '/' + dirHold.at(i);
                        if(-1 == stat(pathTemp.c_str(), &statbuf)) {
                            perror("stat:330");
                            exit(1);
                        }
                        if(aFlag!=0)
                        {
                            count = count + (statbuf.st_blocks/2);
                        }
                        else
                        {
                            if(dirHold.at(i)[0]=='.')
                            {
                                continue;
                                
                            }
                            count = count + (statbuf.st_blocks/2);
                        }
                    }

                    cout << "total " << count << endl;
                }
                int width = maxWidth(dirHold, aFlag);
                int max=80;
                int currWidth=80;
                for(int i=0, exFlag=0, dFlag=0; i < dirHold.size(); i++, exFlag=0, dFlag=0){
                    
                    if(dirHold.at(i)[0] == '.') {//signals hidden files
                         if(aFlag != 0 ) {//a flag set...hidden files
                         struct stat statbuf;
                         pathTemp = pathBegin + '/' + dirHold.at(i);
                         if(-1 == stat(pathTemp.c_str(), &statbuf)) {
                             perror("stat:297");
                             exit(1);
                         }
                             if(S_ISDIR(statbuf.st_mode)){
                                dFlag=1;
                             }
                             if(S_IXUSR & statbuf.st_mode){
                                exFlag=1;
                             }

                             if(lFlag !=0) {//list file properties
                                 //I know super inefficiency in code... But It's barely working atm
                                 struct stat statbuf;
                                
                                 if(-1 == stat(pathTemp.c_str(), &statbuf)) {
                                     perror("stat:300");
                                     cerr << "error: " << dirHold.at(i) << endl;
                                     exit(1);
                                 }
                                 int n = 80;
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
                                 std::cout <<  std::left <<setw(4) << statbuf.st_nlink << ' ';
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
                                 std::cout.width(bigName); std::cout << std::right <<  statbuf.st_size << ' ';
                                 tmp = localtime(&statbuf.st_mtime);
                                 if(strftime(timeStr, 200 , "%b %d %k:%M ", tmp) ==0){
                                     perror("strftime");
                                     exit(1);
                                 }
                                 cout << timeStr ;
                            }
                            if(dFlag!=0){
                                 dirHold.at(i).append("/");
                            }
                            else if(exFlag!=0){
                                 dirHold.at(i).append("*");
                            }
                            if(lFlag==0)
                            {
                                currWidth -= width;
                                if(currWidth < 0)
                                {
                                    cout << endl;
                                    currWidth=max;
                                }
                                cout << left << setw(width) <<dirHold.at(i);
                            }
                            else
                            {
                                cout << dirHold.at(i);
                                if(lFlag!=0){
                                    cout << endl;
                                }
                            }
                            
                        }
                    }
                    else if(dirHold.at(i)[0] != '.') {//normal dir
                        struct stat statbuf;
                        pathTemp = pathBegin + '/' + dirHold.at(i);
                        if( -1 == stat(pathTemp.c_str(), &statbuf)) {
                                    perror("stat:line 272");
                                    cerr << "error: " << pathBegin << endl;
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
                            if( -1 == stat(pathTemp.c_str(), &statbuf)) {
                                    perror("stat:293");
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
                            std::cout <<  std::left << setw(4) << statbuf.st_nlink << ' ';
                            cout << getUID->pw_name << ' ';
                            cout << groupID->gr_name <<  ' ';
                            
                            cout << setw(bigName); std::cout << std::right <<  statbuf.st_size << ' ';
                            char timeStr[200];
                            struct tm* tmp;
                            tmp = localtime(&statbuf.st_mtime);
                            if(strftime(timeStr, 200 , "%b %d %k:%M ", tmp)==0){
                                perror("strftime"); 
                                exit(1);
                            }
                            cout << timeStr;
                        }
                        
                        //cout << left << setw(width) <<  dirHold.at(i);
                        if(dFlag !=0) { //-l
                            //cout << '/';
                            dirHold.at(i).append("/");
                        }
                        else if(exFlag != 0){ //-l
                             dirHold.at(i).append("*");
                        }
                        if(lFlag==0)
                        {
                            currWidth -= width;
                            if(currWidth < 0)
                            {
                                cout << endl;
                                currWidth=max;
                            }
                            cout << left << setw(width) << dirHold.at(i);
                        }
                        else
                        {
                            cout << dirHold.at(i);
                            if(lFlag !=0)
                                cout << endl;
                            else {}
                        }
                            //cout << "  ";
                    }
                    
                    pathTemp.clear();
                }
                
                if(aFlag !=0 && lFlag==0){
                    cout << endl ;
                }
                if(closedir(dirp)==-1){
                    perror("closedir:517");
                    exit(1);
                }

                cout << endl << endl;
            }
         }   
    }
    return 0;
}
