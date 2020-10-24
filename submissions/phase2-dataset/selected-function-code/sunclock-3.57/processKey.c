void
processKey(win, keysym)
Window  win;
KeySym  keysym;
{
        double v;
        int i, j, old_mode;
        KeySym key;
        struct Sundata * Context = NULL;

        Context = getContext(win);
        if (!Context) return;
        if (Context->flags.colorlevel == FULLCOLORS) {
           if (!Context->xim) return;
        } else {
           if (!Context->mappix) return;
        }

        key = keysym;
        Context->flags.update = 1;
        if (key>=XK_A && key<=XK_Z) key += 32;
        old_mode = Context->flags.map_mode;

        if (win==Filesel) {
           switch(key) {
             case XK_Escape:
                if (do_filesel) 
                  PopFilesel(Context);
                return;
             case XK_Page_Up:
                if (filesel_shift == 0) return;
                filesel_shift -= num_lines/2;
                if (filesel_shift <0) filesel_shift = 0;
                break;
             case XK_Page_Down:
                if (num_table_entries-filesel_shift<num_lines) return;
                filesel_shift += num_lines/2;
                break;
             case XK_Up:
                if (filesel_shift == 0) return;
                filesel_shift -= 1;
                break;
             case XK_Down:
                if (num_table_entries-filesel_shift<num_lines) return;
                filesel_shift += 1;
                break;
             case XK_Home:
                if (filesel_shift == 0) return;
                filesel_shift = 0;
                break;
             case XK_End:
                if (num_table_entries-filesel_shift<num_lines) return;
                filesel_shift = num_table_entries - num_lines+2;
                break;
             case XK_Left:
             case XK_Right:
                return;
             default :
                goto general;
           }
           setupFilesel(1);
           return;
        }

        if (win==Zoom) {
           switch(key) {
             case XK_Escape:
                if (do_zoom) 
                  PopZoom(Context);
                return;
             default:
                goto general;
           }
        }

        if (win==Option) {
	   if (text_input!=OPTION_INPUT) goto general;
           switch(keysym) {
             case XK_Escape:
               if (do_option) 
                  PopOption(Context);
               return;
             case XK_KP_Enter:
             case XK_Return:
                  activateOption();
               return;
	     default:
	       processStringEntry(keysym, &option_entry);
               setupOption(0);
	       return;
	   }
        }

        if (win==Urban) {
	   if (text_input<URBAN_INPUT && keysym!=XK_Escape) goto general;
	   i = text_input-URBAN_INPUT;
           switch(keysym) {
             case XK_Escape:
               if (do_urban) 
                  PopUrban(Context);
               return;
             case XK_KP_Enter:
             case XK_Return:
	       key = keysym = XK_section;
	       goto general;
	       break;
	     default:
	       processStringEntry(keysym, &urban_entry[i]);
               setupUrban(0);
	       return;
	   }
        }

     general:
        switch(key) {
           case XK_Escape:
             if (do_menu) PopMenu(Context);
             return;
	   case XK_percent:
	     if (win==Option && do_option && text_input!=OPTION_INPUT) {
	        option_entry.oldcaret = 0;
	        option_entry.oldlength = strlen(option_entry.string);
	        option_entry.oldchar = *option_entry.string;
	        *option_entry.string = '\0';
	        option_entry.caret = 0;
                setupOption(0);
	     }
	     if (win==Urban && do_urban && text_input<URBAN_INPUT) {
	        for (i=0; i<=4; i++) {
	           urban_entry[i].oldcaret = 0;
	           urban_entry[i].oldlength = strlen(urban_entry[i].string);
	           urban_entry[i].oldchar = *urban_entry[i].string;
	           *urban_entry[i].string = '\0';
	           urban_entry[i].caret = 0;
		}
                setupUrban(0);
		goto erasemarks;
	     }
	     break;
           case XK_degree:
	     erase_obj = 1;
             if (Context->flags.objectmode == 2) drawSunAndMoon(Context);
             if (Context->flags.colorlevel != MANYCOLORS) drawCities(Context);
             Context->flags.dms = 1 -Context->flags.dms;
	     erase_obj = 0;
             if (Context->flags.objectmode == 2) drawSunAndMoon(Context);
             if (Context->flags.colorlevel == MONOCHROME) drawCities(Context);
             if (do_urban) {
	        if (Context->mark1.city) 
                   updateUrban(Context, Context->mark1.city);
                else {
		   for (i=2; i<=3; i++)
                      (void) num2str(dms2decim(urban_entry[i].string),
		         urban_entry[i].string, Context->flags.dms);
                   setupUrban(0);
		}
	     }
	     Context->flags.update = 2;
             return;
	   case XK_section:
	     if (do_urban) {
	        char params[256];
		sprintf(params, "%s\037|%s\037|%s\037",
                    urban_entry[0].string,
                    urban_entry[2].string,
                    urban_entry[3].string);
	        if (!markLocation(Context, params))
                   (void) markLocation(Context, urban_entry[0].string);
                updateUrban(Context, Context->mark1.city);
		Context->flags.update = 2;
	     }
	     break;
	   case XK_asciitilde:
	   case XK_parenright:
	     if (win == Urban && Context->mark1.city &&
                    Context->mark1.city != &Context->pos1) {
                City *c = Context->mark1.city;
		if (c) {
		   erase_obj = 1;
		   drawObject(Context, c->lon, c->lat, c->size, 1, c->name);
		   erase_obj = 0;
		} else return;
	        deleteMarkedCity();
	        Context->flags.update = 2;
	     }
	     if (keysym==XK_parenright)
	        break;
	   case XK_parenleft:
	     if (win == Urban) {
	        City * c = addCity(NULL);
		if (c) {
	           if (Context->mark1.city) {
		      if (Context->mark1.city == &Context->pos1) {
			 erase_obj = 1;
                         drawMarks(Context);
                         erase_obj = 0;
		      }
		   }
                   Context->mark1.city = c;
		   if (Context->flags.colorlevel==MONOCHROME) {
		      drawObject(Context,
                         c->lon, c->lat, c->size, 1, c->name);
                      Context->mark1.flags = -1;
		   }
	           Context->flags.update = 2;
		} else
		   setupUrban(0);
	     }
	     break;
           case XK_less:
             if (Context->prevgeom.width && 
                 (Context->prevgeom.width != Context->geom.width ||
                  Context->prevgeom.height != Context->geom.height)) {
                Context->geom = Context->prevgeom;
                Context->prevgeom.width = 0;
                adjustGeom(Context, 0);
		XResizeWindow(dpy, Context->win,
		      Context->geom.width, 
		      Context->geom.height+Context->gdata->menustrip);
                warningNew(Context);
                shutDown(Context, 0);
                buildMap(Context, Context->wintype, 0);
             }
             break;
           case XK_Home:
             label_shift = 0;
             return;
           case XK_End:
             label_shift = 50;
             clearStrip(Context);
             return;
           case XK_Page_Up: 
             if (label_shift>0)
               --label_shift;
             return;
           case XK_Page_Down: 
             if (label_shift<50)
               ++label_shift;
             return;
           case XK_equal:
	     if (do_sync & 1)
                do_sync = do_sync & 2;
	     else
                do_sync |= 1;
	     menu_lasthint = '\0';
	     option_lasthint = '\0';
	     option_newhint = keysym;
	     showOptionHint(getNumCmd(key));
             break;
           case XK_Delete:
	   case XK_BackSpace:
	   case XK_guillemotleft:
	     if (!memcmp(&Context->newzoom, &Context->oldzoom,
                         sizeof(ZoomSettings))) return;
	     Context->newzoom = Context->oldzoom;
	     setZoomDimension(Context);
             zoom_mode |= 15;
             activateZoom(Context, zoom_active);
             return;
           case XK_Left:
             v = 0.5/Context->newzoom.fx;
             Context->newzoom.fdx -= v;
             if (Context->newzoom.fdx<v) Context->newzoom.fdx = v;
             zoom_mode |= 14;
             activateZoom(Context, zoom_active);
             return;
           case XK_Right:
             v = 0.5/Context->newzoom.fx;
             Context->newzoom.fdx += v;
             if (Context->newzoom.fdx>1.0-v) Context->newzoom.fdx = 1.0-v;
             zoom_mode |= 14;
             activateZoom(Context, zoom_active);
             return;
           case XK_Up:
             v = 0.5/Context->newzoom.fy;
             Context->newzoom.fdy -= v;
             if (Context->newzoom.fdy<v) Context->newzoom.fdy = v;
             zoom_mode |= 14;
             activateZoom(Context, zoom_active);
             return;
           case XK_Down:
             v = 0.5/Context->newzoom.fy;
             Context->newzoom.fdy += v;
             if (Context->newzoom.fdy>1.0-v) Context->newzoom.fdy = 1.0-v;
             zoom_mode |= 14;
             activateZoom(Context, zoom_active);
             return;
           case XK_greater:
             if (do_dock && Context==Seed) break;
             Context->prevgeom = Context->geom;
	     i = DisplayWidth(dpy, scr);
             Context->geom.width = i - extra_width;
	     if (Context->geom.width<i/2) Context->geom.width = i/2;
	     if (Context->geom.width>i) Context->geom.width = i;
           case XK_KP_Divide:
             if (key == XK_KP_Divide) key = XK_slash;
           case XK_colon:
             if (key == XK_colon) key = XK_slash;
           case XK_slash:
             if (do_dock && Context==Seed) break;
             if (key == XK_slash) {
                Context->prevgeom = Context->geom;
                Context->zoom.mode = 2;
                Context->newzoom.mode = Context->zoom.mode;
             }
             if (!do_zoom)
                Context->newzoom = Context->zoom;
             if (setWindowAspect(Context, &Context->zoom)) {
                if (key == XK_greater || key == XK_slash) {
                   adjustGeom(Context, 0);
		   XResizeWindow(dpy, Context->win,
		      Context->geom.width, 
		      Context->geom.height+Context->gdata->menustrip);
                   Context->geom.x = extra_width/2;
                   XMoveWindow(dpy, Context->win, 
                      Context->geom.x, Context->geom.y);
                }
                warningNew(Context);
                shutDown(Context, 0);
                buildMap(Context, Context->wintype, 0);
                MapGeom = Context->geom;
             }
             break;
           case XK_apostrophe:
	     menu_lasthint = ' ';
	     option_lasthint = ' ';
	     Context->flags.animate = 1 - Context->flags.animate;
	     if (Context->flags.animate == 0)
	         Context->jump -= progress_value[Context->flags.progress];
	     Context->flags.update = 4;
	     break;
           case XK_quotedbl:
             if (do_zoom) zoom_active = 1 - zoom_active;
             zoom_mode = 30;
             activateZoom(Context, zoom_active);
             break;
           case XK_KP_Multiply:
           case XK_asterisk:
	     key = XK_asterisk;
             if (!memcmp(&Context->newzoom, 
                         &Context->zoom, sizeof(ZoomSettings))) break;
             activateZoom(Context, 1);
             break;
           case XK_period:
             if (Context->mark1.city) {
                Context->newzoom.fdx = 0.5+Context->mark1.city->lon/360.0;
                Context->newzoom.fdy = 0.5-Context->mark1.city->lat/180.0;
                zoom_mode |= 14;
                zoom_lasthint = ' ';
                activateZoom(Context, zoom_active);
             }
             break;
           case XK_at:
                activateOption();
		return;
           case XK_space:
           case XK_exclam:
	     key = XK_exclam;
             menu_newhint = XK_exclam;
             if (Context==Seed && do_dock) return;
             Context->wintype = 1 - Context->wintype;
             if (Context->wintype) {
                Context->geom.width = MapGeom.width;
                Context->geom.height = MapGeom.height;
             } else {
                Context->geom.width = ClockGeom.width;
                Context->geom.height = ClockGeom.height;
	     }
             adjustGeom(Context, 1);
     	     XSelectInput(dpy, Context->win, 0);
             setSizeHints(Context, Context->wintype);
             setClassHints(Context->win, Context->wintype);
             XMoveResizeWindow(dpy, Context->win, 
                 Context->geom.x, Context->geom.y, 
                 Context->geom.width, 
                 Context->geom.height+((Context->wintype)?
                     Context->gdata->mapstrip:Context->gdata->clockstrip));
             warningNew(Context);
             shutDown(Context, 0);
             buildMap(Context, Context->wintype, 0);
             return;
           case XK_1:
           case XK_KP_1:
	     key = XK_1;
             if (memcmp(&Context->newzoom, 
                        &gzoom, sizeof(ZoomSettings))) {
                Context->newzoom = gzoom;
                zoom_mode |= 15;
                activateZoom(Context, zoom_active);
             }
             break;
           case XK_numbersign:
             if (memcmp(&Context->newzoom, 
                        &Context->zoom, sizeof(ZoomSettings))) {
                Context->newzoom = Context->zoom;
                zoom_mode |= 15;
                activateZoom(Context, zoom_active);
             }
             break;
           case XK_plus:
           case XK_KP_Add:
	     key = XK_plus;
             Context->newzoom.fx *= ZFACT;
             Context->newzoom.fy *= ZFACT;
             setZoomDimension(Context);
             zoom_mode |= 14;
             activateZoom(Context, zoom_active);
             break;
           case XK_minus:
           case XK_KP_Subtract:
	     key = XK_minus;
             Context->newzoom.fx /= ZFACT;
             Context->newzoom.fy /= ZFACT;
             setZoomDimension(Context);  
             zoom_mode |= 14;
             activateZoom(Context, zoom_active);
             break;
           case XK_bracketright:
	     menu_lasthint = ' ';
	     option_lasthint = ' ';
	     drawImageToRootWindow(Context, -1);
	     break;
           case XK_bracketleft:
	     menu_lasthint = ' ';
	     option_lasthint = ' ';
	     drawImageToRootWindow(Context, 1);
	     break;
           case XK_ampersand:
             Context->newzoom.mode = (Context->newzoom.mode+1) %3;
             setZoomDimension(Context);             
             zoom_mode |= 13;
             activateZoom(Context, zoom_active);
             break;
           case XK_a: 
             Context->jump += progress_value[Context->flags.progress];
             Context->flags.update = 4;
             menu_lasthint = ' ';
             break;
           case XK_b: 
             Context->jump -= progress_value[Context->flags.progress];
             Context->flags.update = 4;
             menu_lasthint = ' ';
             break;
           case XK_c: 
             if (!Context->wintype) break;
             if (Context->flags.map_mode != COORDINATES) 
               Context->flags.dms = gflags.dms;
             else
               Context->flags.dms = 1 - Context->flags.dms;
             Context->flags.map_mode = COORDINATES;
             if (Context->mark1.city == &Context->pos1) {
		if (Context->flags.colorlevel==FULLCOLORS) {
		   erase_obj = 1;
		   drawMarks(Context);
                   erase_obj = 0;
		}
                Context->mark1.city = NULL;
	     }
             if (Context->mark1.city)
               setDayParams(Context);
             if (Context->mark2.city) {
		if (Context->flags.colorlevel==FULLCOLORS) {
		   erase_obj = 2;
		   drawMarks(Context);
                   erase_obj = 0;
		}
	     }
             Context->mark2.city = NULL;
             Context->flags.update = 2;
             break;
           case XK_d: 
             if (!Context->wintype) break;
             if (Context->flags.map_mode != DISTANCES) 
               Context->flags.dms = gflags.dms;
             else
               Context->flags.dms = 1 - Context->flags.dms;
             Context->flags.map_mode = DISTANCES;
             break;
           case XK_e: 
             if (!Context->wintype) break;
             Context->flags.map_mode = EXTENSION;
             old_mode = EXTENSION;
             Context->flags.hours_shown = 0;
             showHours(Context);
             break;
           case XK_f:
             if (!do_filesel)
                PopFilesel(Context);
             else {
		if (Filesel) {
                   XMapWindow(dpy, Filesel);
                   XMapRaised(dpy, Filesel);
		}
                if (FileselCaller != Context) {
                   PopFilesel(Context);
                   PopFilesel(Context);
                }
             }
             break;
           case XK_g: 
             if (!do_menu && win != Option)
               PopMenu(Context);
             else {
               menu_lasthint = ' ';
	       option_lasthint = ' ';
               if (keysym==XK_g) {
                  Context->flags.progress = (Context->flags.progress+1) % 6;
                  if (!progress_value[Context->flags.progress])
                     Context->flags.progress = 0;
               }
               if (keysym==XK_G) {
                  Context->flags.progress = (Context->flags.progress+5) % 6;
                  if (!progress_value[Context->flags.progress])
                     Context->flags.progress = 4;
               }
             }
             break;
           case XK_h:
             if (!do_menu) {
                menu_newhint = XK_space;
                PopMenu(Context);
		return;
             } else {
                if (MenuCaller != Context) {
                   PopMenu(Context);
                   PopMenu(Context);
                } else {
		   if (getState(Menu) == IsViewable) {
                      XMapRaised(dpy, Menu);
                      showManual();
		   } else
                      XMapWindow(dpy, Menu);
		}
             }
             break;
           case XK_i: 
             setAuxilWins(Context, ICONIFY);
             XIconifyWindow(dpy, Context->win, scr);
  	     Context->flags.mapped = 0;
             break;
           case XK_j:
             Context->jump = 0;
             Context->flags.update = 4;
             menu_lasthint = ' ';
	     option_lasthint = ' ';
             break;
           case XK_k:
	     if (Context==Seed && do_dock) return;
             if (do_menu) PopMenu(Context);
             if (do_filesel) PopFilesel(Context);
             if (Context==Seed && Seed->next==NULL)
                shutDown(Context, -1);
             else
                shutDown(Context, 1);
             return;
           case XK_l:
             if (!Context->wintype) {
                clearStrip(Context);
                if (!Context->wintype)
                   Context->flags.clock_mode = 
                     (Context->flags.clock_mode+1) % num_formats;
                Context->flags.update = 1;
                break;
             }
             Context->flags.map_mode = LEGALTIME;
	erasemarks:
	     if (Context->flags.colorlevel==FULLCOLORS) {
	        erase_obj = 3;
	        drawMarks(Context);
                erase_obj = 0;
	     }
             Context->mark1.city = NULL;
             Context->mark2.city = NULL;
             Context->flags.update = 2;
             break;
           case XK_m:
             if (!Context->wintype) break;
             if (Context->flags.colorlevel!=MANYCOLORS) {
	        erase_obj = 1;
	        drawLines(Context);
                erase_obj = 0;
	     }
	     if (keysym == XK_M)
                Context->flags.meridian = (Context->flags.meridian + 3) % 4;
	     else
                Context->flags.meridian = (Context->flags.meridian + 1) % 4;
             if (Context->flags.colorlevel<=FEWCOLORS) drawLines(Context);
             Context->flags.update = 2;
             break;
           case XK_n:
             if ((Context->flags.colorlevel<FULLCOLORS) || 
                 (Context->flags.colorscale == 1))
		Context->flags.shading = 1 - Context->flags.shading;
	     else {
                if (keysym==XK_n) 
                   Context->flags.shading = (Context->flags.shading + 1) % 6;
                if (keysym==XK_N) 
                   Context->flags.shading = (Context->flags.shading + 5) % 6;
             }
             drawShadedArea(Context);
             Context->flags.update = 4;
             break;
           case XK_o:
             if (!do_option)
                PopOption(Context);
             else {
                XMapWindow(dpy, Option);
                XMapRaised(dpy, Option);
                if (OptionCaller != Context) {
                   PopOption(Context);
                   PopOption(Context);
                }
             }
             break;
           case XK_p:
             if (!Context->wintype) break;
             if (Context->flags.colorlevel!=MANYCOLORS) {
	        erase_obj = 1;
	        drawLines(Context);
                erase_obj = 0;
	     }
	     i = Context->flags.parallel;
             if (keysym == XK_P) 
                Context->flags.parallel = ((i+3)&3) + (i&8);
	     else
                Context->flags.parallel = ((i+1)&3) + (i&8);
             if (Context->flags.colorlevel<=FEWCOLORS) drawLines(Context);
             Context->flags.update = 2;
             break;
           case XK_q: 
	     if (!do_dock)
                shutDown(Context, -1);
             break;
           case XK_s: 
             if (Context->flags.map_mode != SOLARTIME) 
               Context->flags.dms = gflags.dms;
             else
               Context->flags.dms = 1 - Context->flags.dms;
             Context->flags.map_mode = SOLARTIME;
             if (Context->mark2.city) {
		if (Context->flags.colorlevel==FULLCOLORS) {
		   erase_obj = 2;
		   drawMarks(Context);
                   erase_obj = 0;
		}
	     }
             Context->mark2.city = NULL;
             if (Context->mark1.city)
               setDayParams(Context);
             Context->flags.update = 2;
             break;
           case XK_t:
             if (!Context->wintype) break;
             if (Context->flags.colorlevel!=MANYCOLORS) {
	        erase_obj = 1;
	        drawLines(Context);
                erase_obj = 0;
	     }
             Context->flags.parallel = (Context->flags.parallel + 8) & 15;
             if (Context->flags.colorlevel<=FEWCOLORS) drawLines(Context);
             Context->flags.update = 2;
             break;
           case XK_u:
             if (!Context->wintype) break;
	     if (!do_urban) {
	        PopUrban(Context);
	        updateUrban(Context, Context->mark1.city);
		break;
	     } else {
	        if (getState(Urban)!=IsViewable) {
                   XMapWindow(dpy, Urban);
	           XMapRaised(dpy, Urban);
	           updateUrban(Context, Context->mark1.city);
		   break;
                } else
	           XMapRaised(dpy, Urban);
	     }
             if (Context->flags.colorlevel!=MANYCOLORS) {
	        erase_obj = 1;
	        drawCities(Context);
                erase_obj = 0;
	     }
             if (keysym == XK_U)
                Context->flags.citymode = (Context->flags.citymode + 3) % 4;
	     else
                Context->flags.citymode = (Context->flags.citymode + 1) % 4;
             if (Context->flags.colorlevel==MONOCHROME) drawCities(Context);
             Context->flags.update = 2;
             break;
           case XK_w: 
             if (Context->time<=last_time+2) return;
             if (do_menu) do_menu = -1;
             if (do_filesel) do_filesel = -1;
             if (do_zoom) do_zoom = -1;
             if (do_option) do_option = -1;
             buildMap(Context, 1, 1);
	     keysym = ' ';
             break;
           case XK_r:
             clearStrip(Context);
             Context->flags.update = 4;             
             break;
           case XK_x:
             if (ExternAction) 
                system(ExternAction);
             break;
           case XK_y:
	     erase_obj = 1;
             drawSunAndMoon(Context);
             erase_obj = 0;
             if (keysym==XK_y) 
                Context->flags.objectmode = (Context->flags.objectmode+1) % 3;
             if (keysym==XK_Y) 
                Context->flags.objectmode = (Context->flags.objectmode+2) % 3; 
             drawSunAndMoon(Context);
             Context->flags.update = 2;
             break;
           case XK_z:
             if (!do_zoom)
                PopZoom(Context);
             else {
                XMapWindow(dpy, Zoom);
                XMapRaised(dpy, Zoom);
                if (ZoomCaller != Context) {
                   PopZoom(Context);
                   PopZoom(Context);
                }
             }
             break;
           default:
             if (!Context->wintype) {
               Context->flags.clock_mode = 
                   (1+Context->flags.clock_mode) % num_formats;
               Context->flags.update = 1;
             }
             break ;
        }

        if (old_mode == EXTENSION && Context->flags.map_mode != old_mode) 
           clearStrip(Context);

        if (do_menu) {
	   if (getNumCmd(toupper(key))>=0)
	      menu_newhint = toupper(key);
	   j = -1;
	   for (i=0; i<N_MENU; i++) if (MenuKey[2*i]==toupper(key)) { 
	      j=i; 
	      break; 
	   }
           showMenuHint(j);
	}

        if (do_zoom) {
	   if (getNumCmd(toupper(key))>=0)
	      zoom_newhint = toupper(key);
	   j = -1;
	   for (i=0; i<N_ZOOM; i++) if (ZoomKey[2*i]==toupper(key)) { 
	      j=i; 
	      break; 
	   }
           showZoomHint(j);
	}

        if (do_option) {
	   if (getNumCmd(toupper(key))>=0)
	      option_newhint = toupper(key);
	   j = -1;
	   for (i=0; i<N_OPTION; i++) if (OptionKey[2*i]==toupper(key)) { 
	      j=i; 
	      break; 
	   }
           showOptionHint(j);
	}
}
