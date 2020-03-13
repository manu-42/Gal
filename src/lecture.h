#ifndef LECTURE_H
#define LECTURE_H

/*
 * Taille maximale de l'expression.
 */
// #define MAX_SIZE_EXP 1024

/*
 * Prend en paramètre le chemin complet vers un fichier, et renvoie le
 * nom du fichier en enlevant le chemin et l'extension.
 */
char *get_filename(char *fullpath);

/*
 * Lit le fichier `nom` contenant une expression régulière, et renvoie
 * son contenu.
 * Si le contenu du fichier ne finit pas par \n ou qu'il contient un
 * caractère non reconnu, affiche un message d'erreur et termine le
 * programme.
 */
char *lecture(char *nom);

/*
 * Ajoute des points à la chaîne `src` aux endroits des concaténations
 * (par exemple ab devient a.b), remplace les couples de parenthèses
 * vides () par EPSILON et renvoie le résultat.
 */
char *add_concat(char *src);

#endif
