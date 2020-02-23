#ifndef H_PILE
#define H_PILE

/*
 * une structure de pile
 */
typedef struct pile{
    void *val;
    struct pile *next;
} MAILLON, *PILE;

/*
 * Créé en renvoie une pile vide.
 */
PILE create_PILE();

/*
 * Indique si la pile est vide.
 */
int isEmpty(PILE p);

/*
 * Empile un élément.
 */
void push(PILE *p, void *val);

/*
 * Empile un caractère.
 */
void push_char(PILE *p, char ch);

/*
 * Dépile un élément.
 */
void *pop(PILE *p);

/*
 * Dépile un caractère.
 */
char pop_char(PILE *p);

/*
 * Renvoie le sommet de la pile.
 */
void *sommet(PILE p);

/*
 * Renvoie le caractère au sommet de la pile.
 */
char sommet_char(PILE p);

#endif
