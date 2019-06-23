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

ssize_t test_frame_vector_pages(struct file *fd ,  char __user *buf,
				  size_t len, loff_t *unknow)
{
  printk(KERN_DEBUG "test frame_vector_pages !!! \n");
  if (vector)
    ft_frame_vector_pages(vector);
  printk(KERN_DEBUG "ok frame_vector_pages !!! \n\n\n");
  return (0);
}
EXPORT_SYMBOL(test_frame_vector_pages);
