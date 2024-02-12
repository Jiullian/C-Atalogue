#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "bdd.h"
#include <gtk/gtk.h>

extern void affichage_menu_gtk();

int main(int argc, char *argv[]) {
    printf("Lancement de l'application...\n");
    gtk_init(&argc, &argv);
    connexion();
    affichage_menu();
    deconnexion();
    return 0;
}
