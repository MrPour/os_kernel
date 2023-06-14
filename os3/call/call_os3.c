#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("Dual BSD/GPL");

extern int (*fptr_Operation)(int *, int, int, char *);

static int call_os3_init(void)
{
    int result;
    char op = '+';
    printk("call_os3 init...\n");

    printk("add call");
    fptr_Operation(&result, 10, 10, &op);

    op = '-';
    printk("sub call");
    fptr_Operation(&result, 20, 10, &op);

    op = '*';
    printk("mul call");
    fptr_Operation(&result, 10, 10, &op);

    op = '/';
    printk("div call");
    fptr_Operation(&result, 20, 10, &op);
}

static void call_os3_exit(void)
{
    printk("call_os3_exit...\n");
}

module_init(call_os3_init);
module_exit(call_os3_exit);