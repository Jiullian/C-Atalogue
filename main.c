#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "bdd.h"

int main() {
    printf("Lancement de l'application...\n");
    connexion();
    affichage_menu();
    choix_menu();
    deconnexion();
    return 0;
}
