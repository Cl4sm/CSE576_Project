{
    int pid;
    int i,e,c,is_default,optidx=0;
    char *drivename,*mountpoint=NULL,def_mountpoint[40],devname[40];
    int drivetype;
    static int opt_force=0, opt_list=0, opt_daemon=0,
	opt_interval=10,opt_help=0,opt_umount=0,opt_nosync=0,
	opt_noexec=0,opt_nodev=0,opt_nosuid=0, opt_vfat=0;
    int mountflags=0;
    char *opt_pidfile="/var/run/fdmount.pid"; 
#ifdef FLOPPY_ONLY
    gid_t groups[NGROUPS_MAX];
    int not_allowed = 1, ngroups;
#endif                             

    static struct option longopt[] = {
	{ "silent",	0, &opt_silent,	1 },
	{ "detach",	0, &opt_detach,	1 },
	{ "readonly",	0, NULL,	'r' },
	{ "pidfile",	1, NULL,	'p' },
	{ "noexec",	0, &opt_noexec,	MS_NOEXEC },
	{ "nodev",	0, &opt_nodev,	MS_NODEV },
	{ "nosuid",	0, &opt_nosuid,	MS_NOSUID },
	{ "sync",	0, NULL,	's' },
	{ "nosync",	0, &opt_nosync,	1 },
	{ "force",	0, &opt_force,	1 },
	{ "list",	0, &opt_list,	1 },
	{ "daemon",	0, NULL,	'd' },
	{ "options",	0, NULL,	'o' },
	{ "interval",	1, NULL,	'i' },
	{ "vfat",	0, &opt_vfat,	'v' },
	{ "help",	0, &opt_help,	1 },
	{0}
    };

    progname = strrchr(argv[0], '/');
    if(progname)
	progname++;
    else
	progname = argv[0];

    opt_umount=(strcmp(progname,"fdumount")==0);
    opt_daemon=(strcmp(progname,"fdmountd")==0);
    opt_list  =(strcmp(progname,"fdlist")==0);

#ifdef FLOPPY_ONLY
    if ((ngroups = getgroups (NGROUPS_MAX, groups)) != -1) {
    	int     i;
    	struct group *gr;

		not_allowed = getuid();

        for (i = 0; not_allowed && i < ngroups; i++)
            if ((gr = getgrgid (groups[i])))
            	not_allowed = strcmp (gr -> gr_name, "floppy");
    }
    if (not_allowed)
        die("Must be member of group floppy");
#endif


    if (geteuid()!=0) 
	die("Must run with EUID=root");
    ruid = getuid();
    rgid = getgid();
    _umask = umask(0);
    umask(_umask);

    *dos_options = 0;
    *ext2_options = 0;

    while(1) {
	c=getopt_long(argc,argv,"rsfldi:hp:o:v",longopt,&optidx);
	if (c==-1) break;
	switch(c) {
	case 'o':
	    add_opts(optarg);
	    break;
	case 'r':
	    mountflags |= MS_RDONLY;
	    break;
	case 's':
	    mountflags |= MS_SYNCHRONOUS;
	    break;
	case 'l':
	    opt_list=1;
	    break;
	case 'f':
	    opt_force=1;
	    break;
	case 'd':
	    mountflags |= MS_SYNCHRONOUS;
	    opt_daemon=1;
	    break;
	case 'i':
	    opt_interval=atoi(optarg);
	    break;
	case 'p':
	    opt_pidfile = optarg;
	    break;
	case 'h':
	    opt_help=1;
	    break;
	case 'v': 
	    opt_vfat=1; 
	    break; 
	case 0:
	    break;
	default:
	    syntax();
	}
    }

    if (opt_help) {
	syntax();
    }

    mountflags |= opt_noexec | opt_nosuid | opt_nodev;

    if (opt_vfat) {
	fsnames[T_DOS] = "vfat"; 
    }
    
    if(opt_nosync)
	mountflags &= ~MS_SYNCHRONOUS;
    
    if(opt_detach && !opt_daemon)
	die("Detach option can only be used when running as daemon\n");
    
    if(opt_force && ruid)
	die("You must be root to use the force option");

    drivename=argv[optind++];
    if (drivename) mountpoint=argv[optind++];
    if (optind<argc) syntax();
   
    if (!drivename) drivename="fd0";
   
    for(i=0;i<8;i++) {
	char dr[8];
	sprintf(dr,NAME_PATTERN,i);
	if (strcmp(dr,drivename)==0) {
	    sprintf(devname,DEVICE_PATTERN,i);
	    sprintf(def_mountpoint,DEFAULT_DIR_PATTERN,i);
	    break;
	}
    }
    if (i==8) {
	die("invalid drive name: %s\n"
            "use fd[0-7]\n",
            drivename);
    }
    if (!mountpoint) mountpoint=def_mountpoint;
    is_default=(strcmp(mountpoint,def_mountpoint)==0);

    if (opt_interval==0) opt_interval=10;
    if (opt_interval>10000) opt_interval=10000;

    /* get drive type */
    drivetype=probe_drive(devname);
    if (!drivetype) {
	die("drive %s does not exist",drivename);
    }

    e=0;
    if (opt_list) {
	list_drives();
    }
    else if (opt_daemon) {
	if (ruid!=0)
	    die("Must be root to run as daemon");

	if (opt_detach) {
	    pid = fork();
	    if (pid == -1)
		die("Failed to fork");
	    if (pid) {
		dump_pid(opt_pidfile, pid);
		exit(0); /* the father */
	    }
	    openlog(progname, 0, LOG_DAEMON); 
	    use_syslog = 1;

	    setsid();
	    chdir ("/"); /* no current directory. */
	    
	    /* Ensure that nothing of our device environment is inherited. */
	    close (0);
	    close (1);
	    close (2);      
	} else if (opt_pidfile)
	    dump_pid(opt_pidfile, getpid());
	daemon_mode(devname,mountpoint,mountflags,
		    opt_interval,drivetype);
    }
    else if (opt_umount) {
	e=do_umount(devname,opt_force);
    }
    else {
	if (ruid!=0) mountflags |= MS_NODEV|MS_NOSUID;
	e=do_mount(devname,mountpoint,mountflags,
		   opt_force,is_default,drivetype);
    }

    return e;
}
