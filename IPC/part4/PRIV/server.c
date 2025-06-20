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
    const char* pszSpn = NULL; // Server principal name
 
    status = RpcServerUseProtseqEp(
        (RPC_CSTR)"ncacn_ip_tcp",         
        RPC_C_PROTSEQ_MAX_REQS_DEFAULT,
        (RPC_CSTR)"9999",
        NULL);
 
    if (status) {
        printf("RpcServerUseProtseqEp failed: %d\n", status);
        return status;
    }

    // Register authentication info
    status = RpcServerRegisterAuthInfo(
        pszSpn,             // Server principal name
        RPC_C_AUTHN_WINNT,  // using NTLM as authentication service provider
        NULL,               // Use default key function, which is ignored for NTLM SSP
        NULL                // No arg for key function
    );

 
    status = RpcServerRegisterIf2(
        ExampleInter_v1_0_s_ifspec,
        NULL,
        NULL,
        0,
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