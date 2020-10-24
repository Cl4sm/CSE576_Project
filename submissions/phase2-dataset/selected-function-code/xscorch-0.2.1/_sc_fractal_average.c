static void _sc_fractal_average(unsigned char *data, int size, int spacing)
{
  int spacing2 = (spacing >> 1); /*used as offset*/
  int basey;                     /*1D offset of current row*/
  int basex;                     /*...and x/y combo*/
  int bases = spacing2 * size;   /*distance spacing2 rows away*/
  int yinc  = (bases << 1);      /*similar but with spacing*/
  int size2 = size * size;       /*first illegal index*/
  int xsize = size;              /*Last index of current row + 1*/

  /*Average horizonal; use "parent" values to left and right*/
  for(basey = 0; basey < size2; basey += yinc)
  {
    for(basex = basey + spacing2; basex < xsize; basex += spacing)
      data[basex] = (data[basex - spacing2] + data[basex + spacing2]) >> 1;
    xsize += yinc;
  }
  
  /*Average vertical; use "parent" values above and below*/
  xsize = bases + size;
  for(basey = bases; basey < size2; basey += yinc)
  {
    for(basex = basey; basex < xsize; basex += spacing)
      data[basex] = (data[basex + bases] + data[basex - bases]) >> 1;
    xsize += yinc;
  }

  /*Average center; use above, below, left, and right*/
  xsize = bases + size;
  for(basey = bases; basey < size2; basey += yinc)
  {
    for(basex = basey + spacing2; basex < xsize; basex += spacing)
    {
      data[basex] = (data[basex + bases]    + data[basex - bases] +
                     data[basex + spacing2] + data[basex - spacing2]) >> 2;
    }
    xsize += yinc;
  }
}
