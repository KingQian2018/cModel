set(CJSON_C 
    ./cJSON_Utils.c
    ./cJSON.c
)

add_library(lib_cjson ${CJSON_C})
