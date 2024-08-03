#include<iostream>
#include<fstream>
#include<string.h>
#include<iomanip>
using namespace std;

//#################################################//
//Utility functions								  //
//###############################################//

int isGreater(string, string);				
int showInterface();
string removeSpaces(string);
string toUpper(string);
string capitalizeFirstLetter(string);
string toLower(string);
int getNumberOfWordsInFile();
bool searchPhrase(string, string);

//#################################################//
//Binary tree    								  //
//###############################################//
class Node
{
	public:
		string word;
		string meaning;
		Node* left;
		Node* right;
		
		Node()
		{
			word = "";
			meaning = "";
			left = NULL;
			right = NULL;
		}
		
		Node(string word, string meaning)
		{
			this->word = word;
			this->meaning = meaning;
			left = NULL;
			right = NULL;
		}
};

class BinaryTree 
{
	private:
		Node* root;
		int numberOfWords;
	
	public:
		BinaryTree()
		{
			root = NULL;
			numberOfWords = 0;
		}
		
		// Getters for root & words#			
		Node* getRoot(){ return root; }
		int getNumberOfWords(){ return numberOfWords; } // for printing no. of words
		
		Node* insert(Node* currentNode, string passed_word, string passed_meaning)
		{
			if (currentNode == NULL) // when no word in dictionery case
			{
				Node* newNode = new Node(passed_word, passed_meaning);
				currentNode = newNode;
				numberOfWords++;
			} 
			//word is smaller or equal
			else if ( isGreater( passed_word, currentNode->word ) == -1 || isGreater( passed_word, currentNode->word) == 0 ) 		
				currentNode->left = insert(currentNode->left, passed_word, passed_meaning);
			//word is greater
			else 
				currentNode->right = insert(currentNode->right, passed_word, passed_meaning);
			
			root = currentNode;
			return currentNode;
		}
		
		bool wordExist(Node* currentNode, string passed_word)
		{
			if (currentNode == NULL)
				return false;
		
			if (toUpper(currentNode->word) == toUpper(passed_word)) // equal case
				return true;
			else if (isGreater( toUpper(passed_word) , toUpper(currentNode->word)) == -1) // if word is smaller traverse to left of BST
				return wordExist(currentNode->left, passed_word);
			else 
				return wordExist(currentNode->right, passed_word);	// if word is smaller traverse to right of BST
		}
		
		void viewAllWords()
		{
			Node* currNode = NULL;
			int k = 0;
			for (int i=1; i<=numberOfWords; i++)
			{
				k = i;
				currNode = kthSmallest(root, k);
				cout<<left<<setw(25)<<currNode->word<<" :"<<currNode->meaning<<endl;
			}
		}
		
		Node* searchWord(Node* currentNode, string wordToSearch)
		{
			if (currentNode == NULL)
				return NULL;
				
			if ( toUpper(currentNode->word) == toUpper(wordToSearch) ) // when word found !!
				return currentNode;
			else if ( isGreater( toUpper(wordToSearch), toUpper(currentNode->word) ) == 1 ) // if entered word is greater then current node word
				return searchWord(currentNode->right, wordToSearch);
			else
				return searchWord(currentNode->left, wordToSearch); // if entered word is smaller then current node word
		}
		
		void searchAllWords(Node* currentNode, string wordToSearch)
		{
			Node* currNode = NULL;
			int k = 0;
			for (int i=1; i<=numberOfWords; i++) // accessing all nodes in IN-ORDER travering (Left, current node, Right)
			{
				k = i;
				currNode = kthSmallest(root, k); 
				if ( searchPhrase( toUpper( wordToSearch ) , toUpper( currNode->word ) )) // for printing similar case match
					cout<<left<<setw(25)<<currNode->word<<" :"<<currNode->meaning<<endl;
			}
		}
		
		Node* deleteWord(Node* currentNode, string wordToDelete)
		{
			if (currentNode == NULL)
				return currentNode;
				
			if ( isGreater(wordToDelete, currentNode->word) == -1) //wordToDelete is smaller than current node word so traverse left
			{
				currentNode->left = deleteWord(currentNode->left, wordToDelete);
			} 
			else if (isGreater(wordToDelete, currentNode->word) == 1) //wordTodelete is greater than current node word so traverse right
			{			
				currentNode->right = deleteWord(currentNode->right, wordToDelete);
			} 
			
			else 												//wordToDelete and current node word are equal (means node found after traversing)
			{																	
				if (currentNode->left == NULL && currentNode->right == NULL) //currentNode has no children
				{		
					delete currentNode;
					currentNode = NULL;
				} 
				else if (currentNode->left == NULL) //currentNode has child on right (means left is NULL)
					{
					Node* temp = currentNode;
					currentNode = currentNode->right;
					delete temp;
				} 
				
				else if (currentNode->right == NULL) //currentNode has child on left (means right is NULL)
				{
					Node* temp = currentNode;
					currentNode = currentNode->left;
					delete temp;
				} 
				
				else // two children case
				{
					Node* temp = findSmallestNode(currentNode->right);
					currentNode->word = temp->word;
					currentNode->right = deleteWord(currentNode->right, temp->word);
				}
				numberOfWords--;
			}
			
			root = currentNode;
			return currentNode;
		}
		
		Node* findSmallestNode(Node* currentNode) // function to reach most left & last node
		{
			if (currentNode == NULL)
				return NULL;
			else if ( currentNode->left ==NULL)
				return currentNode;
			
			return findSmallestNode(currentNode->left);
		}
		
		
		//This function uses a in-order traversal to get the kth smallest element of the BST. 
		//It starts from the leftmost node, and returns the kth element it has visited.
		Node* kthSmallest(Node* currentNode, int& k) // in-order approach
		{
			if (currentNode == NULL)
		    	return NULL;
		 
		    Node* left = kthSmallest(currentNode->left, k); // left traversal till end
		 	    
		    if (left != NULL)
		        return left;		 
		    
		    k--;
		    if (k == 0)
		        return currentNode;
		 
		    return kthSmallest(currentNode->right, k); // right traversal
		}
};

//#################################################//
//program execution functions					  //
//###############################################//

void searchFunction(BinaryTree&);
void insertFunction(BinaryTree&);
void deleteFunction(BinaryTree&);
void searchAllFunction(BinaryTree&);

//#################################################//
//global variables								  //
//###############################################//

string filename = "WordsFile.txt";


//#################################################//
//Main											  //
//###############################################//

int main()
{
	BinaryTree BT;
	ifstream file(filename.c_str(), ios::in);
	
	// Loading content
	while(!file.eof())
	{
		string word, meaning;
		getline(file, word, ':');
		getline(file, meaning);
		BT.insert(BT.getRoot(), word, meaning);	
	}	
	
	file.close();
	
	while (true)
		switch(showInterface())
		{
			case 1:
			{
				BT.viewAllWords(); // Direct use of BT function
				system("PAUSE");
				system("CLS");   //Clearing the sreen
				break;
			}
			case 2:
			{
				searchFunction(BT);
				system("PAUSE");
				system("CLS");   //Clearing the sreen
				break;
			}
			case 3:
			{
				searchAllFunction(BT);
				system("PAUSE");
				system("CLS");   //Clearing the sreen
				break;
			}
			case 4:
			{
				insertFunction(BT);
				system("PAUSE");
				system("CLS");   //Clearing the sreen
				break;
			}
			case 5:
			{
				deleteFunction(BT);
				system("PAUSE");
				system("CLS");   //Clearing the sreen
				break;
			}
			case 6:
			{
				cout<<"				Number of Words in the Dictionary = "<<BT.getNumberOfWords()<<endl;
				system("PAUSE");
				system("CLS");   //Clearing the sreen
				break;
			}
			case 7:
			{
				cout<<"\nExiting Dictionary!";
				exit(0);
				break;
			}
			default:
			{
				cout<<"Exception Occured";
				system("PAUSE");
				system("CLS");   //Clearing the sreen
			}
		}
	
}


//#################################################//
//Logic functions								  //
//###############################################//

int isGreater(string word1, string word2){
	for (int i=0; i<word1.length(); i++){
		if (word1[i] == word2[i])
			continue;
		else if (word1[i] < word2[i])
			return -1;						//word1 is smaller than word2 return -1
		else
			return 1;						//word1 is bigger than word2 return 1
	}
	return 0;								//both words are equal return 0
}

int showInterface() // Function to show menus as well get user decsion with exception handling !!!
{
	cout<<"				 ----------------------"<<endl;
	cout<<"				|                      |"<<endl;
	cout<<"				|     DICTIONARY       |"<<endl;
	cout<<"				|               BY A^2 |"<<endl;
	cout<<"				|                      |"<<endl;
	cout<<"				 ----------------------"<<endl;
	
	int option;
	cout<<"\n\n		========================================================"<<endl;
	cout<<"				1 -> View All Words"<<endl;
	cout<<"				2 -> Search for a Word"<<endl;
	cout<<"				3 -> Search for all Words"<<endl;
	cout<<"				4 -> Insert a Word into Dictionary"<<endl;
	cout<<"				5 -> Delete a Word from Dictionary"<<endl;
	cout<<"				6 -> Show number of words in Dictionary"<<endl;
	cout<<"				7 -> Exit Program"<<endl;
	
	
	// !!!!!!!!!! INPUT SANITIZATION !!!!!!!!!!! (to catch oversmart one!!! :)   --but BUFFER OVERLOADING upto 256 charaters :( 
	do {
		cout<<"\n\n				Option : ";
		cin>>option;
		
		if (cin.fail())
		{
			cout<<"				Please Enter an integer!!!"<<endl;
			cin.clear();
        	cin.ignore(256,'\n');
		}
		else if (option < 1 || option > 7)
			cout<<"				Please Enter a Number Between 1 and 7!!!"<<endl;
	} while (option < 1 || option > 7);
	
	return option;
}

void searchFunction(BinaryTree &BT)
{
	string wordToSearch;
	cout<<"				Enter word to search : ";
	cin>>wordToSearch;
	cout<<endl;
	
	Node* searchedNode = BT.searchWord(BT.getRoot(), wordToSearch);
	if (searchedNode)
		cout<<"				"<<searchedNode->word<<": "<<searchedNode->meaning<<endl;
	else 
		cout<<"				"<<wordToSearch<<"				Not found in Dictionary!"<<endl;
}

void searchAllFunction(BinaryTree &BT)
{
	string wordToSearch;
	cout<<"				Enter word to search : ";
	cin>>wordToSearch;
	cout<<endl;
	
	BT.searchAllWords(BT.getRoot(), wordToSearch);
}

void insertFunction(BinaryTree &BT)
{
	bool wordAlreadyExist;
	string wordToInsert, meaning;
	
	cout<<"				Enter word to insert : ";
	cin>>wordToInsert;
	
	wordAlreadyExist = BT.wordExist(BT.getRoot(), wordToInsert); // checking word already exist or NOT!!!
	
	if (wordAlreadyExist)
		cout<<"				"<<wordToInsert<<" already exists in Dictionary!"<<endl;
	else
	{
		cout<<"				Enter meaning of "<<wordToInsert<<" : ";
		cin.ignore();
		getline(cin, meaning);
		
		wordToInsert = capitalizeFirstLetter(wordToInsert);
		meaning = capitalizeFirstLetter(meaning);
		BT.insert(BT.getRoot(), wordToInsert, meaning);
		
		// Inserting in file
		ofstream file(filename.c_str(), ios::app);
		file<<"\n"<<wordToInsert<<": "<<meaning;
		file.close();
	}		
}

void deleteFunction(BinaryTree &BT)
{
	string wordToDelete, fileData;
	cout<<"				Enter word that you want to delete : ";
	cin>>wordToDelete;
	
	//delete word from tree
	if (!BT.wordExist(BT.getRoot(), wordToDelete))
		cout<<"				"<<wordToDelete<<" does not exist in Dictionary!"<<endl;
	else 
		BT.deleteWord(BT.getRoot(), wordToDelete);
	
	
	//delete word from file
	string currentWordInFile;
	ifstream file(filename.c_str(), ios::in);
	ofstream temp("temp.txt", ios::out);
	bool firstWord = true; 
	
	while (getline(file, fileData, ':'))
	{
		if (toUpper(fileData) != toUpper(wordToDelete))
		{
			if (firstWord)
			{
				temp<<fileData<<":";
				firstWord = false;
			}
			else
				temp<<"\n"<<fileData<<":";
			getline(file, fileData, '\n');
			temp<<fileData;
		} 
		else 
			getline(file, fileData, '\n');
	}
	file.close();
	temp.close();
	
	remove(filename.c_str());
	rename("temp.txt", filename.c_str());
}

//#################################################//
//Utility functions								  //
//###############################################//


string toUpper(string str)
{
	for (int i=0; i<str.length(); i++)
		str[i] = toupper(str[i]);	
	return str;
}

string capitalizeFirstLetter(string str)
{
	str[0] = toupper(str[0]);
	return str;
}

string toLower(string str)
{
	for (int i=0; i<str.length(); i++)
		str[i] = tolower(str[i]);	
	return str;
}

int getNumberOfWordsInFile()
{
	int numberOfWords = 0;
	string line;
	ifstream file(filename.c_str(), ios::in);
	while (getline(file, line))
		numberOfWords++;
		
	file.close();
	
	return numberOfWords;
}

/*
This function checks whether a search phrase is present in a sentence or not. 
It does so by comparing the search phrase with each substring of the sentence of the same length as the search phrase.
*/

bool searchPhrase(string search, string sentence)
{	
	if (search.length() > sentence.length()) // given word is greater then current node word
		return false;
	
	for (int i=0; i<sentence.length(); i++)
	{
		bool equal = true;
		
		for (int j=0; j<search.length(); j++)
			if (sentence[i+j] != search[j])
			{
				equal = false;
				break;
			}
		
		if (equal)
			return true;
	}
	return false;
}
