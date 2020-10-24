WaitForCarriageReturn (long *win, int *x, int *y)
{
   int i, len;

   fgets (line, LINESZ, infd);
   line [LINESZ-1] = 0;
   len = strlen (line);
   for (i = 0; i < len; i++)
     {
	if (line [i] == '\033')
	  {
	     int ret = 1;
	     i++;
	     switch (line[i]) {
	      case 'R': ret++;
		/* drop */
	      case 'P':
		sscanf (&line[i+1],"%ld;%d;%d", win, x, y);
		return ret;
		break;
	     }
	  }
     }
   return 0;
}
