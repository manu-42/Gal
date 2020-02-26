#! /bin/bash

for exp in regexp/*.exp; do
  name=${exp%.*}
  name=${name##*/}
  bin/main "$exp" && gcc -o "analyseur_bin/$name" "analyseur_src/$name.c"
done
