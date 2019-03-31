echo "algo,taille,capacite,temps,revenu" > ./results.csv
ex_folder="tp2/exemplaires"
for algo in {"glouton","progdyn","local"}; do
	for ex in $(LC_ALL=C ls ${ex_folder}); do
		size=$(echo $ex | cut -d- -f2)
		qty=$(echo $ex | cut -d- -f3)
		t=$(./tp.sh -e ./${ex_folder}/${ex} -a $algo -t -r)
		echo $algo,$size,$qty,$t
	done
done >> results.csv
