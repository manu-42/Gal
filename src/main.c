#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "lecture.h"
#include "arbre.h"
#include "nfa.h"
#include "dfa.h"
#include "dfa_min.h"
#include "gal.h"

/*
 * Affiche la façon d'utiliser le programme.
 */
void usage(char *nom) {
    fprintf(stderr, "Usage : %s [--nograph] exp\n", nom);
    fprintf(stderr, "    exp est un fichier contenant une expression"
            " rationnelle sur une ligne, finissant par \\n.\n");
    fprintf(stderr, "    l'option --nograph permet de ne pas générer"
            " les représentations graphiques.\n");
}

/*
 * Génère le pdf grâce à dot.
 */
void dot(char *filename, char *extname, char *ext) {
    char *dotstring = malloc(sizeof(char) * (2*strlen(extname)+50));
    sprintf(dotstring,
            "dot -T pdf -o pdf/%s_%s.pdf %s", filename, ext, extname);
    system(dotstring);
}


int main(int argc, char **args) {
    unsigned char gen_graph = 1;
    if (argc >= 2 && strcmp(args[1], "--nograph")==0) {
        gen_graph = 0;
        args[1] = args[2];
        argc--;
    }
    if (argc != 2) {
        usage(args[0]);
        exit(1);
    }
    char *exp = lecture(args[1]);
    printf("expression lue '%s'\n", exp);
    char *filename = get_filename(args[1]);
    char *extname = malloc(sizeof(char) * (strlen(filename)+50));
    char *exp2 = add_concat(exp);
    printf("après modif '%s'\n", exp2);
    char *postfix = to_postfix(exp2);
    printf("Postfix : '%s'\n", postfix);

    printf("Conversion en arbre ...\n");
    TREE tree = npi_to_tree(postfix);
    sprintf(extname, "dot_src/%s.tree", filename);
    if (gen_graph) {
        printf("Création de %s_tree.pdf ...\n", filename);
        tree2file(tree, extname, exp);
        dot(filename, extname, "tree");
    }

    printf("Conversion en NFA ...\n");
    NFA nfa = tree2nfa(tree);
    sprintf(extname, "dot_src/%s.nfa", filename);
    nfa2file(nfa, extname, exp); // obligatoire pour numéroter les états
    if (gen_graph) {
        printf("Création de %s_nfa.pdf ...\n", filename);
        dot(filename, extname, "nfa");
    }

    printf("Conversion en DFA ...\n");
    DFA dfa = nfa2dfa(nfa);
    sprintf(extname, "dot_src/%s.dfa", filename);
    if (gen_graph) {
        printf("Création de %s_dfa.pdf ...\n", filename);
        dfa2file(dfa, extname, exp);
        dot(filename, extname, "dfa");
    }

    printf("Minimisation du DFA ...\n");
    DFAMIN dfamin = dfa2min(dfa);
    sprintf(extname, "dot_src/%s.min", filename);
    if (gen_graph) {
        printf("Création de %s_min.pdf ...\n", filename);
        dfamin2file(dfamin, extname, exp);
        dot(filename, extname, "min");
    }

    printf("Génération de l'analyseur lexical ...\n");
    sprintf(extname, "analyseur_src/%s.c", filename);
    gal(dfamin, extname, exp);
    char *gccstring = malloc(sizeof(char) * (2*strlen(extname)+50));
    sprintf(gccstring,"gcc -o analyseur_bin/%s %s", filename, extname);
    system(gccstring);
    printf("L'analyseur lexical a été généré.\n");
    printf("Tapez 'analyseur_bin/%s mot' pour tester un mot.\n", filename);
    return EXIT_SUCCESS;
}


