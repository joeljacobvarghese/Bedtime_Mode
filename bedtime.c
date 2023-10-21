#include <linux/init.h>
#include <linux/module.h>
#include <linux/time.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/timekeeping.h>
#include <linux/string.h>
#include <linux/time64.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/init.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("GROUP 6");






static int check(void);
static int set_brightnessnight(void);
static int set_brightnessday(void);




static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs) {
   
   
    while(1){


    if(!check())  //check kept here to prevent indefinite loop
    {
      return -1;
    }

    struct timespec64 ts;

    ktime_get_real_ts64(&ts);
    
    // Extract the time fields from the timespec64 struct
    int hours = ts.tv_sec / 3600 % 24;
    int minutes = (ts.tv_sec / 60) % 60;
    int seconds = ts.tv_sec % 60;
   
    if(minutes +30 > 60)
    {
    printk(KERN_INFO "Current time: %02d:%02d:%02d\n", (hours +6)%24 , (minutes+30)%60 , seconds);
    hours = (hours +6)%24;
    minutes = (minutes+30)%60;
    }
    
    else
    {    
    printk(KERN_INFO "Current time: %02d:%02d:%02d\n", (hours +5)%24 , (minutes+30)%60, seconds);
    hours = (hours +5)%24 ;
    minutes = (minutes+30)%60;

    }
    
     
      
    printk(KERN_INFO "Current local time: %02d:%02d:%02d\n",
           hours, minutes , seconds);



        if( hours >= 20 || hours  <= 6 )
    {
        set_brightnessnight();
        printk(KERN_INFO "Brightness set to 20000\n");
       
    }  
       else 
    {
       
       set_brightnessday();
       printk(KERN_INFO "Brightness set to 50000\n");
    
    }
    msleep(5000);

    }
   

    
return 0;


}



static int driver_open(struct inode *device_file, struct file *instance) {
	printk("dev_nr - open was called!\n");
	return 0;
}

static int driver_close(struct inode *device_file, struct file *instance) {
	printk("dev_nr - close was called!\n");
	return 0;
}


static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_close,
	.write = driver_write
};



//file to start and stop the driver (to prevent indefinite while loop)
static int check(void)
{
    struct file *filep;
    char buf[10];
    int nbytes;
    int ret;

    filep = filp_open("/home/sniikz/Desktop/driverimplementation/6.changes/target.txt", O_RDONLY, 0);
    if (!filep || IS_ERR(filep)) {
        printk(KERN_ALERT "Failed to open file\n");
        return 0;
    }

    nbytes = kernel_read(filep, buf, sizeof(buf), 0);
    if (nbytes < 0) {
        printk(KERN_ALERT "Failed to read file\n");
        ret = 0;
        goto out_close;
    }

    if (strstr(buf, "start")) {
        printk(KERN_ALERT "check is start\n");
        return 1;
    } else if (strstr(buf, "stop")) {
        printk(KERN_ALERT "check is stop\n");
        return 0;
    } else {
        printk(KERN_ALERT "Unknown command!\n");
        return 0;
    }

    ret = 0;

out_close:
    filp_close(filep, NULL);
    return ret;
   




    
}

static int set_brightnessnight(void)
{
    struct file *f;
    int err = 0;

    // Open the driver file 
    f = filp_open("/sys/class/backlight/intel_backlight/brightness", O_WRONLY|O_CREAT, 0644);
    
    
    if (IS_ERR(f)) {
        err = PTR_ERR(f);
        printk(KERN_ALERT "Error opening file sys/class/backlight/intel_backlight/brightness Error code: %d\n", err);
        return err;
    }

    // Writing the brightness value
    err = kernel_write(f, "20000", 5, 0);

    // Close the file
    filp_close(f, NULL);

    return err;
}

static int set_brightnessday(void)
{
    struct file *f;
    int err = 0;

    // Open the driver file
    f = filp_open("/sys/class/backlight/intel_backlight/brightness", O_WRONLY|O_CREAT, 0644);
    
    
    if (IS_ERR(f)) {
        err = PTR_ERR(f);
        printk(KERN_ALERT "Error opening file sys/class/backlight/intel_backlight/brightness Error code: %d\n", err);
        return err;
    }

    // Writing the brightness value
    err = kernel_write(f, "50000", 5, 0);

    // Close the file
    filp_close(f, NULL);

    return err;
}





static int bedtime_init(void)
{
  
  
  
    int t = register_chrdev(93, "myfirst" , &fops);//register driver with major 90
    if(t <0 )
    {
        printk(KERN_ALERT "Reg failed\n");
        return -1;
    }
    else 
    {
    printk(KERN_ALERT "Reg success\n");
    }
    return 0;

           
}

static void bedtime_exit(void)
{
    unregister_chrdev(93,"myfirst");
    printk(KERN_ALERT "Goodbye, cruelest world\n");

}

module_init(bedtime_init);
module_exit(bedtime_exit);

