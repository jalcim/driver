#include <linux/pagemap.h>

#include "../data/mm.h"
#include "../include/frame_vector.h"

extern struct ft_frame_vector *vector;
/*
unsigned long *ft_frame_vector_pfns(struct ft_frame_vector *vec)
{
  printk(KERN_DEBUG "ok frame_vector_pfns !!! \n");
  if (!vec->is_pfns)
    ft_frame_vector_to_pfns(vec);
  else
    printk(KERN_DEBUG "vec->is_pfn!!! \n");
  return (unsigned long *)(vec->ptrs);
}
EXPORT_SYMBOL(ft_frame_vector_pfns);
*/
void ft_frame_vector_to_pfns(struct ft_frame_vector *vec)
{
  int i;
  unsigned long *nums;
  struct page **pages;

  if (vec->is_pfns)
    return;
  pages = (struct page **)(vec->ptrs);
  nums = (unsigned long *)pages;
  for (i = 0; i < vec->nr_frames; i++)
    nums[i] = page_to_pfn(pages[i]);
  vec->is_pfns = true;
  printk(KERN_DEBUG "ok frame_vector_to_pfns !!! \n\n\n");
}
EXPORT_SYMBOL(ft_frame_vector_to_pfns);
