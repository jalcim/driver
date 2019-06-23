#ifndef __DATA_FRAME_VECTOR__
#define __DATA_FRAME_VECTOR__

struct ft_frame_vector {
  unsigned int nr_allocated;/* Number of frames we have space for */
  unsigned int nr_frames;/* Number of frames stored in ptrs array */
  bool got_ref;/* Did we pin pages by getting page ref? */
  bool is_pfns;/* Does array contain pages or pfns? */
  void *ptrs[0];/* Array of pinned pfns / pages. Use
		 * pfns_vector_pages() or pfns_vector_pfns()
		 * for access */
};

#endif
