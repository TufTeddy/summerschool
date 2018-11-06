#include <linux/module.h> 
#include <linux/printk.h> 
#include <linux/kobject.h> 
#include <linux/sysfs.h> 
#include <linux/init.h> 
#include <linux/fs.h> 
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>

static struct kobject *example_kobject;
static int foo;

static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf)
{
	return sprintf(buf, "%d\n", foo);
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count)
{
	sscanf(buf, "%du", &foo);
	return count;
}


static struct kobj_attribute foo_attribute =
__ATTR(foo, 0660, foo_show, foo_store);

static int __init mymodule_init (void)
{
        int error = 0;
        pr_info("Module initialized successfully \n");
        example_kobject = kobject_create_and_add("kobject_example",
                                                 kernel_kobj);
        if(!example_kobject)
                return -ENOMEM;

        error = sysfs_create_file(example_kobject, &foo_attribute.attr);
        if (error) {
                pr_info("failed to create the foo file in "
						"/sys/kernel/kobject_example \n");
        }

        return error;
}

static void __exit mymodule_exit (void)
{
        pr_info ("Module un initialized successfully \n");
        kobject_put(example_kobject);
}


module_init(mymodule_init);
module_exit(mymodule_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple sysfs Linux module.");
MODULE_VERSION("0.1");
