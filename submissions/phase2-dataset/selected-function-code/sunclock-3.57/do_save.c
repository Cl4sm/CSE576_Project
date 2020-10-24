void do_save()
{
	char bakfile[NLEN+1];

	/* prompt on empty filename */
	if((ewin.name[0] == 0)) { 
		file_save(-1,0); 
		return;
	}

	executive=MAIN;
	if(file_fout() ) {
		return; /* exit if can't write output */
	}

	fclose(fo);
	fo = 0;	/* set fo to known condition */
	/* if editing old file then delete old file */
	if (flag[NEW] == 0) {
		strcpy(bakfile, ewin.name);
		strcat(bakfile, "~");
		unlink(bakfile);
		rename(ewin.name, bakfile);
#ifdef MINIMAL
		unlink(bakfile);
#endif
	}

	rename(bbuf, ewin.name);	/* ren new temp to old name */
#ifdef X11
	set_title(ewin.name);
#endif
	flag[CHG] = 0;
	show_top();
}
