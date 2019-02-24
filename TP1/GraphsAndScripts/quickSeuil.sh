echo "algo,serie,taille,seuil,temps" > ./resultsSeuil.csv
ex_folder="exemplaires"
for algo in {"quickSeuil","quickRandomSeuil"}; do
    for testset_folder in $(ls $ex_folder); do
        for ex in $(ls ${ex_folder}/${testset_folder}); do
			for i in {1..20}; do
				size=$(echo $ex | cut -d_ -f2)
				t=$(./tp.sh -e ./${ex_folder}/${testset_folder}/${ex} -a $algo -t -x $i)
				echo $algo,$testset_folder,$size,$i,$t
			done
        done
    done
done >> resultsSeuil.csv
