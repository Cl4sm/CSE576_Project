pvmgettmp()
{
	static char *td = 0;

	if (!td) {
#ifndef WIN32
		if ( !(td=getenv("PVM_TMP")) )
#ifdef IMA_BEOLIN
			td = "/tmps";	/* the shared tmp */
#else
			td = "/tmp";
#endif
#else
		if ( !(td=getenv("PVM_TMP")) ) {
			if ( !(td=read_pvmregistry("PVM_TMP")) ) {
				if ( !(td=getenv("TEMP")) ) {
					td = "C:\\TEMP";
					fprintf(stderr,
							"Could not get %%PVM_TMP%% or %%TEMP%% ");
					fprintf(stderr, "-> using \"%s\".\n", td);
					fprintf(stderr,
							"Please check your PVM installation.\n");
				}
			}
		}
#endif
	}

	return td;
}
