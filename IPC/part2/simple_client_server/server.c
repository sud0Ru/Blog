#include <windows.h>
#include <stdio.h>
#include "exampleInterface.h"
 
// Function implementation
int PrintString(const unsigned char* str) {
    printf("Received string: %s\n", str);
    return 3;
}
 
// Entry point
int main() {
    RPC_STATUS status;
    RPC_BINDING_VECTOR* bindingVector = NULL;
 
    status = RpcServerUseProtseqEp(
        (RPC_CSTR)"ncalrpc",         // Use ncalrpc protocol
        RPC_C_PROTSEQ_MAX_REQS_DEFAULT,
        (RPC_CSTR)"example_endpoint", // Endpoint name
        NULL);
 
    if (status) {
        printf("RpcServerUseProtseqEp failed: %d\n", status);
        return status;
    }
 
    status = RpcServerRegisterIf2(
        ExampleInter_v1_0_s_ifspec,
        NULL,
        NULL,
        RPC_IF_ALLOW_LOCAL_ONLY,
        RPC_C_LISTEN_MAX_CALLS_DEFAULT,
        (unsigned)-1,
        NULL);
 
    if (status) {
        printf("RpcServerRegisterIf2 failed: %d\n", status);
        return status;
    }
 
    printf("Server is listening ...\n");
    status = RpcServerListen(1, RPC_C_LISTEN_MAX_CALLS_DEFAULT, FALSE);
    if (status) {
        printf("RpcServerListen failed: %d\n", status);
        return status;
    }
    return 0;
}
 
// Memory allocation functions
void* __RPC_USER midl_user_allocate(size_t size) {
    return malloc(size);
}
 
void __RPC_USER midl_user_free(void* p) {
    free(p);
}