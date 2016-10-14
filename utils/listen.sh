#!/bin/bash

if [ $1 -ge 1 ] && [ $1 -le 65535 ] ; then # there's a better way to do this I DONT CARE
echo "[+] Listening on" $1
socat file:`tty`,raw,echo=0 tcp4-listen:$1
else
echo "[-] Not a valid port"
fi

