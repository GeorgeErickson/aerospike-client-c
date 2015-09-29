/*******************************************************************************
 * Copyright 2013-2014 Aerospike, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/

#include <node.h>
#include "enums.h"
#include <aerospike/as_status.h>

using namespace v8;

#define set(__obj, __name, __value) __obj->Set(Nan::New(__name).ToLocalChecked(), Nan::New(__value))

Local<Object> status()
{
    Nan::EscapableHandleScope scope;
    Local<Object> obj = Nan::New<Object>();
    set(obj, "AEROSPIKE_OK",                        AEROSPIKE_OK);
    set(obj, "AEROSPIKE_ERR",                       AEROSPIKE_ERR);
    set(obj, "AEROSPIKE_ERR_CLIENT",                AEROSPIKE_ERR_CLIENT);
    set(obj, "AEROSPIKE_ERR_PARAM",                 AEROSPIKE_ERR_PARAM);
    set(obj, "AEROSPIKE_ERR_SERVER",                AEROSPIKE_ERR_SERVER);
    set(obj, "AEROSPIKE_ERR_RECORD_NOT_FOUND",      AEROSPIKE_ERR_RECORD_NOT_FOUND);
    set(obj, "AEROSPIKE_ERR_RECORD_GENERATION",     AEROSPIKE_ERR_RECORD_GENERATION);
    set(obj, "AEROSPIKE_ERR_REQUEST_INVALID",       AEROSPIKE_ERR_REQUEST_INVALID);
    set(obj, "AEROSPIKE_ERR_RECORD_EXISTS",         AEROSPIKE_ERR_RECORD_EXISTS);
    set(obj, "AEROSPIKE_ERR_BIN_EXISTS",            AEROSPIKE_ERR_BIN_EXISTS);
    set(obj, "AEROSPIKE_ERR_CLUSTER_CHANGE",        AEROSPIKE_ERR_CLUSTER_CHANGE);
    set(obj, "AEROSPIKE_ERR_SERVER_FULL",           AEROSPIKE_ERR_SERVER_FULL);
    set(obj, "AEROSPIKE_ERR_TIMEOUT",               AEROSPIKE_ERR_TIMEOUT);
    set(obj, "AEROSPIKE_ERR_NO_XDR",                AEROSPIKE_ERR_NO_XDR);
    set(obj, "AEROSPIKE_ERR_CLUSTER",               AEROSPIKE_ERR_CLUSTER);
    set(obj, "AEROSPIKE_ERR_BIN_INCOMPATIBLE_TYPE", AEROSPIKE_ERR_BIN_INCOMPATIBLE_TYPE);
    set(obj, "AEROSPIKE_ERR_RECORD_TOO_BIG",        AEROSPIKE_ERR_RECORD_TOO_BIG);
    set(obj, "AEROSPIKE_ERR_RECORD_BUSY",           AEROSPIKE_ERR_RECORD_BUSY);
    set(obj, "AEROSPIKE_ERR_SCAN_ABORTED",          AEROSPIKE_ERR_SCAN_ABORTED);
    set(obj, "AEROSPIKE_ERR_UNSUPPORTED_FEATURE",   AEROSPIKE_ERR_UNSUPPORTED_FEATURE);
    set(obj, "AEROSPIKE_ERR_BIN_NOT_FOUND",         AEROSPIKE_ERR_BIN_NOT_FOUND);
    set(obj, "AEROSPIKE_ERR_DEVICE_OVERLOAD",       AEROSPIKE_ERR_DEVICE_OVERLOAD);
    set(obj, "AEROSPIKE_ERR_RECORD_KEY_MISMATCH",   AEROSPIKE_ERR_RECORD_KEY_MISMATCH);
    set(obj, "AEROSPIKE_ERR_NAMESPACE_NOT_FOUND",   AEROSPIKE_ERR_NAMESPACE_NOT_FOUND);
    set(obj, "AEROSPIKE_ERR_BIN_NAME",              AEROSPIKE_ERR_BIN_NAME);
    set(obj, "AEROSPIKE_ERR_UDF",                   AEROSPIKE_ERR_UDF);
    set(obj, "AEROSPIKE_ERR_LDT_NOT_FOUND",         AEROSPIKE_ERR_LDT_NOT_FOUND);
    set(obj, "AEROSPIKE_ERR_LARGE_ITEM_NOT_FOUND",  AEROSPIKE_ERR_LARGE_ITEM_NOT_FOUND);
    set(obj, "AEROSPIKE_ERR_UDF_NOT_FOUND",         AEROSPIKE_ERR_UDF_NOT_FOUND);
    set(obj, "AEROSPIKE_ERR_LUA_FILE_NOT_FOUND",    AEROSPIKE_ERR_LUA_FILE_NOT_FOUND);

    // LDT errors.
    // Currently server does not return LDT errors as a error
    // code, but returns as a string with message and status code.
    // The error message are parsed and populated into
    // as_error for the application to use these error codes.
    set(obj, "AEROSPIKE_ERR_LDT_INTERNAL",              AEROSPIKE_ERR_LDT_INTERNAL          );  
    set(obj, "AEROSPIKE_ERR_LDT_UNIQUE_KEY",            AEROSPIKE_ERR_LDT_UNIQUE_KEY        );  
    set(obj, "AEROSPIKE_ERR_LDT_INSERT",                AEROSPIKE_ERR_LDT_INSERT            );  
    set(obj, "AEROSPIKE_ERR_LDT_SEARCH",                AEROSPIKE_ERR_LDT_SEARCH            );  
    set(obj, "AEROSPIKE_ERR_LDT_DELETE",                AEROSPIKE_ERR_LDT_DELETE            );  
    set(obj, "AEROSPIKE_ERR_LDT_INPUT_PARAM",           AEROSPIKE_ERR_LDT_INPUT_PARM       );   
    set(obj, "AEROSPIKE_ERR_LDT_TYPE_MISMATCH",         AEROSPIKE_ERR_LDT_TYPE_MISMATCH     );  
    set(obj, "AEROSPIKE_ERR_LDT_NULL_BIN_NAME",         AEROSPIKE_ERR_LDT_NULL_BIN_NAME     );  
    set(obj, "AEROSPIKE_ERR_LDT_BIN_NAME_NOT_STRING",   AEROSPIKE_ERR_LDT_BIN_NAME_NOT_STRING); 
    set(obj, "AEROSPIKE_ERR_LDT_BIN_NAME_TOO_LONG",     AEROSPIKE_ERR_LDT_BIN_NAME_TOO_LONG );  
    set(obj, "AEROSPIKE_ERR_LDT_TOO_MANY_OPEN_SUBRECS",AEROSPIKE_ERR_LDT_TOO_MANY_OPEN_SUBRECS);    
    set(obj, "AEROSPIKE_ERR_LDT_TOP_REC_NOT_FOUND",     AEROSPIKE_ERR_LDT_TOP_REC_NOT_FOUND  ); 
    set(obj, "AEROSPIKE_ERR_LDT_SUB_REC_NOT_FOUND",     AEROSPIKE_ERR_LDT_SUB_REC_NOT_FOUND  ); 
    set(obj, "AEROSPIKE_ERR_LDT_BIN_DOES_NOT_EXIST",  AEROSPIKE_ERR_LDT_BIN_DOES_NOT_EXIST );
    set(obj, "AEROSPIKE_ERR_LDT_BIN_ALREADY_EXISTS",  AEROSPIKE_ERR_LDT_BIN_ALREADY_EXISTS );
    set(obj, "AEROSPIKE_ERR_LDT_BIN_DAMAGED",         AEROSPIKE_ERR_LDT_BIN_DAMAGED        );
    set(obj, "AEROSPIKE_ERR_LDT_SUBREC_POOL_DAMAGED", AEROSPIKE_ERR_LDT_SUBREC_POOL_DAMAGED);
    set(obj, "AEROSPIKE_ERR_LDT_SUBREC_DAMAGED",      AEROSPIKE_ERR_LDT_SUBREC_DAMAGED     );
    set(obj, "AEROSPIKE_ERR_LDT_SUBREC_OPEN",         AEROSPIKE_ERR_LDT_SUBREC_OPEN        );
    set(obj, "AEROSPIKE_ERR_LDT_SUBREC_UPDATE",       AEROSPIKE_ERR_LDT_SUBREC_UPDATE      );
    set(obj, "AEROSPIKE_ERR_LDT_SUBREC_CREATE",       AEROSPIKE_ERR_LDT_SUBREC_CREATE      );
    set(obj, "AEROSPIKE_ERR_LDT_SUBREC_DELETE",       AEROSPIKE_ERR_LDT_SUBREC_DELETE      );
    set(obj, "AEROSPIKE_ERR_LDT_SUBREC_CLOSE",        AEROSPIKE_ERR_LDT_SUBREC_CLOSE       );
    set(obj, "AEROSPIKE_ERR_LDT_TOPREC_UPDATE",       AEROSPIKE_ERR_LDT_TOPREC_UPDATE      );
    set(obj, "AEROSPIKE_ERR_LDT_TOPREC_CREATE",       AEROSPIKE_ERR_LDT_TOPREC_CREATE      );
    set(obj, "AEROSPIKE_ERR_LDT_FILTER_FUNCTION_BAD", AEROSPIKE_ERR_LDT_FILTER_FUNCTION_BAD);
    set(obj, "AEROSPIKE_ERR_LDT_FILTER_FUNCTION_NOT_FOUND", AEROSPIKE_ERR_LDT_FILTER_FUNCTION_NOT_FOUND);
    set(obj, "AEROSPIKE_ERR_LDT_KEY_FUNCTION_BAD",    AEROSPIKE_ERR_LDT_KEY_FUNCTION_BAD   );
    set(obj, "AEROSPIKE_ERR_LDT_KEY_FUNCTION_NOT_FOUND",AEROSPIKE_ERR_LDT_KEY_FUNCTION_NOT_FOUND  );
    set(obj, "AEROSPIKE_ERR_LDT_TRANS_FUNCTION_BAD",   AEROSPIKE_ERR_LDT_TRANS_FUNCTION_BAD);
    set(obj, "AEROSPIKE_ERR_LDT_TRANS_FUNCTION_NOT_FOUND", AEROSPIKE_ERR_LDT_TRANS_FUNCTION_NOT_FOUND);
    set(obj, "AEROSPIKE_ERR_LDT_UNTRANS_FUNCTION_BAD", AEROSPIKE_ERR_LDT_UNTRANS_FUNCTION_BAD);
    set(obj, "AEROSPIKE_ERR_LDT_UNTRANS_FUNCTION_NOT_FOUND", AEROSPIKE_ERR_LDT_UNTRANS_FUNCTION_NOT_FOUND);
    set(obj, "AEROSPIKE_ERR_LDT_USER_MODULE_BAD",     AEROSPIKE_ERR_LDT_USER_MODULE_BAD    );
    set(obj, "AEROSPIKE_ERR_LDT_USER_MODULE_NOT_FOUND", AEROSPIKE_ERR_LDT_USER_MODULE_NOT_FOUND);

    return scope.Escape(obj);
}
