/*
 * pf.cc
 *
 *  Created on: Apr 6, 2013
 *      Author: cmarcelmalcolm
 */
#include "pf.h"
#include<fstream>
#include<iostream>
#include <iostream>
#include <vector>
#include <map>
using namespace std;

PF_Manager* PF_Manager::_pf_manager = 0;

PF_Manager* PF_Manager::Instance() {
	if (!_pf_manager)
		_pf_manager = new PF_Manager();
	return _pf_manager;
}

PF_Manager::PF_Manager() {
}

PF_Manager::~PF_Manager() {
}

RC PF_Manager::CreateFile(const char *fileName) {
	fstream file;
	file.open(fileName, ios::in | ios::binary);	//testing for duplicate file name.

	if (file.is_open()) {
		file.close();
		return -1;
	}

	file.open(fileName, ios::out | ios::binary);	//creating a new file
	file.close();
	return 0;
}

RC PF_Manager::DestroyFile(const char *fileName) {
	if (remove(fileName) == 0)				//file present and deleted
		return 0;
	return -1;
}

RC PF_Manager::OpenFile(const char *fileName, PF_FileHandle &fileHandle) {
	if (!fileHandle.file.is_open()) {
		fileHandle.file.open(fileName, ios::in | ios::out | ios::binary);
		return 0;
	} else
		return -1;
}

RC PF_Manager::CloseFile(PF_FileHandle &fileHandle) {
	if (!fileHandle.file.is_open())
		return -1;
	else if (fileHandle.file.is_open()) {
		fileHandle.file.close();
		return 0;
	}
	return -1;
}

PF_FileHandle::PF_FileHandle() {
}

PF_FileHandle::~PF_FileHandle() {
	if (file.is_open())
		file.close();
}

RC PF_FileHandle::ReadPage(PageNum pageNum, void *data) {
	if (!file.is_open())
		return -1;

	if (GetNumberOfPages() <= pageNum)
		return -1;

	file.seekg(pageNum * PF_PAGE_SIZE, ios::beg);
	file.read((char*) data, PF_PAGE_SIZE);
	return 0;
}

RC PF_FileHandle::WritePage(PageNum pageNum, const void *data) {
	if (!file.is_open())
		return -1;

	if (GetNumberOfPages() <= pageNum)
		return -1;

	else {
		file.seekg(pageNum * PF_PAGE_SIZE, ios::beg);
		file.write((char*) data, PF_PAGE_SIZE);
	}
	return 0;

}

RC PF_FileHandle::AppendPage(const void *data) {
	if (!file.is_open())
		return -1;

	file.seekg(0, ios::end);
	file.write((char*) data, PF_PAGE_SIZE);
	return 0;
}

unsigned PF_FileHandle::GetNumberOfPages() {

	if (!file.is_open())
		return 0;

	int current_length;
	file.seekg(0, ios::end);
	current_length = file.tellg();
	int nop = current_length / 4096;
	return nop;
}



using namespace std;

template<class K, class T>
struct LRUCacheEntry
{
	K key;
	T data;
	LRUCacheEntry* prev;
	LRUCacheEntry* next;
};

template<class K, class T>
class LRUCache
{
private:
	map< K, LRUCacheEntry<K,T>* >_mapping;
	vector< LRUCacheEntry<K,T>* >_freeEntries;
	LRUCacheEntry<K,T> *head;
	LRUCacheEntry<K,T> *tail;
	LRUCacheEntry<K,T> *entries;
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
	void put(K key, T data)
	{
		LRUCacheEntry<K,T>* node = _mapping[key];
		if(node)
		{
			// refresh the link list
			detach(node);
			node->data = data;
			attach(node);
		}
		else{
			if ( _freeEntries.empty() )
			{
				node = tail->prev;
				detach(node);
				node->data = data;
				node->key = key;
				attach(node);
			}
			else{
				node = _freeEntries.back();
				_freeEntries.pop_back();
				node->key = key;
				node->data = data;
				_mapping[key] = node;
				attach(node);
			}
		}
	}

	T get(K key)
	{
		LRUCacheEntry<K,T>* node = _mapping[key];
		if(node)
		{
			detach(node);
			attach(node);
			return node->data;
		}
		else return NULL;
	}

private:
	void detach(LRUCacheEntry<K,T>* node)
	{
		node->prev->next = node->next;
		node->next->prev = node->prev;
	}
	void attach(LRUCacheEntry<K,T>* node)
	{
		node->next = head->next;
		node->prev = head;
		head->next = node;
		node->next->prev = node;
	}
};

