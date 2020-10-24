void
plotdata(u, v, s)
int u, v, s;
{
  int v1=0, v2=0, c, w, ind;

  c = u;
  if (c<0) c+=map->zoom.width;
  if (c>=map->zoom.width) c-=map->zoom.width;
  c -= map->zoom.dx-1;
  v -= map->zoom.dy;

  if (fillmode==0) {
     if (c>=0 && c<=mapwidth && v>=0 && v<mapheight) {
        ind = c*mapheight+v;
        if (s>0) grid[ind] = s * 65536;
        if (s<0) grid[ind] = -s * 65536;
     }
     return;
  }

  if (c>=0 && c<=mapwidth) {
     ind = c*mapheight;

     if (v>=0 && v<mapheight) {
        if (s>0) grid[ind+v] |= s;
        if (s<0) grid[ind+v] |= -s;
     }

     s *= 65536;
      
     if (v<0) v1=0; else v1=v+1;
     if (v>=mapheight) v2=mapheight-1; else v2=v;

     if (u==uu) {
        if (v>vv)
           for (w=vv1; w<=v2; w++) grid[ind+w] += s;
        if (v<vv)
           for (w=v1; w<=vv2; w++) grid[ind+w] -= s;
     }
     if (u>uu)
        for (w=0; w<=v2; w++) grid[ind+w] += s;
  }

  if (u<uu && cc>=0 && cc<=mapwidth) {
     ind = cc*mapheight;
     for (w=0; w<=vv2; w++) grid[ind+w] -= s;
  }

  uu = u;
  cc = c;
  vv = v;
  vv1 = v1;
  vv2 = v2;
}
