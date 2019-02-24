echo "algo,serie,taille,temps" > ./resultsSeuil.csv
ex_folder="exemplaires"
algo="quickRandomSeuil"
for testset_folder in $(ls $ex_folder); do
	for ex in $(ls ${ex_folder}/${testset_folder}); do
		for i in {1..10}; do
			size=$(echo $ex | cut -d_ -f2)
			t=$(./tp.sh -e ./${ex_folder}/${testset_folder}/${ex} -a ${algo} -t -x 20)
			echo $algo,$testset_folder,$size,$t
		done
	done
done >> resultsSeuil.csv
