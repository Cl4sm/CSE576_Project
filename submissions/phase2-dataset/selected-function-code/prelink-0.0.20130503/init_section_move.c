init_section_move (DSO *dso)
{
  struct section_move *move;
  int i;

  move = malloc (sizeof (struct section_move)
		 + (dso->ehdr.e_shnum * 2 + 20) * sizeof (int));
  if (move == NULL)
    {
      error (0, ENOMEM, "%s: Could not move sections", dso->filename);
      return move;
    }
  move->old_shnum = dso->ehdr.e_shnum;
  move->new_shnum = dso->ehdr.e_shnum;
  move->old_to_new = (int *)(move + 1);
  move->new_to_old = move->old_to_new + move->new_shnum;
  for (i = 0; i < move->new_shnum; i++)
    {
      move->old_to_new[i] = i;
      move->new_to_old[i] = i;
    }
  return move;
}
