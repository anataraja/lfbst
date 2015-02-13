

seekRecord_t * insseek(thread_data_t * data, long key, int op){
	
	node_t * gpar = NULL; // last node (ancestor of parent on access path) whose child pointer field is unmarked
	node_t * par = data->rootOfTree;
	node_t * leaf;
	node_t * leafchild;
	
	
	AO_t parentPointerWord = 0; // contents in gpar
	AO_t leafPointerWord = par->lchild; // contents in par. Tree has two imaginary keys \inf_{1} and \inf_{2} which are larger than all other keys. 
	AO_t leafchildPointerWord; // contents in leaf
	
	bool isparLC = false; // is par the left child of gpar
	bool isleafLC = true; // is leaf the left child of par
	bool isleafchildLC; // is leafchild the left child of leaf
	
	
	leaf = (node_t *)get_addr(leafPointerWord);
		if(key < leaf->key){
			leafchildPointerWord = leaf->lchild;
			isleafchildLC = true;
			
		}
		else{
			leafchildPointerWord = leaf->rchild;
			isleafchildLC = false;
		}
	
	leafchild = (node_t *)get_addr(leafchildPointerWord);
	
	
	
	while(leafchild != NULL){
		if(!is_marked(leafPointerWord)){
			gpar = par;
			parentPointerWord = leafPointerWord;
			isparLC = isleafLC;
		}
		
		par = leaf;
		leafPointerWord = leafchildPointerWord;
		isleafLC = isleafchildLC;
		
		leaf = leafchild;
		
		
		if(key < leaf->key){
			leafchildPointerWord = leaf->lchild;
			isleafchildLC = true;
		}
		else{
			leafchildPointerWord = leaf->rchild;
			isleafchildLC = false;
		}	
		
		leafchild = (node_t *)get_addr(leafchildPointerWord);
		
	}
	
	if(key == leaf->key){
    // key matches that being inserted	
	  return NULL;
	}
	
	seekRecord_t * R = data->sr;
	
	R->leafKey = leaf->key;
		
	R->parent = par;
	
	R->pL = leafPointerWord;
	
	R->isLeftL = isleafLC;
	
	
	R->lum = gpar;
	R->lumC = parentPointerWord;	
	R->isLeftUM = isparLC;
	return R;
}


seekRecord_t * delseek(thread_data_t * data, long key, int op){
	node_t * gpar = NULL; // last node (ancestor of parent on access path) whose child pointer field is unmarked
	node_t * par = data->rootOfTree;
	node_t * leaf;
	node_t * leafchild;
	
	
	AO_t parentPointerWord = 0; // contents in gpar
	AO_t leafPointerWord = par->lchild; // contents in par. Tree has two imaginary keys \inf_{1} and \inf_{2} which are larger than all other keys. 
	AO_t leafchildPointerWord; // contents in leaf
	
	bool isparLC = false; // is par the left child of gpar
	bool isleafLC = true; // is leaf the left child of par
	bool isleafchildLC; // is leafchild the left child of leaf
	
	
	leaf = (node_t *)get_addr(leafPointerWord);
		if(key < leaf->key){
			leafchildPointerWord = leaf->lchild;
			isleafchildLC = true;
			
		}
		else{
			leafchildPointerWord = leaf->rchild;
			isleafchildLC = false;
		}
	
	leafchild = (node_t *)get_addr(leafchildPointerWord);
	
	
	
	while(leafchild != NULL){
		if(!is_marked(leafPointerWord)){
			gpar = par;
			parentPointerWord = leafPointerWord;
			isparLC = isleafLC;
		}
		
		par = leaf;
		leafPointerWord = leafchildPointerWord;
		isleafLC = isleafchildLC;
		
		leaf = leafchild;
		
		
		if(key < leaf->key){
			leafchildPointerWord = leaf->lchild;
			isleafchildLC = true;
		}
		else{
			leafchildPointerWord = leaf->rchild;
			isleafchildLC = false;
		}	
		
		leafchild = (node_t *)get_addr(leafchildPointerWord);
		
	}
		
			// op = DEL
	if(key != leaf->key){
	  // key is not found in the tree.
		return NULL;
	}
		
	seekRecord_t * R = data->sr;
	
	R->leafKey = leaf->key;
		
	R->parent = par;
	
	R->pL = leafPointerWord;
	
	R->isLeftL = isleafLC;
	
	
	R->lum = gpar;
	R->lumC = parentPointerWord;	
	R->isLeftUM = isparLC;

	return R;
}


seekRecord_t * secondary_seek(thread_data_t * data, long key, seekRecord_t * sr){
	
	node_t * flaggedLeaf = (node_t *)get_addr(sr->pL);
	node_t * gpar = NULL; // last node (ancestor of parent on access path) whose child pointer field is unmarked
	node_t * par = data->rootOfTree;
	node_t * leaf;
	node_t * leafchild;
	
	AO_t parentPointerWord = 0; // contents in gpar
	AO_t leafPointerWord = par->lchild; // contents in par. Tree has two imaginary keys \inf_{1} and \inf_{2} which are larger than all other keys. 
	AO_t leafchildPointerWord; // contents in leaf
	
	bool isparLC = false; // is par the left child of gpar
	bool isleafLC = true; // is leaf the left child of par
	bool isleafchildLC; // is leafchild the left child of leaf
	
	
	leaf = (node_t *)get_addr(leafPointerWord);
	if(key < leaf->key){
	  leafchildPointerWord = leaf->lchild;
		isleafchildLC = true;
	}
	else{
		leafchildPointerWord = leaf->rchild;
		isleafchildLC = false;
	}
	
	leafchild = (node_t *)get_addr(leafchildPointerWord);
	
  while(leafchild != NULL){
		if(!is_marked(leafPointerWord)){
			gpar = par;
			parentPointerWord = leafPointerWord;
			isparLC = isleafLC;
		}
		
		par = leaf;
		leafPointerWord = leafchildPointerWord;
		isleafLC = isleafchildLC;
		
		leaf = leafchild;
		
		if(key < leaf->key){
			leafchildPointerWord = leaf->lchild;
			isleafchildLC = true;
		}
		else{
			leafchildPointerWord = leaf->rchild;
			isleafchildLC = false;
		}	
		
		leafchild = (node_t *)get_addr(leafchildPointerWord);
		
	}
			
	if( !is_flagged(leafPointerWord) || (leaf != flaggedLeaf) ){
		// operation has been completed by another process.
		return NULL;		
	 }
	
	seekRecord_t * R = data->ssr;
	
	R->leafKey = leaf->key;
		
	R->parent = par;
	
	R->pL = leafPointerWord;
	
	R->isLeftL = isleafLC;
	
	
	R->lum = gpar;
	R->lumC = parentPointerWord;	
	R->isLeftUM = isparLC;
	
  return R;
}

bool search(thread_data_t * data, long key){
	
	node_t * cur = (node_t *)get_addr(data->rootOfTree->lchild);
	long lastKey;	
	while(cur != NULL){
	  lastKey = cur->key;
		cur = (key < lastKey? (node_t *)get_addr(cur->lchild): (node_t *)get_addr(cur->rchild));
	}
	
  return (key == lastKey);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------

int help_conflicting_operation (thread_data_t * data, seekRecord_t * R){

	if(is_flagged(R->pL)){
		// leaf node is flagged for deletion by another process.
		//1. mark sibling of leaf node for deletion and then read its contents.
		AO_t pS;
		
		if(R->isLeftL){
			// L is the left child of P
			mark_Node(&R->parent->rchild);
			pS = R->parent->rchild;
			
		}
		else{
			mark_Node(&R->parent->lchild);
			pS = R->parent->lchild;
		}
		
		// 2. Execute cas on the last unmarked node to remove the 
		// if pS is flagged, propagate it. 
		AO_t newWord;
		
		if(is_flagged(pS)){
			newWord = create_child_word((node_t *)get_addr(pS), UNMARK, FLAG);	
		}
		else{
			newWord = create_child_word((node_t *)get_addr(pS), UNMARK, UNFLAG);
		}
		
		int result;
		
		if(R->isLeftUM){
			 result = atomic_cas_full(&R->lum->lchild, R->lumC, newWord);
		}
		else{
			 result = atomic_cas_full(&R->lum->rchild, R->lumC, newWord);
		}
		
		return result; 
		
	}
	else{
		// leaf node is marked for deletion by another process.
		// Note that leaf is not flagged, as it will be taken care of in the above case.
		
		AO_t newWord;
		
		if(is_flagged(R->pL)){
			newWord = create_child_word((node_t *)get_addr(R->pL), UNMARK, FLAG);
		}
		else{
			newWord = create_child_word((node_t *)get_addr(R->pL), UNMARK, UNFLAG);
		}
		
		int result;
		
		if(R->isLeftUM){
			 result = atomic_cas_full(&R->lum->lchild, R->lumC, newWord);
		}
		else{
			result = atomic_cas_full(&R->lum->rchild, R->lumC, newWord);
		}
		
    return result; 
	}	
		
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------


int inject(thread_data_t * data, seekRecord_t * R, int op){
	
		// pL is free
		
		//1. Flag L
		
		AO_t newWord = create_child_word((node_t *)get_addr(R->pL),UNMARK,FLAG);
		
		int result; 
		
		if(R->isLeftL){
			result = atomic_cas_full(&R->parent->lchild, R->pL, newWord);
			
		}
		else{
			result = atomic_cas_full(&R->parent->rchild, R->pL, newWord);
		}
		
		return result;
}

bool insert(thread_data_t * data, long key){
  int injectResult;
  int fasttry = 0;	
	
	while(true){
		seekRecord_t * R = insseek(data, key, INS);
		fasttry++;
		if(R == NULL){
			if(fasttry == 1){
				return false;
			}
			else{
				return true;
			}	
		}
		
		if(!is_free(R->pL)){
		  help_conflicting_operation(data, R);
			continue;
		}
		
		// key not present in the tree. Insert		
		injectResult = perform_one_insert_window_operation(data, R, key);
		
		if(injectResult == 1){
			// Operation injected and executed
			
			return true;
		}
		
	}
	// execute insert window operation.	
} 

bool delete_node(thread_data_t * data, long key){
	int injectResult;
	
	while(true){
		seekRecord_t * R = delseek(data, key, DEL);
		
		if(R == NULL){
			return false;
		}
		
		// key is present in the tree. Inject operation into the tree
		
		if(!is_free(R->pL)){
			
				help_conflicting_operation(data, R);
			
			continue;
		}
		
		injectResult = inject(data, R, DEL);

		if(injectResult == 1){
			// Operation injected 
			
			data->numActualDelete++;
			
			int res = perform_one_delete_window_operation(data, R, key);
			
			if(res == 1){
				// operation successfully executed.
				return true;
			}
			else{
				// window transaction could not be executed.
				// perform secondary seek.
				
				while(true){
					R = secondary_seek(data, key, R);
					
					if(R == NULL){
						// flagged leaf not found. Operation has been executed by some other process.
						return false;
					}
					
					res = perform_one_delete_window_operation(data, R, key);
					
					if(res == 1){
						return true;
					}
				}
			}
		}
		// otherwise, operation was not injected. Restart.
	}
}

