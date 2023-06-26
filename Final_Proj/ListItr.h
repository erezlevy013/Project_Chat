#ifndef __LISTITR_H__
#define __LISTITR_H__

#include "GenList.h"
#define TRUE 1
#define FALSE 0

typedef void* ListItr;

/** 
 * @brief Get itertator to the list's beginning
 * @params _list : list to return begin iterator, pointing at first element
 *                 or at the end if list is empty
 * @return iterator pointing at the list's beginning or null if given pointer is null
 */
ListItr ListItrBegin(const List *_list);

/** 
 * @brief Get itertator to the list end
 * @params _list : list to return end iterator
 * @return an iterator pointing at the list's end or null if given pointer is null
 */
ListItr ListItrEnd(const List *_list);

/** 
 * @brief retruns TRUE if _a and _b iterators refer to same node
 * @params _a : A list iterator to compare
 * @params _b : A list iterator to compare to
 * @return TRUE if both iterators refer to the same node, FALSE otherwise
 * @warning might be implemented as a macro
 */
int ListItrEquals(const ListItr _a, const ListItr _b);

/** 
 * @brief Get itertator to the next element from a given iterator
 * @params _a : A list iterator from which to return the next element
 * @return iterator pointing to the next element, iterator to tail if it is the last one 
 *         in the list, or null if given pointer is null
 * @warning if _itr is end iterator the tail will be returned
 */
ListItr ListItrNext(ListItr _itr);

/** 
 * @brief Get itertator to the previous element
 * @params _a : A list iterator from which to return the previous element
 * @return iterator pointing to the previous element, iterator to given iterator if it is the first one 
 * @warning if _itr is begin iterator it will be returned
 */
ListItr ListItrPrev(ListItr _itr);

/** 
 * @brief Get data from the list node the iterator is pointing to
 *
 * @params _itr : A list iterator
 * @return the data the iterator is pointing at or NULL if pointing to the end
 */
void* ListItrGet(ListItr _itr);

/** 
 * @brief Set data at the node where the iterator is pointing at
 * @params _itr : A list iterator and data to set
 * @return the data from the node before it changed to the given data, null if either given pointer is null
 */
void* ListItrSet(ListItr _itr, void *_element);

/** 
 * @brief Inserts a new node before node the iterator is pointing at
 * @params _itr : A list iterator and data to insert to list
 * @return an iterator pointing at the element inserted, null if either given pointer is null or 
 *         if failed to allocate new node
 */
ListItr ListItrInsertBefore(ListItr _itr, void *_element);

/** 
 * @brief Removes the node the iterator is pointing at
 * @params _itr : A list iterator
 * @return the removed data
 */
void* ListItrRemove(ListItr _itr);

#endif /* __LISTITR_H__ */