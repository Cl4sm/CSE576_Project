void readdat(void)
{
   UINT i;

   i = (size_rdb - 2) << 2;
   rpos -= size_rdb - 2;
   buf_rd[0] = buf_rd[size_rdb - 2];
   buf_rd[1] = buf_rd[size_rdb - 1];
   read_adds_blk((CHAR *) & buf_rd[2], i);
#ifdef HI_LO_BYTE_ORDER
   {
      ULONG *p;
      i>>=2;    // count LONGs not BYTEs
      p=&buf_rd[2];
      while (i--)
      {
         LONGswap(p);
         p++; 
      }
   }
#endif
}