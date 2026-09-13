#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

/**
    Affichage Console : 
        -> Arrière Petit Fils 
        -> Petit Fils 
        -> Fils 
*/


void erreur(char *msg){
    fprintf(stderr, "ERREUR : %s\n", msg); 
    exit(1); 
}

void arrierePetitFils(){
    printf("arriere petit fils pid : %d\n", getpid()); 
    exit(0); 
}

void petitFils(){
    switch(fork()){
        case -1 : erreur("fork"); 
        case 0 : 
            arrierePetitFils(); 
    }
    if (wait(NULL) == -1) erreur("wait"); 
    printf("petit-fils pid = %d\n", getpid()); 
    exit(0); 
}

void fils(){
    switch(fork()){
        case -1 : erreur("fork"); 
        case 0 : 
            petitFils(); 
    }
    if (wait(NULL) == -1) erreur("wait"); 
    printf("fils pid = %d\n", getpid()); 
    exit(0); 
}

int main(int argc, char* argv[]){
    switch(fork()){
        case -1 : erreur("fork"); 
        case 0 : 
            fils(); 
    }
    exit(0); 
}