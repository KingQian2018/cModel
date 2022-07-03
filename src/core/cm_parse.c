/**
 * @file parse_json.c
 * @author KisWang (KingQian2018)
 * @brief 解析Json数据
 * @version 0.1
 * @date 2022-07-03
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "cm_parse.h"
#include "../component/cJson/cJSON_Utils.h"

#define LOG_TAG "Parse_Json"
#include "cm_log.h"

#include <stdio.h>
#include <stdlib.h>
char *read_file(const char *filename)
{
    FILE *file = NULL;
    long length = 0;
    char *content = NULL;
    size_t read_chars = 0;

    /* open in read binary mode */
    file = fopen(filename, "rb");
    if (file == NULL)
    {
        goto cleanup;
    }

    /* get the length */
    if (fseek(file, 0, SEEK_END) != 0)
    {
        goto cleanup;
    }
    length = ftell(file);
    if (length < 0)
    {
        goto cleanup;
    }
    if (fseek(file, 0, SEEK_SET) != 0)
    {
        goto cleanup;
    }

    /* allocate content buffer */
    content = (char *)malloc((size_t)length + sizeof(""));
    if (content == NULL)
    {
        goto cleanup;
    }

    /* read the file into memory */
    read_chars = fread(content, sizeof(char), (size_t)length, file);
    if ((long)read_chars != length)
    {
        free(content);
        content = NULL;
        goto cleanup;
    }
    content[read_chars] = '\0';

cleanup:
    if (file != NULL)
    {
        fclose(file);
    }

    return content;
}

CModel *_PID;
CMODEL_STATUS_e parse_PID(const cJSON *par, unsigned char idx)
{
    return pid_create(&_PID[idx], cJSON_GetNumberValue(cJSON_GetObjectItem(par, "id")),
                      cJSON_GetNumberValue(cJSON_GetObjectItem(par, "dt")));
}

/**
 * @brief 获取Json数据
 *
 * @param cm_file json文件
 * @return CMODEL_STATUS_OK 解析成功
 * @return CMODEL_STATUS_PS_FILEINVALID 文件路径非法
 * @return CMODEL_STATUS_PS_FAILFILE 文件打开失败
 * @return CMODEL_STATUS_PS_FAILPARSE 文件解析失败
 */
CMODEL_STATUS_e parse_file(const char *cm_file)
{
    LOG_I("cJson Version: %s\n", cJSON_Version());

    cJSON *parsed = NULL;
    cJSON *cmodel = NULL;
    if (cm_file == NULL)
    {
        LOG_E("File path is Null.");
        return CMODEL_STATUS_PS_FILEINVALID;
    }
    char *f = read_file(cm_file);
    if (f == NULL)
    {
        LOG_E("Failed to read %s.", cm_file);
        return CMODEL_STATUS_PS_FAILFILE;
    }
    parsed = cJSON_Parse(f);
    if (parsed == NULL)
    {
        LOG_E("Failed to parse: %s.", cm_file);
        goto fail_parse;
    }

    //... parse json datas
    cmodel = cJSON_GetObjectItem(parsed, "PID");
    if (cmodel == NULL)
    {
        LOG_E("Failed to parse cmodel.");
        goto fail_parse;
    }
    else
    {
        unsigned char cmodel_size = 0;
        cmodel_size = cJSON_GetArraySize(cmodel);
        LOG_I("cmodel: PID, sum: %d", cmodel_size);
        _PID = (CModel *)calloc(cmodel_size, sizeof(CModel));
        if (_PID == NULL)
        {
            LOG_E("Failed to create _PIDs.");
            goto fail_parse;
        }
        for (unsigned char i = 0; i < cmodel_size; i++)
        {
            if (parse_PID(cJSON_GetArrayItem(cmodel, i), i) != CMODEL_STATUS_OK)
            {
                LOG_E("Failed to create cmodel.");
                goto fail_parse;
            }
        }
    }

    free(f);
    cJSON_Delete(parsed);
    return CMODEL_STATUS_OK;

fail_parse:
    if (f != NULL)
    {
        free(f);
    }
    if (parsed != NULL)
    {
        cJSON_Delete(parsed);
    }
    return CMODEL_STATUS_PS_FAILPARSE;
}
