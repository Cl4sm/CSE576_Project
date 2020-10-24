int
remove_readonly_section (GElf_Ehdr *ehdr, GElf_Shdr *shdr, int n,
			 struct readonly_adjust *adjust)
{
  int i, ret = -1;

  memmove (&shdr[n], &shdr[n + 1],
	   (ehdr->e_shnum - n) * sizeof (GElf_Shdr));
  --ehdr->e_shnum;
  for (i = 0; i < adjust->newcount; ++i)
    if (adjust->new[i] > n)
      --adjust->new[i];
    else if (adjust->new[i] == n)
      {
	adjust->new[i] = -1;
	ret = i;
      }

  return ret;
}
