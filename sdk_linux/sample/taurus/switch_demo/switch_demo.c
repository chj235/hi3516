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

typedef enum {
    EAGE_0 = 0,
    EAGE_1,
    EAGE_2,
    EAGE_3
} GPioEage;
/*
 * @berf 用于将指定编号的引脚导出，作为GPIO使用
 * @param pin:指定导出的引脚
 *
 * @berf It is used to export the pin with the specified number and use it as GPIO
 * @param pin: Specify the exported pin
 */
static int GpioExport(int pin)
{
    char buffer[64] = {0};
    int len = -1;
    int fd = -1;

    fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd < 0) {
        MSG("Failed to open export for writing!\n");
        close(fd);
        return -1;
    }

    len = snprintf_s(buffer, sizeof(buffer), sizeof(buffer) - 1, "%d", pin);
    if (len < 0) {
        MSG("printf msg failed\r\n");
    }
    if (write(fd, buffer, len) < 0) {
        MSG("Failed to export gpio!");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

/*
 * @berf 用于将导出的GPIO删除掉
 * @param pin:指定删除的引脚
 *
 * @berf Used to delete the exported GPIO
 * @param pin: Specifies the pin to delete
 */
static int GpioUnexport(int pin)
{
    char buffer[64] = {0};
    int len = -1;
    int fd = -1;

    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (fd < 0) {
        MSG("Failed to open unexport for writing!\n");
        close(fd);
        return -1;
    }

    len = snprintf_s(buffer, sizeof(buffer), sizeof(buffer) - 1, "%d", pin);
    if (len < 0) {
        MSG("printf msg failed\r\n");
    }
    if (write(fd, buffer, len) < 0) {
        MSG("Failed to unexport gpio!");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

/*
 * direction设置输出还是输入模式，0-->IN为输入，1-->OUT为输出
 * direction sets the output or input mode, 0-->IN is input, 1-->OUT is output
 */
static int GpioDirection(int pin, int dir)
{
    static const char dirStr[] = "in\0out";
    char path[64] = {0};
    int fd = -1;

    int len = snprintf_s(path, sizeof(path), sizeof(path) - 1, "/sys/class/gpio/gpio%d/direction", pin);
    if (len < 0) {
        MSG("printf msg failed\r\n");
    }
    fd = open(path, O_WRONLY);
    if (fd < 0) {
        MSG("Failed to open gpio direction for writing!\n");
        close(fd);
        return -1;
    }

    if (write(fd, &dirStr[dir == 0 ? 0 : 3], dir == 0 ? 2 : 3) < 0) { /* 3, 2, 3 gpio register */
        MSG("Failed to set direction!\n");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

/*
 * @brief 设置输出值
 * @param pin: 引脚
 * @param value: 输出值
 *
 * @brief set output value
 * @param pin: pin
 * @param value: output value
 */
static int GpioWrite(int pin, int value)
{
    static const char valuesStr[] = "01";
    char path[64] = {0};
    int fd = -1;

    int len = snprintf_s(path, sizeof(path), sizeof(path) - 1, "/sys/class/gpio/gpio%d/value", pin);
    if (len < 0) {
        MSG("printf Msg failed\r\n");
    }
    fd = open(path, O_WRONLY);
    if (fd < 0) {
        MSG("Failed to open gpio value for writing!\n");
        close(fd);
        return -1;
    }

    if (write(fd, &valuesStr[value == 0 ? 0 : 1], 1) < 0) { /* 1, 1 gpio register */
        MSG("Failed to write value!\n");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

/*
 * @brief 读取输入值
 * @param pin: 引脚
 *
 * @brief reads the input value
 * @param pin: pin
 */
static int GpioRead(int pin)
{
    char path[64] = {0};
    char value_str[3] = {0};
    int fd = -1;

    int len = snprintf_s(path, sizeof(path), sizeof(path) - 1, "/sys/class/gpio/gpio%d/value", pin);
    if (len < 0) {
        MSG("printf msg failed\r\n");
    }
    fd = open(path, O_RDONLY);
    if (fd < 0) {
        MSG("Failed to open gpio value for reading!\n");
        close(fd);
        return -1;
    }

    if (read(fd, value_str, 3) < 0) { /* 3: gpio register */
        MSG("Failed to read value!\n");
        close(fd);
        return -1;
    }

    close(fd);
    return (atoi(value_str));
}

/*
 * none表示引脚为输入，不是中断引脚
 * rising表示引脚为中断输入，上升沿触发
 * falling表示引脚为中断输入，下降沿触发
 * both表示引脚为中断输入，边沿触发
 *
 * none indicates that the pin is an input, not an interrupt pin
 * rising indicates that the pin is an interrupt input and is triggered by a rising edge
 * falling means that the pin is an interrupt input, triggered by a falling edge
 * both indicates that the pin is an interrupt input, edge-triggered
 * 0-->none, 1-->rising, 2-->falling, 3-->both
 */
static int GpioEdge(int pin, int edge)
{
    const char dirStr[] = "none\0rising\0falling\0both";
    char ptr = 0;
    char path[64] = {0};
    int fd = -1;

    switch (edge) {
        case EAGE_0:
            ptr = 0;
            break;
        case EAGE_1:
            ptr = 5; /* 5: gpio register */
            break;
        case EAGE_2:
            ptr = 12; /* 12: gpio register */
            break;
        case EAGE_3:
            ptr = 20; /* 20: gpio register */
            break;
        default:
            ptr = 0;
    }
    int len = snprintf_s(path, sizeof(path), sizeof(path) - 1, "/sys/class/gpio/gpio%d/edge", pin);
    if (len < 0) {
        MSG("printf Msg failed\r\n");
    }

    fd = open(path, O_WRONLY);
    if (fd < 0) {
        MSG("Failed to open gpio edge for writing!\n");
        close(fd);
        return -1;
    }

    if (write(fd, &dirStr[ptr], strlen(&dirStr[ptr])) < 0) {
        MSG("Failed to set edge!\n");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

/*
 * 初始化按键1
 * Initialize button 1
 */
void InitGpio1(void)
{
    MSG("\n =============== InitGpio1 start ========== \n");
    GpioUnexport(1); /* 1: gpio pin */
    GpioExport(1); /* 1: gpio pin */
    GpioDirection(1, 0); /* 1, 0: gpio pin */
    GpioEdge(1, 2); /* 1, 2: gpio pin */
    MSG("\n =============== InitGpio1 end ========== \n");
}

/*
 * 初始化按键2
 * Initialize button 2
 */
void InitGpio2(void)
{
    MSG("\n =============== InitGpio2 start ========== \n");
    GpioUnexport(2); /* 2: gpio pin */
    GpioExport(2); /* 2: gpio pin */
    GpioDirection(2, 0); /* 2, 0: gpio pin */
    GpioEdge(2, 2); /* 2, 2: gpio pin */
    MSG("\n =============== InitGpio2 end ========== \n");
}

int main(void)
{
    /*
     * 按键初始化定义
     * Key initialization definition
     */
    int gpio1_fd = -1;
    int gpio2_fd = -1;
    int ret1     = -1;
    int ret2     = -1;
    char key1_value[10];
    char key2_value[10];

    InitGpio1();
    InitGpio2();

    gpio1_fd = open("/sys/class/gpio/gpio1/value", O_RDONLY);
    if (gpio1_fd < 0)
    {
        MSG("Failed to open gpio1 !\n");
    }

    gpio2_fd = open("/sys/class/gpio/gpio2/value", O_RDONLY);
    if (gpio2_fd < 0)
    {
        MSG("Failed to open gpio1 !\n");
    }

    while(1)
    {
        memset(key1_value, 0, sizeof(key1_value));
        lseek(gpio1_fd, 0, SEEK_SET);
        ret1 = read(gpio1_fd, key1_value, sizeof(key1_value));
        if ((key1_value[0] - '0') == 0) {
            MSG("F2 is pressed\n");
        }

        memset(key2_value, 0, sizeof(key2_value));
        lseek(gpio2_fd, 0, SEEK_SET);
        ret2 = read(gpio2_fd, key2_value, sizeof(key2_value));
        if ((key2_value[0] - '0') == 0) {
            MSG("F1 is pressed\n");
        }
        usleep(5);
    }

    return 0;
}