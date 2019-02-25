#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#define SIZE 64
void rgrpnna(char * numb,char * alf,char* origin){
    char num=0;
    char al='\0';
    int k=0;
    char adres[64]="./";
    char dirr[5]="./";
    int key_dir;
    DIR *dir = opendir(origin);
    if(dir) {
        struct dirent *de = readdir(dir);
        while (de) {
            if(de->d_type==8){
                key_dir=0;
                for (int i=0;i<k;i++){
                    for(int j=0;j<k;j++) {
                        if (de->d_name[j] == numb[i]&&tolower(de->d_name[j])==alf[i]) { // есть папка
                            key_dir++; //Есть 0.5 папка
                            key_dir++;
                            al=alf[i];
                            num=numb[i];
                            printf("num = %c, al = %c, key_dir = %d\n",num, al, key_dir);
                        }
                        /*if(tolower(de->d_name[j])==alf[i]){
                            key_dir++;
                            al=alf[i];
                            printf("al = %c, key_dir = %d\n",al,key_dir);
                        }
                        */
                        if(key_dir==2){
                            adres[2]=num;
                            adres[3]=al;
                            adres[4]='/';
                            printf("dirr appends %s to %c%c\n ",de->d_name,num,al);
                            strcat(adres, de->d_name);
                            rename(de->d_name, adres);
                            for (int x = 63; x > 1; x--) {
                                adres[x] = '\0';
                            }
                            break;
                        }
                        //num='-1';
                        //al='-1';

                            /*
                            adres[2] = de->d_name[0];
                            adres[3] = '/';

                            strcat(adres, de->d_name);
                            rename(de->d_name, adres);
                            for (int x = 63; x > 1; x--) {
                                adres[x] = '\0';
                            }
                            */


                    }
                }

                if(key_dir!=2){ // нет папки
                    //printf("3 else, %s\n",de->d_name);
                    for(int i=0;i<strlen(de->d_name);i++){
                        if(isdigit(de->d_name[i])){
                            num=de->d_name[i];
                            break;
                        }
                    }
                    for(int i=0;i<strlen(de->d_name);i++){
                        if(isalpha(de->d_name[i])){
                            al= (char) tolower(de->d_name[i]);
                            break;
                        }
                    }
                    dirr[2]=num;
                    dirr[3]=al;
                    dirr[4]='\0';
                    mkdir(dirr,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                    //printf("3 dir, %s\n",dirr);
                    adres[2]=num;
                    adres[3]=al;
                    adres[4]='/';
                    strcat(adres,de->d_name);
                    printf("no dirr ,%s to %c%c\n",de->d_name,num,al);
                    rename(de->d_name,adres);
                    alf[k]=al;
                    numb[k]=num;
                    
                    for (int x=63;x>1;x--){  // костыль для очищения массива, буду рад, если подскажешь, как заменить его
                        adres[x]='\0';
                    }
                    k++;
                    //printf("%d ------- %c\n",k,tmp);
                    //printf("%s\n", alf);
                    //printf("%s\n", numb);

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
    char* numb=calloc(sizeof(int),SIZE);
    char* alf= calloc(sizeof(char),SIZE);
    rgrpnna(numb,alf,"/home/kot/etu_v2.0/EXAM");
    printf("%s\n", alf);
    printf("%s\n", numb);
    free(numb);
    free(alf);
    return 0;
}