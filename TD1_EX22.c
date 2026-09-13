#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void erreur(char *message){
    fprintf(stderr, "ERR : %s\n", message); 
    exit(1); 
}

void petitFils(int * pfd_fpf){
    if (close(1) == -1) erreur("close"); 
    if (dup(pfd_fpf[1]) == -1) erreur("dup"); 
    if (close(pfd_fpf[1]) == -1) erreur("close"); 
    if (close(pfd_fpf[0]) == -1) erreur("close"); 

    execlp("ps", "ps", "-e", NULL);
    erreur("execlp"); 
}

void fils(int * pfd_pf, char* param){
    int pfd_fpf[2]; 
    if (pipe(pfd_fpf) == -1) erreur("pipe"); 

    switch(fork()){
        case -1 : erreur("fork"); 
        case 0 : 
            petitFils(pfd_fpf); 
    }

    if (close(0) == -1) erreur("close"); 
    if (dup(pfd_fpf[0]) == -1) erreur("dup"); 
    if (close(pfd_fpf[0]) == -1) erreur("close"); 
    if (close(pfd_fpf[1]) == -1) erreur("close"); 

    if (close(1) == -1) erreur("close");
    if (dup(pfd_pf[1]) == -1) erreur("dup"); 
    if (close(pfd_pf[1]) == -1) erreur("close"); 
    if (close(pfd_pf[0]) == -1) erreur("close"); 

    execlp("grep", "grep", param, NULL); 
    erreur("execlp"); 
}

int main(int argc, char* argv[]){
    if (argc != 2) erreur("param"); 

    int pfd_pf[2]; 
    if (pipe(pfd_pf) == -1) erreur("pipe"); 

    switch(fork()){
        case -1 : erreur("fork"); 
        case 0 : 
            fils(pfd_pf, argv[1]); 
    }

    if (close(0) == -1) erreur("close"); 
    if (dup(pfd_pf[0]) == -1) erreur("dup"); 
    if (close(pfd_pf[0]) == -1) erreur("close"); 
    if (close(pfd_pf[1]) == -1) erreur("close"); 

    execlp("sort", "sort", "-rn", NULL); 
    erreur("execlp"); 
}