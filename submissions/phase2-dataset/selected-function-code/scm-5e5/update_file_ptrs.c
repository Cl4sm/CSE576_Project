update_file_ptrs (file, hdr, auxhdr, location, offset)
     int file;
     struct header *hdr;
     struct som_exec_auxhdr *auxhdr;
     unsigned int location;
     int offset;
{
  struct subspace_dictionary_record subspace;
  int i;
  
  /* Increase the overall size of the module */
  hdr->som_length += offset;
  
  /* Update the various file pointers in the header */
#define update(ptr) if (ptr > location) ptr = ptr + offset
  update (hdr->aux_header_location);
  update (hdr->space_strings_location);
  update (hdr->init_array_location);
  update (hdr->compiler_location);
  update (hdr->symbol_location);
  update (hdr->fixup_request_location);
  update (hdr->symbol_strings_location);
  update (hdr->unloadable_sp_location);
  update (auxhdr->exec_tfile);
  update (auxhdr->exec_dfile);
  
  /* Do for each subspace dictionary entry */
  lseek (file, hdr->subspace_location, 0);
  for (i = 0; i < hdr->subspace_total; i++)
    {
      if (read (file, &subspace, sizeof (subspace)) != sizeof (subspace))
	{ perror ("Can't read subspace record"); exit (1); }
      
      /* If subspace has a file location, update it */
      if (subspace.initialization_length > 0 
	  && subspace.file_loc_init_value > location)
	{
	  subspace.file_loc_init_value += offset;
	  lseek (file, -sizeof (subspace), 1);
	  if (write (file, &subspace, sizeof (subspace)) != sizeof (subspace))
	    { perror ("Can't update subspace record"); exit (1); }
	}
    } 
  
  /* Do for each initialization pointer record */
  /* (I don't think it applies to executable files, only relocatables) */
#undef update
}
