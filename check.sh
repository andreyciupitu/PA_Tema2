#!/bin/bash

total=0
MAX_POINTS=140
UTILS=_utils
SOURCE_TMP_DIR=src_check

check_readme() {
	README=README

	echo "------------------------------------------------------------"
	echo "------------------------------------------------------------"
	echo "================>>>>>> Check $README <<<<<< ================"

	score=0
	max_score=7.5

	if (( $(echo "$total == 0" |bc -l)  )); then
		echo "Punctaj $README neacordat. Punctajul pe teste este $total!"
	elif [ ! -f $README ]; then
		echo "$README lipsa."
	elif [ -f $README ] && [ "`ls -l $README | awk '{print $5}'`" == "0" ]; then
		echo "$README gol."
	else
		score=$max_score
		echo "$README OK. Punctajul final se va acorda la corectare."
	fi

	total=$(bc <<< "$total + $score")

	echo "===============>>>>>> $README: $score/$max_score <<<<<< ==============="
	echo "-------------------------------------------------------------"
	echo "-------------------------------------------------------------"
}

check_cpp_errors() {
	cnt_cpp=$(cat $1 | grep "Total errors found" | cut -d ':' -f2 | cut -d ' ' -f2)
}

check_java_errors() {
	cnt_java=$(cat $1 | grep -E "(WARN)|(ERR)" | wc -l)
}

check_coding_style() {
	echo "-------------------------------------------------------------"
	echo "-------------------------------------------------------------"
	echo "===============>>>>>> Check coding style <<<<<< ==============="

	max_score=7.5
	score=$max_score

	if (( $(echo "$total == 0" |bc -l)  )); then
		score=0
		echo "Punctaj Coding style neacordat. Punctajul pe teste este $total!"
	else
		# Make a copy of sources in a temporary directory
		rm -rf $SOURCE_TMP_DIR
		mkdir $SOURCE_TMP_DIR
		cd $SOURCE_TMP_DIR
		sources=$(find .. -name *  -a \( -name '*.cpp' -o -name '*.c' -o -name '*.java' -o -name '*.h' -o -name '*.hpp'  \))
		cp $sources .

		check_cpp=../$UTILS/coding_style/check_cpp.py
		python $check_cpp *.cpp &> cpp.errors

		check_java=../$UTILS/coding_style/check_java.jar
		java_errors=../$UTILS/coding_style/java_errors.xml
		java -jar $check_java -c $java_errors *.java 2&> java.errors

		check_cpp_errors cpp.errors
		check_java_errors java.errors

		cat cpp.errors
		cat java.errors
		if [ $cnt_cpp -gt 0 ] || [ $cnt_java -gt 0 ]; then
			score=0

			echo "$cnt_cpp C/C++ errors found."
			echo "$cnt_java Java errors found."
		else
			echo "Coding style OK. Punctajul final se poate modifica la corectare."
		fi

		# Clean temporary directory
		cd ..
		rm -rf $SOURCE_TMP_DIR
	fi

	total=$(bc <<< "$total + $score")

	echo "===============>>>>>> Coding style: $score/$max_score <<<<<< ==============="
	echo "-------------------------------------------------------------"
	echo "-------------------------------------------------------------"

	rm -f cpp.errors java.errors
}

timeout_test() {
	tag=$1
	timeout=$2

	(time timeout $timeout make $tag) &> error

	cnt=`cat error | grep "'$tag' failed" | wc -l`

	if [ $cnt -gt 0 ]; then
		t=`cat error | grep "real" | cut -d 'm' -f2 | cut -d 's' -f1`
		if [  $(echo "$t > $timeout" | bc) -eq 1  ]; then
			rm -f error
			echo "$t s" > tle
		fi
	else
		t=`cat error | grep "real" | cut -d 'm' -f2 | cut -d 's' -f1`
		echo "$t s" > output.time
		rm -f error
	fi
}

run_problem() {
	name=$1
	id=$2
	verif=$3

	echo "------------------------------------------------------------"
	echo "------------------------------------------------------------"
	echo "---------------------- Problema $id: $name -----------------"

	score=0

	if  [ `find . -name ${src_names[0]} -o -name ${src_names[1]} -o -name ${src_names[2]} -o -name ${src_names[3]} | wc -l` -gt 1  ]; then
		echo "Surse multiple pentru problema $name! Trimite doar una!"
		echo "Numele sursei care contine functia main trebuie sa fie:"
		echo "${src_names[0]}, ${src_names[1]}, ${src_names[2]} sau ${src_names[3]}"
		echo "=============>>>>>> Scor : $score/$pmax <<<<<< ============="
		echo "------------------------------------------------------------"
		echo "------------------------------------------------------------"
		return
	elif [ `find . -name ${src_names[0]} | wc -l` -eq 1  ]; then
		timeout=`cat $UTILS/timeout/c.timeout$id`
		echo "---------------------- timp C => $timeout s -----------------"
	elif [ `find . -name ${src_names[1]} | wc -l` -eq 1  ]; then
		timeout=`cat $UTILS/timeout/c.timeout$id`
		echo "---------------------- timp C++ => $timeout s -----------------"
	elif [ `find . -name ${src_names[2]} | wc -l` -eq 1  ]; then
		timeout=`cat $UTILS/timeout/java.timeout$id`
		echo "---------------------- timp Java => $timeout s -----------------"
	elif [ `find . -name ${src_names[3]} | wc -l` -eq 1  ]; then
		timeout=`cat $UTILS/timeout/py.timeout$id`
		echo "---------------------- timp Python => $timeout s -----------------"
	else
		echo "Numele sursei care contine functia main trebuie sa fie:"
		echo "${src_names[0]}, ${src_names[1]}, ${src_names[2]} sau ${src_names[3]}"
		echo "=============>>>>>> Scor : $score/$pmax <<<<<< ============="
		echo "------------------------------------------------------------"
		echo "------------------------------------------------------------"
		return
	fi

	for i in `seq 0 9`; do
		if [ -z "`cat Makefile | grep run-p$id`" ]; then
			echo "Test $i problema $id .......... 0/$pointsPerTest - Regula make inexistenta!"
			continue
		fi

		IN=$TESTS_DIR/$name/input/$i-$name.in
		REF=$TESTS_DIR/$name/ref/$i-$name.ref
		OUT=$TESTS_DIR/$name/out/$i-$name.out

		if [ ! -f $IN ]; then
			echo "Test $i problema $id .......... 0/$pointsPerTest - $IN lipseste!"
			continue
		fi

		cp $IN $name.in
		cp $REF res.ok

		rm -rf $TESTS_DIR/$name/out/
		mkdir $TESTS_DIR/$name/out/

		touch $name.out
		chmod 666 $name.out

		timeout_test run-p$id $timeout

		if [ -f error ]; then
			echo "Test $i problema $id .......... 0.0/$pointsPerTest - Run time error!"
		elif [ -f tle ]; then
			echo "Test $i problema $id .......... 0.0/$pointsPerTest - TLE - $(cat tle)!"
		else
			./$verif $name $pointsPerTest

			STATUS=$(cat output.verif)
			TIME=$(cat output.time)
			SCORE=$(cat score.verif)
			echo "Test $i problema $id .......... $SCORE/$pointsPerTest - $STATUS - $TIME"
			total=$(bc <<< "$total + $SCORE")
			score=$(bc <<< "$score + $SCORE")
		fi

		# cp $name.out  $OUT

		rm -f $name.in $name.out res.ok score.verif output.verif output.time\
		  error.time error.exec error expected.time tle time.err run.err sd run.out

	done

	echo "=============>>>>>> Scor : $score/$pmax <<<<<< ============="
	echo "------------------------------------------------------------"
	echo "------------------------------------------------------------"

}


test_permutari() {
	src_names=(permutari.c permutari.cpp Permutari.java permutari.py)
	pointsPerTest=3.5
	pmax=35.0

	run_problem permutari 1 verif_p1
}

test_pokemoni() {
	src_names=(pokemoni.c pokemoni.cpp Pokemoni.java pokemoni.py)
	pointsPerTest=4.0
	pmax=40.0

	run_problem pokemoni 2 verif_p2
}

test_patrula() {
	src_names=(patrula.c patrula.cpp Patrula.java patrula.py)
	pointsPerTest=5.0
	pmax=50.0

	run_problem patrula 3 verif_p3
}

TESTS_DIR=public_tests

# Check if Makefile exists
if [ ! -f Makefile ]; then
	echo "Makefile lipsa. STOP"
	echo "=============>>>>>> Total: $total/$MAX_POINTS <<<<<< ============="
	exit
fi

# Compile and check errors
make -f Makefile build &> out.make

cnt=$(cat out.make | grep failed | wc -l)
cat out.make
rm -f out.make
if [ $cnt -gt 0 ]; then
	echo "Erori de compilare. STOP"
	echo "=============>>>>>> Total: $total/$MAX_POINTS <<<<<< ============="
	exit
fi

# Compile checker
make -f Makefile-PA all &> /dev/null
rm -rf $SOURCE_TMP_DIR

# Run tests
echo "---------------------- Run $TESTS_DIR -------------------"
test_permutari
test_pokemoni
test_patrula

# Run readme and coding style checker
check_readme
check_coding_style

# Clean junk
make -f Makefile clean &> /dev/null
make -f Makefile-PA clean &> /dev/null

# Display result
echo "=============>>>>>> Total: $total/$MAX_POINTS <<<<<< ============="
