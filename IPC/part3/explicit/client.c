#include <windows.h>
#include <rpc.h>
#include <stdio.h>
#include <stdlib.h>

#include "ExampleInterface.h" // Generated header file

int main() {
    RPC_STATUS status;
    RPC_BINDING_HANDLE binding;
    int result;

    // Create the string binding
    status = RpcBindingFromStringBinding(
        (RPC_CSTR)"ncalrpc:[ExampleEndpoint]",  // String binding
        &binding);
    if (status) {
        printf("RpcBindingFromStringBinding returned 0x%x\n", status);
        exit(status);
    }

    // Make the RPC call
    result = HelloWorld(binding,4);
    printf("Received result from server: %d\n", result);

    // Free the binding handle
    status = RpcBindingFree(&binding);
    if (status) {
        printf("RpcBindingFree returned 0x%x\n", status);
        exit(status);
    }

    return 0;
}

// Memory allocation and deallocation functions
void* __RPC_USER midl_user_allocate(size_t size) {
    return malloc(size);
}

void __RPC_USER midl_user_free(void* ptr) {
    free(ptr);
}
