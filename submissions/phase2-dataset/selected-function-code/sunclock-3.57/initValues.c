initValues()
{
        int i;

        gflags.colorlevel = FULLCOLORS;
        gflags.fillmode = 2;
        gflags.vmfflags = -1;
        gflags.dotted = 0;
        gflags.colorscale = 16;

	gflags.animate = 0;
	gflags.animperiod = 0;
        gflags.progress = 0;

        gflags.update = 4;
        gflags.bottom = 0;
        gflags.map_mode = LEGALTIME;
        gflags.clock_mode = 0;
        gflags.hours_shown = 0;
        gflags.dms = 0;
        gflags.shading = 2;
        gflags.citymode = 1;
        gflags.objectmode = 1;
        gflags.objects = 3;
        gflags.meridian = 0;
        gflags.parallel = 0;

        gzoom.mode = 0;
        gzoom.fx = 1.0;
        gzoom.fy = 1.0;
        gzoom.fdx = 0.5;
        gzoom.fdy = 0.5;
        gzoom.meridspacing = 0.0;
        gzoom.paralspacing = 0.0;

        option_entry.string = NULL;
        for (i=0; i<=4; i++) urban_entry[i].string = NULL;

        for (i=0; i<NUMPIXELS; i++)
            Color[i] = strdup(DefaultColor[i]);
        for (i=0; i<NUMFONTS; i++)
            SunFont[i] = strdup(DefaultFont[i]);

        position.lat = 100.0;
        position.tz = NULL;
   
        StringReAlloc(&share_maps_dir, SHAREDIR"/earthmaps/");
        StringReAlloc(&image_dir, share_maps_dir);

        StringReAlloc(&Default_img_file, SHAREDIR"/earthmaps/vmf/timezones.vmf");
        StringReAlloc(&Zoom_img_file, SHAREDIR"/earthmaps/vmf/landwater.vmf");
        StringReAlloc(&Clock_img_file, Default_img_file);
        StringReAlloc(&Map_img_file, Default_img_file);
   
        StringReAlloc(&ListFormats, STDFORMATS);
        StringReAlloc(&EditorCommand, EDITORCOMMAND);
        StringReAlloc(&SpotSizes, "1|2|3|4|5");
        StringReAlloc(&SizeLimits, "0|580|2500|6000|12000");

	for (i=0; i<L_END; i++) Label[i] = strdup(Label[i]);
	for (i=0; i<N_HELP; i++) Help[i] = strdup(Help[i]);
}
