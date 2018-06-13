//
//  SGMHelperC.h
//  SGMobileInjection
//
//  Created by 杨胜超 on 2018/5/26.
//  Copyright © 2018年 SkyGuard. All rights reserved.
//

#ifndef SGMHelperC_h
#define SGMHelperC_h

#include <time.h>
#include <stdio.h>

//========================================
//
// 非.c文件引入此头文件需要放入extern "C" {}
// 否则会造成头文件引用混乱！
//
//========================================

// 打开文件（不用hook）
extern FILE *fopen_nohook(const char *file_path, const char *mode);
// 打开文件（不用hook）
extern int open_nohook(const char *file_path, int oflag, int mode);
// 获取文件大小
extern uint64_t get_file_size(const char *file_path);
// 读取文件头前n个字节
extern void get_file_type(const char *file_path, char type[], int flag_len);
// 字节数组转16进制字符串
extern void bytes_to_hexstr( const char *source,  char *dest, int src_len );
// 获取当前时间
extern time_t now_time(void);

#endif /* SGMHelperC_h */
