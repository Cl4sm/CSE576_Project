SFdrawStrings(w, dir, from, to)
	register Window	w;
	register SFDir	*dir;
	register int	from;
	register int	to;
{
	register int		i;
	register SFEntry	*entry;
	int			x;

	x = SFtextX - dir->hOrigin * SFcharWidth;

	if (dir->vOrigin + to >= dir->nEntries) {
		to = dir->nEntries - dir->vOrigin - 1;
	}
	for (i = from; i <= to; i++) {
		entry = &(dir->entries[dir->vOrigin + i]);
		if (!(entry->statDone)) {
			if (SFstatAndCheck(dir, entry)) {
				if (dir->vOrigin + to >= dir->nEntries) {
					to = dir->nEntries - dir->vOrigin - 1;
				}
				i--;
				continue;
			}
		}
		XDrawImageString(
			SFdisplay,
			w,
			SFtextGC,
			x,
			SFtextYoffset + i * SFentryHeight,
			entry->shown,
			strlen(entry->shown)
		);
		if (dir->vOrigin + i == dir->beginSelection) {
			XDrawLine(
				SFdisplay,
				w,
				SFlineGC,
				SFlineToTextH + 1,
				SFlowerY + i * SFentryHeight,
				SFlineToTextH + SFentryWidth - 2,
				SFlowerY + i * SFentryHeight
			);
		}
		if (
			(dir->vOrigin + i >= dir->beginSelection) &&
			(dir->vOrigin + i <= dir->endSelection)
		) {
			SFcompletionSegs[0].y1 = SFcompletionSegs[1].y1 =
				SFlowerY + i * SFentryHeight;
			SFcompletionSegs[0].y2 = SFcompletionSegs[1].y2 =
				SFlowerY + (i + 1) * SFentryHeight - 1;
			XDrawSegments(
				SFdisplay,
				w,
				SFlineGC,
				SFcompletionSegs,
				2
			);
		}
		if (dir->vOrigin + i == dir->endSelection) {
			XDrawLine(
				SFdisplay,
				w,
				SFlineGC,
				SFlineToTextH + 1,
				SFlowerY + (i + 1) * SFentryHeight - 1,
				SFlineToTextH + SFentryWidth - 2,
				SFlowerY + (i + 1) * SFentryHeight - 1
			);
		}
	}
}
