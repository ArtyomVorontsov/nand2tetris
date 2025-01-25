#!/bin/bash

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

    echo "Test run for individual folder ${folder}" 

    # Store file names in an array
    mapfile -t files < <(ls ./test-programs/programs/${folder})

    for file in "${files[@]}"; do

        echo "Test run for individual file ${file}"

        ./jack-compiler.out ./test-programs/programs/${folder}/${file} >> /dev/null

        # format snapshot files
        (cat ./test-programs/snapshots/${folder}/${file%.*}.xml |
        sed -E 's|>([^<[:space:]]+)<|> \1 <|g' |
        xmllint --format  - > ./test-programs/snapshots/${folder}/${file%.*}.tmp) &&
        mv ./test-programs/snapshots/${folder}/${file%.*}.tmp ./test-programs/snapshots/${folder}/${file%.*}.xml

        # format compiled files
        (cat ./${file%.*}.xml | 
        sed -E 's|>([^<[:space:]]+)<|> \1 <|g' | 
        xmllint --format  - > ./${file%.*}.tmp) && 
        mv ./${file%.*}.tmp ./${file%.*}.xml

        res=$(diff ./test-programs/snapshots/${folder}/${file%.*}.xml ./${file%.*}.xml)

        if [ -n "$res" ]; then
            echo "${folder} ${file} - NOT OK" >> ${rootDir}/test-result/test-result.txt 
            mkdir  ${rootDir}/test-result/${folder} 2> /dev/null
            cat ./${file%.*}.xml > ${rootDir}/test-result/${folder}/${file%.*}.xml
            cat ./test-programs/snapshots/${folder}/${file%.*}.xml > ${rootDir}/test-result/${folder}/${file%.*}-snapshot.xml
            echo $res > ${rootDir}/test-result/${folder}/${file%.*}Diff.xml
        else 
            echo "${folder}/${file} - OK" >> ${rootDir}/test-result/test-result.txt 
        fi
    done   
done


# cleanup
cd $rootDir
rm -rf  ./test 
