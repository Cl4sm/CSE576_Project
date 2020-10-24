getSpacing(Context, mode)
Sundata * Context;
int mode;   /* 0=parallel 1=meridian spacing */
{
  double val[12] = 
    { 0.1, 0.2, 0.5, 1.0, 2.0, 5.0, 10.0, 15.0, 20.0, 30.0, 45.0, 90.0 };
  double quot = 0.0;
  int i = 0;

  i = 11;

  if (mode == 0)
     quot = 4200.0/(double)Context->zoom.height;

  if (mode == 1)
     quot = 10800.0/(double)Context->zoom.width;
  
  if (quot<0.5 && mode) quot = quot*1.2;

  while (i>0 && val[i-1] > quot) --i;
     
  if (mode==1 && i==8) ++i;
  return val[i];  
}
