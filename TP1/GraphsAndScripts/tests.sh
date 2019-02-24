echo "algo,serie,taille,temps" > ./results.csv
ex_folder="exemplaires"
for algo in {"counting","quick","quickSeuil","quickRandomSeuil"}; do
    for testset_folder in $(ls $ex_folder); do
        for ex in $(ls ${ex_folder}/${testset_folder}); do
            size=$(echo $ex | cut -d_ -f2)
            t=$(./tp.sh -e ./${ex_folder}/${testset_folder}/${ex} -a $algo -t -x 5)
            echo $algo,$testset_folder,$size,$t
        done
    done
done >> results.csv
