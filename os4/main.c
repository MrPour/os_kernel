#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

//major为0，则由内核自动分配主设备号
//主设备号--标识同一类型的设备使用同一驱动程序
#define charDev_major  0
//连续注册的次设备号的个数
//次设备号--使用该驱动程序的设备
#define charDev_num    2
//字符缓存
#define BUFFER_LENGTH  1024

int chardev_major = charDev_major;
int chardev_minor = 0;
int chardev_num = charDev_num;

module_param(chardev_major, int, S_IRUGO);
module_param(chardev_minor, int, S_IRUGO);
module_param(chardev_num, int, S_IRUGO);

//定义字符设备
struct chardev{
    //cdev是linux用来管理字符设备的描述符，1个设备对应1个cdev
    struct cdev cdev;
    //缓存字节数据
    char c[BUFFER_LENGTH];
};
struct chardev *chrdev;

int chrdev_open(struct inode *inode, struct file *filp)
{   //打印设备的次设备号
    printk(KERN_INFO "open chrdev%d %d\n",iminor(inode),MINOR(inode->i_cdev->dev));
    return 0;
}

ssize_t chrdev_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    //内核空间driver到用户空间的复制
    if(!copy_to_user(buf,chrdev->c,count))
        return count;
    printk(KERN_INFO "bytes is read...\n");
    return -1;
}

ssize_t chrdev_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
    //用户空间到内核空间driver的复制
    if(!copy_from_user(chrdev->c,buf,count))
        return count;
    printk(KERN_INFO "bytes is write...\n");
    return -1;
}

int chrdev_release(struct inode *inode, struct file *filp)
{
    printk(KERN_INFO "release chrdev\n");
    return 0;
}

//cdev的成员变量1 -- 32位设备号：主设备号和次设备号两部分
dev_t char_dev;
//cdev的成员变量2 -- 函数集，实现文件操作
struct file_operations chrdev_fops = {
        .owner =   THIS_MODULE,
        .read =    chrdev_read,
        .write =   chrdev_write,
        .open =    chrdev_open,
        .release = chrdev_release,
};

static int __init chardev_init(void)
{
    int ret,i;
    printk(KERN_INFO "---BEGIN CHARDEV LINUX MODULE---\n");

    //注册设备号
    if(chardev_major)
    {
        //手动生成设备号作为起始编号
        char_dev = MKDEV(chardev_major,chardev_minor);
        //为字符驱动注册设备号：需要开发者指定设备的主设备号，设备名称为char_dev
        ret = register_chrdev_region(char_dev,chardev_num,"char_dev");
    }
    else
    {
        //如果major为0，为字符驱动注册设备号：由内核自动分配设备号
        ret = alloc_chrdev_region(&char_dev,chardev_minor,chardev_num,"char_dev");
        //更新分配后的major
        chardev_major = MAJOR(char_dev);
    }
    if(ret < 0)
    {
        printk(KERN_WARNING "chardev: can't get major %d\n",chardev_major);
        goto fail;
    }

    //分配空间，得到设备数组
    chrdev = kzalloc(sizeof(struct chardev)*chardev_num,GFP_KERNEL);
    if(!chrdev)
    {
        printk(KERN_WARNING "alloc mem failed");
        ret = -ENOMEM;
        goto failure_kzalloc;
    }

    for(i = 0;i < chardev_num;i++)
    {
        //为设备cdev绑定函数集
        cdev_init(&chrdev[i].cdev,&chrdev_fops);
        chrdev[i].cdev.owner = THIS_MODULE;
        //为设备cdev绑定设备编号，并加入到cdev_map中，可通过open系统调用找到这个cdev
        ret = cdev_add(&chrdev[i].cdev,MKDEV(chardev_major,chardev_minor+i),1);

        if(ret)
        {
            printk(KERN_WARNING "fail add chrdev%d",i);
        }
    }
    printk(KERN_INFO "--END CHARDEV LINUX MODULE--\n");
    return 0;

    failure_kzalloc:
    unregister_chrdev_region(char_dev,chardev_num);
    fail:
    return ret;
}

static void __exit chardev_exit(void)
{
    int i;
    for(i = 0;i < chardev_num;i++)
    {
        //从cdev_map中删除cdev
        cdev_del(&chrdev[i].cdev);
    }
    kfree(chrdev);
    //注销设备号
    unregister_chrdev_region(char_dev,chardev_num);
    printk(KERN_INFO "GOODBYE！\n");
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("NZY");
MODULE_VERSION("v1.0");
