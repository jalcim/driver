struct frame_vector *ft_vb2_create_framevec(unsigned long start,
					 unsigned long length,
					 bool write)
{
  int ret;
  unsigned long first, last;
  unsigned long nr;
  struct frame_vector *vec;

  first = start >> PAGE_SHIFT;
  last = (start + length - 1) >> PAGE_SHIFT;
  nr = last - first + 1;
  vec = ft_frame_vector_create(nr);
  if (!vec)
    return ERR_PTR(-ENOMEM);
  ret = ft_get_vaddr_frames(start, nr, write, 1, vec);
  if (ret < 0)
    goto out_destroy;
  /* We accept only complete set of PFNs */
  if (ret != nr) {
    ret = -EFAULT;
    goto out_release;
  }
  return vec;
 out_release:
  //put_vaddr_frames(vec);
 out_destroy:
  ft_frame_vector_destroy(vec);
  return ERR_PTR(ret);
}
EXPORT_SYMBOL(ft_vb2_create_framevec);

static void *vb2_vmalloc_get_userptr(void *alloc_ctx, unsigned long vaddr,
				     unsigned long size,
				     enum dma_data_direction dma_dir)
{
  struct vb2_vmalloc_buf *buf;
  struct frame_vector *vec;
  int n_pages, offset, i;

  buf = kzalloc(sizeof(*buf), GFP_KERNEL);
  if (!buf)
    return NULL;

  buf->dma_dir = dma_dir;
  offset = vaddr & ~PAGE_MASK;
  buf->size = size;
  vec = vb2_create_framevec(vaddr, size, dma_dir == DMA_FROM_DEVICE);
  if (IS_ERR(vec))
    goto fail_pfnvec_create;
  buf->vec = vec;
  n_pages = frame_vector_count(vec);
  if (frame_vector_to_pages(vec) < 0) {
    unsigned long *nums = frame_vector_pfns(vec);

    /*
     * We cannot get page pointers for these pfns. Check memory is
     * physically contiguous and use direct mapping.
     */
    for (i = 1; i < n_pages; i++)
      if (nums[i-1] + 1 != nums[i])
	goto fail_map;
    buf->vaddr = (__force void *)
      ioremap_nocache(nums[0] << PAGE_SHIFT, size);
  } else {
    buf->vaddr = vm_map_ram(frame_vector_pages(vec), n_pages, -1,
			    PAGE_KERNEL);
  }

  if (!buf->vaddr)
    goto fail_map;
  buf->vaddr += offset;
  return buf;

 fail_map:
  vb2_destroy_framevec(vec);
 fail_pfnvec_create:
  kfree(buf);

  return NULL;
}
