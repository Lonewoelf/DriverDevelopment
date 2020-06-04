#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <asm/io.h>

#define GPIO1_ADDR 0x4804c000
#define GPIO_OE 0x134
#define GPIO_DATAIN 0x138
#define GPIO_CLEARDATAOUT 0x190
#define GPIO_SETDATAOUT 0x194
#define GPIO_MAX 0x198
/* define addresses. Is more convenient in underlaying code */
#define GPIO1_OE (uint32_t*)(GPIO1_ADDR+GPIO_OE)
#define GPIO1_DATAIN (uint32_t*)(GPIO1_ADDR+GPIO_DATAIN)
#define GPIO1_CLEARDATAOUT (uint32_t*)(GPIO1_ADDR+GPIO_SETDATAOUT)
#define GPIO1_SETDATAOUT (uint32_t*)(GPIO1_ADDR+GPIO_SETDATAOUT)
#define PIN 19

//request_region(GPIO1_ADDR, GPIO_MAX, "myGPIO_driver");
uint32_t* gpio1 = ioremap( GPIO1_ADDR, GPIO_MAX );
barrier();
uint32_t oe = ioread32( GPIO1_OE );
rmb();
iowrite32( (oe & (~(1<<PIN))), GPIO1_OE );
wmb(); // write memory barrier
iounmap(gpio1);
/* ledje aan en uit zetten */
uint32_t* gpio1 = ioremap(GPIO1_ADDR, GPIO_MAX);
28
barrier();
iowrite32( (1<<PIN), GPIO1_SETDATAOUT ); // Pin 19 aan
iowrite32( (1<<PIN), GPIO1_CLEARDATAOUT ); // Pin 19 uit
wmb(); // write memory barrier
iounmap(gpio1);
//release_region(GPIO1_ADDR, GPIO_MAX);

