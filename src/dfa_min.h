#ifndef H_DFA_MIN
#define H_DFA_MIN

#include "dfa.h"

/*
 * Construit un DFA minimal en partant du DFA dfa, et renvoie son état
 * initial.
 */
DSTATE *dfa2min(DSTATE dfa);

#endif
