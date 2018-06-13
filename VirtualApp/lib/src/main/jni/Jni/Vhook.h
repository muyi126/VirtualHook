//
//  SGMHookCMethods.h
//  SGMobileInjection
//
//  Created by 杨胜超 on 2018/5/3.
//  Copyright © 2018年 SkyGuard. All rights reserved.
//

#ifndef SGMHookCMethods_h
#define SGMHookCMethods_h

#include <stdlib.h>
__BEGIN_DECLS
#include <jni.h>
#include "SGMFileEncrypt.h"
#include "SGMHelperC.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>
#include <pthread.h>

__END_DECLS
//========================================
//
// 对C方法的hook
//
//========================================

typedef struct {
    int enableCamera;
    int enableClipBoard;
    int enableStoreEncrypted;
} VaConfig;




long long getCurrentTime(void);
int sgm_handleOpen(const char *path, int oflag, int mode);
#endif /* SGMHookCMethods_h */
void sgm_start_Vhook(VaConfig *);