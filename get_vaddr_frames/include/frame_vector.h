#ifndef __INCLUDE_FRAME_VECTOR__
#define __INCLUDE_FRAME_VECTOR__

#include <linux/fs.h>
int test_frame_vector_create(struct inode *, struct file *);
int test_frame_vector_destroy(struct inode *, struct file *);
ssize_t test_frame_vector_pfns(struct file *,  char __user *, size_t, loff_t *);
ssize_t test_get_vaddr_frames(struct file *fd, const char __user *user, size_t size, loff_t *lofft);




struct ft_frame_vector *ft_frame_vector_create(unsigned int nr_frames);
void ft_frame_vector_destroy(struct ft_frame_vector *vec);
int ft_get_vaddr_frames(unsigned long start, unsigned int nr_frames,
			bool write, bool force, struct ft_frame_vector *vec);

unsigned long *ft_frame_vector_pfns(struct ft_frame_vector *vec);
void ft_frame_vector_to_pfns(struct ft_frame_vector *vec);

int ft_frame_vector_to_pages(struct ft_frame_vector *vec);

long ft_get_user_pages_locked(struct task_struct *tsk, struct mm_struct *mm,
			      unsigned long start, unsigned long nr_pages,
			      int write, int force, struct page **pages,
			      int *locked);

#endif
