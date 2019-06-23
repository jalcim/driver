#include <linux/fs.h>
#include <linux/slab.h>

#include "../data/mm.h"
#include "../include/videobuf2.h"

struct ft_frame_vector *vector;

int test_vb2_create_framevec(struct inode *node, struct file *fd)
{
  printk(KERN_DEBUG "test ft_vb2_create_framevec !!! \n");
  vector = ft_vb2_create_framevec(65536, 4096, 1);
  if (vector)
    printk(KERN_DEBUG "ok ft_vb2_create_framevec !!! \n");
  else
    printk(KERN_DEBUG "ERROR : ft_vb2_create_framevec !!! \n\n\n");
  return (0);
}
EXPORT_SYMBOL(test_vb2_create_framevec);

int test_vb2_destroy_framevec(struct inode *node, struct file *fd)
{
  printk(KERN_DEBUG "test ft_vb2_destroy_framevec!!! \n");
  if (vector)
    ft_vb2_destroy_framevec(vector);
  else
    printk(KERN_DEBUG "ERROR : ft_vb2_destroy_framevec !!! \n");
  printk(KERN_DEBUG "ok ft_vb2_destroy_framevec !!! \n\n\n");
  return (0);
}
EXPORT_SYMBOL(test_vb2_destroy_framevec);

int test_vb2_vmalloc_put_userptr(struct file *fd, char __user *user, size_t size, loff_t *lofft)
{
  printk(KERN_DEBUG "test ft_vb2_vmalloc_put_userptr!!! \n");
  if (vector)
    ft_vb2_destroy_framevec(vector);
  else
    printk(KERN_DEBUG "ERROR : ft_vb2_vmalloc_put_userptr!!! \n");
  printk(KERN_DEBUG "ok ft_vb2_vmalloc_put_userptr!!! \n\n\n");
  return (0);
}
EXPORT_SYMBOL(test_vb2_vmalloc_put_userptr);
