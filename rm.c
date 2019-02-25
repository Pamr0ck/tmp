#include <sys/types.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#define SIZE 256

void rm(char * strok,char* origin){
    char file[SIZE]="";
    size_t len=strlen(strok);
    int k;
    DIR *dir = opendir(origin);
    if(dir) {
        struct dirent *de = readdir(dir);
        while (de) {
            k=0;
            if(de->d_type==4 && de->d_name[0]!='.') {
                char buf[SIZE];
                strcat(buf,origin);
                strcat(buf,"/");
                strcat(buf,de->d_name);
                rm(strok,buf);
                de = readdir(dir);
            }
            
            else if(de->d_type!=4 && len<=strlen(de->d_name)){
                if(!strncmp(strok,de->d_name,len)){
                    strcat(file,origin);
                    strcat(file,"/");
                    strcat(file,de->d_name);
                    k=1;
                    printf("want to dell %s\n",file);
                    remove(file);
                }
                de = readdir(dir);
            }
            else{
                de = readdir(dir);
            }
            for (int x = 63; x>=0; x--) {
                file[x] = '\0';
            }
        }
    }
    closedir(dir);
}

int main(){
    char strok[SIZE];
    fgets(strok,SIZE,stdin);
    size_t size=strlen(strok);
    strok[size-1]='\0';
    rm(strok,".");
    return 0;
}