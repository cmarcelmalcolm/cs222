/*
 * pf.h
 *
 *  Created on: Apr 6, 2013
 *      Author: cmarcelmalcolm
 */
#ifndef _pf_h_
#define _pf_h_
#define _DEFINE_DEPRECATED_HASH_CLASSES 0
using namespace std;


typedef int RC;
typedef unsigned PageNum;

#define PF_PAGE_SIZE 4096
#include<fstream>
#include <vector>
#include <map>
#include <stdio.h>
class PF_FileHandle;

class PF_Manager {
public:
	static PF_Manager* Instance();         // Access to the _pf_manager instance

	RC CreateFile(const char *fileName);                    // Create a new file
	RC DestroyFile(const char *fileName);                      // Destroy a file
	RC OpenFile(const char *fileName, PF_FileHandle &fileHandle); // Open a file
	RC CloseFile(PF_FileHandle &fileHandle);                     // Close a file

protected:
	PF_Manager();                                                 // Constructor
	~PF_Manager();                                                 // Destructor

private:
	static PF_Manager *_pf_manager;
};

class PF_FileHandle {
public:
	fstream file;
	PF_FileHandle();								// Default constructor
	~PF_FileHandle();								// Destructor
	RC ReadPage(PageNum pageNum, void *data);		// Get a specific page
	RC WritePage(PageNum pageNum, const void *data);	// Write a specific page
	RC AppendPage(const void *data);				// Append a specific page
	unsigned GetNumberOfPages();		// Get the number of pages in the file
};
class LRUCache
{
private:
	map< K, LRUCacheEntry<K,T>* >	_mapping;
	vector< LRUCacheEntry<K,T>* >		_freeEntries;
	LRUCacheEntry<K,T> *			head;
	LRUCacheEntry<K,T> *			tail;
	LRUCacheEntry<K,T> *			entries;
public:
	LRUCache(size_t size){
		entries = new LRUCacheEntry<K,T>[size];
		for (int i=0; i<size; i++)
			_freeEntries.push_back(entries+i);
		head = new LRUCacheEntry<K,T>;
		tail = new LRUCacheEntry<K,T>;
		head->prev = NULL;
		head->next = tail;
		tail->next = NULL;
		tail->prev = head;
	}
	~LRUCache()
	{
		delete head;
		delete tail;
		delete [] entries;
	}

#endif

