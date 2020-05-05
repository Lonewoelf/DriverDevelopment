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