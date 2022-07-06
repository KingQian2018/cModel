/** This file was automantically generated by auto_generate.py. DO NOT CHANGE IT!**/
#include "../component/cJson/cJSON_Utils.h"

#include "const.h"
#include "gain.h"
#include "limit.h"
#include "pid.h"
#include "switch.h"
#include "translate.h"

static CMODEL_STATUS_e prase_const_pars(unsigned int id, const cJSON* pars);
static CMODEL_STATUS_e prase_translate_pars(unsigned int id, const cJSON* pars);
static CMODEL_STATUS_e prase_limit_pars(unsigned int id, const cJSON* pars);
static CMODEL_STATUS_e prase_switch_pars(unsigned int id, const cJSON* pars);
static CMODEL_STATUS_e prase_gain_pars(unsigned int id, const cJSON* pars);
static CMODEL_STATUS_e prase_pid_pars(unsigned int id, const cJSON* pars);

typedef struct _Prase_Datas_s {
	 const char* name;
	 CModel (*createCB)(uint32_t, uint32_t);
	 CMODEL_STATUS_e (*parsCB)(uint32_t, const cJSON*);
}Prase_Datas_s;

const Prase_Datas_s Prase_Datas[] = {
	{"CONST", const_create, prase_const_pars},
	{"TRANSLATE", translate_create, prase_translate_pars},
	{"LIMIT", limit_create, prase_limit_pars},
	{"SWITCH", switch_create, prase_switch_pars},
	{"GAIN", gain_create, prase_gain_pars},
	{"PID", pid_create, prase_pid_pars},
};

unsigned int Prase_Datas_len = sizeof(Prase_Datas)/sizeof(Prase_Datas[0]);

