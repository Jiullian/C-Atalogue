cmake_minimum_required(VERSION 3.27)
project(C_Atalogue C)

set(CMAKE_C_STANDARD 11)

# Ajouter les chemins d'inclusion pour MySQL
include_directories(/usr/include/mariadb /usr/include/mariadb/mysql)

# Trouver GTK3 et ajouter ses chemins d'inclusion
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

# Cette ligne ajoute le préfixe de chemin spécifique à vos chemins d'inclusion
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

# Ajoutez vos fichiers source ici
add_executable(C_Atalogue main.c menu.c menu.h fonctions.c fonctions.h bdd.c bdd.h)

# Lier avec les bibliothèques
target_link_libraries(C_Atalogue mariadb ${GTK3_LIBRARIES} m)
