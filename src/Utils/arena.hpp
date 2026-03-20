#ifndef ARENA_HPP
#define ARENA_HPP
#include <cstddef>
// Struct to hold the arena
struct Arena {
    std::byte *arena_buffer;
    size_t arena_capacity;
    size_t arena_offset;
};
// Initialize the arena
void init_arena(Arena *arena, size_t capacity);
// Allocate memory for the arena
void *alloc_arena(Arena *arena, size_t size);
// Free the memory
void free_arena(Arena *arena);

#endif