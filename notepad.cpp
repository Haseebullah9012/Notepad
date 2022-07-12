#include<iostream>
#include<string>
using namespace std;

struct Node
{
    char character;
    Node *next;
    Node *prev;
};

class Notepad
{
    private:
        Node *head;
        Node *pointer;

    public:
        Notepad()
        {
            head = NULL;
            pointer = NULL;
        }
        void Display();
        void InsertChar(char);
};

int main()
{
    cout << endl;
    Notepad notepad;

    char choice;
    string data;
    bool exit = false;
    
    while(!exit)
    {
        cout << "\t\t Do you Want to \n"
             << "\t\t   D- Display Notepad    E- Enter Data \n"
             << "\t\t   Q- Quit Notepad: ";
        cin >> choice;
        cin.ignore(255,'\n');
        cout << endl;

        switch(choice)
        {
            case 'q':
            case 'Q':
                exit = true;
                break;
            
            case 'd':
            case 'D':
                notepad.Display();
                break;

            case 'e':
            case 'E':
                cout << "Enter Data: ";
                cin >> data;
                for(int i=0; i<data.length(); i++)
                    notepad.InsertChar(data[i]);
                break;
            
            default:
                cout << "Illegal Choice! Choose from the Menu. " << endl;
        }

        cout << endl;
    }

    cout << "Notepad Project \n"
        << "All Rights Reserved. \n";
    
    getchar();
    return 0;
}

void Notepad::Display()
{
    Node *temp = head;
    while(temp!=NULL) {
        cout << temp->character;    
        temp = temp->next;
    }
    cout << endl;
}

void Notepad::InsertChar(char c)
{
    Node *node = new Node;
    node->character = c;
    node->prev = NULL;
    node->next = NULL;
    
    if(head == NULL) {
        head = node;
        pointer = head;
        return;
    }
    
    pointer->next = node;
    node->prev = pointer;
    pointer = node;
}
