                  apivector ctr, flt wx, flt wy) {
  int totalsize, x, y;
  flt * field; 
  unsigned int rndval; /* random number.. */

  totalsize=m*n;
  rndval = totalsize;
 
  field=(flt *) malloc(totalsize*sizeof(flt));

  for (y=0; y<n; y++) {
    for (x=0; x<m; x++) {
       field[x + y*m]=0.0;
    }
  }

  field[0 + 0            ] = rt_rand(&rndval) / RT_RAND_MAX;
  field[m - 1            ] = rt_rand(&rndval) / RT_RAND_MAX;
  field[0     + m*(n - 1)] = rt_rand(&rndval) / RT_RAND_MAX;
  field[m - 1 + m*(n - 1)] = rt_rand(&rndval) / RT_RAND_MAX;

  subdivide(field, m, n, wx, wy, 0, 0, m-1, n-1, &rndval);

  rt_sheightfield(scene, tex, ctr, m, n, field, wx, wy);

  free(field);
}
