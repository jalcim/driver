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

int ft_get_vaddr_frames(unsigned long start, unsigned int nr_frames,
		     bool write, bool force, struct ft_frame_vector *vec)
{
  struct mm_struct *mm = NULL;
  struct vm_area_struct *vma;
  int ret = 0;
  int err;
  int locked;

  if (nr_frames == 0)
    return 0;
  printk(KERN_DEBUG "try 1 get_vaddr_frames !!! \n");
  mm = current->mm;
  if (WARN_ON_ONCE(nr_frames > vec->nr_allocated))
    nr_frames = vec->nr_allocated;

  down_read(&mm->mmap_sem);
  locked = 1;
  vma = find_vma_intersection(mm, start, start + 1);
  if (!vma) {
    ret = -EFAULT;
    goto out;
  }
  if (!(vma->vm_flags & (VM_IO | VM_PFNMAP))) {
    vec->got_ref = true;
    vec->is_pfns = false;
    //fonction tres instable !!!!!!!!!!!
    printk(KERN_DEBUG "try 2 get_vaddr_frames !!! \n");
    ret = ft_get_user_pages_locked(current, mm, start, nr_frames,
				write, force, (struct page **)(vec->ptrs), &locked);
    printk(KERN_DEBUG "try 3 get_vaddr_frames !!! \n");
    goto out;
  }

  vec->got_ref = false;
  vec->is_pfns = true;
  do {
    unsigned long *nums = ft_frame_vector_pfns(vec);

    while (ret < nr_frames && start + PAGE_SIZE <= vma->vm_end) {
      err = follow_pfn(vma, start, &nums[ret]);
      if (err) {
	if (ret == 0)
	  ret = err;
	goto out;
      }
      start += PAGE_SIZE;
      ret++;
    }

    if (ret >= nr_frames || start < vma->vm_end)
      break;
    vma = find_vma_intersection(mm, start, start + 1);
  } while (vma && vma->vm_flags & (VM_IO | VM_PFNMAP));
 out:
  printk(KERN_DEBUG "ok get_vaddr_frames !!! \n");
  if (locked)
    up_read(&mm->mmap_sem);
  if (!ret)
    ret = -EFAULT;
  if (ret > 0)
    vec->nr_frames = ret;
  return ret;
}
EXPORT_SYMBOL(ft_get_vaddr_frames);
