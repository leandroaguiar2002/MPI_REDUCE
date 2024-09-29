#!/bin/bash
killall worker
killall manager
gcc -o worker worker.c
if [ $? -ne 0 ]
then
  echo "Erro ao compilar worker.c"
  exit 1
fi

gcc -o manager manager.c
if [ $? -ne 0 ] 
then
  echo "Erro ao compilar manager.c"
  exit 1
fi

./manager &
if [ $? -ne 0 ]
then
  echo "Erro ao iniciar o processo manager"
  exit 1
fi

for i in {0..7}
do
  ./worker $i &
  if [ $? -ne 0 ] 
  then
    echo "Erro ao iniciar o processo worker $i"
    exit 1
  fi
  sleep 1
done

rm -f manager worker