{
#ifdef EMACS
    static  char k1[]="ABCDFH256"; /* up,dn,rt,le,ins,pgup,pgdn */
    static  char k2[]="PNFBEAnvw";
    static  char k3[]=" ._%<>3bfgjlmnpquvwx";
#else
    static  char k1[]="ABCD256"; /* up,dn,rt,le,ins,pgup,pgdn */
    static  char k2[]="EXDSnvw";
#endif
#define DEL_CHAR 8

    int key1=0,key2=0,key3=0,key4=0;
    char    *s;

    if((key1 = getch()) == -1) {    /* -1 is ERR */
        key1 = getch();
    }
#ifdef EMACS
    if (doCtrlQ)
	return key1;
#endif
    if (key1 == 9 || key1 == 17)
	return key1;
    if(key1 == 13)
    	return 10;

    if(key1 == 127)
	return DEL_CHAR; /* delete cursor char */

    if(key1 == 27) {
        key2 = getch();
#ifdef EMACS
	key2 = tolower(key2);
	if (strchr(k3, key2)) {
        	doEscap = 1;
		return key2;
	}
#endif
  	if (key2=='!') {
		doEscap = 2;
		show_note(ALT_KEY_BINDINGS);
		key3 = getch();
		return tolower(key3); 
	}
        key3 = getch();
        if ((key3 <= 0x40)||(key3 >= 0x5b))
		key4 = getch();
        else 
		key4 = 0x7e;
#ifdef SUPERDEBUG
    {
        char tbuf[80];
        show_top();
	/* sprintf(tbuf, "Line %d/%d, Col %d, char %2x_%2x-%2x+%2x", */
        sprintf(tbuf, "Line %d/%d, Col %d, S/char %d %d %d %d",
            ytru+1, ytot, xtru, key1, key2, key3, key4);
        show_note(tbuf);
	fflush(stdout);
	usleep(2000000);
    }
#endif /* SUPERDEBUG */
        if ((key4 != 0x7e)&(key3 != 0x5b)) { getch();}
	if (key3=='1' || key3=='2') {
	    if (key3=='1') key4 = key4-'0'-(key4>='6');
	    if (key3=='2') key4 = key4-'0'+9;
	    funckey=1;
	    return key4;
	}
	if (key3=='3') return DEL_CHAR;
        if((s=strchr(k1, key3)) != NULL && (key1 = k2[s-k1]) > 'a')
            funckey=1;
        key1=key1&0x1F;
        if (key1==0x1b) key1=0;
    }
#ifdef DEBUG
    {
        char tbuf[80];
        show_top();
	/* sprintf(tbuf, "Line %d/%d, Col %d, char %2x_%2x-%2x+%2x", */
        sprintf(tbuf, "Line %d/%d, Col %d, char %d %d %d %d",
            ytru+1, ytot, xtru, key1, key2, key3, key4);
        show_note(tbuf);
    }
#endif /* DEBUG */
    return key1;
}
