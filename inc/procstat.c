FILE * procstat (const char *filename) { // HOLY SHIT SUCH SCANF
  char line[LINE_MAX], parent[20], newline[LINE_MAX];
  char comm[100], state;
  int ppid;
  struct stat s_fstat;
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_fopen) old_fopen = dlsym (libc, "fopen");
  if (!old_xstat) old_xstat = dlsym(libc, "__xstat");
  FILE *tmp = tmpfile();
  // put shiet here
  return tmp;
}
