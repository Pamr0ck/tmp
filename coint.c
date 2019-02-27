#include <sys/types.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#define SIZE 256

int coint(char * strok,char* origin){
    char file[SIZE]="";
    static int total=0;
    int len_myne=0;
    size_t len=strlen(strok);
    char buf[SIZE]="";
    int k;
    DIR *dir = opendir(origin);
    if(dir) {
        struct dirent *de = readdir(dir);
        while (de) {
            k=0;
            if(de->d_type==4 && de->d_name[0]!='.') {
                strcat(buf,origin);
                strcat(buf,"/");
                strcat(buf,de->d_name);
                coint(strok,buf);
                de = readdir(dir);
            }

            else if(de->d_type!=4 && len<=strlen(de->d_name)){
                for(int t= (int) (strlen(de->d_name) - 1); t > 0; t--){
                    if (de->d_name[t]=='.'){
                        len_myne=t+1;
                        break;
                    }
                }
                if(!strncmp(strok,de->d_name+len_myne,len)){
                    strcat(file,origin);
                    strcat(file,"/");
                    strcat(file,de->d_name);
                    printf("want to count %s\n",file);
                    total++;
                    for (int x = 63; x>=0; x--) {
                        file[x] = '\0';
                    }
                    //remove(file);
                }
                de = readdir(dir);
            }
            else{
                de = readdir(dir);
            }

        }
    }
    closedir(dir);
    return total;
}

int main(){
    char strok[SIZE];
    fgets(strok,SIZE,stdin);
    size_t size=strlen(strok);
    strok[size-1]='\0';
    printf("%d\n",coint(strok,"."));
    return 0;
}