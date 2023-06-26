#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#include <stddef.h>  /* size_t */
#define MINIMUM_SIZE 10
#define TRUE 1
#define FALSE 0

/** 
 *  @file HashMap.h
 *  @brief Generic Hash map of key-value pairs implemented with separate chaining using linked lists.
 *	
 *  @details  The hash map (sometimes called dictionary or associative array)
 *  is a set of distinct keys (or indexes) mapped (or associated) to values.
 *  size of allocated table will be the nearest prime number greater than requestedcapacity
 *  Lists used for chaining will be allocated lazely.
 *
 */

typedef struct HashMap HashMap;

typedef enum Map_Result {
	MAP_SUCCESS = 0,
	MAP_UNINITIALIZED_ERROR, 		/**< Uninitialized map error 	*/
	MAP_KEY_NULL_ERROR, 			/**< Key was null 				*/
	MAP_KEY_DUPLICATE_ERROR, 		/**< Duplicate key error 		*/
	MAP_KEY_NOT_FOUND_ERROR, 		/**< Key not found 				*/
	MAP_ALLOCATION_ERROR 			/**< Allocation error 	 		*/
} Map_Result;

/** 
 * @brief Hash function that determines the data's index within the hash structure
 * @param _key : key to insert
 * @return index in which the data will be inserted
 */
typedef size_t (*HashFunction)(const void *_key);

/** 
 * @brief Equality function that compares between two elements
 * @param _firstKey, _secondKey : two elements to compare to each other
 * @return TRUE if they are equal, FALSE otherwise
 */
typedef int (*EqualityFunction)(void *_firstKey, void *_secondKey);

/** 
 * @brief Action function that does a certain action on a key in the hash
 * @param _key : key of the element in the hash
 * @param _value : value of the element in the hash (can be NULL if nonexistent)
 * @param _context : context to be used in action function (can be NULL if unnecessary)
 * @return 0 on success, 1 on failure
 */
typedef int	(*KeyValueActionFunction)(const void *_key, void *_value, void *_context);

/** 
 * @brief function that destroys the key of an element in the hash (can be NULL if unnecessary)
 * @param _key : key of the element in the hash
 */
typedef void (*KeyDestroy)(void *_key);

/** 
 * @brief function that destroys the value of an element in the hash (can be NULL if unnecessary)
 * @param _value : value of the element in the hash
 */
typedef void (*ValDestroy)(void *_value);

/** 
 * @brief Create a new hash map with given capcity and key characteristics.
 * @param[in] _capacity - Expected max capacity 
 * 						  shall be rounded to nearest larger prime number.
 * @param[in] _hashFunc - hashing function for keys
 * @param[in] _keysEqualFunc - equality check function for keys. 
 * @return pointer to newly created map or NULL on failure
 */
HashMap* HashMap_Create(size_t _capacity, HashFunction _hashFunc, EqualityFunction _keysEqualFunc);

/**
 * @brief destroy hash map and set *_map to NULL
 * @param[in] _map : map to be destroyed
 * @param[optional] _keyDestroy : pointer to function to destroy keys
 * @param[optional] _valDestroy : pointer to function to destroy values 
 * @details optionally destroy all keys and values using user provided functions
 */
void HashMap_Destroy(HashMap **_map, KeyDestroy _keyDestroy, ValDestroy _valDestroy);

/** 
 * @brief Insert a key-value pair into the hash map.
 * @param[in] _map - Hash map adt to insert to, must be initialized
 * @param[in] _key - key to serve as index 
 * @param[in] _value - The value to associate with the key 
 * @return Success indicator
 * @retval  MAP_SUCCESS	on success
 * @retval  MAP_KEY_DUPLICATE_ERROR	if key alread present in the map
 * @retval  MAP_KEY_NULL_ERROR if given pointer to key is NULL
 * @retval  MAP_ALLOCATION_ERROR on failure to allocate key-value pair, failure to allocate list, or failure to allocate node in the list
 * @retval  MAP_UNINITIALIZED_ERROR if given pointer to hash is NULL
 * 
 * @warning key must be unique and destinct
 */
Map_Result HashMap_Insert(HashMap *_map, const void *_key, const void *_value);

/** 
 * @brief Remove a key-value pair from the hash map.
 * @param[in] _map - Hash map to remove pair from, must be initialized
 * @param[in] _key - key to serve as index to locate _value
 * @param[in] _pKey  - pointer to variable to recieve key from map if found
 * @param[in] _pValue - pointer to variable that will get the value if found.
 * @return Success indicator
 * @retval  MAP_SUCCESS	on success
 * @retval  MAP_KEY_NULL_ERROR
 * @retval  MAP_KEY_NOT_FOUND_ERROR if search key not found
 * @retval  MAP_UNINITIALIZED_ERROR if any of the other pointers are NULL
 * 
 * @warning key must be unique and distinct
 */
Map_Result HashMap_Remove(HashMap *_map, const void *_searchKey, void **_pKey, void **_pValue);

/** 
 * @brief Find a value by key
 * @param[in] _map - Hash map to remove pair from, must be initialized
 * @param[in] _key - key to serve as index 
 * @param[in] _pValue - pointer to variable that will get the value if found.
 * @return Success indicator
 * @retval  MAP_SUCCESS	on success
 * @retval  MAP_KEY_NULL_ERROR
 * @retval  MAP_KEY_NOT_FOUND_ERROR if key not found
 * @retval  MAP_UNINITIALIZED_ERROR
 * 
 * @warning key must be unique and distinct
 */
Map_Result HashMap_Find(const HashMap *_map, const void *_key, void **_pValue);

/**
 * @brief Get number of key-value pairs inserted into the hash map
 * @return number of key-value pairs, 0 if given pointer is NULL
 * @warning complexity is O(1)
 */
size_t HashMap_Size(const HashMap *_map);

/** 
 * This method is optional in the homewor 
 * @brief Iterate over all key-value pairs in the map.
 *                   Iteration will stop if _act returns a zero for a given pair
 * @details The user provided KeyValueActionFunction _action will be called for each element.  
 * @param[in] _map - Hash map to iterate over.
 * @param[in] _act - User provided function pointer to be invoked for each element
 * @returns number of times the user's function was invoked, 0 if given map or action pointers were NULL
 */
size_t HashMap_ForEach(const HashMap *_map, KeyValueActionFunction _action, void *_context);

#endif /* __HASHMAP_H__ */
