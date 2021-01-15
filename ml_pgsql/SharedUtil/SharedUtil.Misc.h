#pragma once

#include <cmath>

namespace SharedUtil
{
    class CArgMap;

    template <class T>
    int Round(T value)
    {
        return static_cast<int>(std::floor(value + 0.5f));
    }

    bool ShouldUseInt(double dValue, int* piNumber)
    {
        if (dValue > -0x1000000 && dValue < 0x1000000)
        {
            // float more accurate with this range, but check if we can use int as it is better for compressing
            *piNumber = static_cast<int>(dValue);
            return (dValue == *piNumber);
        }
        else if (dValue >= -0x7FFFFFFF && dValue <= 0x7FFFFFFF)
        {
            // int more accurate with this range
            *piNumber = Round(dValue);
            return true;
        }
        else
        {
            // Value too large for int
            return false;
        }
    }

    //
    // Container helpers for std::list/vector/map
    //

    //
    // std:: container helpers
    //

    // Returns true if the item is in the itemList
    template <class TL, class T>
    bool ListContains(const TL& itemList, const T& item)
    {
        typename TL ::const_iterator it = itemList.begin();
        for (; it != itemList.end(); ++it)
            if (item == *it)
                return true;
        return false;
    }

    // Add item if it does not aleady exist in itemList
    template <class TL, class T>
    void ListAddUnique(TL& itemList, const T& item)
    {
        if (!ListContains(itemList, item))
            itemList.push_back(item);
    }

    //
    // std::list helpers
    //

    // Remove first occurrence of item from itemList
    template <class T>
    void ListRemoveFirst(std::list<T>& itemList, const T& item)
    {
        typename std::list<T>::iterator it = itemList.begin();
        for (; it != itemList.end(); ++it)
            if (item == *it)
            {
                itemList.erase(it);
                break;
            }
    }

    // Remove all occurrences of item from itemList
    template <class T>
    void ListRemove(std::list<T>& itemList, const T& item)
    {
        itemList.remove(item);
    }

    //
    // std::vector helpers
    //

    // Remove first occurrence of item from itemList
    template <class T>
    void ListRemoveFirst(std::vector<T>& itemList, const T& item)
    {
        typename std::vector<T>::iterator it = itemList.begin();
        for (; it != itemList.end(); ++it)
            if (item == *it)
            {
                itemList.erase(it);
                break;
            }
    }

    // Remove all occurrences of item from itemList
    template <class T>
    void ListRemove(std::vector<T>& itemList, const T& item)
    {
        typename std::vector<T>::iterator it = itemList.begin();
        while (it != itemList.end())
        {
            if (item == *it)
                it = itemList.erase(it);
            else
                ++it;
        }
    }

    // Remove item at index from itemList
    template <class T>
    void ListRemoveIndex(std::vector<T>& itemList, uint index)
    {
        if (index < itemList.size())
            itemList.erase(itemList.begin() + index);
    }

    // Append one list onto another
    template <class T>
    void ListAppend(std::vector<T>& itemList, const std::vector<T>& other)
    {
        itemList.insert(itemList.end(), other.begin(), other.end());
    }

    // Clear and reserve memory for the same size
    template <class T>
    void ListClearAndReserve(std::vector<T>& itemList)
    {
        size_t prevSize = itemList.size();
        itemList.clear();
        itemList.reserve(prevSize);
    }

    //
    // std::deque helpers
    //

    // Remove all occurrences of item from itemList
    template <class T>
    void ListRemove(std::deque<T>& itemList, const T& item)
    {
        typename std::deque<T>::iterator it = itemList.begin();
        while (it != itemList.end())
        {
            if (item == *it)
                it = itemList.erase(it);
            else
                ++it;
        }
    }

    ///////////////////////////////////////////////////////////////
    //
    // CIntrusiveListNode
    //
    // Entry in an CIntrusiveList
    //
    ///////////////////////////////////////////////////////////////
    template <class T>
    class CIntrusiveListNode
    {
    public:
        typedef CIntrusiveListNode<T> Node;

        CIntrusiveListNode(T* pOuterItem) : m_pOuterItem(pOuterItem), m_pPrev(NULL), m_pNext(NULL) {}

        T*    m_pOuterItem;            // Item this node is inside
        Node* m_pPrev;
        Node* m_pNext;
    };

    ///////////////////////////////////////////////////////////////
    //
    // CIntrusiveList
    //
    // A linked list with the links stored inside the list items
    //
    ///////////////////////////////////////////////////////////////
    template <typename T>
    class CIntrusiveList
    {
        void operator=(const CIntrusiveList& other);            // Copy will probably not work as expected
        // CIntrusiveList ( const CIntrusiveList& other );       // Default copy constructor is required by dense_hash for some reason

    public:
        class IteratorBase;

    protected:
        typedef CIntrusiveListNode<T> Node;

        size_t m_Size;
        Node*  m_pFirst;
        Node*  m_pLast;
        Node T::*                  m_pNodePtr;                   // Pointer to the CIntrusiveListNode member variable in T
        std::vector<IteratorBase*> m_ActiveIterators;            // Keep track of iterators

    public:
        //
        // CIntrusiveList < T > :: IteratorBase
        //
        class IteratorBase
        {
        protected:
            CIntrusiveList<T>* m_pList;

        public:
            Node* m_pNode;
            IteratorBase(CIntrusiveList<T>* pList, Node* pNode) : m_pList(pList), m_pNode(pNode) { m_pList->m_ActiveIterators.push_back(this); }
            ~IteratorBase() { ListRemove(m_pList->m_ActiveIterators, this); }
            T*           operator*() { return m_pNode->m_pOuterItem; }
            virtual void NotifyRemovingNode(Node* pNode) = 0;
        };

        //
        // CIntrusiveList < T > :: Iterator
        //
        class Iterator : public IteratorBase
        {
        public:
            Iterator(CIntrusiveList<T>* pList, Node* pNode) : IteratorBase(pList, pNode) {}
            bool         operator==(const Iterator& other) const { return IteratorBase::m_pNode == other.m_pNode; }
            bool         operator!=(const Iterator& other) const { return IteratorBase::m_pNode != other.m_pNode; }
            void         operator++() { IteratorBase::m_pNode = IteratorBase::m_pNode->m_pNext; }
            void         operator++(int) { IteratorBase::m_pNode = IteratorBase::m_pNode->m_pNext; }
            virtual void NotifyRemovingNode(Node* pNode)
            {
                if (IteratorBase::m_pNode == pNode)
                    IteratorBase::m_pNode = IteratorBase::m_pNode->m_pNext;
            }
        };

        //
        // CIntrusiveList < T > :: ReverseIterator
        //
        class ReverseIterator : public IteratorBase
        {
        public:
            ReverseIterator(CIntrusiveList<T>* pList, Node* pNode) : IteratorBase(pList, pNode) {}
            bool         operator==(const ReverseIterator& other) const { return IteratorBase::m_pNode == other.m_pNode; }
            bool         operator!=(const ReverseIterator& other) const { return IteratorBase::m_pNode != other.m_pNode; }
            void         operator++() { IteratorBase::m_pNode = IteratorBase::m_pNode->m_pPrev; }
            void         operator++(int) { IteratorBase::m_pNode = IteratorBase::m_pNode->m_pPrev; }
            virtual void NotifyRemovingNode(Node* pNode)
            {
                if (IteratorBase::m_pNode == pNode)
                    IteratorBase::m_pNode = IteratorBase::m_pNode->m_pPrev;
            }
        };

        //
        // Constructor
        //
        CIntrusiveList(Node T::*pNodePtr) : m_pNodePtr(pNodePtr)
        {
            assert(m_pNodePtr);            // This must be set upon construction
            m_Size = 0;
            m_pFirst = NULL;
            m_pLast = NULL;
        }

        ~CIntrusiveList() { assert(m_ActiveIterators.empty()); }

        bool empty() const { return m_Size == 0; }

        size_t size() const { return m_Size; }

        //
        // Check if list contains item
        //
        bool contains(T* pItem) const
        {
            Node* pNode = &(pItem->*m_pNodePtr);
            return m_pFirst == pNode || pNode->m_pPrev || pNode->m_pNext;
        }

        //
        // Remove item from list
        //
        void remove(T* pItem)
        {
            Node* pNode = &(pItem->*m_pNodePtr);

            if (!pNode->m_pPrev && !pNode->m_pNext && m_pFirst != pNode)
                return;            // Not in list

            // Keep active iterators valid
            for (int i = m_ActiveIterators.size() - 1; i >= 0; i--)
                if (m_ActiveIterators[i]->m_pNode == pNode)
                    m_ActiveIterators[i]->NotifyRemovingNode(pNode);

            if (m_pFirst == pNode)
            {
                if (m_pLast == pNode)
                {
                    // Only item in list
                    assert(!pNode->m_pPrev && !pNode->m_pNext);
                    m_pFirst = NULL;
                    m_pLast = NULL;
                }
                else
                {
                    // First item in list
                    assert(!pNode->m_pPrev && pNode->m_pNext && pNode->m_pNext->m_pPrev == pNode);
                    pNode->m_pNext->m_pPrev = NULL;
                    m_pFirst = pNode->m_pNext;
                }
            }
            else if (m_pLast == pNode)
            {
                // Last item in list
                assert(pNode->m_pPrev && !pNode->m_pNext && pNode->m_pPrev->m_pNext == pNode);
                pNode->m_pPrev->m_pNext = NULL;
                m_pLast = pNode->m_pPrev;
            }
            else
            {
                // Somewhere in the middle
                assert(pNode->m_pPrev && pNode->m_pNext && pNode->m_pPrev->m_pNext == pNode && pNode->m_pNext->m_pPrev == pNode);
                pNode->m_pPrev->m_pNext = pNode->m_pNext;
                pNode->m_pNext->m_pPrev = pNode->m_pPrev;
            }
            pNode->m_pNext = NULL;
            pNode->m_pPrev = NULL;
            m_Size--;
        }

        //
        // Insert item at the start of the list
        //
        void push_front(T* pItem)
        {
            Node* pNode = &(pItem->*m_pNodePtr);
            assert(!pNode->m_pPrev && !pNode->m_pNext && !m_pFirst == !m_pLast);
            if (!m_pFirst)
            {
                // Initial list item
                m_pFirst = pNode;
                m_pLast = pNode;
            }
            else
            {
                pNode->m_pNext = m_pFirst;
                pNode->m_pNext->m_pPrev = pNode;
                m_pFirst = pNode;
            }
            m_Size++;
        }

        //
        // Insert item at the end of the list
        //
        void push_back(T* pItem)
        {
            Node* pNode = &(pItem->*m_pNodePtr);
            assert(!pNode->m_pPrev && !pNode->m_pNext && !m_pFirst == !m_pLast);
            if (!m_pFirst)
            {
                // Initial list item
                m_pFirst = pNode;
                m_pLast = pNode;
            }
            else
            {
                pNode->m_pPrev = m_pLast;
                pNode->m_pPrev->m_pNext = pNode;
                m_pLast = pNode;
            }
            m_Size++;
        }

        Iterator begin() { return Iterator(this, m_pFirst); }

        Iterator end() { return Iterator(this, NULL); }

        ReverseIterator rbegin() { return ReverseIterator(this, m_pLast); }

        ReverseIterator rend() { return ReverseIterator(this, NULL); }

        // Allow use of std iterator names
        typedef Iterator        iterator;
        typedef Iterator        const_iterator;            // TODO
        typedef ReverseIterator reverse_iterator;
        typedef ReverseIterator const_reverse_iterator;            // TODO
    };
};            // namespace SharedUtil

using namespace SharedUtil;
