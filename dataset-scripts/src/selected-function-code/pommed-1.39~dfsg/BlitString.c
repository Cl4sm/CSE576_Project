void
BlitString(char *name, int x, int y)
{
  int i;
  int c;
  int k;

  k = x;
  for (i=0; name[i]; i++)
    {

      c = toupper(name[i]);
      /* A letter */
      if (c >= 'A' && c <= 'Z')
        {
	  c -= 'A';
	  copyXPMArea(c * 6, 74, 6, 8, k, y);
	  k += 6;
	}
      /* A number or symbol */
      else if (c>='0' && c<='9')
	{
	  c -= '0';
	  copyXPMArea(c * 6, 64, 6, 8, k, y);
	  k += 6;
	}
      else
	{
	  copyXPMArea(5, 84, 6, 8, k, y);
	  k += 6;
	}
    }
}