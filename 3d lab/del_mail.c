#include <sys/types.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#include <regex.h>
#define SIZE 256

void del_mail(char* origin){
    char file[SIZE]="";
    char buf[SIZE]="";
    int k;
    FILE *fl;
    DIR *dir = opendir(origin);
    if(dir) {
        struct dirent *de = readdir(dir);
        while (de) {
            k=0;
            if(de->d_type==4 && de->d_name[0]!='.') {
                strcat(buf,origin);
                strcat(buf,"/");
                strcat(buf,de->d_name);
                del_mail(buf);
                de = readdir(dir);
            }

            else if(de->d_type!=4){
                strcat(file,origin);
                strcat(file,"/");
                strcat(file,de->d_name);

                fl=fopen(file, "r");
                if(fl==NULL){
                    printf("AXTUNG fl\n");
                    return;
                }
                FILE* te = fopen("temp", "w");
                if(te==NULL){
                    printf("AXTUNG\n");
                    return;
                }
                char tmp[SIZE]="";
                while (fgets(tmp, SIZE, fl)){
                    char re[] = "[[:alpha:]]+[[:alnum:]]*@[[:alpha:]]+\\.[[:alpha:]]+";
                    regex_t reg;
                    if (regcomp(&reg, re, REG_EXTENDED)){
                        printf("Error\n");
                        return;
                    }
                    if(regexec(&reg, tmp, 0, NULL, 0)){
                        fprintf(te, "%s", tmp);
                        //printf("want to dell %s\n",file);
                    }
                    else{
                        fprintf(te, "\n");
                        printf("want to dell %s\n",file);
                    }
                }
                fclose(te);
                fclose(fl);
                char name[SIZE]="";
                strcpy(name, de->d_name);

                remove(de->d_name);
                rename("temp", name);

                //printf("want to dell %s\n",file);
                for (int x = 63; x>=0; x--) {
                    file[x] = '\0';
                }
                //remove(file);
                de = readdir(dir);
            }
            
            else{
                de = readdir(dir);
            }
        }
    }
    closedir(dir);
}

int main(){
    del_mail(".");
    return 0;
}