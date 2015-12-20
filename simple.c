#include "ioctls.h"
#include "utils.h"
#include <linux/ioctl.h>
typedef struct
{
    int status, dignity, ego;
} query_arg_t;
#define QUERY_GET_VARIABLES _IOR('q', 1, query_arg_t *)
#define QUERY_CLR_VARIABLES _IO('q', 2)
#define QUERY_SET_VARIABLES _IOW('q', 3, query_arg_t *)


static const struct ioctl simple_ioctls[] = {
    IOCTL(QUERY_GET_VARIABLES),
    IOCTL(QUERY_CLR_VARIABLES),
    IOCTL(QUERY_SET_VARIABLES),
};

static const char *const simple_devs[] = {
    "simple-driver",
};

static const struct ioctl_group simple_grp = {
    .devtype = DEV_CHAR,
    .devs = simple_devs,
    .devs_cnt = ARRAY_SIZE(simple_devs),
    .sanitise = pick_random_ioctl,
    .ioctls = simple_ioctls,
    .ioctls_cnt = ARRAY_SIZE(simple_ioctls),
};

REG_IOCTL_GROUP(simple_grp)
