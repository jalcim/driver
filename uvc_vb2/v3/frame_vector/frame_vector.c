#include <linux/slab.h>
#include <linux/pagemap.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/mm_types.h>
#include <asm/current.h>

#include "../data/mm.h"
#include "../include/frame_vector.h"

void * kmalloc (size_t size, gfp_t flags);

extern struct ft_frame_vector *vector;

struct ft_frame_vector *ft_frame_vector_create(unsigned int nr_frames)
{
  struct ft_frame_vector *vec;
  int size = sizeof(struct ft_frame_vector) + sizeof(void *) * nr_frames;

  if (WARN_ON_ONCE(nr_frames == 0))
    return NULL;
  /*
   * This is absurdly high. It's here just to avoid strange effects when
   * arithmetics overflows.
   */
  if (WARN_ON_ONCE(nr_frames > INT_MAX / sizeof(void *) / 2))
    return NULL;
  /*
   * Avoid higher order allocations, use vmalloc instead. It should
   * be rare anyway.
   */
  vec = kmalloc(size, GFP_KERNEL);
  if (!vec)
    return NULL;
  vec->nr_allocated = nr_frames;
  vec->nr_frames = 0;
  printk(KERN_DEBUG "ok frame_vector_create !!! \n\n\n");
  return vec;
}
EXPORT_SYMBOL(ft_frame_vector_create);

void ft_frame_vector_destroy(struct ft_frame_vector *vec)
{
  /* Make sure put_vaddr_frames() got called properly... */
  VM_BUG_ON(vec->nr_frames > 0);
  kfree(vec);
  printk(KERN_DEBUG "ok frame_vector_destroy !!! \n\n\n");
}
EXPORT_SYMBOL(ft_frame_vector_destroy);

void ft_put_vaddr_frames(struct ft_frame_vector *vec)
{
  int i;
  struct page **pages;

  printk(KERN_DEBUG "try ft_put_vaddr_frames !!! \n");
  if (!vec->got_ref)
    goto out;
  pages = ft_frame_vector_pages(vec);
  /*
   * frame_vector_pages() might needed to do a conversion when
   * get_vaddr_frames() got pages but vec was later converted to pfns.
   * But it shouldn't really fail to convert pfns back...
   */
  if (WARN_ON(IS_ERR(pages)))
    goto out;
  for (i = 0; i < vec->nr_frames; i++)
    put_page(pages[i]);
  vec->got_ref = false;
 out:
  vec->nr_frames = 0;
  printk(KERN_DEBUG "ok ft_put_vaddr_frames !!! \n");
}
EXPORT_SYMBOL(ft_put_vaddr_frames);
