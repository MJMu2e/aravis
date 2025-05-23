#include <arvv4l2miscprivate.h>
#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/sysmacros.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/media.h>
#include <linux/videodev2.h>

typedef struct {
        guint32 v4l2;
        guint32 genicam;
} ArvV4l2GenicamPixelFormat;

static ArvV4l2GenicamPixelFormat pixel_format_map[] = {
        {V4L2_PIX_FMT_YUYV,             ARV_PIXEL_FORMAT_YUV_422_YUYV_PACKED},
/* Disable these formats for now, makes gstreamer crash:
        {V4L2_PIX_FMT_RGB24,            ARV_PIXEL_FORMAT_RGB_8_PACKED},
        {V4L2_PIX_FMT_BGR24,            ARV_PIXEL_FORMAT_BGR_8_PACKED},
*/
};

ArvPixelFormat
arv_pixel_format_from_v4l2 (guint32 v4l2_pixel_format)
{
        unsigned int i;

        for (i = 0; i < G_N_ELEMENTS(pixel_format_map); i++) {
                if (v4l2_pixel_format == pixel_format_map[i].v4l2)
                        return pixel_format_map[i].genicam;
        }

        return 0;
}

guint32
arv_pixel_format_to_v4l2 (ArvPixelFormat pixel_format)
{
        unsigned int i;

        for (i = 0; i < G_N_ELEMENTS (pixel_format_map); i++) {
                if (pixel_format_map[i].genicam == pixel_format) {
                        return pixel_format_map[i].v4l2;
                }
        }

        return 0;
}

int
arv_v4l2_ioctl (int fd, int request, void *arg)
{
        int result;

        g_return_val_if_fail (fd != -1, -1);

        do {
                result = ioctl (fd, request, arg);
        } while (-1 == result && EINTR == errno);

        return result;
}

int
arv_v4l2_get_media_fd (int fd, const char *bus_info)
{
	DIR *dp;
	struct dirent *ep;
	int media_fd = -1;
	struct stat sb = {0};
	dev_t dev;
        char *media_path;

	if (fstat(fd, &sb) == -1)
                return -1;

	dev = sb.st_rdev;

        media_path = g_strdup_printf ("/sys/dev/char/%d:%d/device", major(dev), minor(dev));
        if (media_path == NULL)
                return -1;

	dp = opendir(media_path);
        g_clear_pointer (&media_path, g_free);

        if (dp == NULL)
                return -1;

	while ((ep = readdir(dp))) {
		if (!memcmp(ep->d_name, "media", 5) && isdigit(ep->d_name[5])) {
			struct media_device_info mdinfo = {0};
                        char *devname;

                        devname = g_strdup_printf ("/dev/%s", ep->d_name);
			media_fd = open(devname, O_RDWR);
                        g_free (devname);

			if (bus_info &&
			    (ioctl(media_fd, MEDIA_IOC_DEVICE_INFO, &mdinfo) ||
			     g_strcmp0 (mdinfo.bus_info, bus_info))) {
				close(media_fd);
				continue;
			}
			break;
		}
	}
	closedir(dp);

	return media_fd;
}

gboolean
arv_v4l2_set_ctrl (int fd, int ctrl_id, gint32 value)
{
        struct v4l2_control control = {0};

        control.id = ctrl_id;
        control.value = value;

        return ioctl (fd, VIDIOC_S_CTRL, &control) == 0;
}

gint32
arv_v4l2_get_ctrl (int fd, int ctrl_id)
{
        struct v4l2_control control = {0};

        control.id = ctrl_id;

        if (ioctl (fd, VIDIOC_G_CTRL, &control) == 0)
                return control.value;

        return 0;
}

gint64
arv_v4l2_get_int64_ext_ctrl (int fd, int ext_ctrl_class, int ext_ctrl_id)
{
        struct v4l2_ext_controls ext_controls = {0};
        struct v4l2_ext_control ext_control = {0};

        ext_controls.ctrl_class = ext_ctrl_class;
        ext_controls.which = V4L2_CTRL_WHICH_CUR_VAL;
        ext_controls.count = 1;
        ext_controls.controls = &ext_control;
        ext_control.id = ext_ctrl_id;

        if (ioctl (fd, VIDIOC_G_EXT_CTRLS, &ext_controls) == -1)
               return 0;

        return ext_control.value64;
}

gboolean
arv_v4l2_set_int64_ext_ctrl (int fd, int ext_ctrl_class, int ext_ctrl_id, gint64 value)
{
        struct v4l2_ext_controls ext_controls = {0};
        struct v4l2_ext_control ext_control = {0};

        ext_controls.ctrl_class = ext_ctrl_class;
        ext_controls.which = V4L2_CTRL_WHICH_CUR_VAL;
        ext_controls.count = 1;
        ext_controls.controls = &ext_control;
        ext_control.id = ext_ctrl_id;
        ext_control.value64 = value;

        if (ioctl (fd, VIDIOC_S_EXT_CTRLS, &ext_controls) == -1)
               return FALSE;

        return TRUE;
}

gint32
arv_v4l2_get_int32_ext_ctrl (int fd, int ext_ctrl_class, int ext_ctrl_id)
{
        struct v4l2_ext_controls ext_controls = {0};
        struct v4l2_ext_control ext_control = {0};

        ext_controls.ctrl_class = ext_ctrl_class;
        ext_controls.which = V4L2_CTRL_WHICH_CUR_VAL;
        ext_controls.count = 1;
        ext_controls.controls = &ext_control;
        ext_control.id = ext_ctrl_id;

        if (ioctl (fd, VIDIOC_G_EXT_CTRLS, &ext_controls) == -1)
               return 0;

        return ext_control.value;
}

gboolean
arv_v4l2_set_int32_ext_ctrl (int fd, int ext_ctrl_class, int ext_ctrl_id, gint32 value)
{
        struct v4l2_ext_controls ext_controls = {0};
        struct v4l2_ext_control ext_control = {0};

        ext_controls.ctrl_class = ext_ctrl_class;
        ext_controls.which = V4L2_CTRL_WHICH_CUR_VAL;
        ext_controls.count = 1;
        ext_controls.controls = &ext_control;
        ext_control.id = ext_ctrl_id;
        ext_control.value = value;

        if (ioctl (fd, VIDIOC_S_EXT_CTRLS, &ext_controls) == -1)
               return FALSE;

        return TRUE;
}
