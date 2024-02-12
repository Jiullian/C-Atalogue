//
// Created by jiullian on 26/01/24.
//

#ifndef C_ATALOGUE_BDD_H
#define C_ATALOGUE_BDD_H
#include <gtk/gtk.h>
#include <gtk/gtkwidget.h>

void connexion();
void deconnexion();
void insertion(const char *ip, const char *mask, const char *IP_Binaire, const char *IP_Hexadecimal);
void liste();
void suppression(const char *ip);
void recherche(const char *ip_a, const char *ip_b, const char *mask);
void rechercher_et_afficher_par_mask_gtk(const char *mask, GtkWindow *parent_window);

#endif //C_ATALOGUE_BDD_H
