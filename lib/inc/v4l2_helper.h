/*
 * opencv_v4l2 - v4l2_helper.h file
 *
 * Copyright (c) 2017-2018, e-con Systems India Pvt. Ltd.  All rights reserved.
 *
 */
// Header file for v4l2_helper functions.

#ifndef V4L2_HELPER_H
#define V4L2_HELPER_H

#define GET 1
#define SET 2
#include <linux/videodev2.h>
#include <assert.h>
#include <map>

#define ERR -128

#ifdef __cplusplus
extern "C" {
#endif

enum io_method {
	IO_METHOD_READ = 1,
	IO_METHOD_MMAP,
	IO_METHOD_USERPTR
};

/**
 * FIXME: All the state for the library is maintained via global variables.
 * So, it's not possible to use this library to access multiple devices
 * simultaneously. This is done to simplify the process of assessment.
 *
 * Hint: To access multiple devices at the same time using this application,
 * the public helper functions can be made to accept a new structure, that holds
 * the global state, as parameter.
 */

/*
 * All functions return 0 on success and ERR ( a negative value) in case of failure.
 */
class v4l2_helper {
public:

    int helper_init_cam(const char *devname, unsigned int width, unsigned int height, unsigned int format,
                        enum io_method io_meth);

    int helper_get_cam_frame(unsigned char **pointer_to_cam_data, int *size);

    int helper_release_cam_frame();

    int helper_deinit_cam();

    int helper_get_control(const char *control_name);

    bool helper_set_control(const char *name, const int value);


private:
    int helper_enumerate_controls();

    int helper_enumerate_control_menu();

    int helper_query_ioctl(int current_ctrl, struct v4l2_queryctrl *ctrl);

    int open_device(const char *dev_name);

    int xioctl(int fh, unsigned long request, void *arg);

    int set_io_method(enum io_method io_meth);

    int stop_capturing(void);

    int start_capturing(void);

    int uninit_device(void);

    int init_read(unsigned int buffer_size);

    int init_mmap(void);

    int init_userp(unsigned int buffer_size);

    int init_device(unsigned int width, unsigned int height, unsigned int format);

    int close_device(void);


//int helper_change_cam_res(unsigned int width, unsigned int height, unsigned int format, enum io_method io_meth);

//int helper_ctrl(unsigned int, int,int*);

//int helper_queryctrl(unsigned int,struct v4l2_queryctrl* );

private:
    struct buffer {
        void   *start;
        size_t  length;
    };


    enum io_method   io = IO_METHOD_MMAP;
    int              fd = -1;
    struct buffer          *buffers;
    unsigned int     n_buffers;
    struct v4l2_buffer frame_buf;
    struct v4l2_queryctrl queryctrl;
    struct v4l2_querymenu querymenu;
    std::map<std::string, v4l2_queryctrl> valid_control_list;
    char is_initialised = 0, is_released = 1;
};
#ifdef __cplusplus
}
#endif

#endif
