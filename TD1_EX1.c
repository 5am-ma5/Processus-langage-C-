#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void erreur(char *message){
    fprintf(stderr, "ERREUR : %s\n", message); 
    exit(1); 
}


int main(int argc, char* argv[]){
    int pid; 
    switch(pid = fork()){
        case -1 : 
            erreur("Création processus");
            break;  
        case 0 : 
            printf("PID processus père : %d\n", getpid()); 
            if (fork() == 0) {
                printf("PID processus fils : %d\n", getpid()); 
            }
            break; 
        default : 
            printf("PID processus grand-père : %d\n", getpid()); 
            break; 
    }
    exit(0); 
}
