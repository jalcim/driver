#include "../include/frame_vector.h"

struct ft_frame_vector *ft_vb2_create_framevec(unsigned long start,
					       unsigned long length,
					       bool write)
{
  int ret;
  unsigned long first, last;
  unsigned long nr;
  struct ft_frame_vector *vec;

  first = start >> PAGE_SHIFT;
  last = (start + length - 1) >> PAGE_SHIFT;
  nr = last - first + 1;
  vec = ft_frame_vector_create(nr);
  if (!vec)
    return ERR_PTR(-ENOMEM);
  //  ret = ft_get_vaddr_frames(start, nr, write, 1, vec);
  //  if (ret < 0)
  //    goto out_destroy;
  /* We accept only complete set of PFNs */
  //  if (ret != nr) {
  //    ret = -EFAULT;
  //    goto out_release;
  //  }
  return vec;
  // out_release:
  //  ft_put_vaddr_frames(vec);
  // out_destroy:
  //  ft_frame_vector_destroy(vec);
  return ERR_PTR(ret);
}
EXPORT_SYMBOL(ft_vb2_create_framevec);

void ft_vb2_destroy_framevec(struct ft_frame_vector *vec)
{
  ft_put_vaddr_frames(vec);
  ft_frame_vector_destroy(vec);
}
EXPORT_SYMBOL(ft_vb2_destroy_framevec);
