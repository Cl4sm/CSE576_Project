static void dockimlib2_xstartup(DockImlib2 *dock) {
  dock->bg = NULL;
  dockimlib2_set_rect_shape(dock, dock->x0, dock->y0, dock->w, dock->h);
#if 0
  Window parent = 0;
  int exposed = 0, mapped = 0;
  printf("xstartup..\n");
  do {
    XEvent event;
    XNextEvent(dock->display, &event);
    switch (event.type) {
    case ReparentNotify: {
      XReparentEvent ev = event.xreparent;
      if (ev.window == dock->win) {
        parent = ev.parent;
      }
    } break;
    case MapNotify: {
      XMappingEvent ev = event.xmapping;
      printf("MapNotify: ev.win = %lx\n", ev.window);
      if (ev.window == dock->win) mapped = 1;
    } break;
    case Expose: {
      XExposeEvent ev = event.xexpose;
      printf("Expose: ev.win = %lx\n", ev.window);
      if (ev.window == dock->win) exposed = 1;
    } break;
    }
  } while (parent == 0 && !exposed && !mapped);
  if (parent == dock->rootwin) {
    printf("... oups, parent window is rootwin ... are you really running windowmaker?\n");
    dock->bg = imlib_create_image(dock->w, dock->h);
  } else {
    imlib_context_set_drawable(parent);
    dock->bg = imlib_create_image_from_drawable(0, dock->x0, dock->y0, dock->w, dock->h, 1);
    imlib_context_set_drawable(dock->win);
    dockimlib2_set_rect_shape(dock, dock->x0, dock->y0, dock->w, dock->h);
  }
  printf("xstartup : success\n");
#endif
}