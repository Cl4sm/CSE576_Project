      return(i);
  return(-1);  /* not a color name */
}
 

int get_input(FILE *fp, Display *dpy, int lineno, struct plotter *pl)
{
  
  char **tokens;
  int ntokens = 0;
  command *com;

#define parseerror(s) \
  { \
      int i; \
      fprintf(stderr, \
	      "in line number %d: %s\noffending line: ", lineno, s); \
      for (i = 0; i < ntokens; i++) \
      { \
	  fputs(tokens[i], stderr); \
	  fputc(' ', stderr); \
      } \
      fputc('\n', stderr); \
      fflush(stderr); \
      exit(1); \
  }
  do {
    lineno++;
    tokens = gettokens(fp);
    if (tokens == 0) parseerror("EOF before first line of input");
    for (ntokens = 0; tokens[ntokens] != 0; ntokens++);
  } while (ntokens == 1 && mystrcmp(tokens[0], "new_plotter") == 0);
  
  if (ntokens != 2)
    parseerror("invalid input format -- expecting coord type names");

  pl->x_type = parse_coord_name(tokens[0]);
  pl->y_type = parse_coord_name(tokens[1]);

  if (((int) pl->x_type) < 0 || ((int) pl->y_type) < 0)
    parseerror("unknown coord type");
  
  for (;;) {

    lineno++;
    tokens = gettokens(fp);
    if (tokens == 0) break;
    for (ntokens = 0; tokens[ntokens] != 0; ntokens++);
    if (ntokens == 0) continue;
    if (tokens[0][0] == ';') continue;
    
    /* check for color key alone on a line */
    if (ntokens == 1) {
      xpcolor_t c;

      c = parse_color(tokens[0]);
      if (c != -1) {
	/* color keyword */
	pl->default_color = pl->current_color = c;
	continue;
      }
    }

    if (ntokens == 2 &&
	parse_coord_name(tokens[0]) == pl->x_type &&
	parse_coord_name(tokens[1]) == pl->y_type)
      continue;

#define not_ntokens_equal_to_3_or_4	(ntokens != 3 && ntokens != 4)
#define COLORfromTOK3  (com->color = ntokens == 4 ?\
			parse_color(tokens[3]) : pl->current_color)

    if (mystrcmp(tokens[0],"aspect_ratio") == 0) {
      if (ntokens != 2) parseerror("input format error");
      if (pl->x_type != pl->y_type)
	parseerror("aspect_ratio requires identical coordinate types");
      pl->aspect_ratio = atof(tokens[1]);
    } else if (mystrcmp(tokens[0],"x") == 0) {
      if (not_ntokens_equal_to_3_or_4) parseerror("input format error");
      com = new_command(pl);
      com->type = X;
      com->xa = parse_coord(pl->x_type, tokens[1]);
      com->ya = parse_coord(pl->y_type, tokens[2]);
      COLORfromTOK3;
    } else if (mystrcmp(tokens[0],".") == 0
	       || mystrcmp(tokens[0],"dot") == 0) {
      if (not_ntokens_equal_to_3_or_4) parseerror("input format error");
      com = new_command(pl);
      com->type = DOT;
      com->xa = parse_coord(pl->x_type, tokens[1]);
      com->ya = parse_coord(pl->y_type, tokens[2]);
      COLORfromTOK3;
    } else if (mystrcmp(tokens[0],"+") == 0) {
      if (not_ntokens_equal_to_3_or_4) parseerror("input format error");
      com = new_command(pl);
      com->type = PLUS;
      com->xa = parse_coord(pl->x_type, tokens[1]);
      com->ya = parse_coord(pl->y_type, tokens[2]);
      COLORfromTOK3;
    } else if (mystrcmp(tokens[0],"plus") == 0) {
      if (not_ntokens_equal_to_3_or_4) parseerror("input format error");
      com = new_command(pl);
      com->type = PLUS;
      com->xa = parse_coord(pl->x_type, tokens[1]);
      com->ya = parse_coord(pl->y_type, tokens[2]);
      COLORfromTOK3;
    } else if (mystrcmp(tokens[0],"box") == 0) {
      if (not_ntokens_equal_to_3_or_4) parseerror("input format error");
      com = new_command(pl);
      com->type = BOX;
      com->xa = parse_coord(pl->x_type, tokens[1]);
      com->ya = parse_coord(pl->y_type, tokens[2]);
      COLORfromTOK3;
    } else if (mystrcmp(tokens[0],"diamond") == 0) {
      if (not_ntokens_equal_to_3_or_4) parseerror("input format error");
      com = new_command(pl);
      com->type = DIAMOND;
      com->xa = parse_coord(pl->x_type, tokens[1]);
      com->ya = parse_coord(pl->y_type, tokens[2]);
      COLORfromTOK3;
    } else if (mystrcmp(tokens[0],"utick") == 0) {
      if (not_ntokens_equal_to_3_or_4) parseerror("input format error");
      com = new_command(pl);
      com->type = UTICK;
      com->xa = parse_coord(pl->x_type, tokens[1]);
      com->ya = parse_coord(pl->y_type, tokens[2]);
      COLORfromTOK3;
    } else if (mystrcmp(tokens[0],"dtick") == 0) {
      if (not_ntokens_equal_to_3_or_4) parseerror("input format error");
      com = new_command(pl);
      com->type = DTICK;
      com->xa = parse_coord(pl->x_type, tokens[1]);
      com->ya = parse_coord(pl->y_type, tokens[2]);
    } else if (mystrcmp(tokens[0],"ltick") == 0) {
      if (not_ntokens_equal_to_3_or_4) parseerror("input format error");
      com = new_command(pl);
      com->type = LTICK;
      com->xa = parse_coord(pl->x_type, tokens[1]);
      com->ya = parse_coord(pl->y_type, tokens[2]);
      COLORfromTOK3;
    } else if (mystrcmp(tokens[0],"rtick") == 0) {
      if (not_ntokens_equal_to_3_or_4) parseerror("input format error");
      com = new_command(pl);
      com->type = RTICK;
      com->xa = parse_coord(pl->x_type, tokens[1]);
      com->ya = parse_coord(pl->y_type, tokens[2]);
      COLORfromTOK3;
    } else if (mystrcmp(tokens[0],"htick") == 0) {
      if (not_ntokens_equal_to_3_or_4) parseerror("input format error");
      com = new_command(pl);
      com->type = HTICK;
      com->xa = parse_coord(pl->x_type, tokens[1]);
      com->ya = parse_coord(pl->y_type, tokens[2]);
      COLORfromTOK3;
    } else if (mystrcmp(tokens[0],"vtick") == 0) {
      if (not_ntokens_equal_to_3_or_4) parseerror("input format error");
      com = new_command(pl);
      com->type = VTICK;
      com->xa = parse_coord(pl->x_type, tokens[1]);
      com->ya = parse_coord(pl->y_type, tokens[2]);
      COLORfromTOK3;
    } else if (mystrcmp(tokens[0],"uarrow") == 0) {
      if (not_ntokens_equal_to_3_or_4) parseerror("input format error");
      com = new_command(pl);
      com->type = UARROW;
      com->xa = parse_coord(pl->x_type, tokens[1]);
      com->ya = parse_coord(pl->y_type, tokens[2]);
      COLORfromTOK3;
    } else if (mystrcmp(tokens[0],"darrow") == 0) {
      if (not_ntokens_equal_to_3_or_4) parseerror("input format error");
      com = new_command(pl);
      com->type = DARROW;
      com->xa = parse_coord(pl->x_type, tokens[1]);
      com->ya = parse_coord(pl->y_type, tokens[2]);
      COLORfromTOK3;
    } else if (mystrcmp(tokens[0],"larrow") == 0) {
      if (not_ntokens_equal_to_3_or_4) parseerror("input format error");
      com = new_command(pl);
      com->type = LARROW;
      com->xa = parse_coord(pl->x_type, tokens[1]);
      com->ya = parse_coord(pl->y_type, tokens[2]);
      COLORfromTOK3;
    } else if (mystrcmp(tokens[0],"rarrow") == 0) {
      if (not_ntokens_equal_to_3_or_4) parseerror("input format error");
      com = new_command(pl);
      com->type = RARROW;
      com->xa = parse_coord(pl->x_type, tokens[1]);
      com->ya = parse_coord(pl->y_type, tokens[2]);
      COLORfromTOK3;
    } else if (mystrcmp(tokens[0],"line") == 0) {
      if (ntokens != 5 && ntokens != 6) parseerror("input format error");
      com = new_command(pl);
      com->type = LINE;
      com->xa = parse_coord(pl->x_type, tokens[1]);
      com->ya = parse_coord(pl->y_type, tokens[2]);
      com->xb = parse_coord(pl->x_type, tokens[3]);
      com->yb = parse_coord(pl->y_type, tokens[4]);
      com->color = ntokens == 6 ? parse_color(tokens[5]) : pl->current_color;
    } else if (mystrcmp(tokens[0],"dline") == 0) {
      if (ntokens != 5 && ntokens != 6) parseerror("input format error");
      com = new_command(pl);
      com->type = DLINE;
      com->xa = parse_coord(pl->x_type, tokens[1]);
      com->ya = parse_coord(pl->y_type, tokens[2]);
      com->xb = parse_coord(pl->x_type, tokens[3]);
      com->yb = parse_coord(pl->y_type, tokens[4]);
      com->color = ntokens == 6 ? parse_color(tokens[5]) : pl->current_color;
    } else if (mystrcmp(tokens[0], "title") == 0) {
      char buf[1024];
      char *cp;
      buf[0] = '\0';
      (void) fgets(buf, sizeof(buf), fp);
      for (cp=buf;*cp != '\0';cp++)
	if (*cp == '\n') { *cp = '\0'; break; }
      cp = malloc((unsigned)strlen(buf) + 1);
      if (cp == 0) fatalerror("malloc returned null");
      (void) strcpy(cp, buf);

      com = new_command(pl);
      com->type = TITLE;
      com->text = cp;
    } else if (mystrcmp(tokens[0], "ctext") == 0) {
      char buf[80];
      char *cp;
      buf[0] = '\0';
      if (not_ntokens_equal_to_3_or_4) parseerror("input format error");
      (void) fgets(buf, sizeof(buf), fp);
      for (cp=buf;*cp != '\0';cp++)
	if (*cp == '\n') { *cp = '\0'; break; }
      cp = malloc((unsigned)strlen(buf) + 1);
      if (cp == 0) fatalerror("malloc returned null");
      (void) strcpy(cp, buf);
      com = new_command(pl);
      com->type = TEXT;
      com->xa = parse_coord(pl->x_type, tokens[1]);
      com->ya = parse_coord(pl->y_type, tokens[2]);
      com->text = cp;
      com->position = CENTERED;
      COLORfromTOK3;
    } else if (mystrcmp(tokens[0], "atext") == 0) {
      char buf[80];
      char *cp;
      buf[0] = '\0';
      if (not_ntokens_equal_to_3_or_4) parseerror("input format error");
      (void) fgets(buf, sizeof(buf), fp);
      for (cp=buf;*cp != '\0';cp++)
	if (*cp == '\n') { *cp = '\0'; break; }
      cp = malloc((unsigned)strlen(buf) + 1);
      if (cp == 0) fatalerror("malloc returned null");
      (void) strcpy(cp, buf);
      com = new_command(pl);
      com->type = TEXT;
      com->xa = parse_coord(pl->x_type, tokens[1]);
      com->ya = parse_coord(pl->y_type, tokens[2]);
      com->text = cp;
      com->position = ABOVE;
      COLORfromTOK3;
    } else if (mystrcmp(tokens[0], "btext") == 0) {
      char buf[80];
      char *cp;
      buf[0] = '\0';
      if (not_ntokens_equal_to_3_or_4) parseerror("input format error");
      (void) fgets(buf, sizeof(buf), fp);
      for (cp=buf;*cp != '\0';cp++)
	if (*cp == '\n') { *cp = '\0'; break; }
      cp = malloc((unsigned)strlen(buf) + 1);
      if (cp == 0) fatalerror("malloc returned null");
      (void) strcpy(cp, buf);
      com = new_command(pl);
      com->type = TEXT;
      com->xa = parse_coord(pl->x_type, tokens[1]);
      com->ya = parse_coord(pl->y_type, tokens[2]);
      com->text = cp;
      com->position = BELOW;
      COLORfromTOK3;
    } else if (mystrcmp(tokens[0], "ltext") == 0) {
      char buf[80];
      char *cp;
      buf[0] = '\0';
      if (not_ntokens_equal_to_3_or_4) parseerror("input format error");
      (void) fgets(buf, sizeof(buf), fp);
      for (cp=buf;*cp != '\0';cp++)
	if (*cp == '\n') { *cp = '\0'; break; }
      cp = malloc((unsigned)strlen(buf) + 1);
      if (cp == 0) fatalerror("malloc returned null");
      (void) strcpy(cp, buf);
      com = new_command(pl);
      com->type = TEXT;
      com->xa = parse_coord(pl->x_type, tokens[1]);
      com->ya = parse_coord(pl->y_type, tokens[2]);
      com->text = cp;
      com->position = TO_THE_LEFT;
      COLORfromTOK3;
    } else if (mystrcmp(tokens[0], "rtext") == 0) {
      char buf[80];
      char *cp;
      buf[0] = '\0';
      if (not_ntokens_equal_to_3_or_4) parseerror("input format error");
      (void) fgets(buf, sizeof(buf), fp);
      for (cp=buf;*cp != '\0';cp++)
	if (*cp == '\n') { *cp = '\0'; break; }
      cp = malloc((unsigned)strlen(buf) + 1);
      if (cp == 0) fatalerror("malloc returned null");
      (void) strcpy(cp, buf);
      com = new_command(pl);
      com->type = TEXT;
      com->xa = parse_coord(pl->x_type, tokens[1]);
      com->ya = parse_coord(pl->y_type, tokens[2]);
      com->text = cp;
      com->position = TO_THE_RIGHT;
      COLORfromTOK3;
    } else if (mystrcmp(tokens[0], "xlabel") == 0) {
      char buf[1024];
      char *cp;
      buf[0] = '\0';
      (void) fgets(buf, sizeof(buf), fp);
      for (cp=buf;*cp != '\0';cp++)
	if (*cp == '\n') { *cp = '\0'; break; }
      cp = malloc((unsigned)strlen(buf) + 1);
      if (cp == 0) fatalerror("malloc returned null");
      (void) strcpy(cp, buf);

      com = new_command(pl);
      com->type = XLABEL;
      com->text = cp;
    } else if (mystrcmp(tokens[0], "ylabel") == 0) {
      char buf[1024];
      char *cp;
      buf[0] = '\0';
      (void) fgets(buf, sizeof(buf), fp);
      for (cp=buf;*cp != '\0';cp++)
	if (*cp == '\n') { *cp = '\0'; break; }
      cp = malloc((unsigned)strlen(buf) + 1);
      if (cp == 0) fatalerror("malloc returned null");
      (void) strcpy(cp, buf);

      com = new_command(pl);
      com->type = YLABEL;
      com->text = cp;
    } else if (mystrcmp(tokens[0], "xunits") == 0) {
      char buf[1024];
      char *cp;
      buf[0] = '\0';
      (void) fgets(buf, sizeof(buf), fp);
      for (cp=buf;*cp != '\0';cp++)
	if (*cp == '\n') { *cp = '\0'; break; }
      cp = malloc((unsigned)strlen(buf) + 1);
      if (cp == 0) fatalerror("malloc returned null");
      (void) strcpy(cp, buf);

      pl->x_units = cp;
    } else if (mystrcmp(tokens[0], "yunits") == 0) {
      char buf[1024];
      char *cp;
      buf[0] = '\0';
      (void) fgets(buf, sizeof(buf), fp);
      for (cp=buf;*cp != '\0';cp++)
	if (*cp == '\n') { *cp = '\0'; break; }
      cp = malloc((unsigned)strlen(buf) + 1);
      if (cp == 0) fatalerror("malloc returned null");
      (void) strcpy(cp, buf);

      pl->y_units = cp;
    } else if (mystrcmp(tokens[0],"invisible") == 0) {
      if (not_ntokens_equal_to_3_or_4) parseerror("input format error");
      com = new_command(pl);
      com->type = INVISIBLE;
      com->xa = parse_coord(pl->x_type, tokens[1]);
      com->ya = parse_coord(pl->y_type, tokens[2]);
      COLORfromTOK3;
    } else if (mystrcmp(tokens[0],"go") == 0) {
#if 0
      fprintf(stderr,"xplot pid %d go!\n",getpid());
#endif
      break;
    } else if (mystrcmp(tokens[0],"new_plotter") == 0) {
      return lineno;