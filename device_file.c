#include "device_file.h"
#include <linux/fs.h> 	     /* file stuff */
#include <linux/kernel.h>    /* printk() */
#include <linux/errno.h>     /* error codes */
#include <linux/module.h>  /* THIS_MODULE */
#include <linux/cdev.h>      /* char device stuff */
#include <asm/uaccess.h>  /* copy_to_user() */
#include <linux/version.h>
#include "query_ioctl.h"

static const char    g_s_Hello_World_string[] = "Hello world from kernel mode!\n\0";
static const ssize_t g_s_Hello_World_size = sizeof(g_s_Hello_World_string);
static int status = 1, dignity = 3, ego = 5;
/*===============================================================================================*/
static ssize_t device_file_read(
                           struct file *file_ptr
                        , char __user *user_buffer
                        , size_t count
                        , loff_t *possition)
{
   printk( KERN_NOTICE "Simple-driver: Device file is read at offset = %i, read bytes count = %u"
            , (int)*possition
            , (unsigned int)count );

   if( *possition >= g_s_Hello_World_size )
      return 0;

   if( *possition + count > g_s_Hello_World_size )
      count = g_s_Hello_World_size - *possition;

   if( copy_to_user(user_buffer, g_s_Hello_World_string + *possition, count) != 0 )
      return -EFAULT;   

   *possition += count;
   return count;
}

static  int  device_file_ioctl(struct inode * i,struct file* f, unsigned int cmd, unsigned long arg)
{
    query_arg_t q;

    switch (cmd)
    {
    case QUERY_GET_VARIABLES:
        q.status = status;
        q.dignity = dignity;
        q.ego = ego;
        printk(KERN_NOTICE "Simple-driver: useraddr %pK ; kerneladdr: %pK", arg, &q);
        if (copy_to_user((query_arg_t *)arg, &q, sizeof(query_arg_t)))
        {
            return -EACCES;
        }
        break;
    case QUERY_CLR_VARIABLES:
        status = 0;
        dignity = 0;
        ego = 0;
        break;
    case QUERY_SET_VARIABLES:
        if (copy_from_user(&q, (query_arg_t *)arg, sizeof(query_arg_t)))
        {
            return -EACCES;
        }
        status = q.status;
        dignity = q.dignity;
        ego = q.ego;
        break;
    default:
        return -EINVAL;
    }

    return 0;
}
/*===============================================================================================*/
static struct file_operations simple_driver_fops = 
{
   .owner   = THIS_MODULE,
   .read    = device_file_read,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
   .ioctl = device_file_ioctl
#else
   .unlocked_ioctl = device_file_ioctl
#endif
};

static int device_file_major_number = 0;

static const char device_name[] = "Simple-driver";

/*===============================================================================================*/
int register_device(void)
{
      int result = 0;

      printk( KERN_NOTICE "Simple-driver: register_device() is called." );

      result = register_chrdev( 0, device_name, &simple_driver_fops );
      if( result < 0 )
      {
         printk( KERN_WARNING "Simple-driver:  can\'t register character device with errorcode = %i", result );
         return result;
      }

      device_file_major_number = result;
      printk( KERN_NOTICE "Simple-driver: registered character device with major number = %i and minor numbers 0...255"
                  , device_file_major_number );

      return 0;
}
/*-----------------------------------------------------------------------------------------------*/
void unregister_device(void)
{
   printk( KERN_NOTICE "Simple-driver: unregister_device() is called" );
   if(device_file_major_number != 0)
   {
      unregister_chrdev(device_file_major_number, device_name);
   }
}
