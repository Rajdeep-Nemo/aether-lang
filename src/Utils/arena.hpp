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
void arena_init(Arena *arena, size_t capacity);
// Allocate memory for the arena
void *arena_alloc(Arena *arena, size_t size);
// Free the memory
void arena_free(Arena *arena);

#endif