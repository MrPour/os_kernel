#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("Dual BSD/GPL");

int ASMD_Operation(int *result, int num1, int num2, char *op)
{
    if (op)
    {
        if (*op == '+')
        {
            *result = num1 + num2;
        }
        else if (*op == '-')
        {
            *result = num1 - num2;
        }
        else if (*op == '*')
        {
            *result = num1 * num2;
        }
        else if (*op == '/')
        {
            if (num2 != 0)
                *result = num1 / num2;
            else
                printk("num2 can't be zero!\n");
        }
        else
            printk("no such operation\n");
    }
    else
    {
        printk("op is empty.\n");
    }
    return 0;
}

int (*fptr_Operation)(int *, int, int, char *);

static int os4_init(void)
{
    // static int (*fptr_ASMD_Operation)(int*,int,int,char*);
    printk(KERN_ALERT "os3 init...\n");
    fptr_Operation = &ASMD_Operation;
    return 0;
}
static void os4_exit(void)
{
    printk(KERN_ALERT "os3 exit...\n");
}
EXPORT_SYMBOL(fptr_Operation);

module_init(os4_init);
module_exit(os4_exit);