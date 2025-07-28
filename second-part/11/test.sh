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

    echo "Compile files in each individual folder ${folder}" 

    # Store file names in an array
    mapfile -t files < <(ls ./test-programs/programs/${folder})
    mkdir ${folder} 
    cd ${folder}
    for file in "${files[@]}"; do
        echo "Compile run for individual file ${file}"

        ../jack-compiler.out ../test-programs/programs/${folder}/${file} >> /dev/null
    done   
    cd ..
done


for folder in "${folders[@]}"; do

    echo "Test run for individual folder ${folder}" 

    # Store file names in an array
    mapfile -t files < <(ls ./test-programs/snapshots/${folder})

    for file in "${files[@]}"; do

        echo "Test run for individual snapshot ${file}"


        if [[ "$file" == *.xml ]]; then
            # format snapshot files
            (cat ./test-programs/snapshots/${folder}/${file%.*}.xml |
            sed -E 's|>([^<[:space:]]+)<|> \1 <|g' |
            xmllint --format  - > ./test-programs/snapshots/${folder}/${file%.*}.tmp) &&
            mv ./test-programs/snapshots/${folder}/${file%.*}.tmp ./test-programs/snapshots/${folder}/${file%.*}.xml

            # format compiled files
            (cat ./${folder}/${file%.*}.xml | 
            sed -E 's|>([^<[:space:]]+)<|> \1 <|g' | 
            xmllint --format  - > ./${folder}/${file%.*}.tmp) && 
            mv ./${folder}/${file%.*}.tmp ./${folder}/${file%.*}.xml

        fi

        res=$(diff ./test-programs/snapshots/${folder}/${file} ./${folder}/${file})

        if [ -n "$res" ]; then
            echo "${folder}/${file} - NOT OK" >> ${rootDir}/test-result/test-result.txt 
            mkdir  ${rootDir}/test-result/${folder} 2> /dev/null
            cat ./${folder}/${file} > ${rootDir}/test-result/${folder}/${file}
            cat ./test-programs/snapshots/${folder}/${file} > ${rootDir}/test-result/${folder}/${file}-snapshot
            echo $res > ${rootDir}/test-result/${folder}/${file}-diff
        else 
            if [$? -eq 0]; then
                echo "${folder}/${file} - OK" >> ${rootDir}/test-result/test-result.txt 
            else 
                echo "${folder}/${file} - NOT OK" >> ${rootDir}/test-result/test-result.txt 
            fi
        fi
    done   
done


# cleanup
cd $rootDir
rm -rf  ./test 
