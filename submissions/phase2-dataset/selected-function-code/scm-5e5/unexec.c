     char *new_name, *a_name;
     unsigned data_start, bss_start, entry_address;
{
  int new, a_out = -1;

  if (a_name && (a_out = open (a_name, O_RDONLY)) < 0)
    {
      PERROR (a_name);
    }
  if ((new = creat (new_name, 0666)) < 0)
    {
      PERROR (new_name);
    }

  if (make_hdr (new, a_out, data_start, bss_start, entry_address, a_name, new_name) < 0
      || copy_text_and_data (new, a_out) < 0
      || copy_sym (new, a_out, a_name, new_name) < 0
#ifdef COFF
#ifndef COFF_BSD_SYMBOLS
      || adjust_lnnoptrs (new, a_out, new_name) < 0
#endif
#endif
      )
    {
      close (new);
      /* unlink (new_name);	    	/* Failed, unlink new a.out */
      return -1;	
    }

  close (new);
  if (a_out >= 0)
    close (a_out);
  mark_x (new_name);
  return 0;
}
