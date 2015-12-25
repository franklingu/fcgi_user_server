#!/usr/bin/bash
SECONDS=0
for (( i = 0; i < 20; i++ )); do
	./benchmark.py $i pswd &
done
wait
duration=$SECONDS
echo "$(($duration / 60)) minutes and $(($duration % 60)) seconds elapsed."
