#!/bin/bash

if [ "$#" -eq 0 ]
then
  >&2 echo "Erreur: Il manque un paramètre !"
  >&2 echo "Utilisation: ./testAIEfficiency.sh [nombre d'itérations pour tester l'I.A.]"
  exit 1
fi

if [ "$1" -gt 1000 ]
then
  >&2 echo "Erreur: le nombre d'itération est trop grand ! (Max: 1000)"
  exit 1
fi

if [ "$1" -lt 1 ]
then
  >&2 echo "Erreur: le nombre d'itération est trop petit ! (Min: 1)"
  exit 1
fi

[ -d "./temp" ] && rm -rf ./temp

count=0;
total=0;

mkdir ./temp
touch ./temp/AIScore.txt

for ((i=1; i <= $1; i++));
do
    echo 2 | ./build/main.o | tail -1 | awk '{print $7}' >> ./temp/AIScore.txt
    sleep .01
done

for line in $(cat ./temp/AIScore.txt)
do
    total=$(echo $total+$line | bc )
    count=$(($count + 1))
done

echo "Temps moyen pour que l'ia trouva la solution (en tour)(sur $1 iterations): "
echo "scale=2; $total / $count" | bc

rm -rf ./temp
