#include "deploy.h"
#include "lib_io.h"
#include "lib_time.h"
#include "stdio.h"
#include "stdlib.h"

int main(void)
{
    print_time("Begin");
    char *topo[MAX_EDGE_NUM];
    int line_num;

    char *topo_file ="D:/ZHONGXIN/case_example/data2.txt";//数据读取文件，可改动

    line_num = read_file(topo, MAX_EDGE_NUM, topo_file);

   // printf("line num is :%d \n", line_num);
    if (line_num == 0)
    {
        printf("Please input valid topo file.\n");
        return -1;
    }

    char *result_file = "D:/ZHONGXIN/case_example/result2.txt";//结果数据文件，可改动

    deploy_server(topo, line_num, result_file);

    release_buff(topo, line_num);

    print_time("End");
    system("pause");

	return 0;
}

