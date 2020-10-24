void syntax() {
    fprintf(stderr,
	    "usage: fdmount [options] drive_name [mount_dir]\n"
	    "       fdumount [options] drive_name\n"
	    "       fdlist\n"
	    "       fdmountd [options] drive_name [mount_dir]\n"
	    "\n"
	    "    -r --readonly    mount read-only\n"
	    "    -s --sync        synchronize writes\n"
	    "       --nodev       ignore device flags\n"
	    "       --nosuid      ignore suid flags\n"
	    "       --noexec      ignore executable flags\n"
	    "    -f --force       force mount/unmount\n"
	    "    -l --list        list known drives\n"
	    "    -d --daemon      run as daemon\n"
	    "    -i --interval n  set probing interval for -d [0.1 seconds]\n"
	    "    -o --options l   sets filesystem specific options\n"
	    "       --silent      don't print informational messages\n"
	    "       --detach      run daemon in the background\n"
	    "       --nosync      don't mount synchronously, even if daemon\n"
	    "    -p --pidfile     dump the process id of the daemon to a file\n"
	    "    -v --vfat        use vfat fs, instead of msdos\n"
	    "    -h --help        this message\n\n");
    exit(1);
}
