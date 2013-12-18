#!/usr/bin/python

from os import *

fl = ''
for f in listdir('.'):
	fl += f + '\n';
for f in listdir('./compiler/'):
	fl += f + '\n';
print(fl)
