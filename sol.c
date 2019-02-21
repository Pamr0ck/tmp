#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#define SIZE 32
void rgroup(char * alf,char* origin){
    char tmp;
    int k=1;
    char adres[64]="./";
    char dirr[4]="./";
    DIR *dir = opendir(origin);
    if(dir) {
        //printf("1");
        struct dirent *de = readdir(dir);
        while (de) {
            //printf("2,%s\n",de->d_name);
            if(de->d_type!=4&&isalpha(de->d_name[0])){
                for (int i=0;i<k;i++){
                    if(isalpha(de->d_name[0])){           //да, повтор условия:D
                        if (de->d_name[0]==alf[i]){ // есть папка
                            //printf("3 iffffffffffffffffffffffffff, %s\n",de->d_name);
                           
                            strcat(adres,&de->d_name[0]);
                            strcat(adres,"/");
                            strcat(adres,de->d_name);
                            rename(de->d_name,adres);
                            for (int x=63;x>1;x--){
                                adres[x]='\0';
                            }
                             
                        }
                        else{ // нет папки
                            //printf("3 else, %s\n",de->d_name);
                            tmp=de->d_name[0];
                            
                            
                            dirr[2]=tmp;
                            dirr[3]='\0';
                            mkdir(dirr,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                            //printf("3 dir, %s\n",dirr);
                            
                            strcat(adres,&tmp);
                            strcat(adres,"/");
                            strcat(adres,de->d_name);
                            
                            //printf("3 adres, %s\n",adres);
                            rename(de->d_name,adres);
                            
                    
                            
                            strcat(alf,&tmp);
                            for (int x=63;x>1;x--){  // костыль для очищения массива, буду рад, если подскажешь, как заменить его
                                adres[x]='\0';
                            }
                            printf("%d ------- %c\n",k,tmp);
                            printf("%s\n", alf);
                            
                        }
                        de = readdir(dir); 
                        k++;
                    }
                }
            }
            
            else{
                de = readdir(dir); 
            }
        }
    }
    closedir(dir);
}
int main(){
    char* alf= calloc(sizeof(char),SIZE); //алфавит для избежания создания двух одинаковых папок
    rgroup(alf,"./");
    return 0;
}