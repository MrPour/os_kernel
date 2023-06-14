#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include <linux/slab.h>
// #include<moduleparam.h>


MODULE_LICENSE("GPL");

struct listnode{
   int val_;
	struct listnode* next_;
};

int insert(struct listnode *head,int val)
{
  //创建结点
    struct listnode* addnode = (struct listnode*)kmalloc(sizeof(struct listnode),GFP_KERNEL);
    struct listnode *p = head;
    if (addnode != NULL)
    {
        //节点赋值
        addnode->val_ = val;
        addnode->next_ = NULL;
        while(p->next_ != NULL)
        {
           p = p->next_;
        }
        p->next_ = addnode;
        return 1;
    }
    return -1;   
}

int delete(struct listnode *head, int val)
{
    struct listnode *p = head;
    if(head->val_ == val)
    {
        head = head->next_;
        kfree(p);
        return 1;
    }
    while(p->next_ != NULL)
    {
        if(p->next_->val_ == val)
        {
            printk(KERN_ALERT "删除成功,data=%d",p->next_->val_);
            p->next_ = p->next_->next_;
            kfree(p->next_);
            return 1;
        }
        p = p->next_;
    }
    return -1;
}

int lookup(struct listnode *head, int val)
{
    struct listnode *p = head;
   
    while(p->val_ != val)
    {
        if(p->next_ == NULL)
        {
            printk(KERN_ALERT "查找失败,data=%d",val);
            return -1;
        }
        if(p->next_->val_ == val)
        {
            printk(KERN_ALERT "查找成功,data=%d",p->next_->val_);
            return 1;
        }
        else
        {
             p = p->next_;
        }
    }
    printk(KERN_ALERT "查找成功,data=%d",p->next_->val_);
    return 1;
}

//输出单链表
void print(struct listnode *head)
{
   struct listnode *p = head->next_;
	while (p)
	{
		printk(KERN_ALERT "遍历结点：%d,", p->val_);
		p = p->next_;
	}
}

static int listnode_init(void)     
{
   struct listnode *head,*p,*s;
   int i;
   head = (struct listnode*)kmalloc(sizeof(struct listnode),GFP_KERNEL);
   head->val_=0;           //表头结点内容为0
   head->next_=NULL;        //表头指针置空
   
   p=head;                 //指针p指向head指针
   
   for(i=0;i<3;i++)       //循环语句判断mark为1时表示用户需要申请空间
   {
       s = (struct listnode*)kmalloc(sizeof(struct listnode),GFP_KERNEL);
       s->val_=i;        //为结点赋值
       s->next_=NULL;       //结点s的指针置空
       printk(KERN_ALERT "初始化：%d",s->val_);

       p->next_=s;          //将s与前一个指针相连
       p=s;                //p指向该结点
      
   }
   insert(head,88);
   print(head);
   lookup(head,88);
   delete(head,88);
   print(head);
   return 1;            //成功初始化
}

static void hello_exit(void)
{
   printk(KERN_ALERT "bye listnode");
}

//加载or卸载模块
module_init(listnode_init);
module_exit(hello_exit);
// 可选 
MODULE_AUTHOR("zhou-silence");
MODULE_DESCRIPTION("This is a simple example!/n");
MODULE_VERSION("v1.0");
MODULE_ALIAS("A simplest example");
