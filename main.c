#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int n,m,p,fd,fd1,fd2,fd3,fd4,status,status1;
char * myfifo = "/tmp/myfifo";
char * myfifo1 = "/tmp/myfifo1";
char * myfifo2 = "/tmp/myfifo2";
char * myfifo3 = "/tmp/myfifo3";
char * myfifo4 = "/tmp/myfifo4";

int main (){
    char input[1000000]=" ";
    FILE* ptr;
    char ch;
    mkfifo(myfifo, 0666);
    mkfifo(myfifo1,0666);
    mkfifo(myfifo2,0666);
    mkfifo(myfifo3,0666);
    mkfifo(myfifo4,0666);
    ptr = fopen("testcase1.txt","r");
    do {
        ch = fgetc(ptr);
        strncat(input, &ch, 1);
    } while (ch != EOF);
    int len = sizeof(input)/sizeof(input[0]);
    int reallen = 0;
    int i,j;
    for ( i=0;i<len;i++){
        int x = input[i];
        if (!isspace(input[i]) && x != 0){
            reallen = (i+1);
        }
    }
    for(i = 0; i < reallen; i++){
        if(isspace(input[i])){
            for(j=i;j<reallen;j++){
                input[j]=input[j+1];
            }
            reallen--;
            i--;
        }
    }
    int xx = strlen(input);
    int cnt = 1;
    char ch1[4] = "",ch2[4] = "",ch3[4] = "";
    for (i = 0;i<xx;i++){
        if (input[i] == '*'){
            cnt++;
        }
        else if (cnt == 1){
            strncat(ch1,&input[i],1);
        }
        else if (cnt == 2){
            strncat(ch2,&input[i],1);
        }
        else if (cnt == 3 && isdigit(input[i])){
            strncat(ch3,&input[i],1);
        }
        else {
            break;
        }
    }
    n = atoi(ch1); m = atoi (ch2) ; p = atoi (ch3) ;
    int pid1 = fork();
    int pid2 = fork();
    if (pid1 > 0 && pid2 > 0){  // Child #1
        char table [n+1][n+1];
        int x = 1 , y = 1;
        for (i=0;i<xx;i++){
            if (isalpha(input[i])){
                table[x][y] = input[i];
                if (y == n){
                    x++;
                    y = 1;
                }
                else{
                       y++;
                }
            }
        }
        for (i=1;i<=n;i++){
            for (j=1;j<=n;j++){
                table[i][j] = toupper(table[i][j]);
                if (  (table[i][j] >= 'A' && table[i][j] <= 'X' )) {
                    table[i][j] +=2;
                }
                else if (table[i][j] == 'Y'){
                    table[i][j] = 'A';
                }
                else if (table[i][j] == 'Z'){
                    table[i][j] = 'B';
                }
            }
        }
        fd = open(myfifo, O_WRONLY);
        write(fd,table,(n+1)*(n+1));
        close(fd);
        fd1 = open(myfifo1, O_WRONLY);
        write(fd1,table,(n+1)*(n+1));
        close(fd1);
        fd2 = open(myfifo2, O_WRONLY);
        write(fd2,table,(n+1)*(n+1));
        close(fd2);
        exit(0);
    }
    else if (pid1 == 0 && pid2 > 0){  //Child #2
        sleep(1);
        fd = open(myfifo,O_RDONLY);
        char table[n+1][n+1];
        read(fd, table, (n+1)*(n+1));
        close(fd);
        int mark[26] = {0};
        int flag = 1;
        for (i=1;i<=n;i++){
            for (j=1;j<=n;j++){
                int tmp = table[i][j]  - 'A';
                mark[tmp] += 1;
                if (mark[tmp] != i){
                    flag = 0;
                    break;
                }
            }
            if (flag == 0){
                break;
            }
        }
        char wr[] = "0";
        if (flag == 1){
            wr[0] = '1';
        }
        fd3 = open(myfifo3, O_WRONLY);
        write(fd3,wr,1);
        close(fd3);
        exit(0);

    }
    else if (pid1 > 0 && pid2 == 0){ // Child #3
        sleep(2);
        fd1 = open(myfifo1,O_RDONLY);
        char table[n+1][n+1];
        read(fd1, table, (n+1)*(n+1));
        close(fd1);
        int mark[26] = {0};
        int flag = 1;
        for (i=1;i<=n;i++){
            for (j=1;j<=n;j++){
                int tmp = table[j][i]  - 'A';
                mark[tmp] += 1;
                if (mark[tmp] != i){
                    flag = 0;
                    break;
                }
            }
            if (flag == 0){
                break;
            }
        }
        char wr[] = "0";
        if (flag == 1){
            wr[0] = '1';
        }

        fd4 = open(myfifo4, O_WRONLY);
        write(fd4,wr,1);
        close(fd4);
        exit(0);
    }
    else if (pid2 == 0 && pid1 == 0){  //  #Child  #4
        sleep(3);
        fd2 = open(myfifo2,O_RDONLY);
        char table[n+1][n+1];
        read(fd2, table, (n+1)*(n+1));
        close(fd2);
        int mark[26] = {0};
        int flag = 1;
        int x = n / m;
        int y = n / p ;
        int cnt = 0 ;
        for (i=1;i<=x;i++){
            for (j=1;j<=y;j++){
                cnt += 1;
                int indx = 1 , indy = 1;
                if (i > 1){
                    indy += (i-1)*m;
                }
                if (j > 1){
                    indx += (j-1)*p;
                }
                int ii,jj;
                for (ii=indx;ii<indx+p;ii++){
                    for (jj=indy;jj<indy+m;jj++){
                        int tmp = table[ii][jj]  - 'A';
                        mark[tmp] += 1;
                        if (mark[tmp] != cnt){
                            flag = 0;
                            break;
                        }
                    }
                }
            }
        }
        fd3 = open (myfifo3 , O_RDONLY);
        char flag2[1],flag3[1];
        read(fd3,flag2,1);
        close(fd3);
        fd4 = open (myfifo4 , O_RDONLY);
        read(fd4,flag3,1);
        close(fd4);
        if (flag == 1 && flag2[0] == '1' && flag3[0] == '1'){
            printf("Sudoku Puzzle constraints satisfied‬‬\n");
        }
        else{
            printf("Sudoku Puzzle is wrong\n");
        }
    }

}


