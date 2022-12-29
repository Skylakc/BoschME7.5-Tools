#!/bin/sh
for i in bins/*.bin; do
  ./me7sum -r $i.txt $i | grep -E '(ABORT|WARNING)'
  grep ERROR $i.txt
done
