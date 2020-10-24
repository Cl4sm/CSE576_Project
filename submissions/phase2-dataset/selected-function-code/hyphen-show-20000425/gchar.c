int gchar( unsigned char *byt)
{ static char buffer[BUFLEN_E];
  static int cnt=0;
  static char *ptr;

  if (cnt == 0)
  {cnt=read(filed1,buffer,BUFLEN_E);
   ptr=buffer;
  }
  if (cnt > 0)
  {*byt = *ptr; 
   cnt--;  ptr++;  anzz++;
   return (1);
  }
  else if (cnt < 0)
  {printf("\nerror in reading, return code=%d\n",cnt);
   return (cnt);
  }
  else return (cnt);
}
