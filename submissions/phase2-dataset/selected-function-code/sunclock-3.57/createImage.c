createImage(Context)
struct Sundata * Context;
{
   FILE *fd;
   char *file, path[1024]="";
   int code; 

   if (runlevel == IMAGERECYCLE) {
      if (verbose)
         fprintf(stderr, 
           "Recycling image (XID %ld) and changing requested parameters...\n",
	   (gflags.colorlevel==FULLCOLORS)? (long) Context->xim : 
                                       (long) Context->mappix);
      code = 0;
      if (gflags.colorlevel)
	 goto run_direct2;
      else
         goto run_direct1;
   }

   if (color_depth<=8 && Context->flags.colorlevel>0)
     tmp_cmap = XCreateColormap(dpy, Root, visual, AllocNone);
   else
     tmp_cmap = cmap0;

   file = (Context->wintype)? Context->map_img_file : Context->clock_img_file;

 do_path:

   Context->xim = NULL;
   code = -1;

   if (file) {
      strcpy(path, file);
      if (*file != '/' && *file != '.' ) {
         if ((fd=fopen(file, "r"))) {
            fclose(fd);
         } else {
            if (verbose)
              fprintf(stderr, "%s not in current directory ...\n"
                      "Trying to load %s from share directory instead\n", 
                      file, file);
            sprintf(path, "%s%s", share_maps_dir, file);
         }
      }
   }

   if (*path && strcmp(path, Default_img_file)) {
      if ((fd = fopen(path, "r"))) 
         fclose(fd);
      else {
         file = Default_img_file;
         fprintf(stderr, "File %s doesn't seem to exist !!\n"
                         "Trying default  %s\n",
                         path, file);
         goto do_path;
      }
   }

   if (Context->wintype) {
     if (Context->map_img_file && file!=Context->map_img_file)
        StringReAlloc(&Context->map_img_file, file);
   } else {
     if (Context->clock_img_file && file!=Context->clock_img_file) {
        StringReAlloc(&Context->clock_img_file, file);
     }
   }

   if (gflags.colorlevel < FULLCOLORS) {
   retry:
     code = readVMF(path, Context);
     if (code==0 && Context->bits) {
       Context->mappix = XCreatePixmapFromBitmapData(dpy, Root,
          Context->bits, Context->geom.width,
          Context->geom.height, 0, 1, 1);
     run_direct1:
       createGData(Context, 0);
       if (color_alloc_failed) report_failure(path, 6);
       if (Context->bits) free(Context->bits);
       createGCs(Context);
       return 0;
     } else {
       if (strcmp(path, Default_img_file)) {
          report_failure(path, 1);
          strcpy(path, Default_img_file);
          report_failure(path, 7);
          goto retry;
       }
       report_failure(path, 1);
       return 1;
     }
   }

   if (strstr(path, ".gif"))
      code = readGIF(path, Context);
   else   
   if (strstr(path, ".jpg"))
      code = readJPEG(path, Context);
   else
   if (strstr(path, ".png"))
      code = readPNG(path, Context);
   else     
   if (strstr(path, ".vmf"))
      code = readVMF(path, Context);
   else
   if (strstr(path, ".xpm"))
      code = readXPM(path, Context);

   if (code) {
      report_failure(path, code);
      if (strcmp(path, Default_img_file)) {
         file = Default_img_file;
         report_failure(file, 7);
         goto do_path;
      }
   }

 run_direct2:
   if (color_depth<=8)
      quantize(Context);
   
   if (color_alloc_failed) {
         code = 6;
	 if (Context->xim) {
            XDestroyImage(Context->xim);
            Context->xim = 0;
	 }
	 return code;
   }
    
   createGData(Context, 0);
   createGCs(Context);

   return code;
}
