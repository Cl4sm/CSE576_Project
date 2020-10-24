static void
downtree(Window top)
{
    Window *child, dummy;
    unsigned int children, i;
    char **cpp;
    unsigned char *name;
    if (XQueryTree(dpy, top, &dummy, &dummy, &child, &children)==0)
	Fatal_Error("XQueryTree failed");
    for (i=0; i<children; i++)
    if(MyFetchName (dpy, child[i], &name)){
	for(cpp = names;*cpp!=0;cpp++)
	    if(strncmp(*cpp, (char*)name, strlen(*cpp))==0){
		doit(child[i]);
		break;
	    }
	XFree(name);
    } else
	downtree(child[i]);	/* dont go down if found a name */
    if(child)XFree((char *)child);
}