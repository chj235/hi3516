/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <poll.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>

#define MSG(args...) printf(args)
// ADC_DEV_FILE 为光敏电阻所对应的ADC设备节点
#define ADC_DEV_FILE "/sys/devices/platform/media/120e0000.adc/iio:device0/in_voltage0_raw"

int main(void)
{
    int adc_fd            = -1;
    int ret               = -1;
    char buff[4]          = {0};
    int buf_value         = 0;
    double adc_value      = 0.0;

    // 打开光敏电阻所对应的ADC设备节点
    adc_fd = open(ADC_DEV_FILE, O_RDWR);
    if (adc_fd < 0)
    {
        MSG("Failed to open ADC_DEV_FILE !\n");
    }

    // 不断的读取光敏电阻的数值变化
    while(1) {
        lseek(adc_fd, 0, SEEK_SET);
        memset(buff, 0, sizeof(char) * 4);
        ret = read(adc_fd,buff,sizeof(buff));
        if (ret < 0 ) {
            MSG("read adc_fd error\n");
        }

        buf_value = atoi(buff);
        adc_value = (buf_value * 3.3) / 1024.0;
        MSG("adc_value is %.2lf \n", adc_value);

        sleep(1);
    }

    close(adc_fd);
    return 0;
}