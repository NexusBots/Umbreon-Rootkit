#include <stdio.h>
FILE *preload;

int main(int argc){
    if(preload = fopen("/etc/ld.so.preload","w")){
        fprintf(preload, "\n");
        fclose(preload);
        system("/bin/echo '[+] fixed'"); 
    // why system(echo)? because it lets us know the lib error really is fixed
    }
    else printf("[-] NOW YOU FUCKED UP\n[-] http://www.youtube.com/watch?v=OGp9P6QvMjY\n");
}
