//
// Created by jiull on 25/01/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include "fonctions.h"

void affichage_menu(){
    printf(" ::::::::                   ::: ::::::::::: :::     :::        ::::::::   ::::::::  :::    ::: ::::::::::\n");
    printf(":+:    :+:                :+: :+:   :+:   :+: :+:   :+:       :+:    :+: :+:    :+: :+:    :+: :+:\n");
    printf("+:+                      +:+   +:+  +:+  +:+   +:+  +:+       +:+    +:+ +:+        +:+    +:+ +:+\n");
    printf("+#+       +#++:++#++:++ +#++:++#++: +#+ +#++:++#++: +#+       +#+    +:+ :#:        +#+    +:+ +#++:++#\n");
    printf("+#+                     +#+     +#+ +#+ +#+     +#+ +#+       +#+    +#+ +#+   +#+# +#+    +#+ +#+\n");
    printf("#+#    #+#              #+#     #+# #+# #+#     #+# #+#       #+#    #+# #+#    #+# #+#    #+# #+#\n");
    printf(" ########               ###     ### ### ###     ### ########## ########   ########   ########  ##########\n");
    printf("\n");
    printf("Bienvenue sur le C-ATALOGUE !\n");
}

void choix_menu(){
    int choix;
    printf("1 - Ajouter une nouvelle adresse IP\n");
    printf("2 - Afficher toutes les IP\n");
    printf("3 - Rechercher par mask\n");
    printf("4 - Supprimer une IP\n");
    printf("5 - Quitter\n");
    printf("Votre choix : ");
    scanf(" %d", &choix);
    switch (choix) {
        case 1:
            printf("Vous avez choisi d'ajouter une nouvelle adresse IP\n");
            ajouter_ip();
            break;
        case 2:
            printf("Vous avez choisi d'afficher toutes les IP\n");
            lister_ip();
            break;
        case 3:
            printf("Vous avez choisi de rechercher par mask\n");
            rechercher_mask();
            break;
        case 4:
            printf("Vous avez choisi de supprimer une IP\n");
            supprimer_ip();
            break;
        case 5:
            printf("Vous avez choisi de quitter\n");
            break;
        default:
            printf("Vous n'avez pas choisi une option valide\n");
            break;
    }
}