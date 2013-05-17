import os, sys, re, struct

drive = 'j:'

print 'mapping buildbot folder to j:'
try:
	os.system('net use /delete \\buildbot.intranet.bit\\archive')
except:
	pass

try:
	os.system('net use j: \\\\buildbot.intranet.bit\\archive /USER:networkuser buildbot')
except:
	# Ignore the error if the drive is already mapped.
	pass
if not os.path.exists('j:'):
	drive = '/cygdrive/j'

archives=open('archives.config').read().strip().split()
for archive in archives:
	if 'soshare' in archive.lower():
		os.system('cp %s\\%s\\soshare.exe Bundle' % (drive,archive))
	elif 'torque' in archive.lower():
		os.system('cp %s\\%s\\torque.exe Bundle' % (drive,archive))
	else:
		print 'warning: invalid line in archives.config %s' % archive