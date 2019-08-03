/*
 * Copyright (C) 2019 GreenWaves Technologies
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 *
 * Authors: Germain Haugou, ETH (germain.haugou@iis.ee.ethz.ch)
 */

#include "stdio.h"
#include "bsp/bsp.h"
#include "bsp/camera.h"
#include "bsp/camera/himax.h"
#ifdef __FREERTOS__
#include "rtos/pmsis_os.h"
#include "rtos/os_frontend_api/pmsis_task.h"
#include "rtos/os_frontend_api/pmsis_time.h"
#include "rtos/pmsis_driver_core_api/pmsis_driver_core_api.h"
#include "rtos/malloc/pmsis_l2_malloc.h"
#endif


#if defined(CONFIG_HIMAX)
#define WIDTH    324
#define HEIGHT   244
#else
#define WIDTH    320
#define HEIGHT   240
#endif

static inline int get_nb_buffers()
{
#ifdef __PULP_OS__
  if (rt_platform() == ARCHI_PLATFORM_RTL)
    return 4;
  else
#endif
    return 4;
}

#define BUFF_SIZE (WIDTH*HEIGHT)

PI_L2 unsigned char *buff[16];

static int test_entry()
{
  printf("Entering main controller\n");

  struct pi_device device;

#if defined(CONFIG_MT9V034)
  printf("Opening mt9v034 camera\n");
  struct mt9v034_conf cam_conf;
  mt9v034_conf_init(&cam_conf);
#else
  printf("Opening Himax camera\n");
  struct himax_conf cam_conf;
  himax_conf_init(&cam_conf);
#endif

  cam_conf.format = CAMERA_QVGA;

  pi_open_from_conf(&device, &cam_conf);
  if (camera_open(&device))
    return -1;

  for (int i=0; i<get_nb_buffers(); i++)
  {
    buff[i] = pmsis_l2_malloc(WIDTH*HEIGHT);
    if (buff[i] == NULL)
    {
      printf("Unable to allocate buffer\n");
      goto error;
    }
  }

  for (int i=0; i<get_nb_buffers(); i++)
  {
    pi_task_t task;
    #if 0
    camera_capture_async(&device, buff[i], WIDTH*HEIGHT, pi_task(&task));
    camera_control(&device, CAMERA_CMD_START, 0);

    pi_task_wait_on(&task);
    camera_control(&device, CAMERA_CMD_STOP, 0);

    // Now wait some time to start capturing in the middle of the next frame
    pi_time_wait_us((i + 1) * 5000);
    #else
    #if defined(ASYNC)
    camera_control(&device, CAMERA_CMD_STOP, 0);
    camera_capture_async(&device, buff[i], WIDTH*HEIGHT, pi_task(&task));
    camera_control(&device, CAMERA_CMD_START, 0);
    pi_task_wait_on(&task);
    #else
    camera_control(&device, CAMERA_CMD_START, 0);
    camera_capture(&device, buff[i], WIDTH*HEIGHT);
    camera_control(&device, CAMERA_CMD_STOP, 0);
    //for (volatile int i=0; i<44700; i++);
    #endif
    #endif
  }
  camera_close(&device);


  for (int j=0; j<get_nb_buffers(); j++)
  {
    for (int i=0; i<BUFF_SIZE; i++)
    {
      unsigned char expected;
      expected = i & 0x7f;
      if (expected != buff[j][i])
      {
        printf("Error, buffer: %d, index: %d, expected: 0x%x, read: 0x%x\n", j, i, expected, buff[j][i]);
        goto error;
      }
    }
  }

  printf("Test success\n");
  return 0;

error:
  printf("Test failure\n");
  return -1;
}

static void test_kickoff(void *arg)
{
  int ret = test_entry();
  pmsis_exit(ret);
}

int main()
{
    #if defined(GAPUINO)
    printf("\n\t*** PMSIS ili9341 with himax test on gapuino ***\n\n");
    #else
    #if defined(GAPOC_A)
    printf("\n\t*** PMSIS ili9341 with mt9v034 test on gapoc_a ***\n\n");
    #endif
    #endif
  return pmsis_kickoff((void *)test_kickoff);
}
