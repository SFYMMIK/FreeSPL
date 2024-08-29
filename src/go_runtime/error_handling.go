package main

/*
#include "error_handling.h"
*/
import "C"
import "unsafe"

// checkError is a Go function that interfaces with the C error handler.
func checkError(condition bool, message string) {
    if condition {
        msg := C.CString(message)
        C.handle_error(msg)
        C.free(unsafe.Pointer(msg)) // Free the allocated memory
    }
}

func main() {
    // Example usage of error handling
    var divisor int = 0

    checkError(divisor == 0, "Division by zero")

}
