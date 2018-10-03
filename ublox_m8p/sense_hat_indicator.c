#define _GNU_SOURCE
#define DEV_INPUT_EVENT "/dev/input"
#define EVENT_DEV_NAME "event"
#define DEV_FB "/dev"
#define FB_DEV_NAME "fb"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <time.h>
#include <poll.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

#include <linux/input.h>
#include <linux/fb.h>


// Frame buffer
struct fb_t {
	uint16_t pixel[8][8];
};
struct fb_t *fb;

static int is_framebuffer_device(const struct dirent *dir)
{
	return strncmp(FB_DEV_NAME, dir->d_name,
		       strlen(FB_DEV_NAME)-1) == 0;
}

static int open_fbdev(const char *dev_name)
{
	struct dirent **namelist;
	int i, ndev;
	int fd = -1;
	struct fb_fix_screeninfo fix_info;

	ndev = scandir(DEV_FB, &namelist, is_framebuffer_device, versionsort);
	if (ndev <= 0)
		return ndev;

	for (i = 0; i < ndev; i++)
	{
		char fname[64];
		char name[256];

		snprintf(fname, sizeof(fname),
			 "%s/%s", DEV_FB, namelist[i]->d_name);
		fd = open(fname, O_RDWR);
		if (fd < 0)
			continue;
		ioctl(fd, FBIOGET_FSCREENINFO, &fix_info);
		if (strcmp(dev_name, fix_info.id) == 0)
			break;
		close(fd);
		fd = -1;
	}
	for (i = 0; i < ndev; i++)
		free(namelist[i]);

	return fd;
}


void color_fill(int color)
{
	int i,j;
	for (j = 0; j<16; j++) {
		for (i = 0; i < 16; i++) {
			fb->pixel[j][i]=color;
		}
	}
}

void test_colors () {
	int i;
	for (i = 0; i < 16; i++) {
		fprintf(stdout,"color bit=%x\n",i);
		color_fill(1<<i);
		usleep(5000000);
	}
}


int main(int argc, char* args[])
{
	int ret = 0;
	int fbfd = 0;

	int color,i;
	char buffer[255];
	char *s;
	int nline = 0;

	struct pollfd evpoll = {
		.events = POLLIN,
	};
	
	srand (time(NULL));

	fbfd = open_fbdev("RPi-Sense FB");
	if (fbfd <= 0) {
		ret = fbfd;
		printf("Error: cannot open framebuffer device.\n");
		goto err_ev; 
		
	}
	
	fb = mmap(0, 128, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if (!fb) {
		ret = EXIT_FAILURE;
		printf("Failed to mmap.\n");
		goto err_fb;
	}


	while(fgets(buffer, 255, stdin)) {
	if (strncmp("$GNGGA",buffer,6)==0) {
		fprintf(stdout,"GGA=%s\n",buffer);

		s = buffer;
		for (i = 0; i < 6; i++) {
			s = strchr(s, ',');
			if (s == NULL) {
				fprintf(stdout,"???\n");
				break;
			} else {
				s++;
			}
		}
		if (s != NULL) {
			switch (*s) {
				case '0':
				color = 3<<14; // red
				break;
				case '1':
				color = 3<<14 | 2<<9; // amber
				break;
				case '4':
				color = 3<<3;  // blue
				break;
				case '5':
				color = 3<<9; // green
				break;
			}
			color_fill(nline%2==0 ? 0 : color);
		}
		nline++;
	}
	}

	munmap(fb, 128);
err_fb:
	close(fbfd);
err_ev:
	close(evpoll.fd);
	return ret;
}
