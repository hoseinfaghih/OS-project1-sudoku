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

int n,m,p,fd,status;
char * myfifo = "myfifo";

int main (){
    char input[1000000]=" ";
    FILE* ptr;
    char ch;
    mkfifo(myfifo, 0666);
    ptr = fopen("testcase1.txt","r");
    do {
        ch = fgetc(ptr);
        strncat(input, &ch, 1);
    } while (ch != EOF);
    //printf("%s",input);
    int len = sizeof(input)/sizeof(input[0]);
    int reallen = 0;
    int i,j;
    for ( i=0;i<len;i++){
        int x = input[i];
        if (!isspace(input[i]) && x != 0){
            reallen = (i+1);
        }
    }
    //printf("%d",reallen);
    //printf ("%d",len);    
    for(i = 0; i < reallen; i++){  
        if(isspace(input[i])){  
            for(j=i;j<reallen;j++){  
                input[j]=input[j+1];  
            }  
            reallen--;
            i--;  
        }  
    }
    //printf ("%ld",strlen(input));
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
    //printf ("%d  %d  %d " , n , m ,p);
        
    int pid1 = fork();
    if (pid1 == 0){
        int pid2 = fork();
        if (pid2 == 0){
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
            // for (i=1;i<=n;i++){
            //     for (j=1;j<=n;j++){
            //         printf ("%c",table[i][j]);
            //     }
            //     printf ("\n");
            // }
            // printf ("===========\n");
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
                    printf ("%c",table[i][j]);
                }
                printf ("\n");
            }
            // printf ("1===========\n");
            fd = open(myfifo, O_WRONLY);
            write(fd,table,(n+1)*(n+1));
        }
        else if (pid2 != 0){
            waitpid(pid2,&status,0);
            fd = open(myfifo,O_RDONLY);
            char table[n+1][n+1];
            read(fd, table, (n+1)*(n+1));
            // for (i=1;i<=n;i++){
            //     for (j=1;j<=n;j++){
            //         printf ("%c",table[i][j]);
            //     }
            //     printf ("\n");
            // }
            // printf ("2===========\n");
        }
        
    }
    
}

// int fd;

//     // FIFO file path
//     char * myfifo = "/tmp/myfifo";

//     // Creating the named file(FIFO)
//     // mkfifo(<pathname>, <permission>)
//     mkfifo(myfifo, 0666)

// ;

// char arr1[80], arr2[80];
//     while (1)
//     {
//         // Open FIFO for write only
//         fd = open(myfifo, O_WRONLY);

//         // Take an input arr2ing from user.
//         // 80 is maximum length
//         fgets(arr2, 80, stdin);

//         // Write the input arr2ing on FIFO
//         // and close it
//         write(fd, arr2, strlen(arr2)+1);
//         close(fd);

//         // Open FIFO for Read only
//         fd = open(myfifo, O_RDONLY);

//         // Read from FIFO
//         read(fd, arr1, sizeof(arr1));

//         // Print the read message
//         printf("User2: %s\n", arr1);
//         close(fd);
//     }