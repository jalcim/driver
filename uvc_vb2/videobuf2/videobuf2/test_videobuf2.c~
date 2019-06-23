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
  printk(KERN_DEBUG "ERROR : ft_vb2_create_framevec !!! \n");
  return (0);
}
EXPORT_SYMBOL(test_vb2_create_framevec);

int test_vb2_destroy_framevec(struct inode *node, struct file *fd)
{
  printk(KERN_DEBUG "test ft_vb2_destroy_framevec!!! \n");
  ft_vb2_destroy_framevec(vector);
  printk(KERN_DEBUG "ok ft_vb2_destroy_framevec !!! \n");
  return (0);
}
EXPORT_SYMBOL(test_vb2_destroy_framevec);
