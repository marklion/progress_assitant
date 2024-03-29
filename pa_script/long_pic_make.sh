#!/bin/bash

INPUT_FILES="$@"

rm -f /tmp/dest.jpg
for single_file in ${INPUT_FILES}
do
    rm -f /tmp/pic_tmp*
    file_ext="${single_file##*.}"
    if [ "${file_ext}" == "pdf" ]
    then
        mutool draw  -o /tmp/pic_tmp%d.jpg -r 200 ${single_file}  >/dev/null 2>&1
    elif [ "${file_ext}" == "jpg" ]
    then
        base_name=`basename ${single_file}`
        cp $single_file "/tmp/pic_tmp${base_name}"
    fi
    [ -f /tmp/dest.jpg ] && mv /tmp/dest.jpg /tmp/pic_tmp_dest.jpg
    for orig_size_pic in `ls /tmp/pic_tmp*.jpg`
    do
        convert -resize 2000 ${orig_size_pic} ${orig_size_pic}.same_width
    done
    convert /tmp/pic_tmp*.jpg.same_width -append /tmp/dest.jpg  >/dev/null 2>&1
done
rm -f /tmp/pic_tmp*

cat /tmp/dest.jpg 
