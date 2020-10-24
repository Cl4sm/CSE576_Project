void init_SDL()  // sets the video mode
{
  int bpp=BPP,flags=0;
  const SDL_VideoInfo *info;

//  if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0 ) {ComplainAndExit();}
  puts("** Init video **");
  if ( SDL_Init(SDL_INIT_VIDEO ) < 0 ) ComplainAndExit();
//  puts("** Init timer **");
//  if ( SDL_Init(SDL_INIT_TIMER ) < 0 ) ComplainAndExit();
  info = SDL_GetVideoInfo();
  if (info->vfmt->BitsPerPixel==8) bpp=16;
  atexit(SDL_Quit);
// Set the video mode (800x600 at 16-bit depth)  
  puts("** Set video mode **");
  if (fullscreen)  
    Screen = SDL_SetVideoMode(800, 600, bpp, SDL_FULLSCREEN | flags);
  else 
    { Screen = SDL_SetVideoMode(800, 600, bpp, flags); }
  if ( Screen == NULL ) {ComplainAndExit();}
// create BackBuffer
  puts("** Create buffers **");
  BackBuffer = SDL_AllocSurface(Screen->flags,
                               800,
                               600,
                               Screen->format->BitsPerPixel,
                               Screen->format->Rmask,
                               Screen->format->Gmask,
                               Screen->format->Bmask, 0);
  if (BackBuffer == NULL)
  printf("ERROR: Couldn't create BackBuffer: %s\n", SDL_GetError());
  FadeBuffer = SDL_AllocSurface(Screen->flags,
                               800,
                               600,
                               Screen->format->BitsPerPixel,
                               Screen->format->Rmask,
                               Screen->format->Gmask,
                               Screen->format->Bmask, 0);
  if (FadeBuffer == NULL)
  printf("ERROR: Couldn't create FadeBuffer: %s\n", SDL_GetError());
// Figure out what putpixel routine to use
   switch (Screen->format->BytesPerPixel)
   {
    case 1:
      _PutPixel = fast_putpixel1;
      break;
    case 2:
      _PutPixel = fast_putpixel2;
      break;
    case 3:
      _PutPixel = fast_putpixel3;
      break;
    case 4:
      _PutPixel = fast_putpixel4;
      break;
   }
}