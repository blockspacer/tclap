#!/bin/sh

../examples/test2 -i 10 -s hello -i 9 > tmp.out 2>&1

if cmp -s tmp.out test11.out; then
	exit 0
else 
	exit 1
fi

