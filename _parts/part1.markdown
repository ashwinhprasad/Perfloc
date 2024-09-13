---
layout: default
title: "Part 1: Motivation"
date: 2024-08-27
---

Languages like C do not guarantee memory safety. In production-scale applications that use the general C library functions like malloc/free for memory allocation and deallocation, identifying potential memory leaks via dangling pointers and uncollected garbage is tedious. To address such issues, languages like Rust and Zig take a different approach.

## 1) Rust's Ownership Model

Rust has an ownership and borrowing model where a value can only have one owner at a time and when the owner goes out of scope, the memory is collected (irrespective of whether it is on stack or heap). The language itself provides certain compile time rules that evaluate the program and make sure that it is memory-safe.

## 2) Zig's Allocators

Zig does not directly allow the programmer to allocate to the heap. Instead, it introduces “Allocators” within the framework of the language itself. The allocator has access to its own associated dynamic memory and when the allocator is freed, the memory is also released. This ensures that the entire memory occupied by the allocator is released all at once and consequently, prevents memory leaks.


## 3) Java Virtual Machine (JVM)

Java does things quite differently than these traditional system languages. The Java process has its own virtual memory which acts as an abstraction between the operating system memory and the program. Allocations on the heap do not reside directly in OS memory but in this virtual memory. The language uses garbage collectors that scan through the entire heap and release the garbage in cycles.


# What I'm hoping to do with this custom memory allocator (Perfloc)?

Java’s virtual memory concept is good in the aspect that we already have a huge chunk of memory to which we can allocate objects, instead of using system calls like sbrk(), brk() or mmap() for every allocation on the heap. However, the failure of this model to perform in real-time environments is the fact that the mutator program itself does not have any control over when the memory should be freed. The garbage collector does not take any help or hints from the mutator program but rather has to scan the entire heap or entire parts of the heap and has to go through the phases of mark, sweep and compact. Some of these steps lead to STW (stop the world) pauses. They halt the mutator program for a while. These pauses are usually in milliseconds but there are cases where these pauses could extend up to minutes in production systems.

Is there anyhow we could take all the advantages of this system but hint the garbage collecting module based on the knowledge of the mutator program such that we could optimize the GC time? Zig provides a good answer to the question but it does not take all the advantages above system. Every time a zig’s allocator is created or every time a resize is required, a system call is made. A solution that builds upon Zig’s memory allocator model but proceeds to further minimize the number of system calls would be more beneficial. This is where Perfloc comes in. It incorporates the allocator concept but instead of directly taking memory from the OS, there is an intermediate layer that acts as a big memory chunk (Like java’s virtual memory). The only time a system call is made is when the intermediate memory chunk’s size is not enough.

As soon as we do memory pooling like this in the programming layer, the need to have models like ownership and borrowing and constantly having to fight with the borrow checker can be avoided since memory leaks can be avoided regardless by simply releasing the memory chunk of the current function scope.

*Note: There are also cases where we have to forcefully use unsafe code blocks in Rust just to overcome the borrow checker’s constraints and achieve the functionality we want when we know that the code does what it’s intended to do and the borrow checker can relax. This defeats the whole purpose of the model the language is built upon*

With an efficient memory allocator library like this, C overcomes all its default hurdles while also keeping the language’s simplicity, fine-grained memory control and performance.