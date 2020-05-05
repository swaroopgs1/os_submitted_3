#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/ktime.h>
#include <linux/limits.h>
#include <linux/sched.h>
#include <linux/mm_types.h>
#include <linux/time.h>
#include <linux/ioctl.h>
#include <linux/fs.h>    
#include <linux/uaccess.h> 
#include <linux/init.h>
#include <linux/device.h>
#include <linux/sched/signal.h>
#include <linux/slab.h>
#include <linux/miscdevice.h>

#define RD_VALUE _IOR('a','b',char*)

static int pid=10;
static int buffer_length = 0;
static char buffer[50000] = {0};
module_param(pid,int,0660);

#define DEVICE_NAME "k_probe"


static long etx_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
         switch(cmd) {
  		 case RD_VALUE:
                        copy_to_user((char*)arg, buffer, buffer_length);
			break;

        }
        return 0;
}

static int etx_open(struct inode *inode, struct file *file)
{
        return 0;
}

static int etx_release(struct inode *inode, struct file *file)
{
        return 0;
}

static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        printk(KERN_INFO "Read Function\n");
        return 0;
}


static struct file_operations fops =
{
 //.owner = DEVICE_NAME,
 .read = etx_read,
 .open = etx_open,
 .unlocked_ioctl = etx_ioctl,
 .release = etx_release,
};

static struct miscdevice dev_driver =
{
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &fops,
};

static int entry_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
	struct timespec current_time;
	getnstimeofday(&current_time);
	unsigned long vmarea_address = regs->si;
	if(current->pid == pid) {
		//printk(KERN_ALERT "PID: %d -> page fault at virtual address :%lu\n", current->pid, vmarea_address);
		printk(KERN_ALERT "PID:%d " "%lu : %ld\n", current->pid, vmarea_address, current_time.tv_nsec);
		if(buffer_length < 45000) {
			sprintf(buffer + strlen(buffer), "%ld %lu\n", current_time.tv_nsec, vmarea_address);
			buffer_length = strlen(buffer);
		}
	}
	
	return 0;
}

static int ret_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
			return 0;
}

static struct kretprobe my_kretprobe = {
	.handler		= ret_handler,
	.entry_handler		= entry_handler,
	/* Probe up to 2 * NR_CPUs instances concurrently. */
	.maxactive		= 20,
};

static int __init kretprobe_init(void)
{
	int ret;
		
	my_kretprobe.kp.symbol_name = "handle_mm_fault";
	ret = register_kretprobe(&my_kretprobe);
	if (ret < 0) {
		pr_err("register_kretprobe failed, returned %d\n", ret);
		return -1;
	}
	pr_info("Planted return probe at %s: %p\n",
			my_kretprobe.kp.symbol_name, my_kretprobe.kp.addr);
	misc_register(&dev_driver);
	return 0;
}

static void __exit kretprobe_exit(void)
{
	unregister_kretprobe(&my_kretprobe);
	pr_info("kretprobe at %p unregistered\n", my_kretprobe.kp.addr);

	/* nmissed > 0 suggests that maxactive was set too low. */
	pr_info("Missed probing %d instances of %s\n",
		my_kretprobe.nmissed, my_kretprobe.kp.symbol_name);
	misc_deregister(&dev_driver);
}


module_init(kretprobe_init)
module_exit(kretprobe_exit)
MODULE_LICENSE("GPL");
