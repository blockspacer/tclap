#!/bin/sh

../examples/test1 -n mike > tmp.out 2>&1

if cmp -s tmp.out test2.out; then
	exit 0
else 
	exit 1
fi

