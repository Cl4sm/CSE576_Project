void dcpr_comm(INT comm_size)
{
   SHORT hash[comm_cpr_hf(255, 255) + 1];
   INT  dpos = 0,
        c,
        pos,
        len,
        hs;

   memset(&hash, 0, sizeof(hash));
   if (comm_cpr_size)
   {
      dcpr_comm_init();
      len = code_rd >> (32 - 15);
      addbits(15);
      if (len >= comm_size)
         len = comm_size - 1;
      if (read_wd(maxwd_mn, dcpr_code_mn, dcpr_wd_mn, max_cd_mn))
         do
         {
            if (dpos > 1)
            {
               pos = hash[hs = comm_cpr_hf(comm[dpos - 1], comm[dpos - 2])];
               hash[hs] = dpos;
            }
            addbits(dcpr_wd_mn[(c = dcpr_code_mn[code_rd >> (32 - maxwd_mn)])]);
            if (rpos == size_rdb - 3)
               rpos = 0;
            if (c > 255)
            {
               c -= 256;
               c += 2;
               while (c--)
                  comm[dpos++] = comm[pos++];
            }
            else
            {
               comm[dpos++] = c;
            }
         }
         while (dpos < len);
      comm[len] = 0;
   }
}