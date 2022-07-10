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

#define LOG_TAG "Parse_Json"
#include "cm_log.h"

#include <stdio.h>
#include <stdlib.h>

static char *read_file(const char *filename)
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

#include "auto_generate_prase.c"
#include <string.h>

#define CJSON_GETVALUE(pars, type, value)                          \
    type value = 0;                                                \
    {                                                              \
        cJSON *tmp = NULL;                                         \
        tmp = NULL;                                                \
        tmp = cJSON_GetObjectItem(pars, #value);                   \
        value = tmp == NULL ? 0 : (type)cJSON_GetNumberValue(tmp); \
    }
CMODEL_STATUS_e prase_pid_pars(CModel cm, const cJSON *pars)
{
    cJSON *tmp = NULL;
    cJSON *tmp1 = NULL;
    cJSON *tmp2 = NULL;

    CJSON_GETVALUE(pars, float, KP);
    CJSON_GETVALUE(pars, float, KI);
    CJSON_GETVALUE(pars, float, KD);

    pid_setPID(cm, KP, KI, KD);
    LOG_I("Model PID %d, kp: %.3f, ki: %.3f, kd: %.3f.", cm->id, KP, KI, KD);

    tmp = cJSON_GetObjectItem(pars, "SP");
    if (tmp == NULL)
    {
        pid_setSP(cm, 0, 0);
    }
    else
    {
        tmp1 = cJSON_GetObjectItem(tmp, "Biase");
        tmp2 = cJSON_GetObjectItem(tmp, "Gain");
        float Biase = tmp1 == NULL ? 0 : (float)cJSON_GetNumberValue(tmp1);
        float Gain = tmp2 == NULL ? 0 : (float)cJSON_GetNumberValue(tmp2);
        pid_setSP(cm, Biase, Gain);
        LOG_I("SP-Biase: %.3f, Gain: %.3f.", Biase, Gain);
    }

    tmp = cJSON_GetObjectItem(pars, "PV");
    if (tmp == NULL)
    {
        pid_setPV(cm, 0, 0);
    }
    else
    {
        tmp1 = cJSON_GetObjectItem(tmp, "Biase");
        tmp2 = cJSON_GetObjectItem(tmp, "Gain");
        float Biase = tmp1 == NULL ? 0 : (float)cJSON_GetNumberValue(tmp1);
        float Gain = tmp2 == NULL ? 0 : (float)cJSON_GetNumberValue(tmp2);
        pid_setPV(cm, Biase, Gain);
        LOG_I("PV-Biase: %.3f, Gain: %.3f.", Biase, Gain);
    }

    tmp = cJSON_GetObjectItem(pars, "Limit");
    if (tmp == NULL)
    {
        pid_setLimit(cm, 0, 0);
    }
    else
    {
        tmp1 = cJSON_GetObjectItem(tmp, "h");
        tmp2 = cJSON_GetObjectItem(tmp, "l");
        float h = tmp1 == NULL ? 0 : (float)cJSON_GetNumberValue(tmp1);
        float l = tmp2 == NULL ? 0 : (float)cJSON_GetNumberValue(tmp2);
        pid_setLimit(cm, h, l);
        LOG_I("Limit-h: %.3f, l: %.3f.", h, l);
    }
    return CMODEL_STATUS_OK;
}

CMODEL_STATUS_e prase_const_pars(CModel cm, const cJSON *pars)
{
    cJSON *tmp = NULL;

    CJSON_GETVALUE(pars, uint32_t, TargetT);
    CJSON_GETVALUE(pars, a_value, Value);

    const_setTargetT(cm, TargetT);
    const_setValue(cm, Value);
    LOG_I("Model CONST %d, TargetT: %d, Value: %.3f.",
          cm->id, TargetT, Value);

    return CMODEL_STATUS_OK;
}

CMODEL_STATUS_e prase_translate_pars(CModel cm, const cJSON *pars)
{
    CJSON_GETVALUE(pars, float, K);
    CJSON_GETVALUE(pars, uint32_t, T);
    CJSON_GETVALUE(pars, short, n);
    CJSON_GETVALUE(pars, float, Tao);

    translate_setPar(cm, K, T, n, Tao);
    LOG_I("Model Translate %d, K: %.3f, T: %.3f, n: %d, Tao: %.3f.",
          cm->id, K, T, n, Tao);
    return CMODEL_STATUS_OK;
}

CMODEL_STATUS_e prase_limit_pars(CModel cm, const cJSON *pars)
{
    return CMODEL_STATUS_OK;
}

CMODEL_STATUS_e prase_gain_pars(CModel cm, const cJSON *pars)
{
    return CMODEL_STATUS_OK;
}

CMODEL_STATUS_e prase_switch_pars(CModel cm, const cJSON *pars)
{
    return CMODEL_STATUS_OK;
}

/**
 * @brief 通过cJSON文件解析模型
 *
 * @param par cJSON文件
 * @return CMODEL_STATUS_OK 创建成功
 * @return CMODEL_STATUS_CM_CREATE 创建失败
 */
static CMODEL_STATUS_e prase_model(const cJSON *par)
{
    for (unsigned int i = 0; i < Prase_Datas_len; i++)
    {
        cJSON *model = cJSON_GetObjectItem(par, Prase_Datas[i].name);
        if (model != NULL)
        {
            unsigned char model_cnt = cJSON_GetArraySize(model);
            for (unsigned char model_idx = 0; model_idx < model_cnt; model_idx++)
            {
                cJSON *arrayItem = cJSON_GetArrayItem(model, model_idx);
                unsigned int id = cJSON_GetNumberValue(cJSON_GetObjectItem(arrayItem, "id"));
                unsigned int dt = cJSON_GetNumberValue(cJSON_GetObjectItem(arrayItem, "dt"));
                CModel cm = NULL;
                if ((cm = Prase_Datas[i].createCB(id, dt)) == NULL)
                { // create model
                    LOG_E("%s: Parse Model Error.", Prase_Datas[i].name);
                    return CMODEL_STATUS_CM_CREATE;
                }
                LOG_I("%s create. id: %d,dt: %d", Prase_Datas[i].name, id, dt);

                cJSON *pars = cJSON_GetObjectItem(arrayItem, "pars");
                if (pars != NULL)
                {
                    if (Prase_Datas[i].parsCB(cm, pars) != CMODEL_STATUS_OK)
                    { // set pars
                        LOG_E("%s: Parse Model Pars Error.", Prase_Datas[i].name);
                        return CMODEL_STATUS_CM_CREATEPAR;
                    }
                }
            }
        }
    }
    for (unsigned char i = 0; i < Prase_Datas_len; i++)
    { // Model Link
        cJSON *model = cJSON_GetObjectItem(par, Prase_Datas[i].name);
        unsigned char model_cnt = cJSON_GetArraySize(model);
        for (unsigned char model_idx = 0; model_idx < model_cnt; model_idx++)
        {
            cJSON *arrayItem = cJSON_GetArrayItem(model, model_idx);
            unsigned int _iid = cJSON_GetNumberValue(cJSON_GetObjectItem(arrayItem, "id"));
            cJSON *links = cJSON_GetObjectItem(arrayItem, "links");
            if (links != NULL)
            {
                cJSON *AD = cJSON_GetObjectItem(links, "A");
                if (AD != NULL)
                { // Link AO and AI
                    unsigned char link_cnt = cJSON_GetArraySize(AD);
                    for (unsigned char link_idx = 0; link_idx < link_cnt; link_idx++)
                    {
                        cJSON *link = cJSON_GetArrayItem(AD, link_idx);
                        if (link != NULL)
                        {
                            cJSON *l_i = cJSON_GetObjectItem(link, "I");
                            cJSON *l_id = cJSON_GetObjectItem(link, "id");
                            cJSON *l_o = cJSON_GetObjectItem(link, "O");
                            if (l_i == NULL ||
                                l_id == NULL ||
                                l_o == NULL)
                            {
                                LOG_E("Prase Link A Error.");
                                return CMODEL_STATUS_PS_FAILPARSE;
                            }
                            else
                            {
                                IOPIN_e _i = (IOPIN_e)cJSON_GetNumberValue(l_i);
                                unsigned int _id = (unsigned int)cJSON_GetNumberValue(l_id);
                                IOPIN_e _o = (IOPIN_e)cJSON_GetNumberValue(l_o);
                                CModel cmi = cm_getModelByID(_iid);
                                CModel cmo = cm_getModelByID(_id);
                                if (cmi == NULL || cmo == NULL)
                                {
                                    LOG_E("Failed to get model.");
                                    return CMODEL_STATUS_CM_NULL;
                                }
                                cm_setLink(IOTYP_AI, cmi, _i, cmo, _o);
                                LOG_I("Link %s %d AI%d to %s %d AO%d.", cmo->name, cmo->id, _o,
                                      cmi->name, cmi->id, _i);
                            }
                        }
                    }
                }
                AD = cJSON_GetObjectItem(links, "D");
                if (AD != NULL)
                { // Link DO and DI
                    unsigned char link_cnt = cJSON_GetArraySize(AD);
                    for (unsigned char link_idx = 0; link_idx < link_cnt; link_idx++)
                    {
                        cJSON *link = cJSON_GetArrayItem(AD, link_idx);
                        if (link != NULL)
                        {
                            cJSON *l_i = cJSON_GetObjectItem(link, "I");
                            cJSON *l_id = cJSON_GetObjectItem(link, "id");
                            cJSON *l_o = cJSON_GetObjectItem(link, "O");
                            if (l_i == NULL ||
                                l_id == NULL ||
                                l_o == NULL)
                            {
                                LOG_E("Prase Link D Error.");
                                return CMODEL_STATUS_PS_FAILPARSE;
                            }
                            else
                            {
                                IOPIN_e _i = (IOPIN_e)cJSON_GetNumberValue(l_i);
                                unsigned int _id = (unsigned int)cJSON_GetNumberValue(l_id);
                                IOPIN_e _o = (IOPIN_e)cJSON_GetNumberValue(l_o);
                                CModel cmi = cm_getModelByID(_iid);
                                CModel cmo = cm_getModelByID(_id);
                                if (cmi == NULL || cmo == NULL)
                                {
                                    LOG_E("Failed to get model.");
                                    return CMODEL_STATUS_CM_NULL;
                                }
                                cm_setLink(IOTYP_DI, cmi, _i, cmo, _o);
                                LOG_I("Link %s %d DI%d to %s %d DO%d.", cmo->name, cmo->id, _o,
                                      cmi->name, cmi->id, _i);
                            }
                        }
                    }
                }
            }
        }
    }

    return CMODEL_STATUS_OK;
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
        goto fail_parse;
    }
    parsed = cJSON_Parse(f);
    if (parsed == NULL)
    {
        LOG_E("Failed to parse file: %s.", cm_file);
        goto fail_parse;
    }

    //... parse json datas
    if (prase_model(parsed) != CMODEL_STATUS_OK)
    {
        LOG_E("Failed to parse model.");
        goto fail_parse;
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
