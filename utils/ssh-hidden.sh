#!/bin/bash
echo "hidden SSHing to $1 (user $2, port $3)"
ssh -o ProxyCommand="socat - tcp4-connect:$1:22,bind=:$3" $1 -l "$2"
