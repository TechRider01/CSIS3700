
#ifndef BST_DICTIONARY_H
#define BST_DICTIONARY_H

#include <cstdint>
#include <stdexcept>

using namespace std;

static const uint32_t
	NULL_INDEX = 0xffffffff,
	DEFAULT_INITIAL_CAPACITY = 16;
	

template <typename KeyType, typename ValueType>
class BSTDictionary {
	public:
		explicit BSTDictionary (uint32_t cap = DEFAULT_INITIAL_CAPACITY) {
			cout << "Did you even make it to the constructor";
			if(nTrees == 0) {
				//Generate space for trees and make a tree
				counts = new uint32_t[cap];
				heights = new uint32_t[cap];
				left = new uint32_t[cap];
				right = new uint32_t[cap];
				keys = new KeyType[cap];
				values = new ValueType[cap];
				
				capacity = cap;
				
				generateFreeList(0, capacity);
			}
			nTrees++;
			root = NULL_INDEX;
		}
		
		~BSTDictionary () {
			nTrees--;
			
			if(nTrees == 0) {
				delete[] keys;
	        	delete[] values;
	        	delete[] left;
	        	delete[] right;
	        	delete[] counts;
	        	delete[] heights;
			} else {
				prvClear(root);
			}
		}
		
		void clear () { prvClear(root); root = NULL_INDEX; }
		
		
		uint32_t size() {
			if(root == NULL_INDEX)
				return 0;
			return counts[root];
    	}
		
		uint32_t height() {
			if(root == NULL_INDEX)
				return -1;
			
			return heights[root];
    	}
		
		bool isEmpty() { return root == NULL_INDEX; }
		
		ValueType &search(const KeyType &k) {
			uint32_t
				n = root;
			
			while(n != NULL_INDEX) {
				if(k == keys[n])
					return values[n];
				else if(k < keys[n])
					n = left[n];
				else
					n = right[n];
			}
			
			throw std::domain_error("Search: Key not found");
		}
		
		ValueType &operator[](const KeyType &k) {
			uint32_t tmp = prvAllocate();
			uint32_t n = tmp;
			
			root = prvInsert(root, n, k);
			if (n != tmp)
				prvFree(tmp);
			
			return values[n];
		}
		
		void remove(const KeyType &k) {
		ntbd = NULL_INDEX;
			// recursively detach the key node from the tree
       		root = prvRemove(root,k);

        	// if the key was found, ntbd will be set and detached from the tree;
        	// delete the node
        	if (ntbd != NULL_INDEX)
        	prvFree(ntbd);
        	    //delete keys[k];
    	}
	private:
	
	uint32_t
		root,
		ntbd;
	static uint32_t
		*counts,
		*heights,
		*left,
		*right,
		nTrees,
		capacity,
		freeListHead;
	
	static KeyType
		*keys;
	
	static ValueType
		*values;
	
		uint32_t prvAllocate() {
			if(freeListHead == NULL_INDEX) {
				//Allocate temp arrays with double capacity
				uint32_t tmpCap = 2 * capacity;
				
				//Not sure if I need to allocate the other tmp arrays so I've just commented them out for now, will add if anything is broken.
				uint32_t
					*tmpcounts = new uint32_t[tmpCap],
					*tmpheights = new uint32_t[tmpCap],
					*tmpleft = new uint32_t[tmpCap],
					*tmpright = new uint32_t[tmpCap];
				KeyType
					*tmpkeys = new KeyType[tmpCap];
				ValueType
					*tmpvalues = new ValueType[tmpCap];
				
				//If any null bail
				if(tmpcounts == nullptr ||
				   tmpheights == nullptr ||
				   tmpleft == nullptr ||
				   tmpright == nullptr ||
				   tmpkeys == nullptr ||
				   tmpvalues == nullptr) {
					throw std::overflow_error("Dictionary is full");
				}
				
				//Copy data from old array to new array
				for (int i = 0; i < capacity; i++) {
	        		tmpkeys[i] = keys[i];
	        		tmpvalues[i] = values[i];
	        		tmpleft[i] = left[i];
	        		tmpright[i] = right[i];
	        		tmpcounts[i] = counts[i];
	        		tmpheights[i] = heights[i];
	        	}
	        	
	        	//Delete old arrays
	        	delete[] keys;
	        	delete[] values;
	        	delete[] left;
	        	delete[] right;
	        	delete[] counts;
	        	delete[] heights;
				
				//Point shared pointers to temp arrays
				keys = tmpkeys;
				values = tmpvalues;
				left = tmpleft;
				right = tmpright;
				counts = tmpcounts;
				heights = tmpheights;
				
				//Regenerate free list
				generateFreeList(capacity, tmpCap);
				
				capacity = tmpCap;
			}
			uint32_t 
      			tmp = freeListHead;
      		freeListHead = left[freeListHead];
      		
      		left[tmp] = NULL_INDEX;
      		right[tmp] = NULL_INDEX;
      		counts[tmp] = 1;
      		heights[tmp] = 1;
      		
      		return tmp;
		}
		
		void prvFree(uint32_t n) {
			left[n] = freeListHead;
			
			freeListHead = n;
		}
		
		void prvClear(uint32_t r) {
			// if r is null, nothing to do
        	if (r != NULL_INDEX) {
            	prvClear(left[r]);      // delete the subtrees first
            	prvClear(right[r]);
            	prvFree(r);            // then delete r
        	}
    	}
		
		void prvAdjust(uint32_t r) {
			//Counts[r] = getcount(left[r]) + getcount(right[r]) + 1
			counts[r] = counts[left[r]] + counts[right[r]] + 1;
			//Set heights r =  max of the heights of either branch + 1
			if(heights[left[r]] > heights[right[r]]) {
				heights[r] = heights[left[r]] + 1;
			} else {
				heights[r] = heights[right[r]] + 1;
			}
		}
		
		uint32_t prvInsert(uint32_t r, uint32_t &n, const KeyType &k) {
			//If k not in tree insert here
			if(r == NULL_INDEX) {
				keys[n] = k;
				//n is root of formerly empty tree
				return n;
			}
			
			//Key found, remember where		
			if(k == keys[r]) {
				n = r;
			} else if (k < keys[r]) {
				left[r] = prvInsert(right[r], n, k);
			} else {
				right[r] = prvInsert(right[r], n, k);
			}
			
			prvAdjust(r);
			
			return r;
		}
		
		uint32_t prvRemove(uint32_t r, const KeyType &k) {
			uint32_t tmp;
			KeyType
				keyTmp;
			ValueType
				valueTmp;
			if(r == NULL_INDEX) {
				throw std::domain_error("Remove: Key not found");
			}
			
			if(k < keys[r])
				left[r] = prvRemove(left[r], k);
			else if (k > keys[r])
				right[r] = prvRemove(right[r], k);
			else
				ntbd = r;
				
				if(left[r] == NULL_INDEX)
					if(right[r] == NULL_INDEX)
						r = NULL_INDEX;
					else
						r = right[r];
				else
					if(right[r] == NULL_INDEX)
						r = left[r];
					else
						//reduce two child case
						//Remove taller subtree
						if(heights[right[r]] > heights[left[r]]) {
							//Go to root of right subtree
							tmp = right[r];
							//Move left as far as possible
							while(left[tmp] != NULL_INDEX) {
								tmp = left[tmp];
							}
							
							//Swap keys[r] and keys[tmp]
							keyTmp = keys[r];
							keys[r] = keys[tmp];
							keys[tmp] = keyTmp;
							//Swap values[r] and values[tmp]
							valueTmp = values[r];
							values[r] = values[tmp];
							values[tmp] = valueTmp;
							
							//Remove k from left subtree
							left[r] = prvRemove(left[r],k);
							
							
						}
			if(r != NULL_INDEX) 
				prvAdjust(r);
			
			return r;
		}
		
		void generateFreeList(uint32_t start, uint32_t end) {
			for(int i = start; i < end; i++) {
				left[i] = i + 1;
			}
			left[end] = NULL_INDEX;
			freeListHead = start;
		}

};

//Keys init
template <typename KeyType, typename ValueType>
KeyType *BSTDictionary<KeyType,ValueType>::keys = nullptr;
//Values init
template <typename KeyType, typename ValueType>
ValueType *BSTDictionary<KeyType,ValueType>::values = nullptr;
//Counts init
template <typename KeyType, typename ValueType>
uint32_t *BSTDictionary<KeyType,ValueType>::counts = nullptr;
//Heights init
template <typename KeyType, typename ValueType>
uint32_t *BSTDictionary<KeyType,ValueType>::heights = nullptr;
//Left init
template <typename KeyType, typename ValueType>
uint32_t *BSTDictionary<KeyType,ValueType>::left = nullptr;
//Right init
template <typename KeyType, typename ValueType>
uint32_t *BSTDictionary<KeyType,ValueType>::right = nullptr;
//nTrees init
template <typename KeyType, typename ValueType>
uint32_t BSTDictionary<KeyType,ValueType>::nTrees = 0;
//capacity init
template <typename KeyType, typename ValueType>
uint32_t BSTDictionary<KeyType,ValueType>::capacity = 0;
//freeListHead init
template <typename KeyType, typename ValueType>
uint32_t BSTDictionary<KeyType,ValueType>::freeListHead = 0;

#endif

