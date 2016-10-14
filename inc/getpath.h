char *getpath(const char *pathname) {
    char *dup;
    dup = strdup(pathname);
    char *pos = strrchr(dup, '/');
    if (pos != NULL) {
        *pos = '\0';
    }
   return dup;
}
