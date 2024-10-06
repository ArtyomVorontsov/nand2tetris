#!/bin/sh 
echo $1 &&
rm -rf ./compiled && ../../../nand2tetrissoftware/tools/JackCompiler.sh $1 && mkdir compiled && cp -r ./$1/*.vm compiled && rm ./$1/*.vm
