#pragma once

#if WITH_ALLOC_TRACKING
    #define CFastHashMap std::CMap
#else

#ifdef WIN32
    #pragma push_macro("assert")
#endif

#include <google/dense_hash_map>

//
// Default keys for pointers
//
template <class T>
T* GetEmptyMapKey(T**)
{
    return (T*)-3;
}

template <class T>
T* GetDeletedMapKey(T**)
{
    return (T*)-4;
}

unsigned int GetEmptyMapKey(unsigned int*);
unsigned int GetDeletedMapKey(unsigned int*);

namespace SharedUtil
{
    ////////////////////////////////////////////////
    //
    // Using google::dense_hash_map
    // Requires special keys for empty and deleted items
    //
    template <class K, class V>
    class CFastHashMap : public google::dense_hash_map<K, V>
    {
    public:
        CFastHashMap(uint uiInitialSize) : google::dense_hash_map<K, V>(uiInitialSize)
        {
            this->set_empty_key(GetEmptyMapKey((K*)NULL));
            this->set_deleted_key(GetDeletedMapKey((K*)NULL));
        }
        CFastHashMap()
        {
            this->set_empty_key(GetEmptyMapKey((K*)NULL));
            this->set_deleted_key(GetDeletedMapKey((K*)NULL));
        }
    };

    ////////////////////////////////////////////////
    //
    // CFastHashMap helpers
    //

    // Remove key from collection
    template <class T, class V, class T2>
    bool MapRemove(CFastHashMap<T, V>& collection, const T2& key)
    {
        typename CFastHashMap<T, V>::iterator it = collection.find(key);
        if (it == collection.end())
            return false;
        collection.erase(it);
        return true;
    }

    // Find value in collection
    template <class T, class V, class T2>
    V* MapFind(CFastHashMap<T, V>& collection, const T2& key)
    {
        typename CFastHashMap<T, V>::iterator it = collection.find(key);
        if (it == collection.end())
            return NULL;
        return &it->second;
    }

    // Find value in const collection
    template <class T, class V, class T2>
    const V* MapFind(const CFastHashMap<T, V>& collection, const T2& key)
    {
        typename CFastHashMap<T, V>::const_iterator it = collection.find(key);
        if (it == collection.end())
            return NULL;
        return &it->second;
    }

    // Find value reference in collection
    template <class T, class V, class T2>
    V MapFindRef(CFastHashMap<T, V>& collection, const T2& key)
    {
        typename CFastHashMap<T, V>::iterator it = collection.find(key);
        if (it == collection.end())
            return NULL;
        return it->second;
    }

    // Find value reference in const collection
    template <class T, class V, class T2>
    const V MapFindRef(const CFastHashMap<T, V>& collection, const T2& key)
    {
        typename CFastHashMap<T, V>::const_iterator it = collection.find(key);
        if (it == collection.end())
            return NULL;
        return it->second;
    }

    // Find or add a value in collection
    template <class T, class V, class T2>
    V& MapGet(CFastHashMap<T, V>& collection, const T2& key)
    {
        typename CFastHashMap<T, V>::iterator it = collection.find(key);
        if (it == collection.end())
        {
            collection[key] = V();
            it = collection.find(key);
        }
        return it->second;
    }
}            // namespace SharedUtil

inline std::string GetEmptyMapKey(std::string*)
{
    return std::string("\xFF\xFE");
}

inline std::string GetDeletedMapKey(std::string*)
{
    return std::string("\xFF\xFF");
}

#endif  // WITH_ALLOC_TRACKING
