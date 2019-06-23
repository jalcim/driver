#include <linux/pagemap.h>

#include "../data/mm.h"
#include "../include/frame_vector.h"

extern struct ft_frame_vector *vector;

int ft_frame_vector_to_pages(struct ft_frame_vector *vec)
{
  int i;
  unsigned long *nums;
  struct page **pages;

  printk(KERN_DEBUG "try frame_vector_to_pages !!! \n");
  if (!vec->is_pfns)
    return 0;
  nums = ft_frame_vector_pfns(vec);
  for (i = 0; i < vec->nr_frames; i++)
    if (!pfn_valid(nums[i]))
      return -EINVAL;
  pages = (struct page **)nums;
  for (i = 0; i < vec->nr_frames; i++)
    pages[i] = pfn_to_page(nums[i]);
  vec->is_pfns = false;
  printk(KERN_DEBUG "ok frame_vector_to_pages !!! \n");
  return 0;
}
EXPORT_SYMBOL(ft_frame_vector_to_pages);
