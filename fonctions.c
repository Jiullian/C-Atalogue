//
// Created by jiull on 25/01/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "bdd.h"

bool verifier_ip(char ip[16]) {
    int segment1, segment2, segment3, segment4;
    char fin;
    if (sscanf(ip, "%d.%d.%d.%d%c", &segment1, &segment2, &segment3, &segment4, &fin) == 4) {
        // Vérifier que chaque segment est entre 0 et 255
        if (segment1 >= 0 && segment1 <= 255 &&
            segment2 >= 0 && segment2 <= 255 &&
            segment3 >= 0 && segment3 <= 255 &&
            segment4 >= 0 && segment4 <= 255) {
            return true;
        }
    }
    return false;
}

//Améliorer la fonction parce qu'on peut mettre n'importe quoi comme masque !!!!!!!!!!!!!
bool verifier_mask(char mask[16]) {
    int segment1, segment2, segment3, segment4;
    char fin;
    if (sscanf(mask, "%d.%d.%d.%d%c", &segment1, &segment2, &segment3, &segment4, &fin) == 4) {
        // Vérifier que chaque segment est entre 0 et 255
        if (segment1 >= 0 && segment1 <= 255 &&
            segment2 >= 0 && segment2 <= 255 &&
            segment3 >= 0 && segment3 <= 255 &&
            segment4 >= 0 && segment4 <= 255) {
            return true;
        }
    }
    return false;
}

char* conversion_binaire(char ip[16]) {
    printf("Conversion de l'adresse IP %s en binaire...\n", ip);
    static char IP_Binaire[36] = "";
    char ip_copy[16];
    strcpy(ip_copy, ip);
    char *octet = strtok(ip_copy, ".");
    int octetInt;

    while (octet != NULL) {
        octetInt = atoi(octet);
        // Convertir l'octet en binaire et concaténer dans binaryIp
        for (int i = 7; i >= 0; i--) {
            strcat(IP_Binaire, ((octetInt >> i) & 1) ? "1" : "0");
        }
        strcat(IP_Binaire, ".");
        octet = strtok(NULL, ".");
    }
    IP_Binaire[strlen(IP_Binaire) - 1] = '\0';
    printf("Conversion terminée.\n");
    return IP_Binaire;
}

char* conversion_hexadecimal(char ip[16]) {
    printf("Conversion de l'adresse IP %s en hexadecimal...\n", ip);
    static char IP_Hexadecimal[36] = "";
    char ip_copy[16];
    strcpy(ip_copy, ip);
    char *octet = strtok(ip_copy, ".");
    int octetInt;

    while (octet != NULL) {
        octetInt = atoi(octet);
        sprintf(IP_Hexadecimal + strlen(IP_Hexadecimal), "%02X", octetInt);
        octet = strtok(NULL, ".");
    }
    printf("Conversion terminée.\n");
    return IP_Hexadecimal;
}

void ajouter_ip(){
    char ip[16], mask[16];
    printf("Veuillez entrer une adresse IP : ");
    scanf(" %s", ip);
    printf("Veuillez entrer un masque : ");
    scanf(" %s", mask);
    if (verifier_ip(ip) && verifier_mask(mask)){
        printf("L'adresse IP et le masque sont valides.\n", ip);
        char *IP_Binaire = conversion_binaire(ip);
        char *IP_Hexadecimal = conversion_hexadecimal(ip);
        insertion(ip, mask, IP_Binaire, IP_Hexadecimal);
    } else {
        printf("L'adresse IP %s n'est pas valide.\n", ip);
    }
}

void lister_ip(){
    printf("Liste des IP enregistrées :\n");
    liste();
}

void supprimer_ip(){
    char ip[16];
    printf("Veuillez entrer l'adresse IP à supprimer : ");
    scanf(" %s", ip);
    if (verifier_ip(ip)){
        printf("L'adresse IP est valide.\n", ip);
        suppression(ip);
    } else {
        printf("L'adresse IP %s n'est pas valide.\n", ip);
    }
}

int conversion_CIDR(char *Mask_Binaire) {
    int CIDR = 0;
    for (int i = 0; i < strlen(Mask_Binaire); i++) {
        if (Mask_Binaire[i] == '1') {
            CIDR++;
        }
    }
    return CIDR;
}

int ip_disponibles(const char *mask) {
    char *Mask_Binaire= conversion_binaire(mask);
    int CIDR = conversion_CIDR(Mask_Binaire);
    //calculer le nombre d'ip disponibles avec  2^(32 - CIDR) -2
    int nb_ip = pow(2, 32 - CIDR) - 2;
    printf("Il y a %adresses IP disponibles dans le réseau.\n", nb_ip);
    return nb_ip;
}

void rechercher_mask(){
    char ip[16], mask[16];
    printf("Veuillez entrer une adresse IP : ");
    scanf(" %s", ip);
    printf("Veuillez entrer un masque : ");
    scanf(" %s", mask);
    if (verifier_ip(ip) && verifier_mask(mask)){
        printf("L'adresse IP et le masque sont valides.\n", ip);
        int nb_ip = ip_disponibles(mask);

    } else {
        printf("L'adresse IP %s n'est pas valide.\n", ip);
    }
}
