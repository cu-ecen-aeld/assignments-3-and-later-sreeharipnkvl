#!/bin/sh
writefile=$1
writestr=$2
if [ $# -eq 0 ]; then
        echo "Invalid Number of arguments specify file name and write string"
        exit 1
elif [ $# -eq 1 ]; then
        echo "write string is not found as argument"
        exit 1
else
        if [ -d $writefile ]; then
                echo $writestr > $writefile
                exit 0
        else
                dirpath=$( dirname $writefile )
                mkdir -p $dirpath
                if [ -d $dirpath ]; then
                        echo $writestr > $writefile
                        exit 0
                else
                        echo " Unable to create Directory"
                        exit 1
                fi
        fi
fi
