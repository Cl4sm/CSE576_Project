void
init_by_array(uint32_t init_key[], int key_length)
{
  int i, j, k;

  init_genrand(19650218U);

  i = 1; j = 0;
  k = (N > key_length ? N : key_length);

  for (; k; k--)
  {
    mt[i] = (mt[i] ^ ((mt[i - 1] ^ (mt[i - 1] >> 30)) * 1664525U))
          + init_key[j] + j; /* non linear */
    mt[i] &= 0xffffffffU; /* for WORDSIZE > 32 machines */
    i++; j++;

    if (i >= N)
    {
      mt[0] = mt[N - 1];
      i = 1;
    }

    if (j >= key_length)
      j = 0;
  }

  for (k = N - 1; k; k--)
  {
    mt[i] = (mt[i] ^ ((mt[i - 1] ^ (mt[i - 1] >> 30)) * 1566083941U))
          - i; /* non linear */
    mt[i] &= 0xffffffffU; /* for WORDSIZE > 32 machines */
    i++;

    if (i >= N)
    {
      mt[0] = mt[N - 1];
      i = 1;
    }
  }

  mt[0] = 0x80000000U; /* MSB is 1; assuring non-zero initial array */
}
