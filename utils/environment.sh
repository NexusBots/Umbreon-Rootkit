#!/bin/bash
gay=0
if [ -d /proc/vz ]; then echo "[-] We are in an OpenVZ environment."; gay=$((gay + 1)); fi
if [ -d /proc/xen ]; then echo "[.] We are in a Xen environment... this is probably OK"; fi
if [  -x '/usr/sbin/lveps' ]; then echo "[?] We may be in a CloudLinux LVE."; gay=$((gay + 1)); fi
if [ -e /dev/vda ]; then echo "[-] We are in a KVM or Xen paravirtualized environment... this is probably OK"; fi
if [ ! -e /proc ]; then echo "[-] We are in a poorly configured jail. /proc does not exist. BAIL IF YOU SEE THIS."; gay=$((gay + 1000)) fi
spooky_bins=(rkhunter tripwire chkrootkit nethogs unhide unhide.rb busybox sash busybox-static)
# Most of these don't actually bother Umbreon, but are good to be aware of pre-install.

for spooky_bin in ${spooky_bins[@]}; do
if [ -x `which $spooky_bin 2>/dev/null || echo NOPENOPENOPENOPE` ]; then echo "[-] $spooky_bin is on this box."; gay=$((gay + 1)); fi
done

if [ $gay ] ; then echo "[!] Gay shit detected. Are you sure you want to continue?"
else echo "[+] All clear. Commence install."
fi
echo "[!] Box gayness level: $gay"

