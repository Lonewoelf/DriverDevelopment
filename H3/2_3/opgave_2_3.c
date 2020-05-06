#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>

MODULE_LICENSE("Dual BSD/GPL");

#define DRIVER_NAME "myReadWrite"
#define NUM_DEVICES 2
#define MAYOR_NUM 238

static const unsigned int minorBase = 0;
static const unsigned int minorNum = 2;
static unsigned int major;
static struct cdev hallo_cdev;
struct file_operations fops;

static int opdrachtDrie_init(void)
{
    printk(KERN_ALERT "HEY 3:D\n");
    

    int alloc_val = 0;
    int cdev_error = 0;
    dev_t dev;

    alloc_val = alloc_chrdev_region(&dev, minorBase, minorNum, DRIVER_NAME);

    if (alloc_val != 0)
    {
        printk(KERN_ERR "alloc_chrdev_region = %d\n", alloc_val);
        return -1;
    }

    major = MAYOR(dev);
    dev = MKDEV(major, minorBase);

    printk(KERN_ALERT "Major %d Minor %d,%d", major, minorBase, minorNum);

    cdev_init(&hallo_cdev, &fops);
    hallo_cdev.owner = THIS_MODULE;

    cdev_error = cdev_add(&hallo_cdev, dev, minorNum);
    if (cdev_error != 0)
    {
        printk(KERN_ERR "cdev_add = %d\n", alloc_val);
        unregister_chrdev_region(dev, minorNum);
        return -1;
    }

     return 0;
}
static void opdrachtDrie_exit(void)
{
    printk(KERN_ALERT "BYE 3 D:\n");
    dev_t dev = MKDEV(major, minorBase);

    cdev_del(&hallo_cdev);
    unregister_chrdev_region(dev, minorNum);
}

module_init(opdrachtDrie_init);
module_exit(opdrachtDrie_exit);

int hello_open(struct inode *inode, struct file *file)
{
    printk(KERN_ALERT "hello_open()\n");
    return 0;
}

int hello_release(struct inode *inode, struct file *file)
{
    printk(KERN_ALERT "hello_release()\n");
    return 0;
}

ssize_t hello_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos)
{
    printk(KERN_ALERT "hello_read() read: %d\n",file->size;
    return 0;
}

ssize_t hello_write(struct file *file, const char __user *buf, size_t lbuf, loff_t *ppos)
{
    printk(KERN_ALERT "hello_write() wrote: %d\n", file->size);
    return 23;
}

struct file_operations fops = {
    .read = hello_read,
    .write = hello_write,
    .open = hello_open,
    .release = hello_release,};

