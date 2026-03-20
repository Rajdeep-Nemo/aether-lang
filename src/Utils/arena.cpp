#include <arena.hpp>
#include <iostream>

// Initialize the arena
void init_arena(Arena *arena, const size_t capacity) {
    arena->arena_buffer = new std::byte[capacity];
    arena->arena_capacity = capacity;
    arena->arena_offset = 0;
}
// Allocate memory for the arena
void *alloc_arena(Arena *arena, const size_t size) {
    if (arena->arena_offset + size > arena->arena_capacity) {
        std::cerr << "Arena out of memory." << std::endl;
        return nullptr;
    }
    std::byte *ptr = arena->arena_buffer + arena->arena_offset;
    arena->arena_offset = arena->arena_offset + size;
    return ptr;
}
// Free the memory
void free_arena(Arena *arena) {
    delete[] arena->arena_buffer;
    arena->arena_buffer = nullptr;
    arena->arena_capacity = 0;
    arena->arena_offset = 0;
}
