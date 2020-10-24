void * opentgafile(char * filename) {
  tgahandle * tga; 
  tga = malloc(sizeof(tgahandle));
  
  tga->ofp=fopen(filename, "r+b");
  if (tga->ofp == NULL) {
    char msgtxt[2048];
    sprintf(msgtxt, "Cannot open %s for output!", filename);
    rt_ui_message(MSG_ERR, msgtxt);
    rt_ui_message(MSG_ABORT, "Rendering Aborted.");
    return NULL;
  } 

  fseek(tga->ofp, 12, 0);  
  tga->width = fgetc(tga->ofp);
  tga->width |= fgetc(tga->ofp) << 8;
  tga->height = fgetc(tga->ofp);
  tga->height |= fgetc(tga->ofp) << 8;

  return tga;
} 
