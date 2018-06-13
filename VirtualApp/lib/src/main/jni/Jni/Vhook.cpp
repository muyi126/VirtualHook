
#include <string>

#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <xhook/xhook.h>
#include <Vhook.h>
#include <fb/include/fb/ALog.h>


#ifndef MAGIC_MODE_NOHOOK
    #define MAGIC_MODE_NOHOOK               "sg_"   // 只能在hook方法内部使用
#endif

#ifndef MAGIC_CODE_NOHOOK
    #define MAGIC_CODE_NOHOOK               9528    // 只能在hook方法内部使用
#endif

using namespace std;

std::map<int, std::string> g_open_files;
pthread_mutex_t g_mutex;

//===================================================================
//
// 私有方法
//
//===================================================================
// 判断是否特殊格式（数据库，图片，视频，音频等）的文件
int is_special_file_type(const char *flag, int flag_len) {
    char flag_hexstr[flag_len * 2 + 1];
    memset(flag_hexstr, 0, sizeof(flag_hexstr));
    bytes_to_hexstr(flag, flag_hexstr, flag_len);

    const char *exclude_flags[] = {
            // image
            "FFD8FF", // jpg
            "89504E47", // png
            "47494638", // gif
            "49492A00", // tif
            "424D", // bmp
            // video/audio
            "57415645", // wav
            "41564920", // avi
            "2E524D46", // rm
            "000001BA", // mpg
            "000001B3", // mpg
            "6D6F6F76", // mov
            "3026B2758E66CF11",// asf
            "464c560105", //flv与f4v相同
            "0000002066747970", // mp4
            "49443303", // mp3
            "52494646e2780700", // wav
            // db
            "5374616E64617264", //mdb
            "53514C6974652066" // sqlite
    };
    for (int i = 0; i < sizeof(exclude_flags) / sizeof(exclude_flags[0]); i++) {
        const char *exclude_flag = exclude_flags[i];
        if (0 == strcmp(exclude_flag, flag_hexstr)) {
            return 1;
        }
    }
    return 0;
}
int is_special_file_type(const char *file_path) {

    // TODO: 直接根据文件后缀名判断类型

    int flag_len = strlen(MAGIC_FLAG);

    char flag[flag_len + 1];
    memset(flag, 0, sizeof(flag));
    get_file_type(file_path, flag, flag_len);

    return is_special_file_type(flag, flag_len);
}
int is_in_black_dir(const char *file_path) {
    return 1;
}



// 加密文件并删除缓存信息
void encrypt_and_remove_open_info(const int file_id) {
//    SGSQLite lite;
//    if ( ! lite.open(SGMGlobalShareInstance.DBPath.UTF8String)) {
//        lite.close();
//        return;
//    }

    char file_path[PATH_MAX];
    memset(file_path, 0, sizeof(file_path));
    bool is_file_found = false;
    do {
        // 0. 策略要求不加密
//        if ( ! SGMGlobalShareInstance.configModel.isFileEncryptEnable) {
//            break;
//        }

        // 查询file_path
        {
            pthread_mutex_lock(&g_mutex);
            std::map<int, std::string>::iterator iter = g_open_files.find(file_id);
            if (iter != g_open_files.end()) {
                string temp = iter->second;
                strcpy(file_path, temp.c_str());
                g_open_files.erase(file_id);
            }
            pthread_mutex_unlock(&g_mutex);

            // 持久化到DB 2/3
//            NSString *query_cmd = [NSString stringWithFormat:@"SELECT file_path FROM openfile WHERE launch_uuid = '%@' AND bundle_id = '%@' AND file_id = '%s'", SGMGlobalShareInstance.launchUDID, SGMGlobalShareInstance.bundleId, file_id];
//            SGSQLiteDataReader reader = lite.excuteQuery(query_cmd.UTF8String);
//            if (reader.read()) {
//                is_file_found = true;
//                const char *temp_path = reader.getStringValue(0);
//                strcpy(file_path, temp_path);
//            }
//            reader.close();
        }

        // 1. 不存在的文件不处理
        if (0 == strlen(file_path) || 0 != access(file_path, R_OK)) {
            break ;
        }

        // 2. 特殊格式的文件不处理
        if (1 == is_special_file_type(file_path)) {
            break;
        }

        // 3. 文件大小不在规定的范围内不处理
        uint64_t file_len = get_file_size(file_path);
//        if (file_len <= 0 || file_len >= SGMGlobalShareInstance.configModel.maxCheckFileSize) {
//            break ;
//        }

        // 4. 加密文件
        char decrypt_path[strlen(file_path) + 4];
        memset(decrypt_path, 0, sizeof(decrypt_path));
        strcpy(decrypt_path, file_path);
        strcat(decrypt_path, ".sg");
        file_encrypt(file_path, decrypt_path);
        rename(decrypt_path, file_path);
    } while (0);

    // 4. 删除文件记录
    if (is_file_found) {
        // 持久化到DB 3/3
//        SGSQLiteCommand delete_cmd(&lite);
//        delete_cmd.setCommandText("DELETE FROM openfile WHERE launch_uuid = ? AND bundle_id = ? AND file_id = ?");
//        delete_cmd.bindParam(1, SGMGlobalShareInstance.launchUDID.UTF8String);
//        delete_cmd.bindParam(2, SGMGlobalShareInstance.bundleId.UTF8String);
//        delete_cmd.bindParam(3, file_id);
//        if ( !delete_cmd.excute() ){
//            printf("delete open info failed! [%s, %s]\n", file_id, file_path);
//        }
//        delete_cmd.clear();
    }
//    lite.close();
}




long long getCurrentTime()
{
    struct timeval xTime;
    int xRet = gettimeofday(&xTime, NULL);
    long long xFactor = 1;
    return  ( xFactor * xTime.tv_sec * 1000) + (xTime.tv_usec / 1000);
}

int (*origin_close)(int fd);
int sgm_close(int fd){
    int sta = origin_close(fd);
    if (sta < 0) {
        return sta;
    }
    ///char str_fd[20];
//    ALOGI("sgm_close %d",fd);
    encrypt_and_remove_open_info(fd);
    return sta;
}




// 判断文件是否需要加密
// return 0-不加密 1-要加密
int decrypt_and_check_file_encryption(const char *file_path) {
    // 1. 不在app沙盒内的文件或者白名单中的文件不处理
    if ( 0 == is_in_black_dir(file_path)) {
        return 0;
    }

    // 2. 新建的文件需要处理（此时还不知道文件类型，close时需要判断）
    if (0 != access(file_path, R_OK)) {
//        return SGMGlobalShareInstance.configModel.isFileEncryptEnable ? 1 : 0;
    }

    // 3. 获取文件扩展类型
    int flag_len = strlen(MAGIC_FLAG);
    char flag[flag_len + 1];
    memset(flag, 0, sizeof(flag));
    get_file_type(file_path, flag, flag_len);

    // 4. 只要是加密的文件都进行解密，不管大小
    if (0 == strcmp(flag, MAGIC_FLAG)) {
        char decrypt_path[strlen(file_path) + 4];
        memset(decrypt_path, 0, sizeof(decrypt_path));
        strcpy(decrypt_path, file_path);
        strcat(decrypt_path, ".sg");
        file_decrypt(file_path, decrypt_path);
        rename(decrypt_path, file_path);
    } else if (1 == is_special_file_type(flag, flag_len)) { // 特殊文件格式不处理
        return 0;
    }

    // 5. 文件大小超过限制的不处理
    uint64_t file_len = get_file_size(file_path);
//    if (file_len >= SGMGlobalShareInstance.configModel.maxCheckFileSize) {
//        return 0;
//    }

//    return SGMGlobalShareInstance.configModel.isFileEncryptEnable ? 1 : 0;
    return 1;
}
// 将文件的open info做持久化保存
void save_open_info(const char *file_path, const int file_id) {
    pthread_mutex_lock(&g_mutex);
    g_open_files.insert(make_pair(file_id, file_path));
    pthread_mutex_unlock(&g_mutex);
}


//int handleOpen(const char *path, int oflag, int mode) {
int (*origin_handleOpen)(const char *path, int oflag, int mode);
int sgm_handleOpen(const char *path, int oflag, int mode){

    // 解密文件
    int sta = decrypt_and_check_file_encryption(path);

    // 正常打开文件
    int fd = origin_handleOpen(path, oflag, mode);
//    ALOGI("sgm_handleOpen %d %s",fd,path);
    // 保存fd和文件路径
    if (fd > 0 && 1 == sta) {
        save_open_info(path, fd);
    }
    return fd;
}

void sgm_start_Vhook(VaConfig *config) {

    xhook_enable_debug(1);
    xhook_enable_sigsegv_protection(0);
    ALOGI("sgm_start_Vhook %s",(config->enableStoreEncrypted ? "true" : "false"));
    if(config->enableStoreEncrypted){
        xhook_register(".*\\.so$", "close",  (void *)sgm_close,       (void **) &origin_close);
        xhook_register(".*\\.so$", "handleOpen",  (void *)sgm_handleOpen,       (void **) &origin_handleOpen);
    } else{
        if(NULL!=origin_handleOpen){
            xhook_ignore(".*\\.so$", "handleOpen");
            origin_handleOpen =NULL;
        }
        if(NULL!=origin_close){
            xhook_ignore(".*\\.so$", "close");
            origin_close =NULL;
        }
    }

    xhook_refresh(1);
}