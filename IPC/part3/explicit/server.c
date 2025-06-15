#include <windows.h>
#include <stdio.h>
#include <rpc.h>
#include "ExampleInterface.h" // Generated header file

int HelloWorld(handle_t h1, int x) {
    printf("Hello, World from the RPC server!\n");
    printf("Received integer: %d\n", x);
    return 42; // Return an integer value
}

int main() {
    RPC_STATUS status;

    // Register the protocol sequence
    status = RpcServerUseProtseqEp(
        (RPC_CSTR)"ncalrpc",             // Protocol sequence
        RPC_C_PROTSEQ_MAX_REQS_DEFAULT,  // Maximum concurrent calls
        (RPC_CSTR)"ExampleEndpoint",     // Endpoint name
        NULL);                           // Security descriptor
    if (status) {
        printf("RpcServerUseProtseqEp returned 0x%x\n", status);
        exit(status);
    }

    // Register the interface
    status = RpcServerRegisterIf2(
        ExampleInterface_v1_0_s_ifspec,  // Interface to register
        NULL,                            // UUID to associate with this interface
        NULL,                            // MgrTypeUuid
        0,                               // Flags
        RPC_C_LISTEN_MAX_CALLS_DEFAULT,  // Max calls
        (unsigned)-1,                    // Max RPC size
        NULL);                           // Security callback
    if (status) {
        printf("RpcServerRegisterIf2 returned 0x%x\n", status);
        exit(status);
    }

    // Listen for RPC calls
    status = RpcServerListen(
        1,                               // Minimum number of threads
        RPC_C_LISTEN_MAX_CALLS_DEFAULT,  // Max concurrent calls
        FALSE);                          // Don't wait
    if (status) {
        printf("RpcServerListen returned 0x%x\n", status);
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
