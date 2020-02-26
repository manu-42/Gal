#include <stdio.h>
#include <stdlib.h>
#include "alphabet.h"
#include "gal.h"

/*
 * Écriture du code source de l'analyseur lexicale dans le fichier
 * nommé `name`.
 */
void gal(DFAMIN dfa, char *name, char *exp) {
    FILE *fd = fopen(name, "w");
    if (fd == NULL) {
        fprintf(stderr, "Erreur à la création du %s.\n", name);
        perror("gal");
        exit(EXIT_FAILURE);
    }
    fprintf(fd, "#include <stdio.h>\n#include <stdlib.h>\n\n");
    fprintf(fd, "int letter_rank(char ch) {\n");
    fprintf(fd, "     if ('a' <= ch && ch <= 'z') return ch - 'a';\n");
    fprintf(fd, "     if ('A' <= ch && ch <= 'Z') return ch - 'A' + 26;\n");
    fprintf(fd, "     if ('0' <= ch && ch <= '9') return ch - '0' + 52;\n");
    fprintf(fd, "     return -1;\n");
    fprintf(fd, "}\n\n");
    fprintf(fd, "int main(int argc, char **args) {\n");
    fprintf(fd, "    if (argc != 2) {\n");
    fprintf(fd, "        fprintf(stderr, \"Usage : %%s mot\\n\", args[0]);\n");
    fprintf(fd, "        exit(1);\n");
    fprintf(fd, "    }\n");
    fprintf(fd, "    int trans[%d][%d] = {\n", dfa.nb_states, ALPHABET_LEN);
    for (int i_state=0; i_state<dfa.nb_states; i_state++) {
        fprintf(fd, "                        {");
        for (int i_char=0; i_char<ALPHABET_LEN; i_char++) {
            fprintf(fd, "%d, ", (dfa.trans)[i_state][i_char]);
        }
        fprintf(fd, "},\n");
    }
    fprintf(fd, "                       };\n");
    fprintf(fd, "    unsigned char accept[%d] = {", dfa.nb_states);
    for (int i_state=0; i_state<dfa.nb_states; i_state++) {
        fprintf(fd, "%d, ", (dfa.lst_accept)[i_state]);
    }
    fprintf(fd, "};\n");
    fprintf(fd, "    char *mot = args[1];\n");
    fprintf(fd, "    int state = %d;\n", dfa.init_state);
    fprintf(fd, "    char *ch = mot;\n");
    fprintf(fd, "    while (*ch != '\\0') {\n");
    fprintf(fd, "        int i_char = letter_rank(*ch);\n");
    fprintf(fd, "        if (i_char == -1) {\n");
    fprintf(fd, "            fprintf(stderr, \"Caractère non reconnu '%%c'.\\n\", *ch);\n");
    fprintf(fd, "            exit(EXIT_FAILURE);\n");
    fprintf(fd, "        }\n");
    fprintf(fd, "        state = trans[state][i_char];\n");
    fprintf(fd, "        ch++;\n");
    fprintf(fd, "    }\n");
    fprintf(fd, "    printf(\"'%%s' \", mot);\n");
    fprintf(fd, "    if (accept[state]) printf(\"appartient \");\n");
    fprintf(fd, "    else printf(\"n'appartient pas \");\n");
    fprintf(fd, "    printf(\"au langage rationnel défini par '%s'\\n\");\n", exp);
    fprintf(fd, "    return EXIT_SUCCESS;\n");
    fprintf(fd, "}\n");
    fprintf(fd, "\n");
    fprintf(fd, "\n");

}
