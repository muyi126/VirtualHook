//
//  SGMFileEncrypt.c
//  SGMobileInjection
//
//  Created by 杨胜超 on 2018/5/7.
//  Copyright © 2018年 SkyGuard. All rights reserved.
//

#include "SGMFileEncrypt.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#import "SGMHelperC.h"

// 文件加密
void _file_encrypt(const char *source_path, const char *des_path, int type) {
    FILE *source_file = fopen(source_path, "r"); //获取文件的指针
    if (NULL == source_file) {
        return;
    }
    fseek(source_file, 0, SEEK_END); //移动文件的指针到文件结尾
    long source_file_len = ftell(source_file); //获取文件的长度
    rewind(source_file);//将文件指针移动回文件开始
    
    FILE *des_file = fopen(des_path, "w");
    if (NULL == des_file) {
        return;
    }
    rewind(des_file);//将文件指针移动回文件开始
    int flag_len = strlen(MAGIC_FLAG);
    
    if (0 == type) { // 写入特殊字符 只有加密才需要写
        fwrite(MAGIC_FLAG, 1, flag_len, des_file);
        fflush(des_file);
        fseek(des_file, flag_len, SEEK_SET); // 加密的时候，加密文件偏移8个字节开始写
    } else { // 读取原文件
        fseek(source_file, flag_len, SEEK_SET); // 解密的时候，加密文件偏移8个字节开始读
    }
    
    long READ_BUFFER = source_file_len;
    if (READ_BUFFER > 102400) {
        READ_BUFFER = 102400;
    }
    long curr_total_len = 0; // 当前处理的字节数
    
    unsigned char read_buff[READ_BUFFER];
    memset(read_buff, 0, sizeof(read_buff));
    
    size_t read_len = 0;
    while ((read_len = fread(read_buff, 1, READ_BUFFER, source_file)) > 0) {
        unsigned char write_buff[READ_BUFFER];
        memset(write_buff, 0, sizeof(read_buff));
        
        // 简单加解密(10MB 加解密时间 0.02s)  TODO:
        for (int i = 0; i < read_len; i++) {
            write_buff[i] = ~read_buff[i];
        }
        
        // 写之前已经确保当前文件的cur处于正确位置
        fwrite(write_buff, 1, read_len, des_file);
        fflush(des_file);
        curr_total_len += read_len;
        
        if (0 == type) { // 加密
            fseek(des_file, curr_total_len + flag_len, SEEK_SET);
            fseek(source_file, curr_total_len, SEEK_SET);
        } else { // 解密
            fseek(des_file, curr_total_len, SEEK_SET);
            fseek(source_file, curr_total_len + flag_len, SEEK_SET);
        }
    }
    fclose(des_file);
    fclose(source_file);
}

void file_encrypt(const char *source_path, const char *des_path) {
    _file_encrypt(source_path, des_path, 0);
}

void file_decrypt(const char *source_path, const char *des_path) {
    _file_encrypt(source_path, des_path, 1);
}
