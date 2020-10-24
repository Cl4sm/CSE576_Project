int
parseArgs(argc, argv)
int                    argc;
char **                argv;
{
        int     opt;

        while (--argc > 0) {
                ++argv;
                if (!strcasecmp(*argv, "-verbose"))
                        verbose = 1;
                else if (!strcasecmp(*argv, "-reformat"))
                        reformat = 1;
                else if (!strcasecmp(*argv, "-silent"))
                        verbose = 0;
                else if (!strcasecmp(*argv, "-synchro"))
                        do_sync = 1;
                else if (!strcasecmp(*argv, "-nosynchro"))
                        do_sync = 0;
                else if (!strcasecmp(*argv, "-zoomsync"))
                        do_zoomsync = 1;
                else if (!strcasecmp(*argv, "-nozoomsync"))
                        do_zoomsync = 0;
                else if (!strcasecmp(*argv, "-animation"))
                        gflags.animate = 1;
                else if (!strcasecmp(*argv, "-noanimation"))
                        gflags.animate = 0;
                else if (!strcasecmp(*argv, "-coastlines"))
                        gflags.fillmode = 0;
                else if (!strcasecmp(*argv, "-contour"))
                        gflags.fillmode = 1;
                else if (!strcasecmp(*argv, "-landfill"))
                        gflags.fillmode = 2;
                else if (!strcasecmp(*argv, "-dottedlines"))
                        gflags.dotted = 0;
                else if (!strcasecmp(*argv, "-plainlines"))
                        gflags.dotted = 1;
                else if (!strcasecmp(*argv, "-bottomline"))
                        gflags.bottom = 1;
                else if (!strcasecmp(*argv, "-nobottomline"))
                        gflags.bottom = 0;
                else if (!strcasecmp(*argv, "-decimal"))
                        gflags.dms = 0;
                else if (!strcasecmp(*argv, "-dms"))
                        gflags.dms = gflags.dms = 1;
                else if (!strcasecmp(*argv, "-nonight"))
                        gflags.shading = 0;
                else if (!strcasecmp(*argv, "-night"))
                        gflags.shading = 1;
                else if (!strcasecmp(*argv, "-terminator"))
                        gflags.shading = 2;
                else if (!strcasecmp(*argv, "-twilight"))
                        gflags.shading = 3;
                else if (!strcasecmp(*argv, "-luminosity"))
                        gflags.shading = 4;
                else if (!strcasecmp(*argv, "-lightgradient"))
                        gflags.shading = 5;
                else if (!strcasecmp(*argv, "-tropics"))
                        gflags.parallel |= 8;
                else if (!strcasecmp(*argv, "-notropics"))
                        gflags.parallel &= 3;
                else if (!strcasecmp(*argv, "-sun"))
                        gflags.objects |= 1;
                else if (!strcasecmp(*argv, "-nosun"))
                        gflags.objects &= ~1;
                else if (!strcasecmp(*argv, "-moon"))
                        gflags.objects |= 2;
		else if (!strcasecmp(*argv, "-nomoon"))
                        gflags.objects &= ~2;
                else if (!strcasecmp(*argv, "-dock"))
                        do_dock = 1;
                else if (!strcasecmp(*argv, "-undock"))
                        do_dock = 0;
                else if (runlevel == RUNTIMEOPTION) {
                        if (needMore(&argc, argv)) return(1);
                        goto options_with_parameter;
		}
                else if (!strcasecmp(*argv, "-citycheck"))
                        citycheck = 1;
                else if (!strcasecmp(*argv, "-clock"))
                        win_type = 0;
                else if (!strcasecmp(*argv, "-map"))
                        win_type = 1;
                else if (!strcasecmp(*argv, "-screensaver") &&
			 runlevel <= PARSECMDLINE) {
                        screen_saver = 1;
			random_rootpos = 1;
			win_type = 1;
		}
                else if (!strcasecmp(*argv, "-noscreensaver") &&
			 runlevel <= PARSECMDLINE) {
                        screen_saver = 0;
			random_rootpos = 0;
		}
                else if (!strcasecmp(*argv, "-fixedrootpos"))
                        random_rootpos = 0;
                else if (!strcasecmp(*argv, "-randomrootpos"))
                        random_rootpos = 1;
                else if (!strcasecmp(*argv, "-menu"))
                        do_menu = 1;
                else if (!strcasecmp(*argv, "-nomenu"))
                        do_menu = 0;
                else if (!strcasecmp(*argv, "-filesel"))
                        do_filesel = 1;
                else if (!strcasecmp(*argv, "-nofilesel"))
                        do_filesel = 0;
                else if (!strcasecmp(*argv, "-zoom"))
                        do_zoom = 1;
                else if (!strcasecmp(*argv, "-nozoom"))
                        do_zoom = 0;
                else if (!strcasecmp(*argv, "-option"))
                        do_option = 1;
                else if (!strcasecmp(*argv, "-nooption"))
                        do_option = 0;
                else if (!strcasecmp(*argv, "-urban"))
                        do_urban = 1;
                else if (!strcasecmp(*argv, "-nourban"))
                        do_urban = 0;
                else if (!strcasecmp(*argv, "-help")) {
		        if (runlevel == PARSECMDLINE) {
			   Usage();
			   exit(0);
			}
		}
                else if (!strcasecmp(*argv, "-listmenu")) {
                        if (runlevel == PARSECMDLINE) { 
                           ListMenu();
			   exit(0);
			}
                }
                else if (!strcasecmp(*argv, "-version")) {
                        fprintf(stderr, "%s: version %s, %s\n",
                                ProgName, VERSION, COPYRIGHT);
                        if (runlevel == PARSECMDLINE) 
                          exit(0);
                        else
                          return(0);
		}
             else {
                if (needMore(&argc, argv)) return(1);
                if (!strcasecmp(*argv, "-display"))
                        StringReAlloc(&Display_name, *++argv); 
                else if (!strcasecmp(*argv, "-sharedir")) {
                        StringReAlloc(&share_maps_dir, *++argv);
                        strncpy(image_dir, *argv, 1020);
		}
                else if (!strcasecmp(*argv, "-citycategories")) {
                        city_cat = atoi(*++argv);
			if (city_cat <= 0) city_cat = 1;
			if (city_cat > 100) city_cat = 100;
		}
                else 
	        options_with_parameter :
                     if (!strcasecmp(*argv, "-rcfile")) {
		        if (runlevel == RUNTIMEOPTION) {
			   runlevel = READUSERRC;
                           if (readRC(*++argv)) runlevel = FAILEDOPTION;
                           if (runlevel != FAILEDOPTION) 
                              runlevel = RUNTIMEOPTION;
			}
		}
                else if (!strcasecmp(*argv, "-language")) {
                        strncpy(language, *++argv, 2);
		        language[2] = '\0';
			if (strcmp(language, oldlanguage)) readLanguage();
                } 
	        else if (!strcasecmp(*argv, "-title"))
                        StringReAlloc(&Title, *++argv);
	        else if (!strcasecmp(*argv, "-clockclassname"))
                        StringReAlloc(&ClockClassName, *++argv);
	        else if (!strcasecmp(*argv, "-mapclassname"))
                        StringReAlloc(&MapClassName, *++argv);
	        else if (!strcasecmp(*argv, "-auxilclassname"))
                        StringReAlloc(&AuxilClassName, *++argv);
	        else if (!strcasecmp(*argv, "-classname"))
                        StringReAlloc(&ClassName, *++argv);
                else if (!strcasecmp(*argv, "-colorlevel")) {
                        gflags.colorlevel = atoi(*++argv);
			if (gflags.colorlevel < 0) gflags.colorlevel = 0;
			if (gflags.colorlevel >= FULLCOLORS) {
                           gflags.colorlevel = FULLCOLORS;
			   gflags.fillmode = 2;
			} else
 			   gflags.fillmode = 1;
  	        }
		else if (!strcasecmp(*argv, "-vmfflags")) {
		        gflags.vmfflags = atoi(*++argv);
			option_changes |= 4;			
		}
		else if (!strcasecmp(*argv, "-vmfrange")) {
                        StringReAlloc(&vmfrange, *++argv);
		}
		else if (!strcasecmp(*argv, "-vmfcoordformat")) {
                        StringReAlloc(&vmfcoordformat, *++argv);
		}
		else if (!strcasecmp(*argv, "-vmfcolors")) {
                        StringReAlloc(&vmfcolors, *++argv);
                        if (!strcmp(vmfcolors, "|")) {
			   free(vmfcolors);
			   vmfcolors = NULL;
			}
			option_changes |= 4;
		}
		else if (!strcasecmp(*argv, "-clockgeom")) {
                        getGeom(*++argv, &ClockGeom);
			option_changes |= 8;
                }
                else if (!strcasecmp(*argv, "-mapgeom")) {
                        getGeom(*++argv, &MapGeom);
			option_changes |= 16;
                }
                else if (!strcasecmp(*argv, "-image")) {
                        StringReAlloc(&Clock_img_file, *++argv);
                        StringReAlloc(&Map_img_file, *argv);
			option_changes |= 32|64;
                }
                else if (!strcasecmp(*argv, "-clockimage")) {
                        StringReAlloc(&Clock_img_file, *++argv);
			option_changes |= 32;
                }
                else if (!strcasecmp(*argv, "-mapimage")) {
                        StringReAlloc(&Map_img_file, *++argv);
			option_changes |= 64;
                }
                else if (!strcasecmp(*argv, "-zoomimage")) {
		        StringReAlloc(&Zoom_img_file, *++argv);
		        option_changes |= 2;
		}
                else if (!strcasecmp(*argv, "-auxilgeom")) {
                        getGeom(*++argv, &MenuGeom);
			option_changes |= 2;
			ZoomGeom.x = FileselGeom.x 
                                   = OptionGeom.x 
                                   = UrbanGeom.x 
                                   = MenuGeom.x;
			ZoomGeom.y = FileselGeom.y 
                                   = OptionGeom.y 
                                   = UrbanGeom.y 
                                   = MenuGeom.y;
                }
                else if (!strcasecmp(*argv, "-menugeom")) {
                        getGeom(*++argv, &MenuGeom);
			option_changes |= 2;
                }
                else if (!strcasecmp(*argv, "-selgeom")) {
                        getGeom(*++argv, &FileselGeom);
			option_changes |= 2;
                }
                else if (!strcasecmp(*argv, "-zoomgeom")) {
                        getGeom(*++argv, &ZoomGeom);
			option_changes |= 2;
                }
                else if (!strcasecmp(*argv, "-optiongeom")) {
                        getGeom(*++argv, &OptionGeom);
			option_changes |= 2;
                }
                else if (!strcasecmp(*argv, "-urbangeom")) {
                        getGeom(*++argv, &UrbanGeom);
			option_changes |= 2;
                }
                else if (!strcasecmp(*argv, "-mag")) {
                        gzoom.fx = atof(*++argv);
                        if (gzoom.fx < 1) gzoom.fx = 1.0;
                        if (gzoom.fx > 100.0) gzoom.fx = 100.0;
                        gzoom.fy = gzoom.fx;
			option_changes |= 4;
                }
                else if (!strcasecmp(*argv, "-magx")) {
                        gzoom.fx = atof(*++argv);
                        if (gzoom.fx < 1) gzoom.fx = 1.0;
			option_changes |= 4;
                }
                else if (!strcasecmp(*argv, "-magy")) {
                        gzoom.fy = atof(*++argv);
                        if (gzoom.fy < 1) gzoom.fy = 1.0;
			option_changes |= 4;
                }
                else if (!strcasecmp(*argv, "-dx")) {
                        gzoom.fdx = atof(*++argv)/360.0+0.5;
                        checkZoomSettings(&gzoom);
			option_changes |= 4;
                }
                else if (!strcasecmp(*argv, "-dy")) {
                        gzoom.fdy = 0.5-atof(*++argv)/180.0;
                        checkZoomSettings(&gzoom);
			option_changes |= 4;
                }
                else if (!strcasecmp(*argv, "-rootdx"))
		        rootdx = atof(*++argv);
                else if (!strcasecmp(*argv, "-rootdy"))
		        rootdy = atof(*++argv);
                else if (!strcasecmp(*argv, "-setfont"))
                        parseFont(*++argv); 
                else if (!strcasecmp(*argv, "-mapmode")) {
                        if (!strcasecmp(*++argv, "c")) 
                           gflags.map_mode = COORDINATES;
                        if (!strcasecmp(*argv, "d")) 
                           gflags.map_mode = DISTANCES;
                        if (!strcasecmp(*argv, "e")) 
                           gflags.map_mode = EXTENSION;
                        if (!strcasecmp(*argv, "l")) {
                           StringReAlloc(&CityInit, NULL);
                           gflags.map_mode = LEGALTIME;
                        }
                        if (!strcasecmp(*argv, "s")) 
                           gflags.map_mode = SOLARTIME;
                }
                else if (!strcasecmp(*argv, "-parallelmode")) {
                        opt = atoi(*++argv);
                        if (opt<0) opt = 0;
                        if (opt>3) opt = 3;
			gflags.parallel = opt + (gflags.parallel & 8);
                } 
		else if (!strcasecmp(*argv, "-parallelspacing")) {
                        gzoom.paralspacing = atof(*++argv);
                        if (gzoom.paralspacing<0) gzoom.paralspacing = 0;
                        if (gzoom.paralspacing>30.0) gzoom.paralspacing = 30.0;
                        if (gzoom.paralspacing<0.1) gzoom.paralspacing = 0.1;
                } 
		else if (!strcasecmp(*argv, "-meridianmode")) {
                        gflags.meridian = atoi(*++argv);
                        if (gflags.meridian<0) gflags.meridian = 0;
                        if (gflags.meridian>3) gflags.meridian = 3;
                } 
		else if (!strcasecmp(*argv, "-meridianspacing")) {
                        gzoom.meridspacing = atof(*++argv);
                        if (gzoom.meridspacing<0) gzoom.meridspacing = 0;
                        if (gzoom.meridspacing>30.0) gzoom.meridspacing = 30.0;
                        if (gzoom.meridspacing<0.1) gzoom.meridspacing = 0.1;
                } 
		else if (!strcasecmp(*argv, "-citymode")) {
                        gflags.citymode = atoi(*++argv);
                        if (gflags.citymode<0) gflags.citymode = 0;
                        if (gflags.citymode>3) gflags.citymode = 3;
                } 
		else if (!strcasecmp(*argv, "-objectmode")) {
                        gflags.objectmode = atoi(*++argv);
                        if (gflags.objectmode<0) gflags.objectmode = 0;
                        if (gflags.objectmode>=2) gflags.objectmode = 2;
		}
                else if (!strcasecmp(*argv, "-spotsizes"))
                        StringReAlloc(&SpotSizes, *++argv);
                else if (!strcasecmp(*argv, "-sizelimits"))
                        StringReAlloc(&SizeLimits, *++argv);
                else if (!strcasecmp(*argv, "-fillmode")) {
                        gflags.fillmode = atoi(*++argv);
                        if (gflags.fillmode<0) gflags.fillmode = 0;
                        if (gflags.fillmode>3) gflags.fillmode = 3;
                }
                else if (!strcasecmp(*argv, "-darkness")) {
                        darkness = atof(*++argv);
                        if (darkness<0.0) darkness = 0.0;
                        if (darkness>1.0) darkness = 1.0;
                }
                else if (!strcasecmp(*argv, "-diffusion")) {
                        atm_diffusion = atof(*++argv);
                        if (atm_diffusion<0.0) atm_diffusion = 0.0;
                }
                else if (!strcasecmp(*argv, "-refraction")) {
                        atm_refraction = atof(*++argv);
                        if (atm_refraction<0.0) atm_refraction = 0.0;
                }
                else if (!strcasecmp(*argv, "-colorscale")) {
			opt = atoi(*++argv);
			if (opt<=0) opt = 1;
			if (opt>32767) opt = 32767;
                        gflags.colorscale = opt;
                }
		else if (!strcasecmp(*argv, "-setcolor")) {
		        if (parseColor(*++argv)) return(1);
		}
                else if (!strcasecmp(*argv, "-addcity")) 
		        (void) addCity(*++argv);
		else if (!strcasecmp(*argv, "-removecity")) {
		        City * c = searchCityLocation(*++argv);
		        removeCity(c);
		}
		else if (!strcasecmp(*argv, "-position")) {
                        StringReAlloc(&CityInit, NULL);
                        scanPosition(*++argv, &position);
			if (position.lat < -90.0) {
			  fprintf(stderr, 
                             "Error in -position parameters\n");
			  return(1);
			}
		}
                else if (!strcasecmp(*argv, "-city")) {
                        StringReAlloc(&CityInit, *++argv);
                        position.lat = 100.0;
                        gflags.map_mode = COORDINATES;
                }
                else if (!strcasecmp(*argv, "-placement")) {
		        option_changes |= 2;
                        if (strcasecmp(*++argv, "random")==0)
                           placement = RANDOM;
                        if (strcasecmp(*argv, "fixed")==0) {
                           placement = FIXED;
                           MapGeom.mask = XValue | YValue | 
                                          WidthValue | HeightValue;
                        }
                        if (!strcasecmp(*argv, "center"))
                           placement = CENTER;
                        if (!strcasecmp(*argv, "nw"))
                           placement = NW;
                        if (!strcasecmp(*argv, "ne"))
                           placement = NE;
                        if (!strcasecmp(*argv, "sw"))
                           placement = SW;
                        if (!strcasecmp(*argv, "se"))
                           placement = SE;
                }
                else if (!strcasecmp(*argv, "-extrawidth"))
			extra_width = atol(*++argv);
                else if (!strcasecmp(*argv, "-placementshift")) {
		        option_changes |= 2;
			if (sscanf(*++argv, "%d %d", 
                            &place_shiftx, &place_shifty) < 2) {
			  fprintf(stderr, 
                             "Error in -placementshift parameters\n");
			  return(1);
			}
		}
                else if (!strcasecmp(*argv, "-command"))
                        StringReAlloc(&ExternAction, *++argv);
                else if (!strcasecmp(*argv, "-editorcommand"))
                        StringReAlloc(&EditorCommand, *++argv);
                else if (!strcasecmp(*argv, "-dateformat"))
                        StringReAlloc(&ListFormats, *++argv);
                else if (!strcasecmp(*argv, "-shading")) {
                        gflags.shading = atoi(*++argv);
                        if (gflags.shading < 0) gflags.shading = 0;
                        if (gflags.shading > 5) gflags.shading = 5;
                }
                else if (!strcasecmp(*argv, "-progress") ||
                         !strcasecmp(*argv, "-jump")) {
                        char *str, *invalid, c;
                        long value;
			opt = ((*argv)[5]!='\0');
                        str=*++argv;
                        value = strtol(str, &invalid, 10);
                        if (invalid) 
                           c = *invalid;
			else
			   c = 's';
                        if (c>='0' && c<='9') c='s';
                        switch(c) {
                          case 's': break;
                          case 'm': value *= 60 ; break;
                          case 'h': value *= 3600 ; break;
                          case 'd': value *= 86400 ; break;
                          case 'M': value *= 2592000 ; break;
                          case 'y':
                          case 'Y': value *= 31536000 ; break;
                          default : c = ' '; break;
                        }
                        if (c == ' ') Usage();
                        if (opt) {
                           progress_value[5] = abs(value); 
                           if (value) 
                              gflags.progress = 5;
                           else
                              gflags.progress = 0;
                        } else 
                           time_jump = value;
                }
                else if (!strcasecmp(*argv, "-rootperiod")) {
                        root_period = atoi(*++argv);
                        if (root_period<=0) root_period = 1;
                        if (root_period>120) root_period = 120;
                }
                else if (!strcasecmp(*argv, "-animateperiod")) {
                        gflags.animperiod = atoi(*++argv);
                        if (gflags.animperiod<0) gflags.animperiod = 0;
                        if (gflags.animperiod>5) gflags.animperiod = 5;
                }
                else if (!strcasecmp(*argv, "-aspect")) {
                        gzoom.mode = atoi(*++argv);
                        if (gzoom.mode<0) gzoom.mode = 0;
                        if (gzoom.mode>2) gzoom.mode = 2;
                }
	        else {
                   fprintf(stderr, "%s: unknown option !!\n\n", *argv);
                   if (runlevel == PARSECMDLINE) {
                        Usage();
                        exit(1);
                   } 
		   else if (runlevel == RUNTIMEOPTION) {
                        fprintf(stderr, "Option %s : incorrect or not "
				        "available at runtime !!\n", *argv);
                        runlevel = FAILEDOPTION;
		   }
                   else {
                        fprintf(stderr, "Trying to recover.\n");
                        return(1);
                   }
		}
	     }
	}
        return(0);
}
