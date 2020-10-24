int
InitializeGraphics (int scroll_text_up)
{
   int fno, i;
   char *screen_tty;
   struct winsize winsize;

   fno = fileno (stdout);
   if (!isatty (fno))
     {
	fprintf (stderr, "stdout must be a tty\n");
	return 0;
     }
   screen_tty = ttyname (fno);
   ioctl (fno, TCGETS, (char *)&ttmode);
   ttmode.c_lflag &= ~ECHO;
   ioctl (fno, TCSETS, (char *)&ttmode);

   infd = fopen (screen_tty, "rw");

   fno2 = fileno (infd);
   ioctl (fno2, TCGETS, (char *)&ttmode);
   ttmode.c_lflag &= ~ECHO;
   ioctl (fno2, TCSETS, (char *)&ttmode);

   /* query rxvt to find if graphics are available */
   fflush (stdout);
   printf ("\033GQ");
   fflush (stdout);
   while (1)
     {
	if ((fgets (line, LINESZ, infd) != NULL) &&
	    (sscanf (line,"\033G%d", &i) == 1))
	  {
	     if (!i)
	       {
		  fprintf (stderr, "rxvt graphics not available\n");
		  CloseGraphics ();
		  return 0;
	       }
	     break;
	  }
     }
   if (scroll_text_up)
     {
	ioctl (fno, TIOCGWINSZ, &winsize);
	fflush (stdout);
	for (i = 0; i < winsize.ws_row; i++)
	  putchar ('\n');
	fflush (stdout);
     }
   return i;
}
