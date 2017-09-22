#include "Application.h"
#include "ModuleConfig.h"

static int malloc_count;
static void *counted_malloc(size_t size);
static void counted_free(void *ptr);

ModuleConfig::ModuleConfig(bool start_enabled) : Module(start_enabled) {
}

ModuleConfig::~ModuleConfig()
{

}

bool ModuleConfig::Init()
{
	json_set_allocation_functions(counted_malloc, counted_free);
	//JSON_Value *schema = json_parse_string("{\"name\":\"\"}");
	//JSON_Value *user_data = json_parse_file("user_data.json");
	
	const char *name = NULL;
	JSON_Value *root_value = json_value_init_object();
	JSON_Object *root_object = json_value_get_object(root_value);
		char *serialized_string = NULL;
		json_object_set_string(root_object, "name", "John Smith");
		json_object_set_number(root_object, "age", 25);
		json_object_dotset_string(root_object, "address.city", "Cupertino");
		json_object_dotset_value(root_object, "contact.emails", json_parse_string("[\"email@example.com\",\"email2@example.com\"]"));
		serialized_string = json_serialize_to_string_pretty(root_value);
		puts(serialized_string);
		json_serialize_to_file(root_value, "test.json");
		json_free_serialized_string(serialized_string);
		json_value_free(root_value);
		
	

	return true;
}

bool ModuleConfig::Start()
{
	return true;
}

bool ModuleConfig::CleanUp()
{
	return true;
}


static void *counted_malloc(size_t size) {
	void *res = malloc(size);
	if (res != NULL) {
		malloc_count++;
	}
	return res;
}

static void counted_free(void *ptr) {
	if (ptr != NULL) {
		malloc_count--;
	}
	free(ptr);
}