SDL_Surface *LoadImage(char *datafile, int transparent)   // reads one png into the memory
{
  SDL_Surface *pic=NULL,*pic2=NULL;
  char filename[200];
  int i=0;

  sprintf(filename,"%s/gfx/%s",DATAPATH,datafile);
  pic=IMG_Load(filename);
  while ( pic == NULL ) {
    strcpy(DATAPATH,PATH[i]);
    sprintf(filename,"%s/gfx/%s",DATAPATH,datafile);
    pic=IMG_Load(filename);
    i++;
    
    if (i>=PATHNUM)
    {
      fprintf(stderr,"Couldn't load %s: %s\n", filename, SDL_GetError());
      exit(2);
    }
  }
  if (transparent>=3) {
      if ((NoAlpha)&&(transparent!=4)) {
//	  SDL_SetColorKey(pic,SDL_SRCCOLORKEY|SDL_RLEACCEL, GetPixel(pic,0,0));
	  SDL_SetColorKey(pic,SDL_SRCCOLORKEY, GetPixel(pic,0,0));
          pic2 = SDL_DisplayFormat(pic); 
          SDL_FreeSurface(pic);
          return (pic2);
      }
      else {
//	  SDL_SetColorKey(pic,SDL_RLEACCEL, 0);
          pic2 = SDL_DisplayFormatAlpha(pic); 
          SDL_FreeSurface(pic);
          return (pic2);
      }
  }
  if (transparent==1)
    SDL_SetColorKey(pic,SDL_SRCCOLORKEY|SDL_RLEACCEL,SDL_MapRGB(pic2->format,0xFF,0xFF,0xFF));
  if (transparent==2)
    SDL_SetColorKey(pic,SDL_SRCCOLORKEY|SDL_RLEACCEL,0);
  if (transparent>-1) {
      pic2 = SDL_DisplayFormat(pic);
      return (pic2);
  } else
      return pic;
}