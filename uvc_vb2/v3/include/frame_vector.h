#ifndef __INCLUDE_FRAME_VECTOR__
#define __INCLUDE_FRAME_VECTOR__

#include <linux/fs.h>
#include "../data/mm.h"

struct ft_frame_vector *ft_frame_vector_create(unsigned int nr_frames);
void ft_frame_vector_destroy(struct ft_frame_vector *vec);
void ft_put_vaddr_frames(struct ft_frame_vector *vec);

void ft_frame_vector_to_pfns(struct ft_frame_vector *vec);
int ft_frame_vector_to_pages(struct ft_frame_vector *vec);

static inline unsigned int ft_frame_vector_count(struct ft_frame_vector *vec)
{
  return vec->nr_frames;
}

static inline struct page **ft_frame_vector_pages(struct ft_frame_vector *vec)
{
  printk(KERN_DEBUG "try ft_frame_vector_pages !!! \n");
  if (vec->is_pfns)
    {
      int err = ft_frame_vector_to_pages(vec);

      if (err)
	{
	  printk(KERN_DEBUG "ERROR : ft_frame_vector_pages !!! \n");
	  return ERR_PTR(err);
	}
    }
  printk(KERN_DEBUG "ok ft_frame_vector_pages !!! \n");
  return (struct page **)(vec->ptrs);
}

static inline unsigned long *ft_frame_vector_pfns(struct ft_frame_vector *vec)
{
  printk(KERN_DEBUG "ok frame_vector_pfns !!! \n");
  if (!vec->is_pfns)
    ft_frame_vector_to_pfns(vec);
  else
    printk(KERN_DEBUG "vec->is_pfn!!! \n");
  return (unsigned long *)(vec->ptrs);
}

#endif
