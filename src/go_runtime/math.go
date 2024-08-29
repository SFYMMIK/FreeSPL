package main

import "fmt"

// Function to handle division, with error handling directly in Go
func divide(a int, b int) int {
    if b == 0 {
        fmt.Println("Runtime Error: Division by zero")
        return 0 // Handle division by zero
    }
    return a / b
}

// Function to handle modulo, with error handling directly in Go
func modulo(a int, b int) int {
    if b == 0 {
        fmt.Println("Runtime Error: Modulo by zero")
        return 0 // Handle modulo by zero
    }
    return a % b
}

func main() {
    var a, b int
    fmt.Print("Enter two integers: ")
    fmt.Scanf("%d %d", &a, &b)

    fmt.Printf("Add: %d\n", a+b)
    fmt.Printf("Subtract: %d\n", a-b)
    fmt.Printf("Multiply: %d\n", a*b)
    fmt.Printf("Divide: %d\n", divide(a, b))
    fmt.Printf("Modulo: %d\n", modulo(a, b))
}
