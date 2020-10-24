  FILE * dfile;
 
  dfile=fopen(vol->name, "r");
  if (dfile==NULL) {
    char msgtxt[2048];
    sprintf(msgtxt, "Can't load volume %s, using object color", vol->name); 
    rt_ui_message(MSG_ERR, msgtxt);
    return;
  }  
 
  if (rt_mynode()==0) {
    char msgtxt[2048];
    sprintf(msgtxt, "Loading %dx%dx%d volume set from %s",
	vol->xres, vol->yres, vol->zres, vol->name);
    rt_ui_message(MSG_0, msgtxt);
  } 
  vol->data = malloc(vol->xres * vol->yres * vol->zres);

  fread(vol->data, 1, (vol->xres * vol->yres * vol->zres), dfile);

  vol->loaded=1;
}
