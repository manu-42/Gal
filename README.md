# Gal
Générateur d'analyse lexicale

Utilisation : ```bin/main [--nograph] exp``` où `exp` est un fichier
contenant une expression rationnelle simple finissant par '\\n'.

L'option `--nograph` permet de ne pas générer la représentation graphique
avec `dot`. Sinon, génère des fichiers au format dot dans le réperoire
 `dot_src` et les pdf correspondants dans le répertoire `pdf`.

Le code source de l'analyseur lexical lui-même est construit dans le
réperoire `analyseur_src`.
