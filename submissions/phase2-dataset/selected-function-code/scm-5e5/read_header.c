read_header (file, hdr, auxhdr)
     int file;
     struct header *hdr;
     struct som_exec_auxhdr *auxhdr;
{
  
  /* Read the header in */
  lseek (file, 0, 0);
  if (read (file, hdr, sizeof (*hdr)) != sizeof (*hdr))
    { perror ("Couldn't read header from a.out file"); exit (1); }
  
  if (hdr->a_magic != EXEC_MAGIC && hdr->a_magic != SHARE_MAGIC
      &&  hdr->a_magic != DEMAND_MAGIC)
    {
      fprintf (stderr, "a.out file doesn't have legal magic number\n"); 
      exit (1);  
    }
  
  lseek (file, hdr->aux_header_location, 0);
  if (read (file, auxhdr, sizeof (*auxhdr)) != sizeof (*auxhdr))
    {
      perror ("Couldn't read auxiliary header from a.out file");
      exit (1);
    }  
}
