static void add_opts(char *opts)
{
    char *newopt;
    int l;
    int dos_off, ext2_off;

    dos_off = strlen(dos_options);
    ext2_off = strlen(ext2_options);
    
    while(opts && *opts) {
	newopt = strchr(opts, ',');
	if(newopt) {
	    l = newopt - opts;
	    newopt++;
	} else
	    l = strlen(opts);
	
	if((add_opt(allowed_dos_options, dos_options, &dos_off, opts, l) &
	    add_opt(allowed_ext2_options, ext2_options, &ext2_off, opts, l))) {
	    opts[l]='\0';
	    die("Illegal option %s", opts);
	}
	opts = newopt;
    }    
}
