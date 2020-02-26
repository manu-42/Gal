#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lecture.h"
#include "arbre.h"
#include "nfa.h"
#include "dfa.h"
#include "dfa_min.h"
#include "gal.h"

void usage(char *nom) {
    fprintf(stderr, "Usage : %s exp\n", nom);
    fprintf(stderr, "    exp est un fichier contenant une expression"
            " rationnelle sur une ligne, finissant par \\n.\n");
}

int main(int argc, char **args) {
    if (argc != 2) {
        usage(args[0]);
        exit(1);
    }
    char exp[MAX_SIZE_EXP];
    lecture(args[1], exp);
    printf("expression lue '%s'\n", exp);
    char *filename = get_filename(args[1]);
    char exp2[MAX_SIZE_EXP];
    add_concat(exp, exp2);
    printf("après modif '%s'\n", exp2);
    char postfix[MAX_SIZE_EXP];
    to_postfix(exp2, postfix);
    printf("Postfix : '%s'\n", postfix);

    printf("Conversion en arbre ...\n");
    TREE tree = npi_to_tree(postfix);
    char *extname = malloc(sizeof(char) * (strlen(filename)+50));
    sprintf(extname, "dot_src/%s.tree", filename);
    printf("Création de %s_tree.pdf ...\n", filename);
    tree2file(tree, extname, exp);
    char *dotstring = malloc(sizeof(char) * (2*strlen(extname)+50));
    sprintf(dotstring, "dot -T pdf -o pdf/%s_tree.pdf %s", filename, extname);
    system(dotstring);

    printf("Conversion en NFA ...\n");
    NFA nfa = tree2nfa(tree);
    sprintf(extname, "dot_src/%s.nfa", filename);
    printf("Création de %s_nfa.pdf ...\n", filename);
    nfa2file(nfa, extname, exp);
    sprintf(dotstring, "dot -T pdf -o pdf/%s_nfa.pdf %s", filename, extname);
    system(dotstring);

    printf("Conversion en DFA ...\n");
    DFA dfa = nfa2dfa(nfa);
    sprintf(extname, "dot_src/%s.dfa", filename);
    printf("Création de %s_dfa.pdf ...\n", filename);
    dfa2file(dfa, extname, exp);
    sprintf(dotstring, "dot -T pdf -o pdf/%s_dfa.pdf %s", filename, extname);
    system(dotstring);

    printf("Minimisation du DFA ...\n");
    DFAMIN dfamin = dfa2min(dfa);
    sprintf(extname, "dot_src/%s.min", filename);
    printf("Création de %s_min.pdf ...\n", filename);
    dfamin2file(dfamin, extname, exp);
    sprintf(dotstring, "dot -T pdf -o pdf/%s_min.pdf %s", filename, extname);
    system(dotstring);

    printf("Génération de l'analyseur lexical ...\n");
    sprintf(extname, "analyseur_src/%s.c", filename);
    gal(dfamin, extname, exp);
    printf("Lancez 'gcc -o analyseur_bin/%s %s' pour compiler\n", filename, extname);

    return EXIT_SUCCESS;
}


