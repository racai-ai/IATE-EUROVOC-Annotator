#include "http_server.h"
#include "modules/all.h"


http_func getHttpFunc(const char *name){
	if(strcmp(name,"/annotate")==0)return ANNOTATE_handler;

	return NULL;
}
