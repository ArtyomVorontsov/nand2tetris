#!/bin/bash

VERBOSE=false

while [ "$#" -gt 0 ]; do
    
    case "$1" in  
        -v|--verbose)
            VERBOSE=true
            ;;
        *)
            echo "Unknown parameter";
            exit 1;
            ;;
    esac

    shift
done;

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
cp ${rootDir}/jack-compiler.out ./test

# copy test programs
cp -r ${rootDir}/test-programs ./test

# cd in test
cd test

# Store file names in an array
mapfile -t folders < <(ls ${rootDir}/test-programs/programs)

for folder in "${folders[@]}"; do

    if [ "$VERBOSE" == "true" ]; then
        echo "Compile files in each individual folder ${folder}" 
    fi

    # Store file names in an array
    mapfile -t files < <(ls ./test-programs/programs/${folder})
    mkdir ${folder} 
    cd ${folder}
    for file in "${files[@]}"; do
        if [ "$VERBOSE" == "true" ]; then
            echo "Compile run for individual file ${file}"
        fi

        ${rootDir}/jack-compiler.out ${rootDir}/test-programs/programs/${folder}/${file} >> /dev/null
    done   
    cd ..
done


for folder in "${folders[@]}"; do

    if [ "$VERBOSE" == "true" ]; then
        echo "Test run for individual folder ${folder}" 
    fi

    # Store file names in an array
    mapfile -t files < <(ls ./test-programs/snapshots/${folder})

    for file in "${files[@]}"; do

        if [ "$VERBOSE" == "true" ]; then
            echo "Test run for individual snapshot ${file}"
        fi


        if [[ "$file" == *.xml ]]; then
            # format snapshot files
            (cat ./test-programs/snapshots/${folder}/${file%.*}.xml |
            sed -E 's|>([^<[:space:]]+)<|> \1 <|g' |
            xmllint --format  - > ./test-programs/snapshots/${folder}/${file%.*}.tmp) 2> /dev/null &&
            mv ./test-programs/snapshots/${folder}/${file%.*}.tmp ./test-programs/snapshots/${folder}/${file%.*}.xml

            # format compiled files
            (cat ./${folder}/${file%.*}.xml | 
            sed -E 's|>([^<[:space:]]+)<|> \1 <|g' | 
            xmllint --format  - > ./${folder}/${file%.*}.tmp) 2> /dev/null && 
            mv ./${folder}/${file%.*}.tmp ./${folder}/${file%.*}.xml

        fi

        res=$(diff ./test-programs/snapshots/${folder}/${file} ./${folder}/${file} 2> /dev/null)

        #echo "res $? ${file}"
        if [ "$?" -ge 1 ]; then
            echo "${folder}/${file} - NOT OK" >> ${rootDir}/test-result/test-result.txt 
            mkdir  ${rootDir}/test-result/${folder} 2> /dev/null
            cat ./${folder}/${file} > ${rootDir}/test-result/${folder}/${file}
            cat ./test-programs/snapshots/${folder}/${file} > ${rootDir}/test-result/${folder}/${file}-snapshot
            echo $res > ${rootDir}/test-result/${folder}/${file}-diff
        else
            echo "${folder}/${file} - OK" >> ${rootDir}/test-result/test-result.txt 
        fi
    done   
done

RESULT=$(cat ${rootDir}/test-result/test-result.txt | grep "NOT OK" | wc -c)

# cleanup
cd $rootDir
rm -rf  ./test

if [ $RESULT -eq 0 ]; then
    exit 0;
else 
    exit 1;
fi
