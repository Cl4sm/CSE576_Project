int Noise(flt x, flt y, flt z) {
  byte ix, iy, iz;
  flt ox, oy, oz;
  int p000, p001, p010, p011;
  int p100, p101, p110, p111;
  int p00, p01, p10, p11;
  int p0, p1;
  int d00, d01, d10, d11;
  int d0, d1, d;

  x=fabs(x);
  y=fabs(y);
  z=fabs(z);

  ix=((int) x) % NMAX;
  iy=((int) y) % NMAX;
  iz=((int) z) % NMAX;

  ox=(x - ((int) x));
  oy=(y - ((int) y));
  oz=(z - ((int) z));

  p000=NoiseMatrix[ix][iy][iz];
  p001=NoiseMatrix[ix][iy][iz+1];
  p010=NoiseMatrix[ix][iy+1][iz];
  p011=NoiseMatrix[ix][iy+1][iz+1];
  p100=NoiseMatrix[ix+1][iy][iz];
  p101=NoiseMatrix[ix+1][iy][iz+1];
  p110=NoiseMatrix[ix+1][iy+1][iz];
  p111=NoiseMatrix[ix+1][iy+1][iz+1];

  d00=p100-p000;
  d01=p101-p001;
  d10=p110-p010;
  d11=p111-p011;

  p00=(int) ((int) d00*ox) + p000;
  p01=(int) ((int) d01*ox) + p001;
  p10=(int) ((int) d10*ox) + p010;
  p11=(int) ((int) d11*ox) + p011;
  d0=p10-p00;
  d1=p11-p01;
  p0=(int) ((int) d0*oy) + p00;
  p1=(int) ((int) d1*oy) + p01;
  d=p1-p0;

  return (int) ((int) d*oz) + p0;
}
