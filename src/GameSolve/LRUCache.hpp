/**
*
*/

#ifndef CURLIBS_CACHE_LRUCACHE_HPP
#define CURLIBS_CACHE_LRUCACHE_HPP

#include <cstdio>
#include <functional>

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
    LLNode *free = nullptr;
    LLNode *cache = nullptr;
    LLNode **map = nullptr;
    
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
    *
    */
    void insert(const Key &k, const Value &v) {
        // If it is full, delete an old item to make room first
        LLNode *placement;
        if (this->used == this->max_capacity) {
            placement = this->delete_oldest();
            if (placement == nullptr) {
                std::printf("Broke A\n");
            }
        } else {
            placement = &(this->cache[this->used++]);
            if (placement == nullptr) {
                std::printf("Broke B\n");
            }
        }

        // First put the key value pair into the cache
        placement->key = k;
        placement->value = v;
        // Then hook it up to the front of the linked list
        placement->newer = nullptr;
        if (this->newest == nullptr) {
            this->newest = placement;
            this->oldest = placement;
        } else {
            this->newest->newer = placement;
        }
        this->newest = placement;
        // Then find the bucket it belongs to - if the bucket is empty, put a pointer to it there.
        // If the bucket is not empty, follow the pointer trail till you get to the end
        // and then add it there
        unsigned int bucket = std::hash<Key>{}(k) % this->max_capacity;
        if (this->map[bucket] == nullptr) {
            this->map[bucket] = placement;
        } else {
            LLNode *item_in_bucket = this->map[bucket];
            while (item_in_bucket->bucket_next != nullptr) {
                item_in_bucket = item_in_bucket->bucket_next;
            }
            item_in_bucket->bucket_next = placement;
        }
        placement->bucket_next = nullptr;
    }

    void update(LLNode *node) {
        // Remove the node from the linked list
        LLNode *older = node->older;
        LLNode *newer = node->newer;
        // If the node is already at the front of the linked list, there's nothing we have to do
        if (newer != nullptr) {
            if (older == nullptr) {
                // If the node is the oldest one in the linked list, remove and add to front
                this->oldest = newer;
                newer->older = nullptr;
                node->older = this->newest;
                node->newer = nullptr;
                this->newest = node;
            } else {
                // If the node is in the middle of the linked list, connect its neighbors
                newer->older = older;
                older->newer = newer;
                node->newer = nullptr;
                node->older = this->newest;
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
        if (search == nullptr) {
            return false;
        } else {
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

