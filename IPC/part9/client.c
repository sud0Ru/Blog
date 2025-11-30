#include <windows.h>
#include <stdio.h>
#include "example.h"

int main() {
    RPC_STATUS status;
    RPC_CSTR stringBinding = NULL;
    printf("Try to Build string binding");

    status = RpcStringBindingCompose(
        NULL,
        (RPC_CSTR)"ncacn_ip_tcp",
        (RPC_CSTR)"192.168.177.177",
        (RPC_CSTR)"41337",
        NULL,
        &stringBinding);
    
    RPC_SECURITY_QOS secQos;
	secQos.Version = RPC_C_SECURITY_QOS_VERSION_1;				// use version 1 as documented at https://docs.microsoft.com/en-us/windows/win32/api/rpcdce/ns-rpcdce-rpc_security_qos
	secQos.Capabilities = RPC_C_QOS_CAPABILITIES_MUTUAL_AUTH;	// set mutual authentication
	secQos.IdentityTracking = RPC_C_QOS_IDENTITY_DYNAMIC;	

    if (status) {
        printf("RpcStringBindingCompose failed: %d\n", status);
        return status;
    }
 
    status = RpcBindingFromStringBinding(stringBinding, &ImplicitHandle);
    if (status) {
        printf("RpcBindingFromStringBinding failed: %d\n", status);
        return status;
    }
   
    
    RPC_SECURITY_QOS SecurityQOS;
    memset(&SecurityQOS, 0, sizeof(SecurityQOS));
    SecurityQOS.Version = RPC_C_SECURITY_QOS_VERSION;
    SecurityQOS.Capabilities = RPC_C_QOS_CAPABILITIES_DEFAULT;
    SecurityQOS.IdentityTracking = RPC_C_QOS_IDENTITY_DYNAMIC;
    SecurityQOS.ImpersonationType = RPC_C_IMP_LEVEL_IDENTIFY;
    
    status = RpcBindingSetAuthInfoEx(
    ImplicitHandle,		// the client's binding handle
    NULL,			// the server's service principale name (SPN)
    RPC_C_AUTHN_LEVEL_PKT,	// authentication level PKT
    RPC_C_AUTHN_WINNT,		// using NTLM as authentication service provider
    NULL,			// use current thread credentials
    0,		// authorization based on the provided SPN
    &SecurityQOS			// Quality of Service structure
);
    RpcTryExcept {
        PrintString("Hello, RPC Server!");
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
