void dcpr_comm_init(void)
{
   INT  i;

   i = comm_cpr_size > size_rdb * sizeof(LONG) ? size_rdb * sizeof(LONG) : comm_cpr_size;
   if (!f_err)
      memcpy(buf_rd, comm, i);
#ifdef HI_LO_BYTE_ORDER
   {
      ULONG *p;
      i>>=2;    // count LONGs not BYTEs
      p=buf_rd;
      while (i--)
      {
         LONGswap(p);
         p++; 
      }
   }
#endif
   code_rd = buf_rd[0];
   rpos = bits_rd = 0;
}