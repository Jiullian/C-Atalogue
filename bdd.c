//
// Created by jiullian on 26/01/24.
//

#include "bdd.h"
#include "fonctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mysql/mysql.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gtk/gtkwidget.h>

MYSQL *conn;

typedef struct {
    char ip[16];
    char hex[16];
    char binary[36];
} IPInfo;

void connexion(){

    const char *host = "localhost";
    const char *user = "jiullian";
    const char *password = "root";
    const char *dbname = "CATALOGUE";

    // Initialisation de la connexion
    conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "Erreur mysql_init: %s\n", mysql_error(conn));
        return;
    }

    // Connexion à la base de données
    if (mysql_real_connect(conn, host, user, password, dbname, 0, NULL, 0) == NULL) {
        fprintf(stderr, "Erreur mysql_real_connect: %s\n", mysql_error(conn));
        mysql_close(conn);
        return;
    }
}

void deconnexion(){
    mysql_close(conn);
}

void insertion(const char *ip, const char *mask, const char *IP_Binaire, const char *IP_Hexadecimal) {
    // Vérification si la même IP avec le même masque existe déjà
    char check_query[1024];
    sprintf(check_query, "SELECT * FROM Reseau WHERE ip = '%s' AND mask = '%s'", ip, mask);

    if (mysql_query(conn, check_query)) {
        fprintf(stderr, "Erreur lors de la vérification : %s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "Erreur mysql_store_result: %s\n", mysql_error(conn));
        return;
    }

    // Si une entrée existe déjà, on ne procède pas à l'insertion
    if (mysql_num_rows(result) > 0) {
        printf("Une adresse IP identique avec le même masque existe déjà.\n");
        mysql_free_result(result);
        return;
    }
    mysql_free_result(result);

    // Exécution de la requête d'insertion
    char insert_query[1024];
    sprintf(insert_query, "INSERT INTO Reseau (ip, mask, binaire, hexadecimal) VALUES ('%s', '%s', '%s', '%s')",
            ip, mask, IP_Binaire, IP_Hexadecimal);

    if (mysql_query(conn, insert_query)) {
        fprintf(stderr, "Erreur mysql_query: %s\n", mysql_error(conn));
        mysql_close(conn);
        return;
    }

    printf("Insertion effectuée avec succès.\n");
}

void remplir_liste_ip(GtkWidget *list_box) {
    MYSQL_RES *result;
    MYSQL_ROW row;

    if (mysql_query(conn, "SELECT IP FROM Reseau")) {
        fprintf(stderr, "Erreur mysql_query: %s\n", mysql_error(conn));
        mysql_close(conn);
        return;
    }

    result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "Erreur mysql_store_result: %s\n", mysql_error(conn));
        mysql_close(conn);
        return;
    }

    while ((row = mysql_fetch_row(result))) {
        GtkWidget *label = gtk_label_new(row[0]);
        gtk_list_box_insert(GTK_LIST_BOX(list_box), label, -1);
    }

    mysql_free_result(result);
}

void suppression(const char *ip) {
    // Construction de la requête DELETE
    char query[1024];
    sprintf(query, "DELETE FROM Reseau WHERE ip = '%s'", ip);

    // Exécution de la requête DELETE
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Erreur mysql_query: %s\n", mysql_error(conn));
        mysql_close(conn);
        return;
    }

    // Vérification si des lignes ont été affectées
    if (mysql_affected_rows(conn) == 0) {
        printf("Aucune adresse IP correspondante trouvée dans la base de données.\n");
    } else {
        printf("Suppression effectuée avec succès.\n");
    }
}

void rechercher_et_afficher_par_mask_gtk(const char *mask, GtkWindow *parent_window) {
    MYSQL_RES *result;
    MYSQL_ROW row;
    char query[1024];

    // Assurez-vous d'avoir déjà connecté à votre base de données
    sprintf(query, "SELECT * FROM Reseau WHERE mask = '%s'", mask);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Erreur lors de l'exécution de la requête: %s\n", mysql_error(conn));
        return;
    }

    result = mysql_store_result(conn);
    if (!result) {
        fprintf(stderr, "Erreur lors de la récupération des résultats: %s\n", mysql_error(conn));
        return;
    }

    // Création de la fenêtre pour afficher les résultats
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    GtkWidget *list_box = gtk_list_box_new();

    gtk_window_set_title(GTK_WINDOW(window), "Résultats de recherche par mask");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 250);

    gtk_container_add(GTK_CONTAINER(window), scrolled_window);
    gtk_container_add(GTK_CONTAINER(scrolled_window), list_box);

    // Peupler la liste avec les résultats de la requête
    while ((row = mysql_fetch_row(result))) {
        char *display_text = g_strdup_printf("IP: %s, Mask: %s, Binary: %s, Hex: %s", row[1], row[2], row[3], row[4]);
        GtkWidget *label = gtk_label_new(display_text);
        gtk_list_box_insert(GTK_LIST_BOX(list_box), label, -1);
        g_free(display_text);
    }

    gtk_widget_show_all(window);
    mysql_free_result(result);
}
