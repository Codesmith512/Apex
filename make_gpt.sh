#!/bin/bash

gdisk $1 <<EOF
n
1

+1M
ef02
n
2


8300
w
y
EOF