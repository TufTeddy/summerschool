#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple chrdev Linux module.");
MODULE_VERSION("0.05");

int memory_major = 60;

char *memory_buffer;

static ssize_t mem_read(struct file *f,char *buf,size_t count,loff_t *f_pos)
{
	copy_to_user(buf,memory_buffer,1);
	if(*f_pos == 0) {
		*f_pos += 1;
		return 1;
	} else {
		return 0;
	}
}

static ssize_t mem_write(struct file *f,const char *buf,size_t count,loff_t *f_pos)
{
	char *tmp;
	tmp = buf+count-1;
	copy_from_user(memory_buffer,tmp,1);
	return 1;
}

static struct file_operations fops =
{
	.write = mem_write,
	.read = mem_read,
};

static int __init mymodule_init(void) {
	int result;
	result = register_chrdev(memory_major,"chardev",&fops);
	if(result < 0)
	{
		pr_info("<1> registerchrdev : Cannot obtain major number %d\n",
				memory_major);
		return result;
	}
	pr_info("<1>Chrdev insterted %d\n", result);
	return 0;
}
static void __exit mymodule_exit(void) {
	unregister_chrdev(memory_major,"chardev");
	pr_info ("<1>Removing memory module\n");
}

module_init(mymodule_init);
module_exit(mymodule_exit);
