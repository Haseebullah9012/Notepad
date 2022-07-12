#include<iostream>
#include<string>
using namespace std;

struct Node
{
    char character;
    Node *next;
    Node *prev;
    Node *up;
    Node *down;
};

class Notepad
{
    private:
        Node *head;
        Node *lineStart;
        Node *pointer;

    public:
        Notepad()
        {
            head = NULL;
            lineStart = NULL;
            pointer = NULL;
        }
        
        void Display();
        void InsertChar(char);
        void RemoveChar();
        
        void movePointer(char);
        void NewLine();
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
             << "\t\t   D- Display Notepad    E- Enter Data    R- Remove Data \n"
             << "\t\t   M- Move Pointer       N- New Line \n"
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
            
            case 'r':
            case 'R':
                notepad.RemoveChar();
                break;
            
            case 'm':
            case 'M':
                cout << "Where to Move (A-left, D-right, W-Up, S-Down): ";
                cin >> data;
                for(int i=0; i<data.length(); i++)
                    notepad.movePointer(data[i]);
                break;
            
            case 'n':
            case 'N':
                notepad.NewLine();
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
    Node *tempR; //TempRight
    Node *tempD = head; //TempDown
    bool cursorShown = false;

    while(tempD!=NULL)
    {
        if(tempD==lineStart && pointer==NULL) {
            cout << "\033[36m" << "|" << "\033[0m"; //For Start of Line Pointer Display
            cursorShown = true;
        }
    
        tempR = tempD;
        while(tempR!=NULL) {
            cout << tempR->character;    
            if(tempR == pointer)
                cout << "\033[36m" << "|" << "\033[0m";
            
            tempR = tempR->next;
        }
        cout << endl;
        tempD = tempD->down;

        if(tempD!=NULL && tempD->up==NULL) {
            cout << "\033[36m" << "|" << "\033[0m"; //For New MiddleLine Pointer Display
            cout << "\n";
            cursorShown = true;
        }
    }
    
    if(!cursorShown && lineStart==NULL && pointer==NULL)
        cout << "\033[36m" << "|" << "\033[0m"; //For Last EmptyLine Pointer Display
    
    cout << endl;
}

void Notepad::InsertChar(char c)
{
    Node *node = new Node;
    node->character = c;
    node->prev = NULL;
    node->next = NULL;
    node->up = NULL;
    node->down = NULL;
    
    //For First Character Insertion in a Line
    if(pointer == NULL)
    {
        if(head==NULL)
        {
            head = node;
            lineStart = node;
            pointer = lineStart;
        }
        else if(lineStart == NULL)
        {
            Node *temp = head;
            while(temp->down!=NULL)
            {
                if(temp->down->up==NULL) //For New Middle Line
                    break;
                temp = temp->down;
            }
            
            node->down = temp->down;
            temp->down = node;
            node->up = temp;
            if(node->down!=NULL)
                node->down->up = node;

            lineStart = node;
            pointer = lineStart;
        }
        else 
        {
            node->next = lineStart;
            lineStart->prev = node;
            
            node->up = node->next->up;
            node->down = node->next->down;
            if(node->up!=NULL)
                node->up->down = node;
            if(node->down!=NULL)
                node->down->up = node;

            if(lineStart->up == NULL)
                head = node;
            lineStart = node;
            pointer = lineStart;
        }
        
        return;
    }
    
    node->next = pointer->next;
    pointer->next = node;
    node->prev = pointer;
    if(node->next!=NULL)
        node->next->prev = node;
    
    if(pointer->up != NULL)
        node->up = pointer->up->next;
    if(pointer->down != NULL)
        node->down = pointer->down->next;

    pointer = node;
}

void Notepad::RemoveChar()
{
    if(head==NULL || lineStart == NULL || pointer==NULL)
        return;
    
    if(pointer == head)
    {
        Node *temp = head;
        if(head->next != NULL) {
            head = head->next;
            head->prev = NULL;
            head->down = temp->down;
        }
        else
        {
            head = temp->down;
            if(head!=NULL)
                head->up = NULL;
        }
        
        lineStart = head;
        pointer = NULL;
        free(temp);
        return;
    }
    else if(pointer == lineStart)
    {
        Node *temp = lineStart;
        if(lineStart->next != NULL)
        {
            lineStart = lineStart->next;
            lineStart->prev = NULL;
            
            lineStart->up = temp->up;
            lineStart->down = temp->down;
            if(temp->up!=NULL)
                temp->up->down = lineStart;
            if(temp->down!=NULL)
                temp->down->up = lineStart;
        }
        else
        {
            if(temp->up!=NULL)
                temp->up->down = temp->down;
            if(temp->down!=NULL)
                temp->down->up = NULL; //For Middle Line

            lineStart = NULL;
        }
        
        pointer = NULL;
        free(temp);
        return;
    }
    
    Node *temp = pointer;
    if(temp->next!=NULL)
        temp->next->prev = temp->prev;
    if(temp->prev!=NULL)
        temp->prev->next = temp->next;
    
    pointer = temp->prev;
    free(temp);
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
            else if(pointer == lineStart)
                pointer = NULL;
            break;
        
        case 'd':
        case 'D':
            if(pointer==NULL)
                pointer = lineStart;
            else if(pointer->next!=NULL)
                pointer = pointer->next;
            break;

        case 'w':
        case 'W':
            if(head==NULL)
                break;
            else if(lineStart==NULL)
            {
                Node *temp = head;
                while(temp->down!=NULL) {
                    if(temp->down->up==NULL) //For New Middle Line
                        break;
                    temp = temp->down;
                }

                if(temp->down!=NULL)
                    temp->down->up = temp; //Remove Empty Middle Line
                lineStart = temp;
                pointer = lineStart;
                movePointer('A');
            }
            else if(pointer==NULL) {
                movePointer('D');
                movePointer('W');
                movePointer('A');
            }
            else if(pointer->up!=NULL) {
                pointer = pointer->up;
                lineStart = lineStart->up;
            }
            break;
        
        case 's':
        case 'S':
            if(lineStart==NULL)
                break;
            else if(pointer==NULL) {
                movePointer('D');
                movePointer('S');
                movePointer('A');
            }
            else if(pointer->down!=NULL) {
                pointer = pointer->down;
                lineStart = lineStart->down;
            }
            break;
        
        default:
            break;
    }
}

void Notepad::NewLine()
{
    if(head==NULL || lineStart==NULL)
        return;
    
    Node *temp = lineStart->down;
    if(temp!=NULL)
        temp->up = NULL; //For New Middle Line, Necessary for Pointer Diplsay

    lineStart = NULL;
    pointer = NULL;
}
