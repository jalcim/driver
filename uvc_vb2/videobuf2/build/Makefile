obj-m 		+=	devalloc.o
devalloc-y	:=	../init/devalloc.o			\
			../frame_vector/frame_vector.o		\
			../frame_vector/frame_vector_pages.o	\
			../frame_vector/frame_vector_pfns.o	\
			../videobuf2/vb2_framevec.o		\
			../videobuf2/vb2_vmalloc.o		\
			../videobuf2/test_videobuf2.o


default:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
