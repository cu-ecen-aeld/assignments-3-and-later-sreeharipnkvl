#!/bin/sh
filesdir=$1
searchstr=$2
if [ $# -eq 0 ]; then
        echo "Invalid Number of arguments"
        exit 1
elif [ $# -eq 1 ]; then
        echo "Search string is not found as argument"
        exit 1
else
        if [ -d "$filesdir" ]; then
                echo "Directory exits";
                cd $filesdir;
                chmod 777 *;
                X=$( ls | wc -l);
                Y=$( grep -R $searchstr | wc -l );
                echo "The number of files are $X and the number of matching lines are $Y";
                exit 0
        else i
                echo "Directory doesnot exists"
                exit 1
        fi
fi
