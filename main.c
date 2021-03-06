#include <stdio.h>
#include <psp2/ctrl.h>
#include <psp2/io/fcntl.h>
#include <psp2/power.h>
#include "debugScreen.h"

#define printf(...) psvDebugScreenPrintf(__VA_ARGS__)
#define MOUNT_ID (0xE00)

int vshIoUmount(int id, int a2, int a3, int a4);
int _vshIoMount(int id, const char *path, int permission, void *buf);
int vshIoMount(int id, const char *path, int permission, int a4, int a5, int a6);

unsigned int wait_keys(unsigned int key_mask);

int vshIoMount(int id, const char *path, int permission, int a4, int a5, int a6) {
	uint32_t buf[6];

	buf[0] = a4;
	buf[1] = a5;
	buf[2] = a6;
	buf[3] = 0;
	buf[4] = 0;
	buf[5] = 0;

	return _vshIoMount(id, path, permission, buf);
}

int main(int argc, char *argv[])
{
	int ret;
	SceUID fd;

	psvDebugScreenInit();

	psvDebugScreenSetFgColor(COLOR_WHITE);
	printf("CardUnlock by cnsldv\n");
	printf("====================\n");
	printf("\n");
	psvDebugScreenSetFgColor(COLOR_RED);
	printf("WARNING: use of this program is at your own risk!\n");
	psvDebugScreenSetFgColor(COLOR_WHITE);
	printf("\n");
	
	printf("Insert card now, select \"No\" when asked to reboot\n");

	printf("Once card is inserted press X to continue\n");
	wait_keys(SCE_CTRL_CROSS);

	printf("Mounting card\n");
	ret = vshIoMount(MOUNT_ID, NULL, 0, 0, 0, 0);
	if (ret != 0) {
		psvDebugScreenSetFgColor(COLOR_RED);
		printf("Unable to mount card err=0x%08X\n", ret);
		psvDebugScreenSetFgColor(COLOR_WHITE);
		printf("Press X to exit\n");
		wait_keys(SCE_CTRL_CROSS);
		return 0;
	}

	printf("Removing id.dat\n");
	fd = sceIoOpen("xmc0:id.dat", SCE_O_RDONLY, 0);
	if (fd >= 0) {
		sceIoClose(fd);
	
		ret = sceIoRemove("xmc0:id.dat");
		if (ret != 0) {
			psvDebugScreenSetFgColor(COLOR_RED);
			psvDebugScreenPrintf("Unable to remove id.dat err=0x%08X\n", ret);
			psvDebugScreenSetFgColor(COLOR_WHITE);
		}
	}

	printf("Unmounting card\n");
	ret = vshIoUmount(MOUNT_ID, 0, 0, 0);
	if (ret != 0) {
		psvDebugScreenSetFgColor(COLOR_RED);
		printf("Unable to unmount card err=0x%08X\n", ret);
		psvDebugScreenSetFgColor(COLOR_WHITE);
		printf("Press X to exit\n");
		wait_keys(SCE_CTRL_CROSS);
		return 0;
	}

	psvDebugScreenSetFgColor(COLOR_GREEN);
	printf("Done\n");
	psvDebugScreenSetFgColor(COLOR_WHITE);
	printf("Press X to reboot or O to exit\n");
	if (wait_keys(SCE_CTRL_CROSS | SCE_CTRL_CIRCLE) == SCE_CTRL_CROSS) {
		scePowerRequestColdReset();
	}

	return 0;
}

unsigned int wait_keys(unsigned int key_mask)
{
	SceCtrlData pad;
	unsigned int k;

	do {
		sceCtrlReadBufferPositive(0, &pad, 1);
		sceKernelDelayThreadCB(200 * 1000);
	} while (pad.buttons != 0);

	while (1) {
		sceCtrlReadBufferPositive(0, &pad, 1);
		k = pad.buttons & key_mask;

		if (k != 0) {
			return k;
		}
		sceKernelDelayThreadCB(200 * 1000);
	}
}

