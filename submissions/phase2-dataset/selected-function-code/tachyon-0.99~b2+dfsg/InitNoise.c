  byte x,y,z,i,j,k;
  unsigned int rndval = 1234567; /* pathetic random number seed */

  for (x=0; x<NMAX; x++) {
    for (y=0; y<NMAX; y++) {
      for (z=0; z<NMAX; z++) {
        NoiseMatrix[x][y][z]=(short int) ((rt_rand(&rndval) / RT_RAND_MAX) * 12000.0);

        if (x==NMAX-1) i=0; 
        else i=x;

        if (y==NMAX-1) j=0;
        else j=y;

        if (z==NMAX-1) k=0;
        else k=z;

        NoiseMatrix[x][y][z]=NoiseMatrix[i][j][k];
      }
    }
  }
}
