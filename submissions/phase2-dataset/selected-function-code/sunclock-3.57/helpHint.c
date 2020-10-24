void
helpHint(Context, key, hint)
Sundata * Context;
char key;
char *hint;
{
	int i,j,k,l;
        char more[128], prog_str[60];

	if (key=='\033')
           sprintf(hint, " %s", Label[L_ESCMENU]); 
	else
	if (key==' ')
           sprintf(hint, " %s", Label[L_CONTROLS]); 
	else {
	   i = getNumCmd(key);
	   if (i>=0) 
              sprintf(hint, " %s", Help[i]); 
	   else
	      sprintf(hint, " %s", Label[L_UNKNOWN]);
        }

        more[0] = more[1] = ' ';
        more[2] = '\0';

	if (index("CDS", key))
	   sprintf(more, " (%s)", Label[L_DEGREE]);
	if (index("ABGJ'", key)) {
           switch(Context->flags.progress) {
              case 0: sprintf(prog_str, "1 %s", Label[L_MIN]); break;
	      case 1: sprintf(prog_str, "1 %s", Label[L_HOUR]); break;
	      case 2: sprintf(prog_str, "1 %s", Label[L_DAY]); break;
	      case 3: sprintf(prog_str, "7 %s", Label[L_DAYS]); break;
	      case 4: sprintf(prog_str, "30 %s", Label[L_DAYS]); break;
      	      case 5: sprintf(prog_str, "%ld %s", 
                          progress_value[5], Label[L_SEC]); break;
           }
           sprintf(more, " ( %s %s%s   %s %.3f %s )", 
		  Label[L_PROGRESS], 
                  (key=='A')? "+":((key=='B')? "-":""), prog_str, 
                  Label[L_TIMEJUMP], Context->jump/86400.0,
		  Label[L_DAYS]);
	}
	if (key == 'H') {
	    sscanf(RELEASE, "%d %d %d", &i, &j, &k);
	    sprintf(more+1, "(%s %s, %d %s %d, %s)", 
                      ProgName, VERSION, i, Month_name[j-1], k, COPYRIGHT);
	}
	if (key == 'X') {
	    more[1] = ':';
            more[2] = ' ';
	    if (ExternAction)
	       strncpy(more+3, ExternAction, 123);
            else
	       strcpy(more, "(null)");
            more[126] = '0';
	}

	if (key == '=')
	    sprintf(more+2, "(%c)", (do_sync)? '+' : '-');
	if (key == '[' && do_root <= 0)
	    sprintf(more+2, "%s", Label[L_ONCE]);
	if (key == '[' && do_root >= 1)
	    sprintf(more+2, "%s", Label[L_PERIODIC], root_period);
	if (key == ']' && do_root <= 0)
	    sprintf(more+2, "%s", Label[L_BLANKSCREEN]);
	if (key == ']' && do_root >= 1)
	    sprintf(more+2, "%s", Label[L_STARRYSKY]);
        if (more[2])
	   strncat(hint, more, 120 - strlen(hint));
        l = strlen(hint);
	if (l<120)
	    for (i=l; i<120; i++) hint[i] = ' ';
	hint[120] = '\0';
}
