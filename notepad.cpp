#include<iostream>
#include<fstream>
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

        void correctLinksAfterInsertion(Node*);
        void correctLinksAfterLineInsertion(Node*);
        void correctLinksAfterRemoval(Node*);
        void correctLinksAfterLineRemoval(Node*);
    
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
        void SaveFile();
        void ReadFile();
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
             << "\t\t   S- Save Notepad       I- Import Notepad \n"
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
                getline(cin, data);
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
            
            case 's':
            case 'S':
                notepad.SaveFile();
                break;
            case 'i':
            case 'I':
                notepad.ReadFile();
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
            correctLinksAfterLineInsertion(node);
        }
        else
        {
            node->next = lineStart;
            lineStart->prev = node;
            
            if(lineStart->up == NULL)
                head = node;
            lineStart = node;
            pointer = lineStart;
            correctLinksAfterInsertion(node);
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
    correctLinksAfterInsertion(node);
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
            correctLinksAfterRemoval(temp);
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
            correctLinksAfterRemoval(temp);
        }
        else
        {
            correctLinksAfterLineRemoval(temp);
            lineStart = NULL;

            if(temp->down!=NULL)
                temp->down->up = NULL; //For Middle Line
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
    correctLinksAfterRemoval(temp);
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


void Notepad::SaveFile()
{
    ofstream file("Notepad.txt");
    Node *tempR;
    Node *tempD = head;
    
    while(tempD!=NULL)
    {
        tempR = tempD;
        while(tempR!=NULL) {
            file << tempR->character;    
            tempR = tempR->next;
        }
        file << endl;
        tempD = tempD->down;
    }
    file.close();
    file << endl;

    cout << "Successfully Saved to File. " << endl;
}

void Notepad::ReadFile()
{
    ifstream file("Notepad.txt");
    if(!file.is_open()) {
        cout << "No File Exist! " << endl;
        return;
    }
    
    string line;
    while(!file.eof())
    {
        getline(file, line);
        for(int i=0; i<line.length(); i++)
            InsertChar(line[i]);
        NewLine();
    }
    file.close();
    cout << "Successfully Imported from File. " << endl;
}


void Notepad::correctLinksAfterInsertion(Node *node)
{
    Node *link = node;
    while(link!=NULL) //Link Node's Up & Down
    {
        if(link->next!=NULL)
        {
            link->up = link->next->up;
            link->down = link->next->down;

            if(link->up==NULL && link->down==NULL)
                break;
        }
        else if(link->prev!=NULL) //For Last Node
        {
            if(link->up!=NULL)
                link->up = link->prev->up->next;
            if(link->down!=NULL)
                link->down = link->prev->down->next;
        }
        link = link->next;
    }

    Node *temp = node;
    link = node->up;
    while(link!=NULL) //Link UpperLine-Node's Down
    {
        link->down = temp;
        link = link->next;
        temp = temp->next;
        
        if(temp==NULL)
            break;
    }
    
    temp = node;
    link = node->down;
    while(link!=NULL) //Link LowerLine-Node's Up
    {
        link->up = temp;
        link = link->next;
        temp = temp->next;
        
        if(temp==NULL)
            break;
    }
}

void Notepad::correctLinksAfterLineInsertion(Node *node)
{
    Node *temp = node->up;
    if(temp!=NULL)
        temp = temp->next;
    while(temp!=NULL) //DeLink UpperLine-Node's Down
    {
        temp->down = NULL;
        temp = temp->next;
    }
    
    temp = node->down;
    if(temp!=NULL)
        temp = temp->next;
    while(temp!=NULL) //DeLink LowerLine-Node's Up
    {
        temp->up = NULL;
        temp = temp->next;
    }
}

void Notepad::correctLinksAfterRemoval(Node *temp)
{
    Node *link = temp;
    while(link!=NULL)
    {
        if(link->up==NULL && link->down==NULL)
            break;

        if(link->up!=NULL)
            link->up->down = link->next; //Link UpperLine-Node's Down
        if(link->down!=NULL)
            link->down->up = link->next; //Link LowerLine-Node's Up
        link = link->next;
    }
    
    link = temp->up;
    while(link!=NULL) //Link Node's Up
    {
        if(link->down==NULL)
            break;
        else
            link->down->up = link;
        link = link->next;
    }
    
    link = temp->down;
    while(link!=NULL) //Link Node's Down
    {
        if(link->up==NULL)
            break;
        else
            link->up->down = link;
        link = link->next;
    }
}

void Notepad::correctLinksAfterLineRemoval(Node* temp)
{
    Node *linkUp = temp->up;
    Node *linkDown = temp->down;
    while(linkUp!=NULL || linkDown!=NULL)
    {
        if(linkUp!=NULL) {
            linkUp->down = linkDown; //Link UpperLine-Node's Down
            linkUp = linkUp->next;
        }
            
        if(linkDown!=NULL) {
            linkDown->up = linkUp; //Link LowerLine-Node's Up
            linkDown = linkDown->next;
        }
    }
}
