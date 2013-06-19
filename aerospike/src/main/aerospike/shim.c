#include "shim.h"

#include <aerospike/as_bytes.h>
#include <aerospike/as_integer.h>
#include <aerospike/as_list.h>
#include <aerospike/as_map.h>
#include <aerospike/as_string.h>
#include <aerospike/as_val.h>
#include <aerospike/as_record.h>
#include <aerospike/as_status.h>

#include <aerospike/as_msgpack.h>
#include <aerospike/as_serializer.h>

#include <citrusleaf/citrusleaf.h>
#include <citrusleaf/cl_types.h>
#include <citrusleaf/cf_log_internal.h>

#include <stdint.h>


#define ERR_ASSIGN(__enum) \
		err->code = __enum; \
		if (err->message[0] == '\0') strcpy(err->message, #__enum);

as_status as_error_fromrc(as_error * err, cl_rv rc) 
{
	switch (rc) {
	case CITRUSLEAF_OK:
		ERR_ASSIGN(AEROSPIKE_OK);
		break;
	case CITRUSLEAF_FAIL_GENERATION:
		ERR_ASSIGN(AEROSPIKE_ERR_WRITE_GENERATION);
		break;
		// TODO - fill in other error conversions as new as_status labels are
		// defined. Note we'll have to distinguish transaction type to do this
		// properly, i.e. add a parameter to this shim function.
	default:
		ERR_ASSIGN(AEROSPIKE_ERR);
		break;
	}

	return err->code;
}

void asval_to_clobject(as_val * val, cl_object * obj)
{
	switch(val->type) {
		case AS_NIL: {
			citrusleaf_object_init_null(obj);
			break;
		}
		case AS_INTEGER: {
			as_integer * v = as_integer_fromval(val);
			citrusleaf_object_init_int(obj, as_integer_toint(v));
			break;
		}
		case AS_STRING: {
			as_string * v = as_string_fromval(val);
			citrusleaf_object_init_str(obj, as_string_tostring(v));
			break;
		}
		case AS_BYTES: {
			as_bytes * v = as_bytes_fromval(val);
			citrusleaf_object_init_blob2(obj, v->value, v->len, v->type);
			break;
		}
		case AS_LIST:{
			as_buffer buffer;
			as_buffer_init(&buffer);

			as_serializer ser;
			as_msgpack_init(&ser);
			as_serializer_serialize(&ser, val, &buffer);
			as_serializer_destroy(&ser);
			
			citrusleaf_object_init_blob2(obj, buffer.data, buffer.size, CL_LIST);
			break;
		}
		case AS_MAP: {
			as_buffer buffer;
			as_buffer_init(&buffer);

			as_serializer ser;
			as_msgpack_init(&ser);
			as_serializer_serialize(&ser, val, &buffer);
			as_serializer_destroy(&ser);

			citrusleaf_object_init_blob2(obj, buffer.data, buffer.size, CL_MAP);
			break;
		}
		default: {
			// raise an error
			break;
		}
	}
}

void asbinvalue_to_clobject(as_bin_value * binval, cl_object * obj)
{
	asval_to_clobject((as_val *) binval, obj);
}

void asbin_to_clbin(as_bin * as, cl_bin * cl) 
{
	strncpy(cl->bin_name, as->name, AS_BIN_NAME_LEN);
	cl->bin_name[CL_BINNAME_SIZE-1] = '\0';
	asbinvalue_to_clobject(as->valuep, &cl->object);
}

void asrecord_to_clbins(as_record * rec, cl_bin * bins, uint32_t nbins) 
{
	as_bin * rbin = rec->bins.entries;
	for ( int i = 0; i < nbins; i++ ) {
		asbin_to_clbin(&rbin[i], &bins[i]);
	}
}

void clbin_to_asval(cl_bin * bin, as_serializer * ser, as_val ** val) 
{
	if ( val == NULL ) return;

	switch( bin->object.type ) {
		case CL_NULL :{
			*val = NULL;
			break;
		}
		case CL_INT : {
			*val = (as_val *) as_integer_new(bin->object.u.i64);
			break;
		}
		case CL_STR : {
			// steal the pointer from the object into the val
			*val = (as_val *) as_string_new(strdup(bin->object.u.str), true /*ismalloc*/);
			break;
		}
		case CL_LIST :
		case CL_MAP : {
			// use a temporary buffer, which doesn't need to be destroyed
			as_buffer buf = {
				.capacity = (uint32_t) bin->object.sz,
				.size = (uint32_t) bin->object.sz,
				.data = (uint8_t *) bin->object.u.blob
			};
			// print_buffer(&buf);
			as_serializer_deserialize(ser, &buf, val);
			break;
		}
		case CL_BLOB:
		case CL_JAVA_BLOB:
		case CL_CSHARP_BLOB:
		case CL_PYTHON_BLOB:
		case CL_RUBY_BLOB:
		case CL_ERLANG_BLOB:
		default : {
			*val = NULL;
			uint8_t * raw = malloc(sizeof(bin->object.sz));
			memcpy(raw, bin->object.u.blob, bin->object.sz);
			as_bytes * b = as_bytes_new(raw, bin->object.sz, true /*ismalloc*/);
			b->type = bin->object.type;
			*val = (as_val *) b;
			break;
		}
	}
}


void clbins_to_asrecord(cl_bin * bins, uint32_t nbins, as_record * r) 
{
	uint32_t n = nbins < r->bins.capacity ? nbins : r->bins.capacity;
	for ( int i = 0; i < n; i++ ) {
		switch(bins[i].object.type) {
			case CL_NULL: {
				as_record_set_nil(r, bins[i].bin_name);
				break;
			}
			case CL_INT: {
				as_record_set_int64(r, bins[i].bin_name, bins[i].object.u.i64);
				break;
			}
			case CL_STR: {
				as_record_set_str(r, bins[i].bin_name, bins[i].object.u.str);
				break;
			}
			case CL_LIST:
			case CL_MAP: {

				as_val * val = NULL;

				as_buffer buffer;
				buffer.data = (uint8_t *) bins[i].object.u.blob;
				buffer.size = bins[i].object.sz;

				as_serializer ser;
				as_msgpack_init(&ser);
				as_serializer_deserialize(&ser, &buffer, &val);
				as_serializer_destroy(&ser);

				as_record_set(r, bins[i].bin_name, (as_bin_value *) val);
				break;
			}
			default: {
				as_bytes * b = as_bytes_empty_new(bins[i].object.sz);
				as_bytes_append(b, (uint8_t *) bins[i].object.u.blob, bins[i].object.sz);
				as_record_set_bytes(r, bins[i].bin_name, b);
				break;
			}
		}
	}
}


void aspolicywrite_to_clwriteparameters(as_policy_write * policy, as_record * rec, cl_write_parameters * wp) 
{
	if ( !policy || !rec || !wp ) {
		return;
	}

	wp->unique = policy->exists == AS_POLICY_EXISTS_CREATE;
	wp->unique_bin = false;

	wp->use_generation = false;
	wp->use_generation_gt = false;
	wp->use_generation_dup = false;
	
	wp->timeout_ms = policy->timeout;
	wp->record_ttl = rec->ttl;

	switch(policy->gen) {
		case AS_POLICY_GEN_EQ:
			wp->generation = rec->gen;
			wp->use_generation = true;
			break;
		case AS_POLICY_GEN_GT:
			wp->generation = rec->gen;
			wp->use_generation_gt = true;
			break;
		case AS_POLICY_GEN_DUP:
			wp->generation = rec->gen;
			wp->use_generation_dup = true;
			break;
		default:
			break;
	}

	switch(policy->mode) {
		case AS_POLICY_WRITEMODE_ASYNC:
			wp->w_pol = CL_WRITE_ASYNC;
			break;
		case AS_POLICY_WRITEMODE_ONESHOT:
			wp->w_pol = CL_WRITE_ONESHOT;
			break;
		default:
			wp->w_pol = CL_WRITE_RETRY;
			break;
	}
}

void aspolicyoperate_to_clwriteparameters(as_policy_operate * policy, cl_write_parameters * wp) 
{
	if ( !policy || !wp ) {
		return;
	}
	
	wp->unique = false;
	wp->unique_bin = false;

	wp->use_generation = false;
	wp->use_generation_gt = false;
	wp->use_generation_dup = false;
	
	wp->timeout_ms = policy->timeout;
	wp->record_ttl = 0;

	switch(policy->gen) {
		case AS_POLICY_GEN_EQ:
			wp->generation = policy->generation;
			wp->use_generation = true;
			break;
		case AS_POLICY_GEN_GT:
			wp->generation = policy->generation;
			wp->use_generation_gt = true;
			break;
		case AS_POLICY_GEN_DUP:
			wp->generation = policy->generation;
			wp->use_generation_dup = true;
			break;
		default:
			break;
	}

	switch(policy->mode) {
		case AS_POLICY_WRITEMODE_ASYNC:
			wp->w_pol = CL_WRITE_ASYNC;
			break;
		case AS_POLICY_WRITEMODE_ONESHOT:
			wp->w_pol = CL_WRITE_ONESHOT;
			break;
		default:
			wp->w_pol = CL_WRITE_RETRY;
			break;
	}
}
