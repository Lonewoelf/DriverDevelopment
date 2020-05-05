#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");

static int opdrachtTwee_init(void)
{
    printk(KERN_ALERT "HEYYYYYY:D\n");
    return 0;
}
static void opdrachtTwee_exit(void)
{
    printk(KERN_ALERT "BYE D:\n");
}
module_init(opdrachtTwee_init);
module_exit(opdrachtTwee_exit);

//make opgave_2_1.ko obj-m=opgave_2_1.o -C /lib/modules/4.15.0-54-generic/build M=/mnt/hgfs/DRVO/H3/2_1
//modules