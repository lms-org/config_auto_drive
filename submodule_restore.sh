#!/bin/bash

#set some values
set -e

#Check params
force=false
initGit=false
while getopts ":cp" opt; do :
    case $opt in
#TODO f doesn't work?!
      c)
       force=true;;
#      i)
#       initGit=true;;
    esac
done
########################################
################DO STUFF################
########################################

#(re)-init git if wanted
#if [ "$initGit" = true ]; then
rm -rf .git
git init
#fi

#TODO why is a tempfile is needed?
git config -f .gitmodules --get-regexp '^submodule\..*\.path$' > tempfile
#read 3 lines etc.
while read -u 3 path_key path
do
    url_key=$(echo $path_key | sed 's/\.path/.url/')
    url=$(git config -f .gitmodules --get "$url_key")
#TODO Check if directory exists, if not ignore force
    if [ "$force" = false ];then
            read -p "Are you sure you want to delete $path and re-initialize as a new submodule? " yn
    case $yn in
	[Yy]* );;
        [Nn]* ) continue;;
        * ) echo "Please answer yes or no."
#TODO doesn't repeat loop
        continue;;
    esac
    fi
    #create submodule
    #TODO it terminates if submodule has been added before
    rm -rf $path
    git submodule add $url $path
    echo "$path has been initialized"

done 3<tempfile

rm tempfile
