// Gen_buildfw.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include <io.h>
#include <direct.h>
#include <string.h>

int _tmain(int argc, _TCHAR* argv[])
{
    FILE *in, *out;
	char file_out_name[128];
    char file_line[128];
    char txt_file[128];
    char ch;
    long in_len;
    int n = 0;
    struct _finddata_t tmp_finddata;
	long tmp_handle;
	int file_out_name_len;
    
    in = fopen("buildfw_all.bat", "r");
    if (in == NULL)
    {
    	printf("打开 buildfw_all.bat 错误！\n");
    	goto gen_buildfw_exit;
    }
    
    if (_chdir(".\\config_txt") == -1)
    {
    	printf("进入目录 config_txt 错误！\n");
    	goto gen_buildfw_exit;
    }
    
    tmp_handle = _findfirst("*.txt", &tmp_finddata);
	if (tmp_handle == -1)
    {
    	printf("没有 TXT 文件！\n");
    	goto gen_buildfw_exit;
    }
    
    while(1)
    {
		fseek(in, 0, 0);

		strcpy(file_out_name, ".\\..\\buildfw_");
		strcat(file_out_name, tmp_finddata.name);
		file_out_name_len = strlen(file_out_name);
		file_out_name[file_out_name_len-3] = 'b';
		file_out_name[file_out_name_len-2] = 'a';
		file_out_name[file_out_name_len-1] = 't';

		printf("%s --> %s\n", tmp_finddata.name, file_out_name);
		out = fopen(file_out_name, "w");

		while (1)
		{
			if (fgets(file_line, 128, in) == NULL)
			{
				break;
			}
			
			if (strncmp(file_line, "for", 3) == 0)
			{
				char *str_find;

				str_find = strstr(file_line, "*.txt");
				if (str_find != NULL)
				{
					strncpy(txt_file, file_line, str_find - file_line);
					txt_file[str_find - file_line] = 0;
					strcat(txt_file, tmp_finddata.name);
					strcat(txt_file, str_find + 5);
					fputs(txt_file, out);
				}
				else
				{
					printf("批处理语法错误！\n");
					fclose(out);
					goto gen_buildfw_exit;
				}
			}
			else
			{
				fputs(file_line, out);
			}
		}

		fclose(out);
        
        if (_findnext(tmp_handle, &tmp_finddata) == -1)
		{
			break;
		}
    }
    
    fclose(in);

gen_buildfw_exit:
    printf("按任意键退出...");
	ch = getchar();

    return 0;
}

