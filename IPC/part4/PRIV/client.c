#include <windows.h>
#include <stdio.h>
#include <rpcdce.h>
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
        printf("RpcBindingFromStringBinding failed: %d\n", status);
        return status;
    }

    // Define and populate the auth identity structure
    SEC_WINNT_AUTH_IDENTITY identity;
    memset(&identity, 0, sizeof(identity));

    identity.User           = (unsigned short*)L"YourUserName";  // replace with actual username
    identity.UserLength     = wcslen((wchar_t*)identity.User);
    identity.Domain         = (unsigned short*)L"YourDomain";    // replace with actual domain or leave empty for local
    identity.DomainLength   = wcslen((wchar_t*)identity.Domain);
    identity.Password       = (unsigned short*)L"YourPassword";  // replace with actual password
    identity.PasswordLength = wcslen((wchar_t*)identity.Password);
    identity.Flags          = SEC_WINNT_AUTH_IDENTITY_UNICODE;

    RPC_SECURITY_QOS qos;
    memset(&qos, 0, sizeof(qos));
    qos.Version = RPC_C_SECURITY_QOS_VERSION;
    qos.Capabilities = RPC_C_QOS_CAPABILITIES_DEFAULT;
    qos.IdentityTracking = RPC_C_QOS_IDENTITY_STATIC;
    qos.ImpersonationType = RPC_C_IMP_LEVEL_IMPERSONATE;

    // Set authentication
    status = RpcBindingSetAuthInfoEx(
        ImplicitHandle,
        NULL,
        RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
        RPC_C_AUTHN_WINNT,
        &identity,
        RPC_C_AUTHZ_NONE,
        &qos);

    if (status) {
        printf("RpcBindingSetAuthInfoEx failed: %d\n", status);
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
