cmake_minimum_required(VERSION 3.27)
project(C_Atalogue C)

set(CMAKE_C_STANDARD 11)

# Ajouter les chemins d'inclusion pour MySQL
include_directories(/usr/include/mariadb /usr/include/mariadb/mysql)

add_executable(C_Atalogue main.c
        menu.c
        menu.h
        fonctions.c
        fonctions.h
        bdd.c
        bdd.h)

# Ajouter le chemin de la bibliothèque et lier avec la bibliothèque MariaDB
link_directories(/usr/lib/x86_64-linux-gnu/)
target_link_libraries(C_Atalogue mariadb)
