#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void erreur(char *message){
    fprintf(stderr, "ERR : %s\n", message); 
    exit(1); 
}

void fils(int *pfd){
    if (close(1) == -1) erreur("Fermeture stdout"); 
    if (dup(pfd[1]) == -1) erreur("dup pfd[1]"); 
    if (close(pfd[1]) == -1) erreur("Fermeture pfd[1]"); 
    if (close(pfd[0]) == -1) erreur("Fermeture pfd[0]"); 

    execlp("ps", "ps", "-e", NULL); 
    erreur("procédure fils"); 
}




int main(int argc, char* argv[]){
    if (argc != 2) erreur("Nb de paramètres"); 

    int pfd[2];
    if (pipe(pfd) == -1) erreur("Création du tube"); 

    int pid; 
    switch(pid = fork()) {
        case -1 : 
            erreur("fork"); 
        case 0 : 
            fils(pfd); 
        default : 
            if (close(0) == -1) erreur("Fermeture stdin"); 
            if (dup(pfd[0]) == -1) erreur("dup"); 
            if (close(pfd[0]) == -1) erreur("Fermeture pfd[0]"); 
            if (close(pfd[1]) == -1) erreur("Fermeture pfd[1]"); 

            if (wait(NULL) == -1) erreur("Recupération processus fils"); 

            execlp("grep", "grep", argv[1], NULL); 
            erreur("prog incorrect"); 
    }
}