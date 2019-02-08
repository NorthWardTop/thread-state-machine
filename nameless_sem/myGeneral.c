#include "common.h"



//处理错误信息, 致命错误, 程序直接退出
void handle_error(const char* msg)
{
    printf("Code location:[file %s][line %d]", __FILE__, __LINE__);
    printf("Error No.:[%d][%s] , msg : %s\n", errno, strerror(errno), msg);
    exit(EXIT_FAILURE);
}

//显示信息, 非致命错误, 被调程序返回错误码
void show_msg(const char* msg)
{
    printf("Code location:[file %s][line %d]", __FILE__, __LINE__);
    printf("Error No.:[%d][%s] , msg : %s\n", errno, strerror(errno), msg);
}
