#ifndef __FT_VIDEOBUF2__
#define __FT_VIDEOBUF2__

int test_vb2_create_framevec(struct inode *, struct file *);
int test_vb2_destroy_framevec(struct inode *, struct file *);
int test_vb2_vmalloc_put_userptr(struct file*,char __user*,size_t,loff_t*);

struct ft_frame_vector *ft_vb2_create_framevec(unsigned long,unsigned long,bool);
void ft_vb2_destroy_framevec(struct ft_frame_vector *vec);

#endif
