//
//  SGMFileEncrypt.h
//  SGMobileInjection
//
//  Created by 杨胜超 on 2018/5/7.
//  Copyright © 2018年 SkyGuard. All rights reserved.
//

#ifndef SGMFileEncrypt_h
#define SGMFileEncrypt_h
#define MAGIC_FLAG                      "SGENCODE"
/*
 文件加密
 @param   source_path: 要加密的源文件
          des_path: 目标输出文件
 */
extern void file_encrypt(const char *source_path, const char *des_path);
/*
 文件解密
 @param   source_path: 要解密的源文件
 des_path: 目标输出文件
 */
extern void file_decrypt(const char *source_path, const char *des_path);

#endif /* SGMFileEncrypt_h */
