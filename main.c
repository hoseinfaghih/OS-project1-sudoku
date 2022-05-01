#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main (){
    int pid1 = fork();
    int pid2 = fork();
    if (pid1 == 0 && pid2 == 0){
        printf("1\n");
    }
    else if (pid1 != 0 && pid2 == 0){
        printf("2\n");
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