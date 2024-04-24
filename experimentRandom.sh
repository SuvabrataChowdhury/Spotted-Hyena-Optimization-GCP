#!/bin/bash

echo 'Iteration,Uniform,StraightSkewed,ExpSkewed'

for (( i=1 ; $i<=$1 ; i=$i+1 ));
do
	var=`bc -l <<< "$RANDOM/32767"`
	uniform=`bc -l <<< "$var*100"`
	straightSkewed=`bc -l <<< "100 - 100*sqrt($var)"`
	expSkewed=`bc -l <<< "-10*l(e(-10)*(1-$var)+$var)"`

	echo "$i,$uniform,$straightSkewed,$expSkewed"
done
