int check(i, j, which)
int i, j, which;
{
#define MASK -65536
   int ind, indp, inds;

   which *= 65536;

   ind = i*mapheight+j;
   if (j>0 && (grid[ind-1]&MASK)!=which) return 1;
   if (j<mapheight-1 && (grid[ind+1]&MASK)!=which) return 1;

   indp = (i-1)*mapheight+j;
   if (i>0 && (grid[indp]&MASK)!=which) return 1;

   inds = (i+1)*mapheight+j;
   if (i<mapwidth && (grid[inds]&MASK)!=which) return 1;

   if (full) {
     if (j>0) {
        if (i>0 && (grid[indp-1]&MASK)!=which) return 1;
        if (i<mapwidth && (grid[inds-1]&MASK)!=which) return 1;
     }
     if (j<mapheight-1) {
        if (i>0 && (grid[indp+1]&MASK)!=which) return 1;
        if (i<mapwidth && (grid[inds+1]&MASK)!=which) return 1;
     }
   }
   return 0;
}
