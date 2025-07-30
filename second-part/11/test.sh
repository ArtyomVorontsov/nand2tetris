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

echo $rootDir
cd ${rootDir}/second-part/11
# compile compiler 
make 
cd -

# report result dump
rm -rf ${rootDir}/test-result
mkdir ${rootDir}/test-result
: > ${rootDir}/test-result/test-result.txt


rm -rf ./test
# create test folder 
mkdir ./test

# copy jack compiler to test folder  
cp ${rootDir}/second-part/11/jack-compiler.out ./test

# copy test programs
cp -r ${rootDir}/second-part/11/test-programs ./test

# cd in test
cd test

# Store file names in an array
mapfile -t folders < <(ls ./test-programs/programs)

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

        ${rootDir}/secondPart/11/jack-compiler.out ${rootDir}/secondPart/11/test-programs/programs/${folder}/${file} >> /dev/null
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

        if [ -n "$res" ]; then
            echo "${folder}/${file} - NOT OK" >> ${rootDir}/test-result/test-result.txt 
            mkdir  ${rootDir}/test-result/${folder} 2> /dev/null
            cat ./${folder}/${file} > ${rootDir}/test-result/${folder}/${file}
            cat ./test-programs/snapshots/${folder}/${file} > ${rootDir}/test-result/${folder}/${file}-snapshot
            echo $res > ${rootDir}/test-result/${folder}/${file}-diff
        else
            if [ $? -eq 0 ]; then
                echo "${folder}/${file} - OK" >> ${rootDir}/test-result/test-result.txt 
            else 
                echo "${folder}/${file} - NOT OK" >> ${rootDir}/test-result/test-result.txt 
            fi
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
