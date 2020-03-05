#!/bin/bash
if [ $# -eq 4 ]
then
    SCRIPTPATH="$( cd  "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
	echo -e "Running script for testing $2 element values split into $3,  $4 times\n"
	resultfolder=$1_$2_$3_$4
	mkdir -p $resultfolder
	printf "#!/bin/bash\n" >> $resultfolder/runandsplit.sh
	printf "$1, $2, $3, $4" >> $resultfolder/input.txt
	b=$(($3*$4))
	for ((j=1;j<=$4;j++))
	do
		for ((i=1;i<=$3;i++))
		do
			a=$(($2/$3))
			a=$(($a*$i))
			printf "echo $b tests left. Testing n=$a & ./$1 -n -t $a >> ./$resultfolder/raw.txt && " >> $resultfolder/runandsplit.sh
			b=$(($b-1))
		done
	done
	printf "python3 $SCRIPTPATH/splitfile.py $PWD/$resultfolder/raw.txt && python3 $SCRIPTPATH/splitandavg.py  $PWD/$resultfolder/raw.txt" >> $resultfolder/runandsplit.sh
	/bin/bash $resultfolder/runandsplit.sh
	rm $resultfolder/runandsplit.sh
	cp $SCRIPTPATH/plotter.m $resultfolder/
	echo "Done!"
else
	echo -e "Usage: ./complete.sh {test executable location} {max elements} {element split amount} {runs per test}\n"
fi


