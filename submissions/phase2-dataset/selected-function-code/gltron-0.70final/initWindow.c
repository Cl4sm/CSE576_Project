int initWindow(void) {
  int win_id;
  int flags;
  unsigned char fullscreen = 0;
  /* char buf[20]; */

  SystemInitWindow(0, 0, getSettingi("width"), getSettingi("height"));

  if(getSettingi("windowMode") == 0) {
    fullscreen = SYSTEM_FULLSCREEN;
  }

  flags = SYSTEM_RGBA | SYSTEM_DOUBLE | SYSTEM_DEPTH;
  if(getSettingi("bitdepth_32"))
    flags |= SYSTEM_32_BIT;

  if(getSettingi("use_stencil"))
    flags |= SYSTEM_STENCIL;

  SystemInitDisplayMode(flags, fullscreen);

  win_id = SystemCreateWindow("gltron");      

  if (win_id < 0) { 
    if( getSettingi("use_stencil") ) {
      flags &= ~SYSTEM_STENCIL;
      SystemInitDisplayMode(flags, fullscreen);
      win_id = SystemCreateWindow("gltron");      
      if(win_id >= 0) {
	setSettingi("use_stencil", 0);
	goto SKIP;
      }
    }
    printf("[fatal] could not create window...exiting\n");
    exit(1); /* OK: critical, no visual */
  }

 SKIP:

  if(getSettingi("windowMode") == 0 || getSettingi("mouse_warp") == 1) {
    SystemGrabInput();
  } else {
    SystemUngrabInput();
  }

  return win_id;
}
