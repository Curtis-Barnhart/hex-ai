/**
* TODO: make one hash object for whole thing so doesn't need to keep constructing?
*/

#ifndef CURLIBS_CACHE_LRUCACHE_HPP
#define CURLIBS_CACHE_LRUCACHE_HPP

#include <functional>
#include <iostream>

namespace Cache {

template<class Key, class Value>
class LRUCache {
public:
    struct LLNode {
        LLNode *newer = nullptr;
        LLNode *older = nullptr;
        LLNode *bucket_next = nullptr;
        LLNode *bucket_prev = nullptr;
        Value value;
        Key key;
    };
    unsigned int max_capacity = 0;
    unsigned int used = 0;
    LLNode *oldest = nullptr;
    LLNode *newest = nullptr;
    LLNode *cache = nullptr;
    LLNode **map = nullptr;

    void data_dump() {
        std::cout << "newest: " << this->newest << "\noldest: " << this->oldest << "\n";

        for (int i = 0; i < this->max_capacity; i++) {
            std::cout << i << ": " << this->map[i] << "\n";
        }

        for (int i = 0; i < this->max_capacity; i++) {
            std::cout << &this->cache[i] << ": " << this->cache[i].value
                      << "{ newer: " << this->cache[i].newer
                      << ", older: " << this->cache[i].older
                      << ", bucket_next: " << this->cache[i].bucket_next
                      << ", bucket_prev: " << this->cache[i].bucket_prev
                      << " }\n";
        }
    }
    
    LRUCache(unsigned int capacity) {
        cache = new LLNode[capacity];
        map = new LLNode *[capacity];
        for (unsigned int i = 0; i < capacity; i++) {
            map[i] = nullptr;
        }
        this->max_capacity = capacity;
    }

    ~LRUCache() {
        delete[] cache;
        delete[] map;
    }

    /**
    * Assumes that there is an oldest node to delete
    */
    LLNode *delete_oldest() {
        // Remove the oldest node from the linked list
        LLNode *to_remove = this->oldest;
        this->oldest = to_remove->newer;
        this->oldest->older = nullptr;

        // Now remove the node from the hashmap
        LLNode *prev = to_remove->bucket_prev;
        LLNode *next = to_remove->bucket_next;
        if (prev == nullptr) {
            if (next == nullptr) {
                // If to_remove was the only one in its bucket, remove the map's pointer to it
                this->map[std::hash<Key>{}(to_remove->key) % this->max_capacity] = nullptr;
            } else {
                // If to_remove was the first thing of several in its bucket,
                // you need the bucket to point to the next thing after to_remove
                this->map[std::hash<Key>{}(to_remove->key) % this->max_capacity] = next;
                next->bucket_prev = nullptr;
            }
        } else {
            if (next == nullptr) {
                // If to_remove is the last thing in the bucket, just remove it
                prev->bucket_next = nullptr;
            } else {
                // If to_remove was in the middle of the bucket, you need to link its neighbors
                prev->bucket_next = next;
                next->bucket_prev = prev;
            }
        }

        return to_remove;
    }

    /**
    * Insert inserts a key value pair into the cache that does not currently exist.
    * Behavior is not defined if the key is already in the cache.
    * @param k Key to insert into the cache.
    * @parak v Value to associate with the given key.
    */
    void insert(const Key &k, const Value &v) {
        // If it is full, delete an old item to make room first
        LLNode *placement;
        if (this->used == this->max_capacity) {
            placement = this->delete_oldest();
        } else {
            placement = &(this->cache[this->used++]);
        }

        // First put the key value pair into the cache
        placement->key = k;
        placement->value = v;
        // Then hook it up to the front of the linked list
        placement->newer = nullptr;
        if (this->newest == nullptr) {
            this->oldest = placement;
        } else {
            this->newest->newer = placement;
            placement->older = this->newest;
        }
        this->newest = placement;
        // Then find the bucket it belongs to - if the bucket is empty, put a pointer to it there.
        // If the bucket is not empty, add placement to the front of it
        unsigned int bucket = std::hash<Key>{}(k) % this->max_capacity;

        placement->bucket_next = this->map[bucket];
        this->map[bucket] = placement;
        placement->bucket_prev = nullptr;

        if (placement->bucket_next != nullptr) {
            placement->bucket_next->bucket_prev = placement;
        }
    }

    void update(LLNode *node) {
        // Remove the node from the linked list
        LLNode *older = node->older;
        LLNode *newer = node->newer;
        // If the node is already at the front of the linked list, there's nothing we have to do
        if (this->newest != node) {
            if (this->oldest == node) {
                // If the node is the oldest one in the linked list, remove and add to front
                this->oldest = newer;
                newer->older = nullptr;
                this->newest->newer = node;
                node->older = this->newest;
                node->newer = nullptr;
                this->newest = node;
            } else {
                // If the node is in the middle of the linked list, connect its neighbors
                newer->older = older;
                older->newer = newer;
                node->newer = nullptr;
                node->older = this->newest;
                node->older->newer = node;
                this->newest = node;
            }
        }
    }

    /**
    *
    */
    bool lookup(const Key &k, Value &v) {
        // First look up to see if there are any items with that key in the map
        LLNode *search = this->map[std::hash<Key>{}(k) % this->max_capacity];
        if (search != nullptr) {
            do {
                if (k == search->key) {
                    v = search->value;
                    this->update(search);
                    return true;
                }
                search = search->bucket_next;
            } while (search != nullptr);
        }
        return false;
    }
};

}
#endif // !CURLIBS_CACHE_LRUCACHE_HPP

