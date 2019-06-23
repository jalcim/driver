#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>

#include "../include/frame_vector.h"

MODULE_AUTHOR("jalcim");
MODULE_DESCRIPTION("driver d'allocation memoire");
MODULE_SUPPORTED_DEVICE("UVC 1.5");
MODULE_LICENSE("GPL");

#define DEVICE_NAME "devalloc"

static dev_t dev_num;
static struct cdev *driver_object;
static struct class *device_class;

const struct file_operations devalloc_fops =
  {
    .open = test_frame_vector_create,
    .release = test_frame_vector_destroy,
    .read = test_frame_vector_pfns,
    .write = test_frame_vector_pages,
  };

static int __init mod_init(void)
{
  static int minor = 0;
  static int minor_numbers=1;

  printk(KERN_DEBUG "start device my master\n");

  if (alloc_chrdev_region(&dev_num, minor, minor_numbers, DEVICE_NAME) < 0)
    {
      printk(KERN_DEBUG "Device number and space allocating error !!! \n");
      return (-EIO);
    }

  if ((driver_object = cdev_alloc()) == NULL)
    goto free_device_number;
  driver_object->owner = THIS_MODULE;
  cdev_init(driver_object, &devalloc_fops);
  if (cdev_add(driver_object, dev_num, 1))
    goto free_device;

  if ((device_class = class_create(THIS_MODULE, DEVICE_NAME)) == NULL)
    {
      printk(KERN_DEBUG "Device class creating error !!! \n");
      goto free_device;
    }
  device_create(device_class, NULL, dev_num, NULL, "%s", DEVICE_NAME);

  printk(KERN_DEBUG "Creating device is success !!! \n\n\n");
  return (0);

 free_device :
  printk(KERN_DEBUG "Driver registering error or\n"
	  "Device class creation error.\n"
	 "Must free allocated space for device.\n");
  kobject_put(&driver_object->kobj);

 free_device_number :
  printk(KERN_DEBUG "chrdev registering error !!! \n");
  unregister_chrdev_region(dev_num, 1);
  return (-EIO);
}

static void __exit mod_exit(void)
{
  device_destroy(device_class, dev_num);
  class_destroy(device_class);
  cdev_del(driver_object);
  unregister_chrdev_region(dev_num, 1);

  printk(KERN_DEBUG "Device destruction success !!!\n");
  return ;
}

module_init(mod_init);
module_exit(mod_exit);
