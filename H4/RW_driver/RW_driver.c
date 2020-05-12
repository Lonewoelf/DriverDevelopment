#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE("Dual BSD/GPL");

#define DRIVER_NAME "RW_driver"
#define NUM_DEVICES 2
#define MAYOR_NUM 238
#define BUFFER_SIZE 256

static const unsigned int minorBase = 0;
static const unsigned int minorNum = 2;
static unsigned int major;
static struct cdev RW_driver;
struct file_operations fops;
static struct class *RW_class = NULL;

int count = 0;
static char RW_buffer[BUFFER_SIZE];
static char *msg_Ptr;

static int RW_init(void)
{
    printk(KERN_ALERT "Initialising RW_driver\n");

    int alloc_val = 0;
    int cdev_error = 0;
    dev_t dev;

    msg_Ptr = RW_buffer;

    alloc_val = alloc_chrdev_region(&dev, minorBase, minorNum, DRIVER_NAME);

    if (alloc_val != 0)
    {
        printk(KERN_ERR "alloc_chrdev_region = %d\n", alloc_val);
        return -1;
    }

    major = MAJOR(dev);

    dev = MKDEV(major, minorBase);

    printk(KERN_ALERT "Major %d Minor %d,%d", major, minorBase, minorNum);

    cdev_init(&RW_driver, &fops);
    RW_driver.owner = THIS_MODULE;

    cdev_error = cdev_add(&RW_driver, dev, minorNum);
    if (cdev_error != 0)
    {
        printk(KERN_ERR "cdev_add = %d\n", alloc_val);
        unregister_chrdev_region(dev, minorNum);
        return -1;
    }

    RW_class = class_create(THIS_MODULE, "myReadWrite");
    if (IS_ERR(RW_class))
    {
        printk(KERN_ERR "class_create\n");
        cdev_del(&RW_driver);
        unregister_chrdev_region(dev, minorNum);
        return -1;
    }

    device_create(RW_class, NULL, MKDEV(major, minorBase), NULL, "RW_%d", minorBase);

    return 0;
}
static void RW_exit(void)
{
    printk(KERN_ALERT "Uninitialising RW_driver\n");
    dev_t dev = MKDEV(major, minorBase);
    int minor = 0;

    for (minor = minorBase; minor < minorBase + minorNum; minor++)
    {
        device_destroy(RW_class, MKDEV(major, minor));
    }

    class_destroy(RW_class);
    cdev_del(&RW_driver);
    unregister_chrdev_region(dev, minorNum);
}

module_init(RW_init);
module_exit(RW_exit);

int RW_open(struct inode *inode, struct file *file)
{
    printk(KERN_ALERT "RW_open(). Open Count: %d\n",count);
    count++;
    return 0;
}

int RW_release(struct inode *inode, struct file *file)
{
    printk(KERN_ALERT "RW_release()\n");
    return 0;
}

ssize_t RW_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos)
{  
    int bytes_read = 0;

    if (*msg_Ptr == 0){
        return 0;
    }

    while (lbuf && *msg_Ptr)
    {
        put_user(*(msg_Ptr++), buf++);

        lbuf--;
        bytes_read++;
    }
    printk(KERN_ALERT "RW_read() read: %lu\n", bytes_read);

    return bytes_read;
}

ssize_t RW_write(struct file *file, const char __user *buf, size_t lbuf, loff_t *ppos)
{
    int result;
    unsigned long bytes_written = lbuf;
    if (*ppos + lbuf > BUFFER_SIZE)
    {
        printk(KERN_ALERT "TOO MUCH DATA FOR BUFFER : %lu\n", (*ppos + lbuf));
        return -ENOMEM;
    }

    result = copy_from_user(RW_buffer + *ppos, buf, lbuf);
    *ppos += bytes_written;

    printk(KERN_ALERT "RW_write() written: %lu\n", bytes_written);

    return bytes_written;
}

struct file_operations fops = {
    .read = RW_read,
    .write = RW_write,
    .open = RW_open,
    .release = RW_release,
};
