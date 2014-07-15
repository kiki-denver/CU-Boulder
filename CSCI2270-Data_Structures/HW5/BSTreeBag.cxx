//the implementation ( BSTreeBag.cxx ) is actually already inluded in the header,
//therefore, we don't include the header here.

#include <cassert>
using namespace std;


namespace hw5
{
//MEMBER FUNCTIONS OF class BSTreeBag (they all include *this(
//PUBLIC CONSTRUCTORS and DESTRUCTORS of class BSTreeBag
	//default constructor //DONE
	template <class ItemType>
	BSTreeBag<ItemType>::BSTreeBag()
	{
		root_ptr = NULL;
	}
	
	//copy constructor
	template <class ItemType>
	BSTreeBag<ItemType>::BSTreeBag(const BSTreeBag& source) //DONE
	{
		this->root_ptr = tree_copy(source.root_ptr);
	}
	
	//destructor
	template <class ItemType>
	BSTreeBag<ItemType>::~BSTreeBag()  //DONE
	{
		tree_clear(this->root_ptr);
		this->root_ptr = NULL; 				
	}
//PUBLIC MODIFICATION FUNCTIONS of class BSTreeBag  //DONE
	template <class ItemType>
	//this is basically "erase *all* versions of some number" 
	typename BSTreeBag<ItemType>::size_type  BSTreeBag<ItemType>::erase(const ItemType& target)
	{
		//if this doesn't work, make it instead
		//BSTreeBag<ItemType>::size_type size = 0;
		size_type erased = count(target); //unsigned int declared in header
		bst_remove_all(root_ptr, target);
		return erased;
	}
	
	template <class ItemType>	//DONE
	bool BSTreeBag<ItemType>::erase_one(const ItemType& target)
	{
		return bst_remove(root_ptr, target);
	}
	
	template <class ItemType>
	void BSTreeBag<ItemType>::insert(const ItemType& entry) //DONE
//<= to the left; > to the right aka. if the root_ptr->data()==50 and
//the target (or entry in this case) == 49, go to the left
	{
		bool done = false;
		//inserting into an empty tree (like one made by the default constructor		
		if (this->root_ptr == NULL){
			root_ptr = new binary_tree_node<ItemType>(entry);
			done = true;
		}
		else{
			binary_tree_node<ItemType>* cursor_ptr = root_ptr;
			while (done == false){
				if (entry <= cursor_ptr->data() && cursor_ptr->left() == NULL){
					cursor_ptr->left() = new binary_tree_node<ItemType>(entry);
					done = true;
					///	print(cursor_ptr, 0);
					}
				else if (entry <= cursor_ptr->data() && cursor_ptr->left() != NULL) {
						cursor_ptr = cursor_ptr->left();
					}			
				else if (entry > cursor_ptr->data() && cursor_ptr->right() == NULL){ //this is essentially if (entry > cursor_ptr->data())
					cursor_ptr->right() = new binary_tree_node<ItemType>(entry);
					done = true;
				}
				else if (entry > cursor_ptr->data() && cursor_ptr->right() != NULL) {
						cursor_ptr = cursor_ptr->right();
					}
					
				}
		}
	}
	
	template <class ItemType>
	void BSTreeBag<ItemType>::operator+=(const BSTreeBag& addend)
	{
		if (this==&addend){
			BSTreeBag<ItemType> copy = addend;
			this->insert_all(copy.root_ptr); //this copy will be destroyed by the destructor when I leave this function, since I made it in this function
		}
		else{
			this->insert_all(addend.root_ptr);
		}
				
	}
	
	template <class ItemType>
	void BSTreeBag<ItemType>::operator=(const BSTreeBag& source) //DONE
	{
		if (this==&source){
			return;
		}
		else{
			tree_clear(this->root_ptr); //tree_clear already checks to see if root_ptr == NULL
			this->root_ptr = tree_copy(source.root_ptr);
		}
	}
// PUBLIC CONSTANT functions of class BSTreeBag
	template <class ItemType>
	typename BSTreeBag<ItemType>::size_type BSTreeBag<ItemType>::size( ) const //DONE
	{
		unsigned int size = tree_size(root_ptr);
		return size;		
	}
	
	template <class ItemType>	
	typename BSTreeBag<ItemType>::size_type  BSTreeBag<ItemType>::count(const ItemType& target) const	 //DONE
	{
		size_type count = counter(this->root_ptr, target);
		return count;		
	}
	
// PRIVATE functions of class BSTreeBag
	template <class ItemType>
	void BSTreeBag<ItemType>::insert_all(binary_tree_node<ItemType>* addroot_ptr)
	{
	if (addroot_ptr != NULL){
		insert(addroot_ptr->data()); //HELP. can I do this-> ?
		insert_all(addroot_ptr->left());
		insert_all(addroot_ptr->right());
	}
	
	}
	
//this one I added in to help with the public count function //DONE
	template <class ItemType>
	typename BSTreeBag<ItemType>::size_type  BSTreeBag<ItemType>::counter(
	const binary_tree_node<ItemType>* node_ptr, const ItemType& target) const	
	{
		//<= to the left; > to the right aka. if the root_ptr==50 and the target==49, go to the left
		
			if (node_ptr == NULL){
				return 0;
			}
			else if (target == node_ptr->data()){
				if (node_ptr->left() != NULL){
					return (1 + counter(node_ptr->left(), target));
				}
				else{
					return 1;
				}
			}

			else if (target < node_ptr->data()){
				return counter(node_ptr->left(), target);
				}
			else {//if (target > node_ptr->data())
				return counter(node_ptr->right(), target);
				}
		
	}
	
	template <class ItemType> //DONE
	bool  BSTreeBag<ItemType>::bst_remove(binary_tree_node<ItemType>*& node_ptr, const ItemType& target)
	{
		if (node_ptr == NULL){
			return false;
		}
		else if (target == node_ptr->data()){
			if (node_ptr->left() == NULL){
				binary_tree_node<ItemType>* old_node_ptr = node_ptr;
				// replace with right child, even if NULL...that just means we have an empty tree there now
				node_ptr = node_ptr->right();
				delete old_node_ptr; // no leaks
				return true;
			}
			else{
				bst_remove_max(node_ptr->left(), node_ptr->data()); //node_ptr->data == target
				return true;
			}
		}
		else if (target < node_ptr->data()){
			return bst_remove(node_ptr->left(), target);
			}
		else {//if (target > node_ptr->data())
			return bst_remove(node_ptr->right(), target);
			}
	}

	
	template <class ItemType> //DONE
	void  BSTreeBag<ItemType>::bst_remove_max(binary_tree_node<ItemType>*& node_ptr, ItemType& removed)
	{
		if (node_ptr->right() == NULL){
			binary_tree_node<ItemType>* old_node_ptr = node_ptr;
			removed = node_ptr->data();
			node_ptr = node_ptr->left(); //the thing (above) that used to point to node_ptr is now pointing to its left child
			delete old_node_ptr;
			
		}
		else{ //node_ptr has a right-hand child (this has already been called on the left subtree up in remove
			bst_remove_max(node_ptr->right(), removed);
		}
	}	

	template <class ItemType> //DONE
	bool  BSTreeBag<ItemType>::bst_remove_all(binary_tree_node<ItemType>*& node_ptr, const ItemType& target)
	{
	//can make it return a count and then call your count function to see how many there are and go until you reach that...		
		unsigned int orig_count = count(target);
		for (unsigned int i = 0; i < orig_count; i++){
			bst_remove(root_ptr, target);
		}
		return (orig_count > 0);
	}	


//added as well to print
	template <class ItemType>	
	void  BSTreeBag<ItemType>::printtree() const
	{
		print(root_ptr, 0);
	}
		

//NON-MEMBER HELPER FUNCTIONS (no *this)
	template <class ItemType>
    BSTreeBag<ItemType> operator+(const BSTreeBag<ItemType>& b1, const BSTreeBag<ItemType>& b2)
    {
		BSTreeBag<ItemType> result = b1;
		result += b2;
		return result;
	}	
}
