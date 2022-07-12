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
        void movePointer(char);
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
             << "\t\t   M- Move Pointer \n"
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
            
            case 'm':
            case 'M':
                cout << "Where to Move (A-left, D-right): ";
                cin >> data;
                for(int i=0; i<data.length(); i++)
                    notepad.movePointer(data[i]);
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
    bool cursorShown = false;

    if(pointer==NULL) {
        cout << "\033[36m" << "|" << "\033[0m";
        cursorShown = true;
    }
    
    while(temp!=NULL)
    {   
        cout << temp->character;
        if(temp == pointer) {
            cout << "\033[36m" << "|" << "\033[0m";
            cursorShown = true;
        }
        temp = temp->next;
    }

    if(!cursorShown && pointer==NULL)
        cout << "\033[36m" << "|" << "\033[0m";
    
    cout << endl;
}

void Notepad::InsertChar(char c)
{
    Node *node = new Node;
    node->character = c;
    node->prev = NULL;
    node->next = NULL;
    
    if(pointer == NULL)
    {
        if(head!=NULL) {
            node->next = head;
            head->prev = node;
        }
        
        head = node;
        pointer = head;
        return;
    }
    
    node->next = pointer->next;
    pointer->next = node;
    node->prev = pointer;
    pointer = node;
}

void Notepad::movePointer(char c)
{
    switch(c)
    {
        case 'a':
        case 'A':
            if(pointer==NULL)
                break;
            else if(pointer->prev!=NULL)
                pointer = pointer->prev;
            else if(pointer == head)
                pointer = NULL;
            break;
        
        case 'd':
        case 'D':
            if(pointer==NULL)
                pointer = head;
            else if(pointer->next!=NULL)
                pointer = pointer->next;
            break;

        default:
            break;
    }
}
