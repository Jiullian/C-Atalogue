//
// Created by jiull on 25/01/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include "fonctions.h"
#include <gtk/gtk.h>

void affichage_menu(){
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *buttonAjouterIP;
    GtkWidget *buttonAfficherIPs;
    GtkWidget *buttonRechercherParMask;
    GtkWidget *buttonSupprimerIP;
    GtkWidget *buttonQuitter;

    // Initialiser GTK
    gtk_init(NULL, NULL);

    // Créer une nouvelle fenêtre, et définir ses propriétés
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "C-ATALOGUE IP");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Créer une VBox pour organiser les widgets verticalement
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Ajouter un bouton pour chaque option de menu
    buttonAjouterIP = gtk_button_new_with_label("Ajouter une nouvelle adresse IP");
    gtk_box_pack_start(GTK_BOX(vbox), buttonAjouterIP, TRUE, TRUE, 0);

    buttonAfficherIPs = gtk_button_new_with_label("Afficher toutes les IP");
    gtk_box_pack_start(GTK_BOX(vbox), buttonAfficherIPs, TRUE, TRUE, 0);

    buttonRechercherParMask = gtk_button_new_with_label("Rechercher par mask");
    gtk_box_pack_start(GTK_BOX(vbox), buttonRechercherParMask, TRUE, TRUE, 0);

    buttonSupprimerIP = gtk_button_new_with_label("Supprimer une IP");
    gtk_box_pack_start(GTK_BOX(vbox), buttonSupprimerIP, TRUE, TRUE, 0);

    buttonQuitter = gtk_button_new_with_label("Quitter");
    gtk_box_pack_start(GTK_BOX(vbox), buttonQuitter, TRUE, TRUE, 0);
    g_signal_connect_swapped(buttonQuitter, "clicked", G_CALLBACK(gtk_widget_destroy), window);

    // Afficher tous les widgets dans la fenêtre
    gtk_widget_show_all(window);

    g_signal_connect(buttonAjouterIP, "clicked", G_CALLBACK(on_ajouter_ip_clicked), NULL);

    g_signal_connect(buttonAfficherIPs, "clicked", G_CALLBACK(on_afficher_ips_clicked), NULL);

    g_signal_connect(buttonSupprimerIP, "clicked", G_CALLBACK(on_supprimer_ip_clicked), window);

    g_signal_connect(buttonRechercherParMask, "clicked", G_CALLBACK(on_rechercher_par_mask_clicked), window);

    // Passer le contrôle à la boucle principale de GTK
    gtk_main();
}