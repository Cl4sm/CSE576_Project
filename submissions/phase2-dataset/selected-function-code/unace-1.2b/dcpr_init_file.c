void dcpr_init_file(void)
{
   UINT i;

   if (head.HEAD_FLAGS & ACE_PASSW)
   {
      printf("\nFound passworded file. Decryption not supported.\n");
      f_err = ERR_OTHER;
      return;
   }

   rd_crc = CRC_MASK;
   dcpr_size = fhead.SIZE;
   if (fhead.TECH.TYPE == TYPE_LZ1)
   {
      if ((fhead.TECH.PARM & 15) + 10 > dcpr_dic)
      {
         printf("\nNot enough memory or dictionary of archive too large.\n");
         f_err = ERR_MEM;
         return;
      }

      i = size_rdb * sizeof(LONG);
      read_adds_blk((CHAR *) buf_rd, i);
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
      bits_rd = rpos = 0;

      blocksize = 0;
   }
   if (!adat.sol || dcpr_frst_file)
      dcpr_dpos = 0;

   dcpr_oldnum = 0;
   memset(&dcpr_olddist, 0, sizeof(dcpr_olddist));

   dcpr_frst_file = 0;
}