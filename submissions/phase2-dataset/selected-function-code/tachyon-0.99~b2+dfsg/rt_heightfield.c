                    flt * field, flt wx, flt wy) {
  int xx,yy; 
  apivector v0, v1, v2; 
  flt xoff, yoff, zoff;

  xoff=ctr.x - (wx / 2.0);
  yoff=ctr.z - (wy / 2.0);
  zoff=ctr.y;

  for (yy=0; yy<(n-1); yy++) { 
    for (xx=0; xx<(m-1); xx++) {
      v0.x=wx*(xx    )/(m*1.0) + xoff; 
      v0.y=field[(yy    )*m + (xx    )] + zoff;
      v0.z=wy*(yy    )/(n*1.0) + yoff;

      v1.x=wx*(xx + 1)/(m*1.0) + xoff; 
      v1.y=field[(yy    )*m + (xx + 1)] + zoff;
      v1.z=wy*(yy    )/(n*1.0) + yoff;

      v2.x=wx*(xx + 1)/(m*1.0) + xoff; 
      v2.y=field[(yy + 1)*m + (xx + 1)] + zoff;
      v2.z=wy*(yy + 1)/(n*1.0) + yoff;

      rt_tri(scene, tex, v1, v0, v2);

      v0.x=wx*(xx    )/(m*1.0) + xoff;
      v0.y=field[(yy    )*m + (xx    )] + zoff;
      v0.z=wy*(yy    )/(n*1.0) + yoff;

      v1.x=wx*(xx    )/(m*1.0) + xoff;
      v1.y=field[(yy + 1)*m + (xx    )] + zoff;
      v1.z=wy*(yy + 1)/(n*1.0) + yoff;

      v2.x=wx*(xx + 1)/(m*1.0) + xoff;
      v2.y=field[(yy + 1)*m + (xx + 1)] + zoff;
      v2.z=wy*(yy + 1)/(n*1.0) + yoff;
 
      rt_tri(scene, tex, v0, v1, v2);
    }
  } 
} /* end of heightfield */
