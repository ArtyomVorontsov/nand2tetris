#!/bin/bash


printTestRunForFile() {
    echo -n "Test run for individual file $1" 
    for i in 1 2 3 4
    do
            sleep 0.5
            echo -n "."
    done 
    echo "";
}

printTestRunForFolder() {
    echo -n "Test run for individual folder $1" 
    for i in 1 2 3 4
    do
        sleep 0.5
        echo -n "."
    done 
        echo "";
}

# root dir 
rootDir=$(pwd)

# report result dump
rm -rf ${rootDir}/test-result
mkdir ${rootDir}/test-result
: > ${rootDir}/test-result/test-result.txt


rm -rf ./test
# create test folder 
mkdir ./test

# copy jack compiler to test folder  
cp ./jack-compiler.out ./test

# copy test programs
cp -r test-programs ./test

# cd in test
cd test

# Store file names in an array
mapfile -t folders < <(ls ./test-programs/programs)

for folder in "${folders[@]}"; do

    printTestRunForFolder ${folder}

    # Store file names in an array
    mapfile -t files < <(ls ./test-programs/programs/${folder})

    for file in "${files[@]}"; do

        printTestRunForFile ${file};

        ./jack-compiler.out ./test-programs/programs/${folder}/${file} >> /dev/null

        res=$(diff ./test-programs/snapshots/${folder}/${file%.*}.xml ./${file%.*}.xml)

        if [ -n "$res" ]; then
            echo "${folder} ${file} - NOT OK" >> ${rootDir}/test-result/test-result.txt 
            mkdir  ${rootDir}/test-result/${folder} 2> /dev/null
            echo $res > ${rootDir}/test-result/${folder}/${file%.*}.txt
            cat ./${file%.*}.xml > ${rootDir}/test-result/${folder}/${file}.txt
        else 
            echo "${folder} ${file} - OK" >> ${rootDir}/test-result/test-result.txt 
        fi
    done   
done


# cleanup
cd $rootDir
rm -rf  ./test 
echo $rootDir