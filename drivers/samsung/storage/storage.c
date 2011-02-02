#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>

#include <asm/uaccess.h>
#include <../drivers/fsr/Inc/FSR.h>
#include <../drivers/fsr/Inc/FSR_BML.h>
#include <../drivers/fsr/Inc/FSR_LLD_4K_OneNAND.h>

#define STORAGE_NAME	"storage"
#define STORAGE_MINOR	135

#define STORAGE_IOCTL_CMD   0x98
#define STORAGE_READ_DGS    _IOR(STORAGE_IOCTL_CMD, 0x1, char)

extern unsigned int HWREV;

int storage_open(struct inode *inode, struct file *filp)
{
	int num = MINOR(inode->i_rdev);

	printk("Storage Device open Minor : %d\n", num);

	return 0;
}

int storage_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	int dgs_block_offset = 2047;

#ifdef CONFIG_TARGET_LOCALE_KOR
	if (HWREV >= 12) /* uses 8G OneNAND*/
		dgs_block_offset = 4095;
#endif
	printk("Storage Device ioctl -> cmd : %08X, arg : %08X \n", cmd, arg);
	switch (cmd) {
		case STORAGE_READ_DGS:
			{
				int	nRet;
				unsigned char aDGSBuf[4096]; /* DGS Info Cache */

				if (FSR_BML_Open(0, FSR_BML_FLAG_NONE) != FSR_BML_SUCCESS)
				{
					printk("BML_Open Error !!\n");
					while(1);
				}

				/* Acquire SM */
				FSR_BML_AcquireSM(0);

				/* 
				 *    Read DGS Page 
				 *    Read 5th Page of 2047 Block
			     */
#ifdef CONFIG_USE_KMALLOC
				/*
				 *   Modified for FSR Read DMA
				 *   Use kmalloc buffer to read data by using FSR_BML_Read
				 */
				unsigned char *temp = kmalloc(4096, GFP_KERNEL);
				nRet = FSR_OND_4K_Read(0, dgs_block_offset, 5, temp, NULL, FSR_LLD_FLAG_ECC_OFF); 
				memcpy(aDGSBuf, temp, sizeof(aDGSBuf));
				kfree(temp);
#else
				nRet = FSR_OND_4K_Read(0, dgs_block_offset, 5, aDGSBuf, NULL, FSR_LLD_FLAG_ECC_OFF); 
#endif

				/* Release SM*/
				FSR_BML_ReleaseSM(0);
				if(nRet != FSR_LLD_SUCCESS)
				{
					printk("Reading DGS information failed !!\n");
					while(1);
				}

				return copy_to_user((char *) arg, (char *) aDGSBuf, sizeof (aDGSBuf));
		}

		default:
			break;
	}

	return 0x53;
}

static struct file_operations storage_fops = 
{
	.owner = THIS_MODULE,
	.open = storage_open,
	.ioctl = storage_ioctl,
};

static struct miscdevice storage_device = {
	.minor  = STORAGE_MINOR,
	.name   = STORAGE_NAME,
	.fops   = &storage_fops,
};

/* init & cleanup. */
static int __init storage_init(void)
{
	int result;

	printk("Storage device init\n");

    result = misc_register(&storage_device);
    if (result <0) 
		return result;

	return 0;
}

static void __exit storage_exit(void)
{
	printk("Storage device exit\n");
	misc_deregister(&storage_device);
}

module_init(storage_init);
module_exit(storage_exit);

MODULE_AUTHOR("SAMSUNG ELECTRONICS CO., LTD");
MODULE_DESCRIPTION("Storage API Driver.");
MODULE_LICENSE("Samsung Proprietary");
