#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#define SIZE 32
void rgroup(char * alf,char* origin){
    char tmp;
    int k=0;
    char adres[64]="./";
    char dirr[4]="./";
    int key_dir;
    DIR *dir = opendir(origin);
    if(dir) {
        printf("1");
        struct dirent *de = readdir(dir);
        while (de) {
            printf("2,%d %s\n",k,de->d_name);
            if(de->d_type!=4&&isalpha(de->d_name[0])){
                key_dir=0;
                for (int i=0;i<k;i++){
                    if (de->d_name[0]==alf[i]){ // есть папка
                        printf("3 if------------------,%d %s\n",k,de->d_name);
                        key_dir=1; //Есть папка
                        adres[2]=de->d_name[0];
                        adres[3]='/';
                        strcat(adres,de->d_name);
                        rename(de->d_name,adres);
                        for (int x=63;x>1;x--){
                            adres[x]='\0';
                        }
                    
                    }     
                }

                if(key_dir==0){ // нет папки
                    printf("3 else, %s\n",de->d_name);
                    tmp=de->d_name[0];
                    
                    
                    dirr[2]=tmp;
                    dirr[3]='\0';
                    mkdir(dirr,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                    printf("3 dir, %s\n",dirr);
                    adres[2]=de->d_name[0];
                    adres[3]='/';
                    strcat(adres,de->d_name);
                    rename(de->d_name,adres);
                    alf[k]=tmp;
                    for (int x=63;x>1;x--){  // костыль для очищения массива, буду рад, если подскажешь, как заменить его
                        adres[x]='\0';
                    }
                    k++;
                    printf("%d ------- %c\n",k,tmp);
                    //printf("%s\n", alf);
                    
                }
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
    char* alf= calloc(sizeof(char),SIZE); //алфавит для избежания создания двух одинаковых папок
    rgroup(alf,"./");
    printf("%s\n",alf);
    return 0;
}