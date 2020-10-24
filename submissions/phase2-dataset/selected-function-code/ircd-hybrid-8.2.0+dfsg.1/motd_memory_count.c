motd_memory_count(struct Client *source_p)
{
  const dlink_node *ptr = NULL;
  unsigned int mt  = 0;  /* Motd count */
  unsigned int mtc = 0;  /* Motd cache count */
  size_t mtm  = 0;  /* Memory consumed by motd */
  size_t mtcm = 0;  /* Memory consumed by motd cache */

  if (MotdList.local)
  {
    mt++;
    mtm += sizeof(struct Motd);
    mtm += MotdList.local->path ? (strlen(MotdList.local->path) + 1) : 0;
  }

  if (MotdList.remote)
  {
    mt++;
    mtm += sizeof(struct Motd);
    mtm += MotdList.remote->path ? (strlen(MotdList.remote->path) + 1) : 0;
  }

  DLINK_FOREACH(ptr, MotdList.other.head)
  {
    const struct Motd *motd = ptr->data;

    mt++;
    mtm += sizeof(struct Motd);
    mtm += motd->path ? (strlen(motd->path) + 1) : 0;
  }

  DLINK_FOREACH(ptr, MotdList.cachelist.head)
  {
    const struct MotdCache *cache = ptr->data;

    mtc++;
    mtcm += sizeof(struct MotdCache) + (MOTD_LINESIZE * (cache->count - 1));
  }

  sendto_one_numeric(source_p, &me, RPL_STATSDEBUG|SND_EXPLICIT,
                     "z :Motds %u(%u) Cache %u(%u)",
                     mt, mtm, mtc, mtcm);
}
