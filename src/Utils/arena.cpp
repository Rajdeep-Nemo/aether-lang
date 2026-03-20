#include <arena.hpp>
#include <iostream>
// Initialize the arena
void init_arena(Arena *arena, const size_t capacity) {
    auto *first_chunk = new ArenaChunk;
    first_chunk->arena_buffer = new std::byte[capacity];
    first_chunk->next = nullptr;
    arena->head = first_chunk;
    arena->current = first_chunk;
    arena->arena_capacity = capacity;
    arena->arena_offset = 0;
}
// Allocate memory for the arena
void *alloc_arena(Arena *arena, const size_t size) {
    if (arena->arena_offset + size > arena->arena_capacity) {
        auto *new_chunk = new ArenaChunk;
        new_chunk->arena_buffer = new std::byte[arena->arena_capacity];
        new_chunk->next = nullptr;
        arena->current->next = new_chunk;
        arena->current = new_chunk;
        arena->arena_offset = 0;
    }
    std::byte *ptr = arena->current->arena_buffer + arena->arena_offset;
    arena->arena_offset = arena->arena_offset + size;
    return ptr;
}
// Free the memory
void free_arena(Arena *arena) {
    auto chunk = arena->head;
    while (chunk != nullptr) {
        const auto next = chunk->next;
        delete[] chunk->arena_buffer;
        delete chunk;
        chunk = next;
    }
    arena->head = nullptr;
    arena->current = nullptr;
    arena->arena_capacity = 0;
    arena->arena_offset = 0;
}
