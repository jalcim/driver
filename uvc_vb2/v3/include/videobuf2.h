#ifndef __FT_VIDEOBUF2__
#define __FT_VIDEOBUF2__

int test_vb2_create_framevec(struct inode *, struct file *);
int test_vb2_destroy_framevec(struct inode *, struct file *);

struct ft_frame_vector *ft_vb2_create_framevec(unsigned long start,
					       unsigned long length,
					       bool write);
void ft_vb2_destroy_framevec(struct ft_frame_vector *vec);

#endif
