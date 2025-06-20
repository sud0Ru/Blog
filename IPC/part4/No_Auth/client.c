#include <windows.h>
#include <stdio.h>
#include "exampleInterface.h"
 
int main() {
    RPC_STATUS status;
    RPC_CSTR stringBinding = NULL;
    int result;
 
    status = RpcStringBindingCompose(
        NULL,
        (RPC_CSTR)"ncacn_ip_tcp",
        (RPC_CSTR)"192.168.177.100",
        (RPC_CSTR)"9999",
        NULL,
        &stringBinding);
 
    if (status) {
        printf("RpcStringBindingCompose failed: %d\n", status);
        return status;
    }
 
    status = RpcBindingFromStringBinding(stringBinding, &ImplicitHandle);
    if (status) {
        printf("RpcBindingFromStri ngBinding failed: %d\n", status);
        return status;
    }
    RpcTryExcept {
       result = PrintString((unsigned char*)"Hello, RPC Server!");
        printf("Server returned: %d\n", result);
    }
    RpcExcept(1) {
        printf("Runtime reported exception %d\n", RpcExceptionCode());
    }
    RpcEndExcept;
 
    RpcStringFree(&stringBinding);
    RpcBindingFree(&ImplicitHandle);
 
    return 0;
}
 
// Memory allocation functions
void* __RPC_USER midl_user_allocate(size_t size) {
    return malloc(size);
}
 
void __RPC_USER midl_user_free(void* p) {
    free(p);
}