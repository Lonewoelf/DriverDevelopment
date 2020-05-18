#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>

MODULE_LICENSE("Dual BSD/GPL");

#define DRIVER_NAME "ARW_driver"
#define NUM_DEVICES 2

static const unsigned int minorBase = 0;
static const unsigned int minorNum = 2;
static unsigned int major;
static struct cdev ARW_cdev;
struct file_operations fops;
static struct class *ARW_class = NULL;

int test = 1, test2 = 4;

module_param(test, int, 0);
module_param_named(testCase, test2, int, 0);

static irqreturn_t ARW_irq(int irq, void *dev_id)
{
    printk(KERN_ALERT "irq %d\n", irq);

    return IRQ_RETVAL(1);
}

static int ARW_init(void)
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

    major = MAJOR(dev);

    dev = MKDEV(major, minorBase);

    printk(KERN_ALERT "Major %d Minor %d,%d", major, minorBase, minorNum);

    cdev_init(&ARW_cdev, &fops);
    ARW_cdev.owner = THIS_MODULE;

    cdev_error = cdev_add(&ARW_cdev, dev, minorNum);
    if (cdev_error != 0)
    {
        printk(KERN_ERR "cdev_add = %d\n", alloc_val);
        unregister_chrdev_region(dev, minorNum);
        return -1;
    }

    ARW_class = class_create(THIS_MODULE, "myReadWrite");
    if (IS_ERR(ARW_class))
    {
        printk(KERN_ERR "class_create\n");
        cdev_del(&ARW_cdev);
        unregister_chrdev_region(dev, minorNum);
        return -1;
    }

    device_create(ARW_class, NULL, MKDEV(major, minorBase), NULL, "ARW%d", minorBase);

    request_irq(3, ARW_irq, IRQF_IRQPOLL, "ARW_driver", NULL);

    return 0;
}
static void ARW_exit(void)
{
    printk(KERN_ALERT "BYE 3 D:\n");
    dev_t dev = MKDEV(major, minorBase);
    int minor = 0;

    for (minor = minorBase; minor < minorBase + minorNum; minor++)
    {
        device_destroy(ARW_class, MKDEV(major, minor));
    }

    free_irq(3, NULL);
    class_destroy(ARW_class);
    cdev_del(&ARW_cdev);
    unregister_chrdev_region(dev, minorNum);
}

module_init(ARW_init);
module_exit(ARW_exit);

int ARW_open(struct inode *inode, struct file *file)
{
    printk(KERN_ALERT "ARW_open()\n");
    return 0;
}

int ARW_release(struct inode *inode, struct file *file)
{
    printk(KERN_ALERT "ARW_release()\n");
    return 0;
}

ssize_t ARW_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos)
{
    printk(KERN_ALERT "ARW_read() read: %d\n", (int)lbuf);
    return 0;
}

ssize_t ARW_write(struct file *file, const char __user *buf, size_t lbuf, loff_t *ppos)
{
    printk(KERN_ALERT "ARW_write() wrote: %d\n", (int)lbuf);
    return 23;
}

struct file_operations fops = {
    .read = ARW_read,
    .write = ARW_write,
    .open = ARW_open,
    .release = ARW_release,};

