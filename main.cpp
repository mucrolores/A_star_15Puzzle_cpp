#include <iostream>
#include <string>
#include <ctime>
using namespace std;
/*
    The puzzle is set to be number from 0 to 14
    Free space will be set as 15
*/
const int N = 4;
const int length = N*N;
const int Ratio = 2;

class Node{
public:
    Node();
    void SetData(int *input);
    void SetSpaceIndex(int inputIndex);
    void SetGValue(int inputG);
    void SetHValue(int inputH);
    void GenerateFValue();
    void GenerateHValue();
    void PrintState();
    void SwapData(int index1, int index2);

    int* GetData();
    int GetSpaceIndex();
    string GetDataInString();
    int GetHValue();
    int GetGValue();
    int GetFValue();

    void expand();

    Node *Child; // 0~3 goes for Up Down Left Right
    Node *Parent;
    Node *NextPtr;

private:
    int *data;
    string dataInString;
    int GValue;
    int HValue;
    int FValue;
    int SpaceIndex;
};

Node::Node(){
    GValue = INT_MAX;
    HValue = INT_MAX;
    FValue = INT_MAX;
    SpaceIndex = length-1;
    Parent = nullptr;
    NextPtr = nullptr;
}
void Node::SetData(int *input)
{
    data = new int[length];
    for(int i=0;i<length;i++)
    {
        data[i] = input[i];
    }

    dataInString = "";
    for(int i=0;i<length;i++)
    {
        dataInString.append(to_string(data[i]));
        dataInString.append(" ");
    }
}
void Node::SetSpaceIndex(int inputIndex)
{
    SpaceIndex = inputIndex;
}
void Node::SetGValue(int inputG)
{
    GValue = inputG;
}
void Node::SetHValue(int inputH)
{
    HValue = inputH;
}
void Node::GenerateFValue()
{
    FValue = GValue+HValue;
}
void Node::GenerateHValue()
{
    int HCost = 0;

    for(int i=0;i<length;i++)
    {
        if(data[i]!=length-1)
        {
            HCost += Ratio*( abs(data[i]/4 - i/4 ) + abs(data[i]%4 - i%4));
        }
    }
    HValue = HCost;
}
void Node::PrintState()
{
    cout << "Current State : " << endl;
    for(int i=0;i<length;i++)
    {
        if(data[i] < 10 )
        {
            cout << " ";
        }
        if(data[i]==length-1)
        {
            cout << "   ";
        }
        else
        {
            cout << data[i] << " ";
        }
        if((i+1)%N==0)
        {
            cout << endl;
        }
    }
    cout << "Raw space index is " << SpaceIndex << endl;
    cout << "Space index is at : " << SpaceIndex/4 << "," << SpaceIndex%4 << endl;
    cout << "Current HValue is " << HValue << endl;
    cout << "Current FValue is " << FValue << endl;
    cout << endl;
}
void Node::SwapData(int index1, int index2)
{
    int tmp = data[index1];
    data[index1] = data[index2];
    data[index2] = tmp;
}

int* Node::GetData()
{
    int *ToReturn = new int[length];
    for(int i=0;i<length;i++)
    {
        ToReturn[i] = data[i];
    }
    return ToReturn;
}
int Node::GetSpaceIndex()
{
    return SpaceIndex;
}
string Node::GetDataInString()
{
    return dataInString;
}
int Node::GetGValue()
{
    return GValue;
}
int Node::GetHValue()
{
    return HValue;
}
int Node::GetFValue()
{
    return FValue;
}

void Node::expand()
{
    Child = new Node[4];
    int tmpHValue = 0;
    if(SpaceIndex/N < N-1 ) // swipe up state, dir = 0
    {
        SwapData(SpaceIndex,SpaceIndex+N);
        Child[0].SetData(data);
        Child[0].SetSpaceIndex(SpaceIndex+N);
        Child[0].SetGValue(GValue+1);
        SwapData(SpaceIndex,SpaceIndex+N);
        //calculate the next step's HValue
        tmpHValue = HValue;
        tmpHValue -= Ratio*( abs(data[SpaceIndex+N]/N - (SpaceIndex+N)/N) );
        tmpHValue += Ratio*( abs(data[SpaceIndex+N]/N - (SpaceIndex)/N) );
        Child[0].SetHValue(tmpHValue);
        Child[0].GenerateFValue();
    }
    if(SpaceIndex/N > 0) // swipe down state, dir = 1
    {
        SwapData(SpaceIndex,SpaceIndex-N);
        Child[1].SetData(data);
        Child[1].SetSpaceIndex(SpaceIndex-N);
        Child[1].SetGValue(GValue+1);
        SwapData(SpaceIndex,SpaceIndex-N);
        //calculate the next step's HValue
        tmpHValue = HValue;
        tmpHValue -= Ratio*( abs(data[SpaceIndex-N]/N - (SpaceIndex-N)/N) );
        tmpHValue += Ratio*( abs(data[SpaceIndex-N]/N - (SpaceIndex)/N) );
        Child[1].SetHValue(tmpHValue);
        Child[1].GenerateFValue();
    }
    if(SpaceIndex%N < N-1) // swipe left state, dir = 2
    {
        SwapData(SpaceIndex,SpaceIndex+1);
        Child[2].SetData(data);
        Child[2].SetSpaceIndex(SpaceIndex+1);
        Child[2].SetGValue(GValue+1);
        SwapData(SpaceIndex,SpaceIndex+1);\
        //calculate the next step's HValue
        tmpHValue = HValue;
        tmpHValue -= Ratio*( abs(data[SpaceIndex+1]%N - (SpaceIndex+1)%N) );
        tmpHValue += Ratio*( abs(data[SpaceIndex+1]%N - (SpaceIndex)%N) );
        Child[2].SetHValue(tmpHValue);
        Child[2].GenerateFValue();
    }
    if(SpaceIndex%N > 0) // swipe right state, dir = 3
    {
        SwapData(SpaceIndex,SpaceIndex-1);
        Child[3].SetData(data);
        Child[3].SetSpaceIndex(SpaceIndex-1);
        Child[3].SetGValue(GValue+1);
        SwapData(SpaceIndex,SpaceIndex-1);
        //calculate the next step's HValue
        tmpHValue = HValue;
        tmpHValue -= Ratio*( abs(data[SpaceIndex-1]%N - (SpaceIndex-1)%N) );
        tmpHValue += Ratio*( abs(data[SpaceIndex-1]%N - (SpaceIndex)%N) );
        Child[3].SetHValue(tmpHValue);
        Child[3].GenerateFValue();
    }
}

class Ordered_NodeList
{
public:
    Ordered_NodeList();
    Node* First();
    void Pop();
    bool Replace(Node *node);
    void Insert(Node *node);
    bool Contain(Node *node);
    bool IsEmpty();
private:
    Node *Front;
    Node *CurrentNode;
    int size;
};
Ordered_NodeList::Ordered_NodeList()
{
    Front = nullptr;
    CurrentNode = nullptr;
    size = 0;
}
Node* Ordered_NodeList::First()
{
    return Front;
}
void Ordered_NodeList::Pop()
{
    if(!IsEmpty())
    {
        Front = Front->NextPtr;
        size--;
    }
}
bool Ordered_NodeList::Replace(Node *node)//Modify
{
    if(size == 0)
    {
        return false;
    }
    else
    {
        if((Front->GetDataInString() == node->GetDataInString()) && Front->GetFValue() > node->GetFValue())
        {
            node->NextPtr = Front->NextPtr;
            Front = node;
            return true;
        }
        else
        {
            CurrentNode = Front;
            while(CurrentNode->NextPtr!= nullptr)
            {
                if(CurrentNode->NextPtr->GetDataInString() == node->GetDataInString())
                {
                    if(CurrentNode->NextPtr->GetFValue() > node->GetFValue())
                    {
                        if(CurrentNode->NextPtr->NextPtr)
                        {
                            CurrentNode->NextPtr = CurrentNode->NextPtr->NextPtr;
                        }
                        else{
                            CurrentNode->NextPtr = NULL;
                        }
                        Insert(node);
                        cout << size << endl;
                        return true;
                    }
                    return false;
                }
                CurrentNode = CurrentNode->NextPtr;
            }
            return false;
        }
    }

}
void Ordered_NodeList::Insert(Node *node)
{
    Node *Tmp = new Node;
    Tmp->SetData(node->GetData());
    Tmp->SetSpaceIndex(node->GetSpaceIndex());
    Tmp->SetGValue(node->GetGValue());
    Tmp->SetHValue(node->GetHValue());
    Tmp->GenerateFValue();
    if(size == 0)
    {
        Front = Tmp;
    }
    else
    {
        if(Front->GetFValue() > Tmp->GetFValue())
        {
            Tmp->NextPtr = Front;
            Front = Tmp;
        }
        else
        {
            CurrentNode = Front;
            while(CurrentNode->NextPtr!=nullptr)
            {

                if(CurrentNode->NextPtr->GetFValue() > Tmp->GetFValue())
                {
                    Tmp->NextPtr = CurrentNode->NextPtr;
                    CurrentNode->NextPtr = Tmp;
                    break;
                }
                CurrentNode = CurrentNode->NextPtr;
            }
            CurrentNode->NextPtr = Tmp;
        }
    }
    size++;
}
bool Ordered_NodeList::Contain(Node *node)
{
    if(size > 0)
    {
        CurrentNode = Front;
        while(CurrentNode!= nullptr)
        {
            if(CurrentNode->GetDataInString() == node->GetDataInString())
            {
                return true;
            }
            CurrentNode = CurrentNode->NextPtr;
        }
        return false;
    }
    return false;
}
bool Ordered_NodeList::IsEmpty()
{
    return size == 0;
}
bool CheckAvailable(int *data)
{
    int inversion = 0;
    int TotalLength = N*N;
    int FreeSpaceRow = 0;

    for(int i=0;i<TotalLength;i++)
    {
        if(data[i] == TotalLength-1)
        {
            FreeSpaceRow = (N-1) - (i/N);
        }
    }

    for(int i=0;i<TotalLength-1;i++)
    {
        for(int j=i+1;j<TotalLength;j++)
        {
            //check the target place i and j is not at number 15
            //and data in i is greater than j
            //count as an inversion
            if(data[i] != 15 && data[j] != 15 && (data[i] > data[j]))
            {
                inversion++;
            }
        }
    }
    if( N%2!=0 )
    {
        return !(inversion & 2);
    }
    else
    {
        if(FreeSpaceRow % 2 == 0)
        {
            cout << "inversion is " << inversion << endl;
            return !(inversion%2);
        }
        else
        {
            return inversion%2;
        }
    }
}

//Note that the input start should be check available
void AStar(int *startData)
{
    double TimeTag;
    Ordered_NodeList OPENList;
    Ordered_NodeList CLOSEList;
    Node Start;
    Start.SetData(startData);
    Start.SetGValue(0);
    Start.SetSpaceIndex(15);
    Start.GenerateHValue();
    Start.GenerateFValue();
    Start.PrintState();
    OPENList.Insert(&Start);
    Node *Current;
    int test=0;
    TimeTag = clock();
    while(!OPENList.IsEmpty())
    {
        test++;
        Current = OPENList.First();
        OPENList.Pop();
        CLOSEList.Insert(Current);
        if(Current->GetHValue()==0){ break; }
        Current->expand();
        for(int i=0;i<4;i++)
        {
            if(Current->Child[i].GetDataInString() != "")
            {
                if(CLOSEList.Contain(&Current->Child[i]))
                {
                    continue;
                }
                if(OPENList.Contain(&Current->Child[i]))
                {
                    Current->Child[i].Parent = Current;
                    OPENList.Replace(&Current->Child[i]);
                }
                else
                {
                    Current->Child[i].Parent = Current;
                    OPENList.Insert(&Current->Child[i]);
                }

            }
        }
        if(test % 100 == 0)
        {
            cout << Current->GetGValue() << endl;
        }
    }
    TimeTag = clock()-TimeTag;
    cout << "Find Path in " << (double)TimeTag/CLOCKS_PER_SEC << endl;
    Current->PrintState();

}

int main()
{
    int data[length];
    for(int i=0;i<length;i++)
    {
        cin >> data[i];
    }
    if(CheckAvailable(data))
    {
        AStar(data);
    }
    else{
        cout << "Error Map\n";
    }


    return 0;
}