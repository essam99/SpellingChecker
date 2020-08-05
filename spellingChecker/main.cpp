#include <bits/stdc++.h>
using namespace std;

int const alphabetic_count = 26;
int const no_correction = 25;

void insert(string);
int search(string);
void dictionary();
void myFile();
void addDictionary();
bool searchOnly(string);

struct node
{
    bool isEnd;
	struct node *child[alphabetic_count];

	node()
	{
	    isEnd = false;
	    for (int i = 0; i < alphabetic_count; i++)
            child[i] = NULL;
	}
};

node *head = new node();
string arr[no_correction]={};

int main()
{
    dictionary();
    char choose;
    do{
        cout << "\t\t\t Spelling Checker \n\n";
        cout << "choose from the following list: \n\n";
        cout << "1.Add new word to the dictionary \n";
        cout << "2.Check a word \n";
        cout << "3.Check my sentences from a file \n";
        cout << "4.Exit\n";
        cin >> choose;
        if(choose == '1')
        {
            system("cmd /c cls");
            addDictionary();
            cout << "\nPress any key to continue...\n";
            cin.ignore();
            cin.get();
            system("cmd /c cls");
        }
        else if(choose == '2')
        {
            system("cmd /c cls");
            string searchWord;
            cout << "Enter word to check: ";
            cin >> searchWord;

            if(search(searchWord)==1)
                cout << "\nCorrect word.\n";
            else if(search(searchWord)==2)
            {
                if(arr[0]!="")
                    cout << "\nDo you mean ";
                else
                    cout << "\nWrong word.\n";
                for(int i=0; (i<no_correction && arr[i]!=""); i++)
                {
                    if(i==no_correction-1)
                        cout << arr[i] << "?\n";
                    else if(arr[i+1]=="")
                        cout << arr[i] << "?\n";
                    else if(arr[i+2]=="")
                        cout << arr[i] << " or ";
                    else
                        cout << arr[i] << ", ";
                }
            }
            else
                cout << "\nWrong word.\n";

            cout << "\nPress any key to continue...\n";
            cin.ignore();
            cin.get();
            system("cmd /c cls");
        }
        else if(choose == '3')
        {
            system("cmd /c cls");
            cout << "The following words are wrong\n\n";
            myFile();
            cout << "\nPress any key to continue...\n";
            cin.ignore();
            cin.get();
            system("cmd /c cls");
        }
        else
            system("cmd /c cls");
    }while (choose != '4');

	return 0;
}

void insert(string word)
{
	node *current = head;
	for(int i = 0; i < word.length(); ++i)
	{
		int letter = (int)word[i] - (int)'a';
		if(current->child[letter] == NULL)
			current->child[letter] = new node();
        current = current->child[letter];
	}
	current->isEnd = true;
}

int search(string word)
{
	node *current = head;
	int tolerance = 0, correction = 0, y, x=-1;
	// correction = 1 --> 2 letters X, =2 --> wrong letter
    char c[no_correction]={};
	for(int i=0; i<no_correction; i++)
        arr[i]="";
	for(int i = 0; (i < word.length()  &&  tolerance<=1 ); ++i)
	{
	    if(isupper(word[i]))
            word[i]=tolower(word[i]);

		int letter = (int)word[i] - (int)'a';
		if(current->child[letter] == NULL)
        {
            tolerance++;
            int letter2 = (int)word[i+1] - (int)'a';
            bool b = false;
            if(current->child[letter2]!=NULL && current->child[letter2]->child[letter]!=NULL && tolerance<2)
            {
                char temp = word[i];
                word[i]=word[i+1];
                word[i+1]=temp;
                current=current->child[letter2]->child[letter];
                i++;
                correction=1;
            }
            else if(tolerance <2)
            {
                for(int j=0; (j<alphabetic_count && x<no_correction); j++)
                    if(current->child[j] != NULL  && current->child[j]->child[letter2] != NULL)
                    {
                        x++;
                        c[x]='a'+j;
                        y = i;
                        correction=2;
                        b = true;
                    }
                if(b)
                {
                    current=current->child[c[0]-'a']->child[letter2];
                    i++;
                }
                else if(i == word.length()-1)
                {
                    for(int j=0; (j<alphabetic_count && x<no_correction); j++)
                        if(current->child[j] != NULL && current->child[j]->isEnd )
                        {
                            x++;
                            c[x]='a'+j;
                            y = i;
                            correction=2;
                            b = true;
                        }
                    if(b)
                        current=current->child[c[0]-'a'];
                }
            }
        }
        else
            current = current->child[letter];
	}
	// function return = 0 --> wrong & no correction, 1 --> right, 2 --> correction
	if(current->isEnd && tolerance==1)
    {
        if(correction==1)
        {
            arr[0]=word;
        }
        else if(correction==2)
        {
            while(x>=0)
            {
                word[y]=c[x];
                arr[x]=word;
                x--;
            }
        }
        return 2;
    }
    else if(current->isEnd)
        return 1;
    else return 0;
}

void dictionary()
{
    ifstream file ("dictionary.txt");
    string word;
    if(file.is_open())
    {
        while (file >> word)
        {
            insert(word);
        }
        file.close();
    }
    else cout<<"Unable to open dictionary\n";
}

void myFile()
{
    ifstream file ("myFile.txt");
    string word;
    if(file.is_open())
    {
        while (file >> word)
        {
            if(word[word.length()-1]==',' || word[word.length()-1]=='.' || word[word.length()-1]=='?' || word[word.length()-1]=='!')
                word = word.substr(0, word.size()-1);
            for(int i=0; i<word.length(); i++)
                if(isupper(word[i]))
                    word[i]=tolower(word[i]);
            if(search(word)==0)
                cout << word << '\n';
            else if(search(word)==2)
            {
                cout << word;
                if(arr[0]!="")
                    cout << "\tDo you mean ";
                else
                    cout << '\n';

                for(int i=0; (i<no_correction && arr[i]!=""); i++)
                {
                    if(i==no_correction-1)
                        cout << arr[i] << "?\n";
                    else if(arr[i+1]=="")
                        cout << arr[i] << "?\n";
                    else if(arr[i+2]=="")
                        cout << arr[i] << " or ";
                    else
                        cout << arr[i] << ", ";
                }
            }
        }
        file.close();
    }
    else cout<<"Unable to open file\n";
}

bool searchOnly(string word)
{
	node *current = head;
	for(int i = 0; i < word.length(); ++i)
	{
		int letter = (int)word[i] - (int)'a';
		if(current->child[letter] == NULL)
			return false;
		current = current->child[letter];
	}
	return current->isEnd;
}

void addDictionary()
{
    string newWord;
    fstream fs;
    fs.open ("dictionary.txt", fstream::in | fstream::out | fstream::app);
    cout << "Enter the new word: ";
    cin >> newWord;
    if(!searchOnly(newWord))
    {
        insert(newWord);
        fs << endl << newWord;
        cout << "\nWord added successfully.\n";
    }
    else
        cout << "\nWord already exists.\n";
    fs.close();
}

