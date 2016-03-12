#include "route.h"
#include "lib/lib_record.h"
#include <stdio.h>
#include <algorithm>

PATH path,pathTemp;
vector<LINK> links;
LINK *link;
vector<NODE> nodes(700);
unsigned short SourceID,DestinationID;
vector<unsigned short> V;//is V' . And we can sort it,so when we use ,we can save time.

//你要完成的功能总入口
void search_route(char *topo[5000], int edge_num, char *demand)
{
    for(int i=0;; i++)//input topo[
    {
        int num,countNum;
        num=0;
        countNum=0;
        for(int j=0;; j++)
        {
            if(topo[i][j]>='0' && topo[i][j]<='9')
            {
                num=num*10+topo[i][j]-'0';
            }
            else
            {
                switch(countNum)
                {
                case 0:
                    link=new LINK;
                    link->LinkID=num;
                    num=0;
                    countNum++;
                    break;
                case 1:
                    link->SourceID=num;
                    num=0;
                    countNum++;
                    break;
                case 2:
                    link->DestinationID=num;
                    num=0;
                    countNum++;
                    break;
                case 3:
                    link->Cost=num;
                    num=0;
                    countNum=0;
                    links.push_back(*link);
                    nodes[link->SourceID].nodeLinks.push_back(link);
                    nodes[link->SourceID].ID=link->SourceID;
                    nodes[link->DestinationID].ID=link->DestinationID;
                    break;
                }
                if(countNum==0)
                {
                    break;
                }
            }
        }
        if((int)links.size()==edge_num) break;
    }

    for(int i=0,num=0,countNum=0; ((demand[i]>='0'&&demand[i]<='9')||demand[i]==','||demand[i]=='|'); i++) //input demand
    {
        if(demand[i]>='0' && demand[i]<='9')
        {
            num=num*10+demand[i]-'0';
        }
        else
        {
            switch(countNum)
            {
            case 0:
                SourceID=num;
                num=0;
                countNum++;
                break;
            case 1:
                DestinationID=num;
                num=0;
                countNum++;
                break;
            case 2:
                V.push_back(num);
                num=0;
                break;
            }
        }
        if(demand[i]=='\n') break;
    }
    sort(V.begin(),V.end());
    //IsInV(543);

    if(StartDeepSearch()==false)
    {
        cout<<"NA"<<endl;
    }
    else
    {
        int linkSize;
        linkSize=path.linkIDs.size();
        for(int i=0; i<linkSize; i++)
        {
            record_result((unsigned short)path.linkIDs[i]);
        }
    }

    /*unsigned short result[] = {2, 6, 3};//示例中的一个解

    for (int i = 0; i < 3; i++)
        record_result(result[i]);*/
}

NODE::NODE()
{
    isVisted=false;
    isSorted=false;
}

PATH::PATH()
{
    cost=9999999;
}

bool DeepSearch(NODE &node,LINK *link)
{
    if(node.isVisted==false)
    {
        if((pathTemp.cost+link->Cost)<path.cost)
        {
            pathTemp.linkIDs.push_back(link->LinkID);
            pathTemp.nodeIDs.push_back(node.ID);
            pathTemp.cost+=link->Cost;
            if(node.ID!=DestinationID)
            {
                node.isVisted=true;
                bool isFind;
                isFind=false;
                if(node.isSorted==false)
                {
                    sort(node.nodeLinks.begin(),node.nodeLinks.end(),CompareLink);
                    node.isSorted=true;
                }
                int linkSize;
                linkSize=node.nodeLinks.size();
                for(int i=0; i<linkSize; i++)
                {
                    if(DeepSearch(nodes[node.nodeLinks[i]->DestinationID],node.nodeLinks[i])==true) isFind=true;
                }
                pathTemp.linkIDs.erase(--pathTemp.linkIDs.end());
                pathTemp.nodeIDs.erase(--pathTemp.nodeIDs.end());
                pathTemp.cost-=link->Cost;
                node.isVisted=false;
                return isFind;
            }
            else
            {
                bool isVVisted;
                //isVVisted=false;
                vector<unsigned short>  pathNodeIDs(pathTemp.nodeIDs.size()-2);
                copy(pathTemp.nodeIDs.begin()+1,pathTemp.nodeIDs.end()-1,pathNodeIDs.begin());
                sort(pathNodeIDs.begin(),pathNodeIDs.end());
                isVVisted=true;
                for(int i=0;i<(int)V.size();i++){
                    if(binary_search(pathNodeIDs.begin(),pathNodeIDs.end(),V[i])==false){
                        isVVisted=false;
                        break;
                    }
                }
                /*for(int i=0; i<(int)V.size(); i++)
                {
                    bool isJVisted;
                    isJVisted=false;
                    int nodeSize;
                    nodeSize=pathTemp.nodeIDs.size();
                    for(int j=1; j<nodeSize-1; j++)
                    {
                        if(V[i]==pathTemp.nodeIDs[j])
                        {
                            isJVisted=true;
                            break;
                        }
                    }
                    if(isJVisted==false) break;
                    else if(i==(int)(V.size()-1))
                    {
                        isVVisted=true;
                        break;
                    }
                }*/

                if(isVVisted==true)
                {
                    path=pathTemp;
                }
                pathTemp.linkIDs.erase(--pathTemp.linkIDs.end());
                pathTemp.nodeIDs.erase(--pathTemp.nodeIDs.end());
                pathTemp.cost-=link->Cost;
                node.isVisted=false;
                return isVVisted;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool StartDeepSearch()
{
    pathTemp.nodeIDs.push_back(SourceID);
    pathTemp.cost=0;
    nodes[SourceID].isVisted=true;
    bool isFind;
    isFind=false;
    if(nodes[SourceID].isSorted==false)
    {
        sort(nodes[SourceID].nodeLinks.begin(),nodes[SourceID].nodeLinks.end(),CompareLink);
        nodes[SourceID].isSorted=true;
    }
    int linkSize;
    linkSize=nodes[SourceID].nodeLinks.size();
    for(int i=0; i<linkSize; i++)
    {
        if(DeepSearch(nodes[nodes[SourceID].nodeLinks[i]->DestinationID],nodes[SourceID].nodeLinks[i])==true) isFind=true;
    }
    return isFind;
}

bool IsInV(int ID)
{
    return binary_search(V.begin(),V.end(),ID);
}

bool CompareLink(const LINK *a,const LINK *b)
{
    bool isInVa,isInVb;
    isInVa=IsInV(a->DestinationID);
    isInVb=IsInV(b->DestinationID);
    if(isInVa==true && isInVb==false)  return true;
    if(isInVa==false && isInVb==true)  return false;
    //若都在，或都不在，则比较Cost
    if(a->DestinationID==SourceID) return true;
    return a->Cost<b->Cost;
}
