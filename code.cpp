
//-std=c++11


#include <iostream>
#include <string>
#include <map>
#include <vector> //or #include <list>
#include <fstream>

// do not alter the struct declaration
struct hufftreenode {
	char character;
	int frequency;
	hufftreenode* left;
	hufftreenode* right;
};

// do not alter the typedef 
typedef hufftreenode* hufftreeptr;


// you can define and use any additional *structures* if you might need to




/////////////////////////////////////////
// Functions declarations (prototypes) //
/////////////////////////////////////////

// do not alter these functions declarations

//utility function
bool valid_hufftree(const hufftreeptr hufftree);
 // returns true if the input hufftree is a valid Huffman tree
 // i.e. all the terminal nodes (leaves) have characters,
 // all the non-leaf nodes have two sub-trees each,
 // and the occurrence frequency of a non-leaf node equals
 // the sum of the occurrence frequencies of its two sub-trees.



std::string huffencode(const std::string& message, std::map<char, int>& freqtable, hufftreeptr& hufftree, std::map<char, std::string>& hufftable);
// encodes an input message (a string of characters) into an encoded message (string of bits) using the Huffman coding
// builds the Huffman tree and its associated encoding table (i.e. the association between the characters in the tree leaves and their encodings (string of bits))

std::string huffdecode(const std::string& encodedmessage, const hufftreeptr& hufftree);
// decodes an encoded message (a string of bits) into the original message (a string of characters) using the Huffman tree built during the encoding

int lowest_freq_index(const std::vector<hufftreeptr> forest);
void remove_index(int i, std::vector<hufftreeptr>& from);
void connect_2_trees(hufftreeptr tree_1, hufftreeptr tree_2, hufftreeptr& joined);
void generate_codes(hufftreeptr root, std::map<char, std::string>& table, std::string so_far);
bool is_leaf(hufftreeptr root);

// you can define and use any additional *functions* if you might need to

// YOUR CODE HERE FOR ADDIIONAL FUNCTIONS DECLARATION (NOT THE IMPLEMENTATION), IF ANY


std::string huffdecode(const std::string& encodedmessage, const hufftreeptr& hufftree){

	hufftreeptr traverse = hufftree;
	std::string decoded_msg = "";

	for(int i = 0; i <= encodedmessage.length(); i++){
		if(is_leaf(traverse)){
			decoded_msg += traverse->character;
			traverse = hufftree;
			i--;
		}else{
			switch(encodedmessage[i]){
				case '0': traverse = traverse->left;  break;
				case '1': traverse = traverse->right; break; 
			}
		}
	}


	return decoded_msg;


}



std::string huffencode(const std::string& message, std::map<char, int>& freqtable, hufftreeptr& hufftree, std::map<char, std::string>& hufftable){

	for(int i = 0; i < message.length(); i++){
		std::map<char,int>::iterator it = freqtable.find(message[i]);
		if(it != freqtable.end()){
			//found
			int tmp = freqtable[message[i]];
			freqtable[message[i]] = tmp + 1;
		}else{
			freqtable[message[i]] = 1;
		}
	}

	//table generated checked and correct
	//iterate through all the characters and their
	//correspoding frequency and generate these trees

	std::vector<hufftreeptr> forest; 
	std::cout << "\n";
	for(std::map<char,int>::iterator it = freqtable.begin(); it != freqtable.end(); it++){
		//creating the tree
		hufftreeptr binary_tree = new hufftreenode;
		binary_tree -> left = NULL;
		binary_tree -> right = NULL;
		binary_tree -> character = it -> first;
		binary_tree ->frequency = it -> second;

		forest.push_back(binary_tree);
	}

	//forest generated correctly
	while(forest.size() != 1){

		hufftreeptr tree_1;
		hufftreeptr tree_2;
		hufftreeptr tree_3;

		int f1 = lowest_freq_index(forest);
		tree_1 = forest[f1];
		remove_index(f1,forest);
		int f2 = lowest_freq_index(forest);
		tree_2 = forest[f2];
		remove_index(f2,forest);

		connect_2_trees(tree_1,tree_2,tree_3);

		forest.push_back(tree_3);


	}

	hufftree = forest[0];

	// tree generated correctly

	//generating the table

	generate_codes(hufftree, hufftable,"");

	//table generated correctly

	//generating the encoded message

	std::string encoded = "";

	for(int i = 0; i < message.size(); i++){
		encoded += hufftable[message[i]];
	}

	return encoded;

}

bool is_leaf(hufftreeptr root){
	if(root->left == NULL && root->right == NULL){
		return true;
	}else{
		return false;
	}

}

void generate_codes(hufftreeptr root, std::map<char, std::string>& table, std::string so_far){
	if(is_leaf(root)){
		table[root->character] = so_far;
	}else{
		generate_codes(root->left, table, so_far + "0");
		generate_codes(root->right,table,so_far + "1");
	}

}



void connect_2_trees(hufftreeptr tree_1, hufftreeptr tree_2, hufftreeptr& joined){
	hufftreeptr tmp = new hufftreenode;
	int total_freq = tree_2 -> frequency + tree_1 -> frequency;
	tmp -> frequency = total_freq;
	tmp -> character = '\0';
	if(tree_1 -> frequency > tree_2 -> frequency){
		tmp -> left = tree_2;
		tmp -> right = tree_1;
	}else{
		tmp->left = tree_1;
		tmp->right = tree_2;
	}

	joined = tmp;
}



//Need to make sure that the argument to this
//has at least 1 element
int lowest_freq_index(const std::vector<hufftreeptr> forest){
	int lf = forest[0] -> frequency;
	int index = 0;

	for(int i = 0; i < forest.size(); i++){
		if(forest[i] -> frequency < lf){
			lf = forest[i] -> frequency;
			index = i;
		}
	}
	return index;
}

void remove_index(int i, std::vector<hufftreeptr>& from){
	from.erase(from.begin() + i);
}



///////////////////
// main function //
///////////////////


//IMPORTANT (Reminder): before submitting your source code, please make sure to comment your main function (needed for automated testing)



/////////////////////////////////////////////
// Functions definitions (implementations) //
/////////////////////////////////////////////

// YOUR CODE HERE FOR THE FUNCTIONS IMPLEMENTATIONS




int main() {

	// YOUR CODE HERE FOR TESTING YOUR PROGRAM

	// an example of basic encoding/decoding
	std::string message, encodedmessage, decodedmessage;
	hufftreeptr hufftree;
	std::map<char, int> freqtable;
	std::map<char, std::string> hufftable;
	
	std::ifstream ifs("myfile.txt");
  	std::string msg;
	msg.assign( (std::istreambuf_iterator<char>(ifs) ),
                (std::istreambuf_iterator<char>()    ) );
	
	// 1) encoding (compression)
	encodedmessage = huffencode(msg, freqtable, hufftree, hufftable);
	// freqtable should be as in Table 1.
	// hufftree might be as in Figure 2 (remember, the Huffman coding tree IS NOT UNIQUE. If we swap two siblings in a Huffman tree, it is still a Huffman tree of the same message.)
	// hufftable should correspond to hufftree, and might be as in Table 2 <' ',"100">, <'g',"00">, <'o',"01">, <'p',"1110">, <'h',"1101">, <'e',"101">, <'r',"1111">, <'s',"1100">.
	// encodedmessage might then be the 37-bits string "0001100000110000011110110110111111100" (i.e. "00.01.100.00.01.100.00.01.1110.1101.101.1111.1100")

	if(valid_hufftree(hufftree))  {
		std::cout << "valid Huffman tree." << std::endl;
	}
	else {
		std::cout << "not valid Huffman tree." << std::endl;
	}

	// 2) decoding (uncompression)
	decodedmessage = huffdecode(encodedmessage, hufftree);
	// should be "go go gophers"
	
	if(decodedmessage == msg) {
		std::cout << "decoding OK." << std::endl;
	}
	else {
		std::cout << "decoding not OK." << std::endl;
	}

	return 0;
}



bool valid_hufftree(const hufftreeptr hufftree){

 	if(is_leaf(hufftree)){
 		if(hufftree -> character != '\0'){
 			return true;
 		}else{
 			false;
 		}
 	}else{
 		if(hufftree -> left != NULL && hufftree -> right != NULL){
 			//both left and right exist
 			if(hufftree -> left -> frequency + hufftree -> right -> frequency == hufftree -> frequency){
 				if(hufftree -> character == '\0'){
 					// the joint frequency checks out
 					//and also the character is only the escape character. 
 					return (valid_hufftree(hufftree -> left) && valid_hufftree(hufftree -> right));
 				}else{
 					return false;
 				}
 			}else{
 				return false;
 			}
 		}else{
 			return false;
 		}
 		//check that both left and right exist
 		//check if the joint frequency of left and right adds to this frequency
 		//check the character is only \0

 	}

 }






// pass the root node of your binary tree
