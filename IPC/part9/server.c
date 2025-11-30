#include <windows.h>
#include <stdio.h>
#include "example.h"

// Function implementation
void PrintString(const char* str) {
    printf("Received string: %s\n", str);
}

RPC_STATUS CALLBACK SecurityCallback(RPC_IF_HANDLE Interface, void* pBindingHandle){
    printf("The security callback is called");
    return RPC_S_OK; // Whoever binds to the interface, we will allow the connection
}
// Entry point
int main() {
    RPC_STATUS status;
    RPC_BINDING_VECTOR* bindingVector = NULL;

    status = RpcServerUseProtseqEp(
        (RPC_CSTR)"ncacn_ip_tcp",         // Use ncalrpc protocol
        RPC_C_PROTSEQ_MAX_REQS_DEFAULT,
        (RPC_CSTR)"41337", // Endpoint name
        NULL);

    if (status) {
        printf("RpcServerUseProtseqEp failed: %d\n", status);
        return status;
    }

     const char* pszSpn = NULL; // Server principal name, typically this would be set to something meaningful

    // Register authentication info
    status = RpcServerRegisterAuthInfo(
        pszSpn,             // Server principal name
        RPC_C_AUTHN_WINNT,  // using NTLM as authentication service provider
        NULL,               // Use default key function, which is ignored for NTLM SSP
        NULL                // No arg for key function
    );


    status = RpcServerRegisterIf2(
        Example_v1_0_s_ifspec,
        NULL,
        NULL,
        RPC_IF_ALLOW_SECURE_ONLY | RPC_IF_ALLOW_CALLBACKS_WITH_NO_AUTH,
        RPC_C_LISTEN_MAX_CALLS_DEFAULT,
        (unsigned)-1,
        SecurityCallback); 

    if (status) {
        printf("RpcServerRegisterIf2 failed: %d\n", status);
        return status;
    }

    status = RpcServerInqBindings(&bindingVector);
    if (status) {
        printf("RpcServerInqBindings failed: %d\n", status);
        return status;
    }

    status = RpcEpRegister(
        Example_v1_0_s_ifspec,
        bindingVector,
        NULL,
        (RPC_CSTR)"Example RPC Server");

    if (status) {
        printf("RpcEpRegister failed: %d\n", status);
        return status;
    }

    status = RpcServerListen(1, RPC_C_LISTEN_MAX_CALLS_DEFAULT, FALSE);
    if (status) {
        printf("RpcServerListen failed: %d\n", status);
        return status;
    }

    RpcMgmtWaitServerListen();
    return 0;
}

// Memory allocation functions
void* __RPC_USER midl_user_allocate(size_t size) {
    return malloc(size);
}

void __RPC_USER midl_user_free(void* p) {
    free(p);
}
