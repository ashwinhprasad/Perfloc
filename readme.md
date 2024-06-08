GThis is an implementation of a general-purpose memory allocator library written in C (Inspired by Postgres's memory contexts)

## Motivation For Custom Memory Allocator

Languages like C do not guarantee memory safety. In producation scale applications that use the general C library functions like malloc/free for memory allocation and deallocation, identifying potential memory leaks via dangling pointers and uncollected garbage is tedious. To address such issues, languages like rust and zig take a different approach.

1) Rust's ownership model

Rust has an ownership and borrowing model where a value can only have one owner at a time and when the owner goes out of scope, the memory is collected (irrespective of whether it is on stack or heap). The lanugage itself proviedes certain compile time rules that evaluates the program and makes sure that it is memory safe.


2) Zig's Allocators

Zig does not directly allow the programmer to allocate to the heap. Instead, it introduces "Allocators" within the framework of the language itself. The allocator has access to it's own associated dynamic memory and when the allocator is freed, the memory is also released. This ensures that the entire memory occupied by the allocator is released all at once and consequently, prevents memory leaks.


3) Java Virtual Machine (JVM)

Java does things quite differently than these traditional system languages. The java process has it's own virtual memory which acts as an abstraction between the operating system memory and the program. Allocations on the heap does not reside directly in OS memory but on this virtual memory. The language uses garbage collectors that scan through the entire heap and release the garbage in cycles. 


### Why perfloc ?

Java's virtual memory concept is good in the aspect that we already have a huge chunk of memory to which we can allocate to, instead of using system calls like sbrk(), brk() or mmap() for every allocation on the heap. But the failure of this model to perform in real time environments is the fact the the mutator program itself does not have any control over when the memory should be freed. The garbage collector does not take any help or hints from the mutator program but rather have to scan the entire heap or entire parts of heap and has to go through the phases of mark, sweep and compact. Some of these steps lead to STW (stop the world) pauses. They halt the mutator program's for a while. These pauses are usually in milliseconds but there are cases where these pauses could extend upto minutes in production systems.

Is there anyhow we could take the advantages of this system but hint the garbage collecting module based on the knowledge of the mutator program such that we could optimise the GC time ? Zig provides a good answer to the question but it does not take all the advantages above system.

Everytime a zig's allocator is created or evertyime a resize is required, a system call is made. A solution that builds upon the zig's memory allocator model but proceeds to further minimize the number of system calls would be more benefcial. This is where perfloc comes in. It incorporates the allocator concept but instead of directly taking memory from the OS, there is an intermediate layer that acts as a big memory chunk (Like java's virtual memory). The only time a system call is made, is when the intermediate memory chunk's size is not enough. 

As soon as we do memory pooling like this in the programming layer, the need to have models like ownership and borrowing and constantly having to fight with the borrow checker can be avoided since memory leaks can e avoided regardless by simple releasing the memory chunk of the current function scope.

*Note: There are also cases where we have to forcefully use unsafe code blocks in rust just to overcome the borrow checker's constrainsts and achieve the functionality we want, when we know that the code does what it's intended to do and the borrow checker can relax. This defeats the whole purpose of the model the language is built upon**

With an efficient memory allocator library like this, C overcomes all it's defauly hurdles while also keeping the language's simplicity, fine grained memory control and performance.


## Perfloc Internals

Perfloc creates a Top Memory Chunk (similar to java's virtual memory). But objects cannot be directly allocated into this area. Each function or scope or process creates it's own "Process Memory Chunk" that is created from the top memory chunk. Objects can be allocated into this process memory chunk. The lifetime of a process memory chunk is not exceed the function scope within which it is initialized. This ensures that the system calls are minimised.
