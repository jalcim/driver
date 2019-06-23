#include <linux/io.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/dma-direction.h>
#include <linux/mm.h>
#include <media/videobuf2-memops.h>

#include "../include/frame_vector.h"
#include "../include/videobuf2.h"
#include "../data/mm.h"

static void ft_vb2_vmalloc_put_userptr(void *buf_priv)
{
  struct ft_vb2_vmalloc_buf *buf = buf_priv;
  unsigned long vaddr = (unsigned long)buf->vaddr & PAGE_MASK;
  unsigned int i;
  struct page **pages;
  unsigned int n_pages;

  if (!buf->vec->is_pfns) {
    n_pages = ft_frame_vector_count(buf->vec);
    pages = ft_frame_vector_pages(buf->vec);
    if (vaddr)
      vm_unmap_ram((void *)vaddr, n_pages);
    if (buf->dma_dir == DMA_FROM_DEVICE)
      for (i = 0; i < n_pages; i++)
	set_page_dirty_lock(pages[i]);
  } else {
    iounmap((__force void __iomem *)buf->vaddr);
  }
  ft_vb2_destroy_framevec(buf->vec);
  kfree(buf);
}
