//
// Created by jiullian on 26/01/24.
//

#include "bdd.h"
#include "fonctions.h"
#include <stdio.h>
#include <mysql/mysql.h>

MYSQL *conn;

void connexion(){

   // const char *host = "192.168.1.38";
   // const char *user = "desktop-theo-1.home";
    
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

void liste(){
    MYSQL_RES *result;
    MYSQL_ROW row;

    // Exécution de la requête SELECT
    if (mysql_query(conn, "SELECT IP FROM Reseau")) {
        fprintf(stderr, "Erreur mysql_query: %s\n", mysql_error(conn));
        mysql_close(conn);
        return;
    }

    // Récupération du résultat de la requête SELECT
    result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "Erreur mysql_store_result: %s\n", mysql_error(conn));
        mysql_close(conn);
        return;
    }

    int i = 1;
    // Affichage du résultat de la requête SELECT
    while ((row = mysql_fetch_row(result))) {
        printf("%d- %s\n", i, row[0]);
        i++;
    }

    // Libération du résultat de la requête SELECT
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

void recherche(const char *ip_a, const char *ip_b, const char *mask){
    MYSQL_RES *result;
    MYSQL_ROW row;

    // Construction de la requête SELECT
    char query[1024];
    sprintf(query, "SELECT * FROM Reseau WHERE ip BETWEEN '%s' AND '%s' AND mask = '%s'", ip_a, ip_b, mask);

    // Exécution de la requête SELECT
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Erreur mysql_query: %s\n", mysql_error(conn));
        mysql_close(conn);
        return;
    }

    // Récupération du résultat de la requête SELECT
    result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "Erreur mysql_store_result: %s\n", mysql_error(conn));
        mysql_close(conn);
        return;
    }

    int i = 1;
    // Affichage du résultat de la requête SELECT
    while ((row = mysql_fetch_row(result))) {
        printf("- IP [%d] : %s\n", i,row[1]);
        printf("\n");
        i++;
    }

    // Libération du résultat de la requête SELECT
    mysql_free_result(result);

}