


#ifndef ___SKIP_LIST_HPP
#define ___SKIP_LIST_HPP

#include <cmath> // for log2
#include <string>
#include <vector>
#include <iostream>
#include "runtimeexcept.hpp"

/**
 * flipCoin -- NOTE: Only read if you are interested in how the
 * coin flipping works.
 * 
 * @brief Uses the bitwise representation of the key to simulate
 * the flipping of a deterministic coin.
 * 
 * This function looks at the bitwise representation to determine
 * how many layers it occupies in the skip list. It takes the bitwise
 * XOR of each byte in a 32-bit unsigned number and uses the index `previousFlips`
 * to determine the truth value. It's best to look at an example and
 * to start with values that fit into one byte.
 * 
 * Let's start with the function call `flipCoin(0, 0)`. This call is
 * asking whether or not we should insert the key `0` into layer `1`
 * (because `previousFlips` represents the number of previous flips).
 * 
 * This function will get the bitwise representation of 0: 
 * 
 *        00000000 00000000 00000000 00000000
 * 
 * All bytes are XOR'd together: 
 * 
 * 
 *  c = 0 ^ 0 ^ 0 ^ 0
 * 
 * Then the index `previousFlips` is used to obtain the bit in the `previousFlips % 8` position.
 * 
 * So the return value is `0 & 1 = 0`, since the value in the zeroth position 
 * (obtained by bitmasking the value (1 << 0) at the bottom of the function
 * is 0.
 * Thus, this value `0` should never get added beyond the bottom-most layer.
 * 
 * Before:
 * 
 * S_1: -inf ----> inf
 * S_0: -inf ----> inf
 * 
 * And after 0 is inserted
 * 
 * S_1: -inf --------> inf
 * S_0: -inf --> 0 --> inf
 * 
 * Let's look at something more interesting, like the call `flipCoin(5, 0)`.
 * Remember the binary representation for 5 is 00000101.
 * 
 * c = 0 ^ 0 ^ 0 ^ 00000101 = 00000101
 * 
 * Now we get the bit at 0th position (from `previousFlips`).
 * 
 * 00000101 & 1 = 1. Thus, `5` DOES get inserted into the next layer,
 * layer 1.
 * 
 * So the skip list before this insertion might look like:
 * 
 * S_1: -inf ----> inf
 * S_0: -inf ----> inf
 * 
 * And after it would look like:
 * 
 * S_2: -inf --------> inf
 * S_1: -inf --> 5 --> inf
 * S_0: -inf --> 5 --> inf
 * 
 * Observe that a new layer had to get created, because there should always
 * be an empty layer at the top.
 * 
 * Since 5 got inserted into the next layer, we need to flip again to see if
 * it should get propagated once more.
 * 
 * `flipCoin(5, 1)` this will produce a result of `0`, since 00000101 & (1 << 1) = 0.
 * 
 * Thus, `5` will not be pushed up to the next layer.
 * 
 * @param key The integer key which will be added to the skip list
 * @param previousFlips The number of previous flips for this key
 * @return true simulates a "heads" from a coin flip
 * @return false simulates a "tails" from a coin flip
 */
inline bool flipCoin(unsigned key, unsigned previousFlips)
{
	char c;
	unsigned first8Bits = (key & 0xFF000000) / 0x01000000 ; 
	unsigned next8Bits =  (key & 0x00FF0000) / 0x00010000;
	unsigned andThen =	 (key & 0x0000FF00) / 0x00000100;
	unsigned lastBits =   (key & 0x000000FF);
	c = first8Bits ^ next8Bits ^ andThen ^ lastBits;
	previousFlips = previousFlips % 8;
	return ( c & (1 << previousFlips) ) != 0;
}

/**
 * @brief Works the same as the integer version above, except
 * it XORs chars in a string instead of the first four
 * bytes of an integer.
 * 
 * @param key key that will be inserted into the skip list
 * @param previousFlips number of previous flips for this key 
 * @return true simulates a "heads" from a coin flip
 * @return false simulates a "tails" from a coin flip
 */
inline bool flipCoin(std::string key, unsigned previousFlips)
{
	char c = key[0];
	for(unsigned j = 1; j < key.length(); j++)
	{
		c = c ^ key[j];
	}
	previousFlips = previousFlips % 8;
	return ( c & (1 << previousFlips) ) != 0;	
}

//Node class that holds object and the address of next node
template<typename Key, typename Value> class Node{
	public:
	Key key;
	Value value;
	Node* left;
	Node* right;
	Node* up;
	Node* down;

	public: Node(){
		left = nullptr;
		right = nullptr;
		up = nullptr;
		down = nullptr;
	}

	public: Node(Key key1, Value value1){
		key = key1;
		value = value1;
		left = nullptr;
		right = nullptr;
		up = nullptr;
		down = nullptr;
	}
};

template<typename Key, typename Value>
class SkipList
{


private:
	Node<Key, Value>* head;
	Node<Key, Value>* tail;
	int listSize;
	// private variables go here.

public:

	SkipList();

	// You DO NOT need to implement a copy constructor or an assignment operator.

	~SkipList();

	// How many distinct keys are in the skip list?
	size_t size() const noexcept;

	// Does the Skip List contain zero keys?
	bool isEmpty() const noexcept;

	// How many layers are in the skip list?
	// Note that an empty Skip List has two layers by default,
	// the "base" layer S_0 and the top layer S_1.
	//
	// [S_1: Top layer]    -inf ------> inf
	// [S_0: Bottom layer] -inf ------> inf
	//
	// This "empty" Skip List has two layers and a height of one.
	unsigned numLayers() const noexcept;

	// What is the height of this key, assuming the "base" layer S_0
	// contains keys with a height of 1?
	// For example, "0" has a height of 1 in the following skip list.
	//
	// [S_1]  -inf --------> inf
	// [S_0]  -inf --> 0 --> inf
	//
	// Throw an exception if this key is not in the Skip List.
	unsigned height(const Key & k) const;

	// If this key is in the SkipList and there is a next largest key
	// return the next largest key.
	// This function should throw a RuntimeException if either the key doesn't exist
	// or there is no subsequent key. 
	// A consequence of this is that this function will
	// throw a RuntimeException if *k* is the *largest* key in the Skip List.
	Key nextKey(const Key & k) const;

	// If this key is in the SkipList and a next smallest key exists,
	// return the next smallest key.
	// This function should throw a RuntimeException if either the key doesn't exist
	// or there is no previous key. 
	// A consequence of this is that this function will
	// throw a RuntimeException if *k* is the *smallest* key in the Skip List.
	Key previousKey(const Key & k) const;

	// These return the value associated with the given key.
	// Throw a RuntimeException if the key does not exist.
	Value & find(const Key & k);
	const Value & find(Key k) const;

	// Return true if this key/value pair is successfully inserted, false otherwise.
	// See the project write-up for conditions under which the key should be "bubbled up"
	// to the next layer.
	// If the key already exists, do not insert one -- return false.
	bool insert(const Key & k, const Value & v);

	// Return a vector containing all inserted keys in increasing order.
	std::vector<Key> allKeysInOrder() const;

	// Is this the smallest key in the SkipList? Throw a RuntimeException
	// if the key *k* does not exist in the Skip List. 
	bool isSmallestKey(const Key & k) const;

	// Is this the largest key in the SkipList? Throw a RuntimeException
	// if the key *k* does not exist in the Skip List. 
	bool isLargestKey(const Key & k) const;

	// I am not requiring you to implement remove.

	
};

template<typename Key, typename Value>
SkipList<Key, Value>::SkipList() {
	Node<Key, Value>* minusInf1 = new Node<Key, Value>();
	Node<Key, Value>* minusInf2 = new Node<Key, Value>();

	minusInf1->up = minusInf2;
	minusInf2->down = minusInf1;

	Node<Key, Value>* inf1 = new Node<Key, Value>();
	Node<Key, Value>* inf2 = new Node<Key, Value>();

	inf1->up = inf2;
	inf2->down = inf1;
	minusInf1->right = inf1;
	inf1->left = minusInf1;
	minusInf2->right = inf2;
	inf2->left = minusInf2;
	

	this->head = minusInf2;
	this->tail = inf2;
	listSize = 0;
}

template<typename Key, typename Value>
SkipList<Key, Value>::~SkipList() {
	Node<Key, Value>* current = this->head;
	Node<Key, Value>* next = nullptr;
	unsigned count = 0;
	while(current->down != nullptr){
		current = current->down;
		count += 1;
	}
	for(int i = count; i >= 0; i--){
		current = this->head;
		for(int j = 0; j < i; j++){
			current = current->down;
	    }
		while(current->right != nullptr){
			next = current->right;
			delete current;
			current = next;
		}
		delete current;
	}
	this->head = nullptr;
	this->tail = nullptr;
}

template<typename Key, typename Value>
size_t SkipList<Key, Value>::size() const noexcept {
	return listSize;
}

template<typename Key, typename Value>
bool SkipList<Key, Value>::isEmpty() const noexcept {
	if(listSize == 0){
		return true;
	}else{
		return false;
	}
}

template<typename Key, typename Value>
unsigned SkipList<Key, Value>::numLayers() const noexcept {
	Node<Key, Value>* current = this->head;
	unsigned count = 1;
	while(current->down != nullptr){
		current = current->down;
		count += 1;
	}

	return count;
}

template<typename Key, typename Value>
unsigned SkipList<Key, Value>::height(const Key & k) const {
	unsigned count = 1;
    Node<Key, Value>* current = this->head;
	current = current->down;
	while(current->down != nullptr){
		while(current->right->right != nullptr){
			if(current->right->key <= k){
				current = current->right;
			}else{
				break;
			}
		}
		current = current->down;
	}
	while(current->right->right != nullptr){
			if(current->right->key <= k){
				current = current->right;
			}else{
				break;
			}
	}
	while(current->up != nullptr){
		current = current->up;
		count += 1;
	}
	return count;
}

template<typename Key, typename Value>
Key SkipList<Key, Value>::nextKey(const Key & k) const {
    auto current = this->head;
	current = current->down;
	while(current->down != nullptr){
		while(current->right->right != nullptr){
			if(current->right->key <= k){
				current = current->right;
			}else{
				break;
			}
		}
		current = current->down;
	}
	while(current->right->right != nullptr){
			if(current->right->key <= k){
				current = current->right;
			}else{
				break;
			}
	}
	if(current->right->right == nullptr){
		throw RuntimeException("It is a largest key");
	}
	if(current->key == k){
		return current->right->key;
	}else{
		throw RuntimeException("No key match");
	}
}

template<typename Key, typename Value>
Key SkipList<Key, Value>::previousKey(const Key & k) const {
	Node<Key, Value>* current = this->head;
	current = current->down;
	while(current->down != nullptr){
		while(current->right->right != nullptr){
			if(current->right->key <= k){
				current = current->right;
			}else{
				break;
			}
		}
		current = current->down;
	}
	while(current->right->right != nullptr){
			if(current->right->key <= k){
				current = current->right;
			}else{
				break;
			}
	}
	if(current->left->left == nullptr){
		throw RuntimeException("It is a smallest key");
	}
	if(current->key == k){
		return current->left->key;
	}else{
		throw RuntimeException("No key match");
	}
}

template<typename Key, typename Value>
const Value & SkipList<Key, Value>::find(Key k) const {
	auto current = this->head;
	current = current->down;
	while(current->down != nullptr){
		while(current->right->right != nullptr){
			if(current->right->key <= k){
				current = current->right;
			}else{
				break;
			}
		}
		current = current->down;
	}
	while(current->right->right != nullptr){
			if(current->right->key <= k){
				current = current->right;
			}else{
				break;
			}
	}
	if(current->key == k){
		return current->value;
	}else{
		throw RuntimeException("No key match");
	}
}

template<typename Key, typename Value>
Value & SkipList<Key, Value>::find(const Key & k) {
	auto current = this->head;
	current = current->down;
	while(current->down != nullptr){
		while(current->right->right != nullptr){
			if(current->right->key <= k){
				current = current->right;
			}else{
				break;
			}
		}
		current = current->down;
	}
	while(current->right->right != nullptr){
			if(current->right->key <= k){
				current = current->right;
			}else{
				break;
			}
	}
	if(current->key == k){
		return current->value;
	}else{
		throw RuntimeException("No key match");
	}
}

template<typename Key, typename Value>
bool SkipList<Key, Value>::insert(const Key & k, const Value & v) {
	Node<Key, Value>* current = this->head;
	current = current->down;
	while(current->down != nullptr){
		while(current->right->right != nullptr){
			if(current->right->key <= k){
				current = current->right;
			}else{
				break;
			}
		}
		current = current->down;
	}
	while(current->right->right != nullptr){
			if(current->right->key <= k){
				current = current->right;
			}else{
				break;
			}
	}
	if(current->left == nullptr){
		auto newNodeBase = new Node(k, v);
		current->right->left = newNodeBase;
		newNodeBase->right = current->right;
		current->right = newNodeBase;
		newNodeBase->left = current;
		current = newNodeBase;
		unsigned previousFlips = 0;
		while(flipCoin(k, previousFlips)){
			previousFlips += 1;
			auto newNodeAbove = new Node(k, v);
			if(numLayers() == previousFlips + 1){
				Node<Key, Value>* newMinusInf = new Node<Key, Value>();
				Node<Key, Value>* newInf = new Node<Key, Value>();
				
				newMinusInf->down = this->head;
				newMinusInf->right = newInf;
				newInf->down = this->tail;
				newInf->left = newMinusInf;

				this->head = newMinusInf;
				this->tail = newInf;

				newNodeAbove->down = current;
				current->up = newNodeAbove;
				newNodeAbove->left = this->head->down;
				newNodeAbove->right = this->tail->down;
				newNodeAbove->left->right = newNodeAbove;
				newNodeAbove->right->left = newNodeAbove;
				current = newNodeAbove;
			}else{
				newNodeAbove->down = current;
				current->up = newNodeAbove;
				while(current->up == nullptr || current->left != nullptr){
					current = current->left;
				}
				current = current->up;
				newNodeAbove->left = current;
				newNodeAbove->right = current->right;
				current->right = newNodeAbove;
				newNodeAbove->right->left = newNodeAbove;
				current = newNodeAbove;
			}
		}
		listSize += 1;
		return true;
	}
	if(current->key != k){
		auto newNodeBase = new Node(k, v);
		current->right->left = newNodeBase;
		newNodeBase->right = current->right;
		current->right = newNodeBase;
		newNodeBase->left = current;
		current = newNodeBase;
		unsigned previousFlips = 0;
		unsigned numOfDistinctKeys = 0;
		auto justForCount = this->head;
		while(justForCount->down != nullptr){
			justForCount = justForCount->down;
		}
		while(justForCount->right != nullptr){
			justForCount = justForCount->right;
			numOfDistinctKeys += 1;
		}
		bool flipResult = flipCoin(k, previousFlips);
		while(flipResult){
			if(numOfDistinctKeys <= 16 && previousFlips == 11){
				break;
			}else if(numOfDistinctKeys > 16 && previousFlips == 3*ceil(log2(numOfDistinctKeys))-1){
				break;
			}
			previousFlips += 1;
			auto newNodeAbove = new Node(k, v);
			if(numLayers() == previousFlips + 1){
				Node<Key, Value>* newMinusInf = new Node<Key, Value>();
				Node<Key, Value>* newInf = new Node<Key, Value>();
				newMinusInf->down = this->head;
				newMinusInf->right = newInf;
				newInf->down = this->tail;
				newInf->left = newMinusInf;

				this->head = newMinusInf;
				this->tail = newInf;

				newNodeAbove->down = current;
				current->up = newNodeAbove;
				newNodeAbove->left = this->head->down;
				newNodeAbove->right = this->tail->down;
				newNodeAbove->left->right = newNodeAbove;
				newNodeAbove->right->left = newNodeAbove;
				current = newNodeAbove;
			}else{
				newNodeAbove->down = current;
				while(current->up == nullptr && current->left != nullptr){
					current = current->left;
				}
				current = current->up;
				newNodeAbove->left = current;
				newNodeAbove->right = current->right;
				current->right = newNodeAbove;
				newNodeAbove->right->left = newNodeAbove;
				current = newNodeAbove;
				newNodeAbove->down->up = newNodeAbove;
			}
			flipResult = flipCoin(k, previousFlips);
		}
		listSize += 1;
		return true;
	}else{
		return false;
	}
}

template<typename Key, typename Value>
std::vector<Key> SkipList<Key, Value>::allKeysInOrder() const {
    std::vector<Key> vector;
	auto current = this->head;
	while(current->down != nullptr){
		current = current->down;
	}
	while(current->right->right != nullptr){
		current = current->right;
		vector.push_back(current->key);
	}
	return vector;
}

template<typename Key, typename Value>
bool SkipList<Key, Value>::isSmallestKey(const Key & k) const {
    auto current = this->head;
	while(current->down != nullptr){
		current = current->down;
	}
	return current->right->key == k;
}

template<typename Key, typename Value>
bool SkipList<Key, Value>::isLargestKey(const Key & k) const {
	auto current = this->head->right;
	while(current->down != nullptr){
		current = current->down;
	}
	return current->left->key == k;
}



#endif

