#!/bin/bash

join -a1 -a2 <(< "$1" sort -k1,1) <(< "$2" sort -k1,1) | perl -lane 'print "@F[0] " . (@F[1] + @F[2])' | sort -nrk2,2
