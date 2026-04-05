#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/string.h> 

#define PROC_NAME "hen_status"

static ssize_t hen_read(struct file *file, char __user *ubuf, size_t count, loff_t *ppos) 
{
    char buf[] = "HEN Mode: Active (Exynos 9810)\n";
    int len = strlen(buf);
    if (*ppos > 0 || count < len) return 0;
    if (copy_to_user(ubuf, buf, len)) return -EFAULT;
    *ppos = len;
    return len;
}

static const struct file_operations hen_fops = {
    .owner = THIS_MODULE,
    .read = hen_read,
};

static int __init hen_init(void) {
    proc_create(PROC_NAME, 0, NULL, &hen_fops);
    printk(KERN_INFO "--- homebrew Interface: /proc/%s\n", PROC_NAME);
    printk(KERN_INFO "--- h3nm firmware init\n");
    printk(KERN_INFO "--- made by dave lone\n");
    printk(KERN_INFO "--- Target: S9+\n");
    return 0;
}

pure_initcall(hen_init);
