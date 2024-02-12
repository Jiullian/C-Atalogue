//
// Created by jiull on 25/01/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <gtk/gtk.h>
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

bool estSegmentValide(int segment) {
    int valeursValides[] = {0, 128, 192, 224, 240, 248, 252, 254, 255};
    int taille = sizeof(valeursValides) / sizeof(valeursValides[0]);

    for (int i = 0; i < taille; i++) {
        if (segment == valeursValides[i]) {
            return true;
        }
    }
    return false;
}

bool verifier_mask(char mask[16]) {
    int segment1, segment2, segment3, segment4;
    char fin;

    if (sscanf(mask, "%d.%d.%d.%d%c", &segment1, &segment2, &segment3, &segment4, &fin) == 4 &&
        segment1 >= 0 && segment1 <= 255 &&
        segment2 >= 0 && segment2 <= 255 &&
        segment3 >= 0 && segment3 <= 255 &&
        segment4 >= 0 && segment4 <= 255) {
        return estSegmentValide(segment1) && estSegmentValide(segment2) &&
               estSegmentValide(segment3) && estSegmentValide(segment4);
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

void ajouter_ip_gtk(const char *ip, const char *mask, GtkWidget *parent_window) {
    if (verifier_ip((char *)ip) && verifier_mask((char *)mask)) {
        printf("L'adresse IP et le masque sont valides.\n");
        char *IP_Binaire = conversion_binaire((char *)ip);
        char *IP_Hexadecimal = conversion_hexadecimal((char *)ip);
        insertion(ip, mask, IP_Binaire, IP_Hexadecimal);
        // Afficher un message de réussite ici, en supposant que l'insertion a réussi
        GtkWidget *success_dialog = gtk_message_dialog_new(GTK_WINDOW(parent_window),
                                                           GTK_DIALOG_DESTROY_WITH_PARENT,
                                                           GTK_MESSAGE_INFO,
                                                           GTK_BUTTONS_OK,
                                                           "L'adresse IP a été ajoutée avec succès.");
        gtk_dialog_run(GTK_DIALOG(success_dialog));
        gtk_widget_destroy(success_dialog);
    } else {
        printf("L'adresse IP ou le masque n'est pas valide.\n");
        GtkWidget *error_dialog = gtk_message_dialog_new(GTK_WINDOW(parent_window),
                                                         GTK_DIALOG_DESTROY_WITH_PARENT,
                                                         GTK_MESSAGE_ERROR,
                                                         GTK_BUTTONS_CLOSE,
                                                         "L'adresse IP ou le masque n'est pas valide.");
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
    }
}

void on_ajouter_ip_clicked(GtkWidget *widget, gpointer data) {
    // Création d'une boîte de dialogue avec des champs de saisie pour l'IP et le masque
    GtkWidget *dialog, *content_area, *ip_entry, *mask_entry, *grid;

    dialog = gtk_dialog_new_with_buttons("Ajouter une nouvelle adresse IP",
                                         GTK_WINDOW(data), // Assurez-vous de passer la fenêtre principale comme data si nécessaire
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                         "_Ajouter", GTK_RESPONSE_ACCEPT,
                                         "_Annuler", GTK_RESPONSE_REJECT,
                                         NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(content_area), grid);

    ip_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Adresse IP:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), ip_entry, 1, 0, 1, 1);

    mask_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Masque:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), mask_entry, 1, 1, 1, 1);

    gtk_widget_show_all(dialog);
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));

    if (response == GTK_RESPONSE_ACCEPT) {
        const char *ip = gtk_entry_get_text(GTK_ENTRY(ip_entry));
        const char *mask = gtk_entry_get_text(GTK_ENTRY(mask_entry));
        ajouter_ip_gtk(ip, mask, GTK_WIDGET(data)); // Assurez-vous que `data` est la fenêtre principale ou appropriée
    }

    gtk_widget_destroy(dialog);
}

void on_afficher_ips_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *scroll_window = gtk_scrolled_window_new(NULL, NULL);
    GtkWidget *list_box = gtk_list_box_new();

    gtk_window_set_title(GTK_WINDOW(window), "Liste des adresses IP");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    gtk_container_add(GTK_CONTAINER(window), scroll_window);
    gtk_container_add(GTK_CONTAINER(scroll_window), list_box);

    // Appel à une nouvelle fonction pour remplir la liste
    remplir_liste_ip(list_box);

    gtk_widget_show_all(window);
}

void on_supprimer_ip_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog, *ip_entry, *content_area;
    GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
    dialog = gtk_dialog_new_with_buttons("Supprimer une adresse IP",
                                         GTK_WINDOW(data),
                                         flags,
                                         "_Annuler",
                                         GTK_RESPONSE_CANCEL,
                                         "_Supprimer",
                                         GTK_RESPONSE_ACCEPT,
                                         NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    ip_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(ip_entry), "Entrez l'adresse IP à supprimer");
    gtk_container_add(GTK_CONTAINER(content_area), ip_entry);

    gtk_widget_show_all(dialog);

    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_ACCEPT) {
        const char *ip = gtk_entry_get_text(GTK_ENTRY(ip_entry));
        if (verifier_ip((char *)ip)) {
            suppression(ip);
            GtkWidget *success_dialog = gtk_message_dialog_new(GTK_WINDOW(data),
                                                               GTK_DIALOG_DESTROY_WITH_PARENT,
                                                               GTK_MESSAGE_INFO,
                                                               GTK_BUTTONS_OK,
                                                               "Adresse IP supprimée avec succès.");
            gtk_dialog_run(GTK_DIALOG(success_dialog));
            gtk_widget_destroy(success_dialog);
        } else {
            GtkWidget *error_dialog = gtk_message_dialog_new(GTK_WINDOW(data),
                                                             GTK_DIALOG_DESTROY_WITH_PARENT,
                                                             GTK_MESSAGE_ERROR,
                                                             GTK_BUTTONS_CLOSE,
                                                             "L'adresse IP n'est pas valide.");
            gtk_dialog_run(GTK_DIALOG(error_dialog));
            gtk_widget_destroy(error_dialog);
        }
    }
    gtk_widget_destroy(dialog);
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

int ip_disponibles(char *mask) {
    char *Mask_Binaire= conversion_binaire(mask);
    int CIDR = conversion_CIDR(Mask_Binaire);
    int nb_ip = pow(2, 32 - CIDR) - 2;
    printf("Il y a %d adresses IP disponibles dans le réseau.\n", nb_ip);
    return nb_ip;
}

void calculer_IP(const char* ip, const char* mask, char* premiereIP, char* derniereIP) {
    int ipParts[4], maskParts[4], networkParts[4], broadcastParts[4];

    // Convertir les chaînes IP et masque en quatre parties entières
    sscanf(ip, "%d.%d.%d.%d", &ipParts[0], &ipParts[1], &ipParts[2], &ipParts[3]);
    sscanf(mask, "%d.%d.%d.%d", &maskParts[0], &maskParts[1], &maskParts[2], &maskParts[3]);

    // Calcul de l'adresse réseau et de l'adresse de diffusion
    for (int i = 0; i < 4; i++) {
        networkParts[i] = ipParts[i] & maskParts[i];
        broadcastParts[i] = networkParts[i] | (~maskParts[i] & 255);
    }

    // Calcul de la première adresse IP du réseau
    if (maskParts[3] != 255) {
        networkParts[3]++;
    }

    // Calcul de la dernière adresse IP du réseau
    if (maskParts[3] != 255) {
        broadcastParts[3]--;
    }

    // Construction des chaînes de la première et dernière adresse IP du réseau
    sprintf(premiereIP, "%d.%d.%d.%d", networkParts[0], networkParts[1], networkParts[2], networkParts[3]);
    sprintf(derniereIP, "%d.%d.%d.%d", broadcastParts[0], broadcastParts[1], broadcastParts[2], broadcastParts[3]);
}


void on_rechercher_par_mask_clicked(GtkWidget *widget, gpointer data) {
    // Créer une boîte de dialogue pour saisir le mask
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Rechercher par mask",
                                                    GTK_WINDOW(data),
                                                    GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                    "_Rechercher", GTK_RESPONSE_ACCEPT,
                                                    "_Annuler", GTK_RESPONSE_REJECT,
                                                    NULL);
    GtkWidget *mask_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(mask_entry), "Entrez le mask");
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_container_add(GTK_CONTAINER(content_area), mask_entry);

    gtk_widget_show_all(dialog);

    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_ACCEPT) {
        const char *mask = gtk_entry_get_text(GTK_ENTRY(mask_entry));
        rechercher_et_afficher_par_mask_gtk(mask, GTK_WINDOW(data));
    }

    gtk_widget_destroy(dialog);
}
