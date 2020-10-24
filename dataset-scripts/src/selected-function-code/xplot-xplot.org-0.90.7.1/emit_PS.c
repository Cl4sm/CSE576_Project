}
    
/******
  Function to emit a PostScript description of the current plot.
*/
void emit_PS(struct plotter *pl, FILE *fp, enum plstate state)
{
  struct plotter pspl;
  command *c, *cc;
  int counter;
  int currentcolor;
  bool finished_decoration, output_decoration;
  double figwidth;
  double figheight;
  double lmargin,rmargin,bmargin,tmargin;
  double bbllx,bblly,bburx,bbury;
  double limit_height;
  dXPoint a,b;

  /* Make a copy of both the plotter and its commands. */
  pspl = *pl;

#if 0
  /* Copy list of commands.  This is somewhat kludgy because the
   * order of the commands is important (at least all the decoration
   * commands must come first), so I can't use new_command().
   * It always puts the new command at the head of the list,
   * and I can only walk the old list head->tail.
   */

  /* The next statement is only possible because next is the
     first item in the command structure.  */
  cc = (command *) &pspl.commands;
  for (c = pl->commands; c != NULL; c = c->next)  {
    cc->next = (command *) malloc(sizeof(command));
    if (cc->next == 0) fatalerror("malloc returned null");
    *cc->next = *c;
    cc = cc->next;
    cc->next = NULL;
    /* If it is text, copy that too. */
    if (c->type == TEXT || c->type == TITLE)  {
      cc->text = malloc((unsigned)strlen(c->text) + 1);
      if (cc->text == 0) fatalerror("malloc returned null");
      (void) strcpy(cc->text, c->text);
    }
  }
#else
  /* Instead of copying we use the same commands.
   * Now the caller above recomputes everything when we return.
   */
  cc = pl->commands;
  pspl.commands = cc;
#endif

  /* Because xplot only deals with integer output coords, use PS units
   * which are a multiple of the pixels per inch of the actual printer.
   * By doing so, some undesirable effects are avoided.
   * 7200 is the least common multiple of 1440 and 1200.
   * There is some code below that just might depend on this being
   * a multiple of 600.   So think carefully if you are tuning this.
   */
#define PER_INCH 7200

  /* Calculate new window coordinates for everything. */
  { 
    command *c;
    switch(state) {
    default:
      panic("emit_PS: unexpected state");
    case PRINTING:
      /* landscape mode */
      lmargin = 1.25;
      rmargin = 0.75;
      bmargin = 0.85;
      tmargin = 0.75;
      figwidth = 11;
      figheight = 8.5;
      limit_height = figheight;
      break;
    case FIGING:
    case THINFIGING:
      /* portrait mode, for use in documents */
      lmargin = 0.7;
      rmargin = 0.2;
      bmargin = 0.3;
      tmargin = 0.15;
      figwidth = 6.0;
      figheight = 4.0; /* changed below if THINFIGING */
      limit_height = 7.5; /* biggest figure height TeX can handle */
      break;
    }

    if (state == THINFIGING)
      figheight = 2.75;

    if (pl->aspect_ratio != 0) {

      double plotter_width = figwidth - (lmargin+rmargin);
      double plotter_height = figheight - (bmargin+tmargin);

      /* stretch vertically to make aspect ratio correct */
      plotter_height = 1/pl->aspect_ratio * plotter_width;
      figheight = plotter_height + (bmargin+tmargin);

      if (figheight > limit_height) {
	/* figure is too tall, scale back plotter width
	   and height equally to fit */
	double scale =
	  (limit_height - (bmargin+tmargin)) /
	    plotter_height;
	plotter_height *= scale;
	plotter_width *= scale;

      }
	
      figwidth = plotter_width + (lmargin+rmargin);
      figheight = plotter_height + (bmargin+tmargin);
    }


    /* we translate the origin to provide margins, so bb is 0 to figsize */
    bbllx = 0.0 * 72.0;
    bblly = 0.0 * 72.0;
    bburx = figwidth * 72.0;
    bbury = figheight * 72.0;

    pspl.origin.x = 0; /* not necessary? */
    pspl.origin.y = 0;
    pspl.size.x = (int) ((figwidth - lmargin - rmargin) * PER_INCH);
    pspl.size.y = (int) ((figheight - tmargin - bmargin) * PER_INCH);

    /*************** abstraction violation!!! */
    /* code copied from size_window above */
    while (pspl.commands && pspl.commands->decoration) {
      c = pspl.commands;
      pspl.commands = pspl.commands->next;
      free_command(c);
    };
    
    axis(&pspl);
    
    for (c = pspl.commands; c != NULL; c = c->next)
      compute_window_coords(&pspl, c);
  }
  

  /*
   * Print out the prologue for the picture.
   */

  /*
   * GDT: 950310: put dot font hack in.
   */

#ifdef SDO
  fputs("%!PS\n", fp);
#else
  fputs("%!PostScript\n", fp);
#endif
  /* Bracket the PS program with gsave/grestore so these page descriptions
     can be concatenated, then printed. */
  fputs("%%BoundingBox: ", fp);
  fprintf(fp, "%g %g %g %g\n", bbllx, bblly, bburx, bbury);
  fputs("%%EndComments\n", fp);
  fputs("/Docolors true def\n", fp); /* gdt - delete to not print in color */
  fputs("gsave\n", fp);

  /* Set up scale */
  fputs("\n\
%/sign { dup 0 gt { pop 1 } { 0 lt { -1 } { 0 } ifelse } ifelse } def\n\
\n\
%matrix currentmatrix\n\
%aload pop\n\
%6 2 roll sign\n\
%6 1 roll sign\n\
%6 1 roll sign\n\
%6 1 roll sign\n\
%6 1 roll\n\
%matrix astore setmatrix\n\
\n\
", fp);

  fprintf(fp, "72 %d div dup scale\n", PER_INCH);

  fprintf(fp, "/theta {%d mul} def\n", ( (state == PRINTING) ? PER_INCH/150 : PER_INCH/300));

  /* Set up units of measurement. */
  fprintf(fp, "/inch {%d mul} def\n", PER_INCH);
  fputs("/pt {inch 72 div} def\n\
%\n\
%\n\
/tfont /Times-Bold findfont 12 pt scalefont def\n\
%\n\
/lfont /Times-Roman findfont 10 pt scalefont def\n\
%\n\
%tfont /FontBBox get\n\
%  aload pop\n\
%  tfont /FontMatrix get dtransform pop /tascent exch def\n\
%  tfont /FontMatrix get dtransform pop neg /tdescent exch def\n\
lfont /FontBBox get\n\
  aload pop\n\
  lfont /FontMatrix get dtransform pop 0.65 mul /lascent exch def\n\
  lfont /FontMatrix get dtransform pop neg /ldescent exch def\n\
% begin gdt mod\n\
% define font for xplot characters\n\
/BuildCharDict 10 dict def\n\
/Xplotfontdict 7 dict def\n\
Xplotfontdict begin\n\
  /FontType 3 def\n\
  /FontMatrix [1 0 0 1 0 0] def\n\
  /FontBBox [-1 -1 1 1]def\n\
  /Encoding 256 array def\n\
  0 1 255 {Encoding exch /.notdef put} for\n\
  Encoding (.) 0 get /xplotfontdot put\n\
  /CharacterDefs 3 dict def\n\
  CharacterDefs /.notdef {} put\n\
  CharacterDefs /xplotfontdot\n\
    { newpath\n\
	0 0 1 0 360 arc fill\n\
    } put\n\
  /BuildChar\n\
    { BuildCharDict begin\n\
	/char exch def\n\
	/fontdict exch def\n\
	/charname fontdict /Encoding get\n\
	char get def\n\
	/charproc fontdict /CharacterDefs\n\
        get charname get def\n\
	1 0 -1 -1 1 1 setcachedevice\n\
	gsave charproc grestore\n\
      end\n\
    } def\n\
end\n\
/XplotFont Xplotfontdict definefont pop\n\
% scale font according to theta\n\
/dotsetup { /dotfont /XplotFont findfont 4 theta scalefont def } def\n\
% DONE gdt mod\n\
%define procedures for each xplot primitive.\n\
% x y x --\n\
/x {moveto 8 8 rlineto -16 -16 rlineto\n\
    8 8 rmoveto\n\
    -8 8 rlineto 16 -16 rlineto} def\n\
% x y ?arrow --\n\
/darrow {moveto 8 theta 8 theta rmoveto -8 theta -8 theta rlineto\n\
         -8 theta 8 theta rlineto } def\n\
/uarrow {moveto -8 theta -8 theta rmoveto 8 theta 8 theta rlineto\n\
         8 theta -8 theta rlineto } def\n\
/rarrow {moveto -8 theta 8 theta rmoveto 8 theta -8 theta rlineto\n\
         -8 theta -8 theta rlineto } def\n\
/larrow {moveto 8 theta 8 theta rmoveto -8 theta -8 theta rlineto\n\
         8 theta -8 theta rlineto } def\n\
%x y x y line --\n\
/line {moveto lineto} def\n\
%x y dot --\n\
% begin gdt mod\n\
/dot { moveto dotfont setfont (.) show } def\n\
%/dot {stroke 8 theta 0 360 arc fill} def\n\
% end gdt mod\n\
%x y plus --\n\
/plus {moveto -8 theta 0 rmoveto 16 theta 0 rlineto\n\
       -8 theta -8 theta rmoveto 0 16 theta rlineto} def\n\
%x y box --\n\
/box {moveto -8 theta -8 theta rmoveto\n\
      16 theta 0 rlineto\n\
      0 16 theta rlineto\n\
      -16 theta 0 rlineto\n\
      0 -16 theta rlineto} def\n\
%x y diamond --\n\
/diamond { moveto 0 theta 24 theta rmoveto\n\
           -24 theta -24 theta rlineto\n\
            24 theta -24 theta rlineto\n\
            24 theta  24 theta rlineto\n\
           -24 theta  24 theta rlineto} def\n\
%x y ?tick --\n\
/utick {moveto 0 6 theta rlineto} def\n\
/dtick {moveto 0 -6 theta rlineto} def\n\
/ltick {moveto -6 theta 0 rlineto} def\n\
/rtick {moveto 6 theta 0 rlineto} def\n\
/htick {moveto -6 theta 0 rmoveto 12 theta 0 rlineto} def\n\
/vtick {moveto 0 -6 theta rmoveto 0 12 theta rlineto} def\n\
%Separate functions for each text position.\n\
%x y string ?text --\n\
/space 6 pt def\n\
% Set the font, figure out the width.\n\
% x y string tsetup string x width y\n\
/tsetup {lfont setfont dup stringwidth pop exch\n\
         4 1 roll exch} def\n\
%CENTER\n\
/ctext {tsetup lascent 2 div sub\n\
        3 1 roll 2 div sub exch\n\
% stack should now be string x y\n\
        moveto show} def\n\
%ABOVE\n\
/atext {tsetup space ldescent add add\n\
        3 1 roll 2 div sub exch moveto show} def\n\
%BELOW\n\
/btext {tsetup space lascent add sub\n\
        3 1 roll 2 div sub exch moveto show} def\n\
%TO_THE_LEFT\n\
/ltext {tsetup lascent 2 div sub\n\
        3 1 roll space add sub exch moveto show} def\n\
%TO_THE_RIGHT\n\
/rtext {tsetup lascent 2 div sub\n\
        3 1 roll pop space add exch moveto show} def\n\
", fp);

  {
    int i;

    fputs("/XPlotUseColor\n", fp);
    fputs("/Docolors where { pop true } { false } ifelse\n", fp);
    fputs("%product (Ghostscript) eq or\n", fp);
    fputs("def\n", fp);
    fputs("XPlotUseColor\n{\n", fp);
    for ( i = 0; i < NCOLORS; i++ )
      fprintf(fp, "/color%s { %s } def\n",
	      ColorNames[i], ColorPSrep[i]);
    fprintf(fp, "}\n{\n");
    for ( i = 0; i < NCOLORS; i++ )
      fprintf(fp, "/color%s { %s } def\n",
	      ColorNames[i], GrayPSrep[i]);

    fputs("}\nifelse\n", fp);
  }

  fputs("%% string title --\n", fp);
  fprintf(fp, "/title {tfont setfont dup stringwidth pop neg\n");
  fprintf(fp, "        %d add 2 div\n", ((int)rint(pspl.size.x)));
  fprintf(fp, "        %d\n", ((int)rint(pspl.size.y)));
  fprintf(fp, "        moveto show} def\n");

  fputs("%% string xlabel --\n", fp);
  fprintf(fp, "/xlabel {tfont setfont dup stringwidth pop neg\n");
  fprintf(fp, "         %d add\n", ((int)rint(pspl.size.x)));
  fprintf(fp, "         0 lascent ldescent add 3 mul sub\n");
  fprintf(fp, "         moveto show} def\n");

  fputs("%% string ylabel --\n", fp);
  fprintf(fp, "/ylabel {tfont setfont dup stringwidth pop neg\n");
  fprintf(fp, "         0 add 2 div\n");
  fprintf(fp, "         %d lascent ldescent add 1 mul add\n", ((int)rint(pspl.size.y)));
  fprintf(fp, "         moveto show} def\n");

  /* Final prelude:  Change scale, move & rotate origin for landscape
     printing & provide for a margin. */

  /* Orient for landscape printing, margin to lower left corner. */
  if (state == PRINTING) {
    fputs("-90 rotate -11 inch 0 inch translate\n", fp);
  } else {
    fputs("\n\
/notintex { currentdict userdict eq } def\n\
notintex { 1.5 inch 5.0 inch translate } if\n\
", fp);
  }
  /* Move origin to create left & bottom margins. */
  fprintf(fp, "%g inch %g inch translate\n", lmargin, bmargin);
  /* Relatively thick lines for axes & ticks. */
  fputs("4 theta setlinewidth newpath\n", fp);

  fputs("\n% The actual drawing:\n\n", fp);

  /*
   * Now do all the drawing commands.
   */

#define xtoPSx(xxxx) ((int) rint(xxxx))
#define ytoPSy(yyyy) ((int) rint(pspl.size.y - yyyy))

  finished_decoration = output_decoration = FALSE;
  counter = 0;
  currentcolor = 0;		/* black */
  /* loop twice - once for decoration, once for data */
  for (c = pspl.commands; c != NULL; 
       c = c->next
       ? c->next
       : (finished_decoration == FALSE
	  ? (finished_decoration = TRUE, pspl.commands)
	  : NULL))
    {
    if ( finished_decoration && output_decoration == FALSE )  {
      /* Thinner lines for the actual drawing. */
      fputs("stroke\n", fp);
      fprintf(fp, "/theta {%d mul} def\n", ( (state == PRINTING) ? PER_INCH/300 : PER_INCH/600));
      fputs("2 theta setlinewidth\n", fp);
      fputs("dotsetup\n", fp);	/* gdt */
      /* Set clipping region so that we don't draw past the axes. */
      fprintf(fp, "0 0 moveto %d 0 lineto %d %d lineto\n",
	      ((int)rint(pspl.size.x)),
	      ((int)rint(pspl.size.x)), ((int)rint(pspl.size.y)));
      fprintf(fp, "0 %d lineto 0 0 lineto clip newpath\n",
	      ((int)rint(pspl.size.y)));
      output_decoration = TRUE;
    }

    if ( finished_decoration == TRUE
	&& (c->decoration
	    || c->type == TITLE
	    || c->type == XLABEL
	    || c->type == YLABEL
	    ) )
      continue;
    if ( finished_decoration == FALSE
	&& ! (c->decoration
	      || c->type == TITLE
	      || c->type == XLABEL
	      || c->type == YLABEL
	      ) )
      continue;


    if (c->mapped)  {
      if ( !option_mono && c->color != currentcolor ) {
          if ( counter > 0 ) {
            counter = 0;
	    fprintf(fp, "stroke\n");
          }
	  currentcolor = c->color;
	  if ( currentcolor >= 0 && currentcolor < NCOLORS )
	    fprintf(fp, "color%s\n", ColorNames[currentcolor]);
	  else
	    fprintf(fp, "colorwhite\n");
      }

#ifndef WINDOW_COORDS_IN_COMMAND_STRUCT
      a.x = map_coord(pspl.x_type, pspl_x_left, pspl_x_right, pspl.size.x,
		       c->xa);
      a.y = (pspl.size.y - 1) -
	map_coord(pspl.y_type, pspl_y_bottom, pspl_y_top, pspl.size.y,
		  c->ya);
      
      b.x = map_coord(pspl.x_type, pspl_x_left, pspl_x_right, pspl.size.x,
		       c->xb);
      b.y = (pspl.size.y - 1) -
	map_coord(pspl.y_type, pspl_y_bottom, pspl_y_top, pspl.size.y,
		  c->yb);

      a = tomain(&pspl, a);
      b = tomain(&pspl, b);
#else
      a = c->a;
      b = c->b;
#endif

      switch (c->type)  {
      case X:
	fprintf(fp, "%d %d x\n", xtoPSx(a.x), ytoPSy(a.y));
	break;
      case DOT:
	fprintf(fp, "%d %d dot\n", xtoPSx(a.x), ytoPSy(a.y));
	break;
      case PLUS:
	fprintf(fp, "%d %d plus\n", xtoPSx(a.x), ytoPSy(a.y));
	break;
      case BOX:
	fprintf(fp, "%d %d box\n", xtoPSx(a.x), ytoPSy(a.y));
	break;
      case DIAMOND:
	fprintf(fp, "%d %d diamond\n", xtoPSx(a.x), ytoPSy(a.y));
	break;
      case UTICK:
	fprintf(fp, "%d %d utick\n", xtoPSx(a.x), ytoPSy(a.y));
	break;
      case DTICK:
	fprintf(fp, "%d %d dtick\n", xtoPSx(a.x), ytoPSy(a.y));
	break;
      case LTICK:
	fprintf(fp, "%d %d ltick\n", xtoPSx(a.x), ytoPSy(a.y));
	break;
      case RTICK:
	fprintf(fp, "%d %d rtick\n", xtoPSx(a.x), ytoPSy(a.y));
	break;
      case HTICK:
	fprintf(fp, "%d %d htick\n", xtoPSx(a.x), ytoPSy(a.y));
	break;
      case VTICK:
	fprintf(fp, "%d %d vtick\n", xtoPSx(a.x), ytoPSy(a.y));
	break;
      case UARROW:
	fprintf(fp, "%d %d uarrow\n", xtoPSx(a.x), ytoPSy(a.y));
	break;
      case DARROW:
	fprintf(fp, "%d %d darrow\n", xtoPSx(a.x), ytoPSy(a.y));
	break;
      case LARROW:
	fprintf(fp, "%d %d larrow\n", xtoPSx(a.x), ytoPSy(a.y));
	break;
      case RARROW:
	fprintf(fp, "%d %d rarrow\n", xtoPSx(a.x), ytoPSy(a.y));
	break;
      case DLINE:
	fprintf(fp, "%d %d +\n", xtoPSx(a.x), ytoPSy(a.y));
	fprintf(fp, "%d %d +\n", xtoPSx(b.x), ytoPSy(b.y));
	counter += 2;
	/* fall through and draw the line */
      case LINE:
	fprintf(fp, "%d %d %d %d line\n", xtoPSx(a.x), ytoPSy(a.y),
		xtoPSx(b.x), ytoPSy(b.y));
	break;
      case TEXT:
	switch (c->position)  {
	case CENTERED:
	  fprintf(fp, "%d %d (%s) ctext\n", xtoPSx(a.x), ytoPSy(a.y),
		  esc_paren(c->text));
	  break;
	case ABOVE:
	  fprintf(fp, "%d %d (%s) atext\n", xtoPSx(a.x), ytoPSy(a.y),
		  esc_paren(c->text));
	  break;
	case BELOW:
	  fprintf(fp, "%d %d (%s) btext\n", xtoPSx(a.x), ytoPSy(a.y),
		  esc_paren(c->text));
	  break;
	case TO_THE_LEFT:
	  fprintf(fp, "%d %d (%s) ltext\n", xtoPSx(a.x), ytoPSy(a.y),
		  esc_paren(c->text));
	  break;
	case TO_THE_RIGHT:
	  fprintf(fp, "%d %d (%s) rtext\n", xtoPSx(a.x), ytoPSy(a.y),
		  esc_paren(c->text));
	  break;
	}
	break;
      case TITLE:
	fprintf(fp, "(%s) title\n", esc_paren(c->text));
	break;
      case XLABEL:
	fprintf(fp, "(%s) xlabel\n", esc_paren(c->text));
	break;
      case YLABEL:
	fprintf(fp, "(%s) ylabel\n", esc_paren(c->text));
	break;
      case INVISIBLE:
	break;
      }
      if (++counter > 50) {
	counter = 0;
	fprintf(fp, "stroke\n");
      }
    }
  }
  
  fputs("stroke ", fp);
  if (state == PRINTING)
    fputs("showpage ", fp);
  else
    fputs("notintex { showpage } if\n", fp);
  fputs("grestore\n", fp);
  (void) fflush(fp);