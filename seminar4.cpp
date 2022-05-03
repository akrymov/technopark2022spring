#include <iostream>
#include <string>

size_t str_hash(const std::string& data) {
    size_t hash = 0;
    for (char i : data)
        hash = hash * 13 + i;
    return hash;
}

template<class T> struct Hasher {
    size_t operator ()(const T& data) {
        return data;
    }

};

template<> struct Hasher<int> {
    size_t operator ()(const int& data) {
        return data;
    }
};

template<> struct Hasher<std::string> {
    size_t operator ()(const std::string& data) {
        return str_hash(data);
    }
};


template <class Key, class Value, class Hash = Hasher<Key>>
class HashTable {
    static constexpr size_t LIMIT = 7;

    struct Node {
        Key key;
        Value value;

        Node* next;

        Node(const Key& a_key, const Value& a_value, Node* a_next): key(a_key), value(a_value), next(a_next) {
        }
    };
public:
    explicit HashTable(Hash hash = Hash()) : buckets(nullptr), items_count(0), buckets_count(0), hash(hash) {
    }

    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;
    HashTable(HashTable&&) = delete;
    HashTable& operator=(HashTable&&) = delete;

    ~HashTable() {
        for (int i = 0; i < buckets_count; i++) {
            Node* cur = buckets[i];
            while (cur) {
                Node* tmp = cur;
                cur = cur->next;
                delete tmp;
            }
        }
        delete[] buckets;
    }

    bool insert(const Key& key, const Value& value) {
        if (find(key)) {
            return false;
        }

        if (!buckets || items_count > buckets_count * LIMIT) {
            grow();
        }

        size_t bucket_idx = hash(key) % buckets_count;
        buckets[bucket_idx] = new Node(key, value, buckets[bucket_idx]);
        items_count++;
        return true;

    }

    Value* find(const Key& key) {
        if (!buckets) {
            return nullptr;
        }

        size_t bucket_idx = hash(key) % buckets_count;
        Node* cur = buckets[bucket_idx];
        while (cur) {
            if (cur->key == key) {
                return &cur->value;
            }
            cur = cur->next;
        }
        return nullptr;
    }

    bool erase(const Key& key) {
        if (!buckets) {
            return false;
        }
        size_t bucket_idx = hash(key) % buckets_count;
        Node* cur = buckets[bucket_idx];
        Node** prev = &buckets[bucket_idx];
        while (cur) {
            if (cur->key == key) {
                *prev = cur->next;
                items_count--;
                delete cur;
                return true;
            }
            prev = &(cur->next);
            cur = cur->next;
        }
        return false;
    }

private:
    void grow() {
        size_t new_buckets_count = std::max((size_t)7, buckets_count * 2);

        Node** new_buckets = new Node*[new_buckets_count];
        for (int i = 0; i < new_buckets_count; i++) {
            new_buckets[i] = nullptr;
        }

        for (int i = 0; i < buckets_count; i++) {
            Node* cur = buckets[i];
            while (cur) {
                size_t bucket_idx = hash(cur->key) % new_buckets_count;
                Node* tmp = cur;
            	  cur = cur->next;
                tmp->next = new_buckets[bucket_idx];
                new_buckets[bucket_idx] = tmp;
                
            }
        }

        delete[] buckets;
        buckets = new_buckets;
        buckets_count = new_buckets_count;
    }

    Node** buckets;
    size_t items_count;
    size_t buckets_count;

    Hash hash;

};


int main() {
    HashTable<std::string, int> htable;
    std::string key;
    char operation = '\0';
    while (std::cin >> operation >> key) {
        bool res = false;
        switch (operation) {
            case '+':
                res = htable.insert(key, 0);
                break;
            case '-':
                res = htable.erase(key);
                break;
            case '?':
                res = (htable.find(key) != nullptr);
                break;
            default:
                return 1;
        };
        if (res) {
            std::cout << "OK" << std::endl;
        } else {
            std::cout << "FAIL" << std::endl;
        }
    }
    return 0;
}
