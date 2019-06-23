#include <linux/fs.h>
#include <linux/slab.h>

#include "../data/mm.h"
#include "../include/frame_vector.h"

struct ft_frame_vector *vector;

int test_frame_vector_create(struct inode *node, struct file *fd)
{
  printk(KERN_DEBUG "test frame_vector_create !!! \n");
  vector = ft_frame_vector_create(4096);
  return (0);
}
EXPORT_SYMBOL(test_frame_vector_create);

int test_frame_vector_destroy(struct inode *node, struct file *fd)
{
  printk(KERN_DEBUG "test frame_vector_destroy !!! \n");
  ft_frame_vector_destroy(vector);
  return (0);
}
EXPORT_SYMBOL(test_frame_vector_destroy);

ssize_t test_frame_vector_pfns(struct file *fd ,  char __user *buf,
				  size_t len, loff_t *unknow)
{
  printk(KERN_DEBUG "test frame_vector_pfns !!! \n");
  if (vector)
    ft_frame_vector_pfns(vector);
  return (0);
}
EXPORT_SYMBOL(test_frame_vector_pfns);

ssize_t test_get_vaddr_frames(struct file *fd, const char __user *user, size_t size, loff_t *lofft)
{
  printk(KERN_DEBUG "\n\n\ntest get_vaddr_frames !!! \n");
  if (vector)
    ft_get_vaddr_frames(65536, 4096, 2, 1, vector);
  return (0);
}
EXPORT_SYMBOL(test_get_vaddr_frames);
