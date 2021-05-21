#!/bin/bash

set -e

cd `dirname "$0"`

gcc -o checksum checksum.c -I ../include ../bin/isa-l.a
cp checksum ..
