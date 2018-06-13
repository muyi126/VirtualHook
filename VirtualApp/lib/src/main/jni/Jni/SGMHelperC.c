//
//  SGMHelperC.c
//  SGMobileInjection
//
//  Created by 杨胜超 on 2018/5/26.
//  Copyright © 2018年 SkyGuard. All rights reserved.
//

#include "SGMHelperC.h"
#include <fcntl.h>
#include <string.h>

#ifndef MAGIC_MODE_NOHOOK
    #define MAGIC_MODE_NOHOOK               "sg_"   // 只能在hook方法内部使用
#endif

#ifndef MAGIC_CODE_NOHOOK
    #define MAGIC_CODE_NOHOOK               9528    // 只能在hook方法内部使用
#endif


// 打开文件（不用hook）
int open_nohook(const char *file_path, int oflag, int mode) {
    return open(file_path, oflag, mode+MAGIC_CODE_NOHOOK);
}
// 获取文件大小
uint64_t get_file_size(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (NULL == file) {
        return 0;
    }
    fseek(file, 0, SEEK_END); //移动文件的指针到文件结尾
    uint64_t file_len = ftell(file); //获取文件的长度
    fclose(file);
    return file_len;
}
// 读取文件头前n个字节
void get_file_type(const char *file_path, char type[], int flag_len) {
    FILE *file = fopen(file_path, "r");
    if (NULL == file) {
        return ;
    }
    rewind(file);//将文件指针移动回文件开始
    fread(type, 1, flag_len, file);
    fclose(file);
}
// 字节数组转16进制字符串
void bytes_to_hexstr( const char *source,  char *dest, int src_len ) {
    char temp[3];
    memset(temp, 0, 3);
    for(int i = 0; i < src_len; i++ ) {
        sprintf(temp, "%02X", (unsigned char)source[i]);
        memcpy(&dest[i * 2], temp, 2);
    }
}
// 获取当前时间
time_t now_time(void) {
    return time(NULL); // = CFAbsoluteTimeGetCurrent() + 978307199
}
