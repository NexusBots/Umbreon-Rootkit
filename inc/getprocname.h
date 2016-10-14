char *get_my_procname(){
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_fopen) old_fopen = dlsym (libc, "fopen");
  char *myprocname = malloc(1024);
  FILE *cmdline;
  if( (cmdline=old_fopen("/proc/self/cmdline","r") )) {
    fread(myprocname,255,1,cmdline);
    fclose(cmdline);
  }
  else snprintf(myprocname,9,"bullshit");
  return myprocname;
}
char *get_procname(pid_t pid){
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_fopen) old_fopen = dlsym (libc, "fopen");
  char *filename = malloc(35);
  char *myprocname = malloc(1024);
  FILE *cmdline;
  snprintf(filename, 34, "/proc/%d/cmdline", pid);
  if( (cmdline=old_fopen(filename,"r") )) {
    fread(myprocname,255,1,cmdline);
    fclose(cmdline);
  }
  else snprintf(myprocname,9,"bullshit");
  return myprocname;
}

