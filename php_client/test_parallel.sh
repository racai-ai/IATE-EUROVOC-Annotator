#!/bin/sh

for i in `seq 1 100`; do
    php test.php > r$i &
done
