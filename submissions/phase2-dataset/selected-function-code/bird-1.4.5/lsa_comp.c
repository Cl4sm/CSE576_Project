lsa_comp(struct ospf_lsa_header *l1, struct ospf_lsa_header *l2)
			/* Return codes from point of view of l1 */
{
  u32 sn1, sn2;

  sn1 = l1->sn - LSA_INITSEQNO + 1;
  sn2 = l2->sn - LSA_INITSEQNO + 1;

  if (sn1 > sn2)
    return CMP_NEWER;
  if (sn1 < sn2)
    return CMP_OLDER;

  if (l1->checksum != l2->checksum)
    return l1->checksum < l2->checksum ? CMP_OLDER : CMP_NEWER;

  if ((l1->age == LSA_MAXAGE) && (l2->age != LSA_MAXAGE))
    return CMP_NEWER;
  if ((l2->age == LSA_MAXAGE) && (l1->age != LSA_MAXAGE))
    return CMP_OLDER;

  if (ABS(l1->age - l2->age) > LSA_MAXAGEDIFF)
    return l1->age < l2->age ? CMP_NEWER : CMP_OLDER;

  return CMP_SAME;
}
