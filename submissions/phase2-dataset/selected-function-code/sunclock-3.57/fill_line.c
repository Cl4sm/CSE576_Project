fill_line(char *scan, char* c, int w, int zw, int wp, int dx)
{
unsigned char r, g, b;
int i, j, k, m;
  k = 0;
  if (color_depth>16) {
    if (bigendian)
       k = bytes_per_pixel - 3;
    for (i = 0; i < w; i++) {
       j = 3 * (((i+dx) * wp)/zw);
       if (bigendian) {
          c[k] = scan[j];
          c[k+1] = scan[j+1];
          c[k+2] = scan[j+2];
       } else {
          c[k] = scan[j+2];
          c[k+1] = scan[j+1];
          c[k+2] = scan[j];
       }
       k +=  bytes_per_pixel;
    }
  } else
  if (color_depth==16)
     for (i = 0; i < w; i++) {
       j = 3 * (((i+dx) * wp)/zw);
       r = scan[j];
       g = scan[j+1];
       b = scan[j+2];
       /* blue  c[k] = 31;  c[k+1] = 0;
          green c[k] = 224  (low weight) c[k+1] = 7 (high weight)
          red   c[k] = 0;   c[k+1] = 248; */
       if (bigendian) {
          c[k+1] = (((b&248)>>3) | ((g&28)<<3));
          c[k] = (((g&224)>>5) | (r&248));
       } else {
          c[k] = (((b&248)>>3) | ((g&28)<<3));
          c[k+1] = (((g&224)>>5) | (r&248));
       }
       k += 2;
     }
  else
  if (color_depth==15)
     for (i = 0; i < w; i++) {
       j = 3 * (((i+dx) * wp)/zw);
       r = scan[j];
       g = scan[j+1];
       b = scan[j+2];
       /* blue  c[k] = 31;  c[k+1] = 0;
          green c[k] = 224  (low weight) c[k+1] = 7 (high weight)
          red   c[k] = 0;   c[k+1] = 248; */
       if (bigendian) {
          c[k+1] = (b&248)>>3 | (g&56)<<2;
          c[k] = (g&192)>>6 | (r&248)>>1;
       } else {
          c[k] = (b&248)>>3 | (g&56)<<2;
          c[k+1] = (g&192)>>6 | (r&248)>>1;
       }
       k += 2;
     }
  else {
     for (i = 0; i < w; i++) {
       j = 3 * (((i+dx) * wp)/zw);
       r = scan[j];
       g = scan[j+1];
       b = scan[j+2];
       c[k] = (unsigned char) 
       (((7*g)/256)*36)+(((6*r)/256)*6)+((6*b)/256);
       m = (unsigned char)c[k];
       lr[m] += r;
       lg[m] += g;
       lb[m] += b;
       lnum[m] += 1;
       k += 1;
     }
  }
}
