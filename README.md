# Mastermind en C

## Compiler

Vous devez avoir installé make et gcc

```
$ make -b
```
*(-b: recompile même si les fichiers n'ont pas eu de modifications)*

## Exécuter

```
$ ./build/main.o
```

## Tester

Vous devez avoir installé `bc`, `tail` et `awk`

```
$ ./scripts/testAIEfficiency.sh {iterations}
```

`interation`: Nombre d'itération pour tester l'I.A., compris entre 1 et 1000

*(N'oubliez pas d'ajouter la permission pour l'execution)*

## Déboguer

J'ai personnellement utilisé `gdb` en accord avec le paramètre `-g` pour gcc
