#include <linux/slab.h>
#include <linux/pagemap.h>
#include <linux/fs.h>
#include <linux/mm.h>

#define FOLL_TRIED 0x800/* a retry, previous pass started an IO */



static long __ft_get_user_pages_locked(struct task_struct *tsk,
						    struct mm_struct *mm,
						    unsigned long start,
						    unsigned long nr_pages,
						    int write, int force,
						    struct page **pages,
						    struct vm_area_struct **vmas,
						    int *locked, bool notify_drop,
						    unsigned int flags)
{
  long ret, pages_done;
  bool lock_dropped;

  if (locked) {
    /* if VM_FAULT_RETRY can be returned, vmas become invalid */
    BUG_ON(vmas);
    /* check caller initialized locked */
    BUG_ON(*locked != 1);
  }

  if (pages)
    flags |= FOLL_GET;
  if (write)
    flags |= FOLL_WRITE;
  if (force)
    flags |= FOLL_FORCE;

  pages_done = 0;
  lock_dropped = false;
  for (;;) {
    printk(KERN_DEBUG "try 1 get_user_pages_locked !!! \n");
    ret = __get_user_pages(tsk, mm, start, nr_pages, flags, pages,
			   vmas, locked);
    printk(KERN_DEBUG "try 2 get_user_pages_locked !!! \n");
    if (!locked)
      /* VM_FAULT_RETRY couldn't trigger, bypass */
      return ret;

    /* VM_FAULT_RETRY cannot return errors */
    if (!*locked) {
      BUG_ON(ret < 0);
      BUG_ON(ret >= nr_pages);
    }

    if (!pages)
      /* If it's a prefault don't insist harder */
      return ret;

    if (ret > 0) {
      nr_pages -= ret;
      pages_done += ret;
      if (!nr_pages)
	break;
    }
    if (*locked) {
      /* VM_FAULT_RETRY didn't trigger */
      if (!pages_done)
	pages_done = ret;
      break;
    }
    /* VM_FAULT_RETRY triggered, so seek to the faulting offset */
    pages += ret;
    start += ret << PAGE_SHIFT;

    /*
     * Repeat on the address that fired VM_FAULT_RETRY
     * without FAULT_FLAG_ALLOW_RETRY but with
     * FAULT_FLAG_TRIED.
     */
    *locked = 1;
    lock_dropped = true;
    down_read(&mm->mmap_sem);
    printk(KERN_DEBUG "try 3 get_user_pages_locked !!! \ncrash ?");
    ret = __get_user_pages(tsk, mm, start, 1, flags | FOLL_TRIED,
			   pages, NULL, NULL);
    printk(KERN_DEBUG "try 4 get_user_pages_locked !!! \n");
    if (ret != 1) {
      BUG_ON(ret > 1);
      if (!pages_done)
	pages_done = ret;
      break;
    }
    nr_pages--;
    pages_done++;
    if (!nr_pages)
      break;
    pages++;
    start += PAGE_SIZE;
  }
  if (notify_drop && lock_dropped && *locked) {
    /*
     * We must let the caller know we temporarily dropped the lock
     * and so the critical section protected by it was lost.
     */
    up_read(&mm->mmap_sem);
    *locked = 0;
  }
  printk(KERN_DEBUG "ok get_user_pages_locked !!! \n\n\n");
  return pages_done;
}

long ft_get_user_pages_locked(struct task_struct *tsk, struct mm_struct *mm,
			   unsigned long start, unsigned long nr_pages,
			   int write, int force, struct page **pages,
			   int *locked)
{
  printk(KERN_DEBUG "try 0 get_user_pages_locked !!! \n");
  return __ft_get_user_pages_locked(tsk, mm, start, nr_pages, write, force,
				 pages, NULL, locked, true, FOLL_TOUCH);
}

//EXPORT_SYMBOL(ft_get_user_pages_locked);
