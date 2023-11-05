#include<bits/stdc++.h>
#include<regex>
using namespace std;
class Inode{
public:
    char type;//n:空闲 f:文件 d:目录
    int addr;
    int rNum;
    Inode(){
        type='n';
        addr=0;
        rNum=0;
    }
    Inode(char t,int a,int r){
        type=t;
        addr=a;
        rNum=r;
    }
    Inode(char t,int a){
        type=t;
        addr=a;
        rNum=0;
    }
    Inode(const Inode &i){
        type=i.type;
        addr=i.addr;
        rNum=i.rNum;
    }
    void setInode(char t,int a,int r){
        type=t;
        addr=a;
        rNum=r;
    }
    void setInode(char t,int a){
        type=t;
        addr=a;
        rNum=0;
    }
};
class Data{
public:
    char type;//n:空闲 f:文件 d:目录
    map<string,int> dirData;
    string fileData;
    Data(){
        type='n';
    }
    Data(const Data &d){
        type=d.type;
        dirData=d.dirData;
        fileData=d.fileData;
    }
    void setData(char t,int now,int father){
        if(t=='d'){
            type=t;
            dirData.insert(make_pair(".",now));
            dirData.insert(make_pair("..",father));
        }
    }
    void setData(char t,string s){
        if(t=='f'){
            type=t;
            fileData=s;
        }
    }
    void setData(char t){
        type=t;
    }
};
bitset<100> inodeBitmap,dataBitmap;
void outputInfo(vector<Inode> &inode,vector<Data> &data){
    cout<<endl<<"inode bitmap\t";
    for(int i=0;i<inode.size();i++)
        cout<<inodeBitmap[i];
    cout<<endl<<"inodes\t\t";
    for(int i=0;i<inode.size();i++){
        cout<<"[";
        if(inode[i].type=='d')
            printf("%c a:%d",inode[i].type,inode[i].addr);
        if(inode[i].type=='f')
            printf("%c a:%d r:%d",inode[i].type,inode[i].addr,inode[i].rNum);
        cout<<"] ";
    }
    cout<<endl<<"data bitmap\t";
    for(int i=0;i<data.size();i++)
        cout<<dataBitmap[i];
    cout<<endl<<"data\t\t";
    for(int i=0;i<data.size();i++){
        cout<<"[";
        if(data[i].type=='d')
            for(auto x:data[i].dirData)
                cout<<"("<<x.first<<","<<x.second<<")";
        else if(data[i].type=='f')
            cout<<data[i].fileData;
        cout<<"] ";
    }
    cout<<endl<<endl;
}

bool checkPath(string path,vector<Inode> &inodes,vector<Data> &datas,string &name,int &fatherDataIndex){
    regex r("/([^/\"]*)");
    smatch result;
    fatherDataIndex=0;
    //cout<<"path: "<<path<<endl;
    while(regex_search(path,result,r)){
        name=result[1];
        //cout<<"name: "<<name<<endl;
        path=result.suffix();
        if(path.empty()) break;
        if(datas[fatherDataIndex].dirData.find(name)==datas[fatherDataIndex].dirData.end()){
            cout<<"No such file or directory!"<<endl;
            return false;
        }
        int inodeIndex=datas[fatherDataIndex].dirData[name];
        if(inodes[inodeIndex].type=='f'){
            cout<<"Not a directory!"<<endl;
            return false;
        }
        fatherDataIndex=inodes[inodeIndex].addr;
    }

    return true;
}

bool findFreeInode(int &inodeIndex,int size){
    for(int i=0;i<size;i++){
        if(inodeBitmap[i]==0){
            inodeIndex=i;
            return true;
        }
    }
    cout<<"No inode available!"<<endl;
    return false;
}

bool findFreeData(int &dataIndex,int size){
    for(int i=0;i<size;i++){
        if(dataBitmap[i]==0){
            dataIndex=i;
            return true;
        }
    }
    cout<<"No data available!"<<endl;
    return false;
}

void setDirInfo(int inodeIndex,int dataIndex,int fatherDataIndex,string name,vector<Inode> &inodes,vector<Data> &datas){
    inodeBitmap[inodeIndex]=1;
    inodes[inodeIndex].setInode('d',dataIndex);
    dataBitmap[dataIndex]=1;
    datas[dataIndex].setData('d',dataIndex,fatherDataIndex);
    datas[fatherDataIndex].dirData.insert(make_pair(name,inodeIndex));
}

void setFileInfo(int inodeIndex,int fatherDataIndex,string name,vector<Inode> &inodes,vector<Data> &datas){
    inodeBitmap[inodeIndex]=1;
    inodes[inodeIndex].setInode('f',-1,1);
    datas[fatherDataIndex].dirData.insert(make_pair(name,inodeIndex));
}



int main(int argc, char const *argv[]){
    string name=string(argv[1]);
    int n=atoi(argv[2]);
    int m=atoi(argv[3]);
    vector<Inode> inodes;
    vector<Data> datas;
    for(int i=0;i<n;i++)
        inodes.push_back(Inode());
    for(int i=0;i<m;i++)
        datas.push_back(Data());
    //init
    inodeBitmap[0]=1;
    dataBitmap[0]=1;
    inodes[0].setInode('d',0);
    datas[0].setData('d',0,0);
    outputInfo(inodes,datas);

    ifstream fin("operation.txt");
    string line;

    while(getline(fin,line)){
        stringstream ss(line);
        string op;
        getline(ss,op);
        cout<<op<<endl;
        regex mkdir("mkdir\\(\"(.*)\"\\)");
        regex creat("creat\\(\"(.*)\"\\)");
        regex write("fd=open\\(\"(.*)\"\\);\\s*write\\(fd\\);\\s*close\\(fd\\);");
        regex link("link\\(\"(.*)\",\\s*\"(.*)\"\\)");
        regex unlink("unlink\\(\"(.*)\"\\)");
        smatch result;
        //TODO 封装成函数 去掉set函数
        if(regex_match(op,result,mkdir)){
            regex path("/([^/\"]*)");
            string name;
            int fatherDataIndex=0,inodeIndex=-1,dataIndex=-1;
            if(!checkPath(result[1],inodes,datas,name,fatherDataIndex))
                continue;
            if(!findFreeInode(inodeIndex,n)||!findFreeData(dataIndex,m))
                continue;
            if(datas[fatherDataIndex].dirData.find(name)!=datas[fatherDataIndex].dirData.end()){
                cout<<"Directory already exists!"<<endl;
                continue;
            }
            setDirInfo(inodeIndex,dataIndex,fatherDataIndex,name,inodes,datas);
            
        }
        else if(regex_match(op,result,creat)){
            regex path("/([^/\"]*)");
            string name;
            int fatherDataIndex=0,inodeIndex=-1;
            if(!checkPath(result[1],inodes,datas,name,fatherDataIndex))
                continue;
            if(!findFreeInode(inodeIndex,n))
                continue;
            if(datas[fatherDataIndex].dirData.find(name)!=datas[fatherDataIndex].dirData.end()){
                cout<<"File already exists!"<<endl;
                continue;
            }
            setFileInfo(inodeIndex,fatherDataIndex,name,inodes,datas);
        }
        else if(regex_match(op,result,write)){
            regex path("/([^/\"]*)");
            string name;
            int fatherDataIndex=0,dataIndex=-1;
            if(!checkPath(result[1],inodes,datas,name,fatherDataIndex))
                continue;
                //TODO findFreeData
            if(!findFreeData(dataIndex,m))
                continue;
            if(datas[fatherDataIndex].dirData.find(name)==datas[fatherDataIndex].dirData.end()){
                cout<<"No such file!"<<endl;
                continue;
            }
            int inodeIndex=datas[fatherDataIndex].dirData[name];
            if(inodes[inodeIndex].type=='d'){
                cout<<"Not a file!"<<endl;
                continue;
            }
            inodes[inodeIndex].addr=dataIndex;
            dataBitmap[dataIndex]=1;
            datas[dataIndex].setData('f',"u");
        }
        else if(regex_match(op,result,link)){
            regex path("/([^/\"]*)");
            string name;
            int fatherDataIndex=0,dataIndex=-1;
            if(!checkPath(result[1],inodes,datas,name,fatherDataIndex))
                continue;
            if(datas[fatherDataIndex].dirData.find(name)==datas[fatherDataIndex].dirData.end()){
                cout<<"No such file!"<<endl;
                continue;
            }
            int inodeIndex=datas[fatherDataIndex].dirData[name];
            if(!checkPath(result[2],inodes,datas,name,fatherDataIndex))
                continue;

            datas[fatherDataIndex].dirData.insert(make_pair(name,inodeIndex));
            inodes[inodeIndex].rNum++;
        }
        else if(regex_match(op,result,unlink)){
            regex path("/([^/\"]*)");
            string name;
            int fatherDataIndex=0,dataIndex=-1;
            if(!checkPath(result[1],inodes,datas,name,fatherDataIndex))
                continue;
            
            if(datas[fatherDataIndex].dirData.find(name)==datas[fatherDataIndex].dirData.end()){
                cout<<"No such file!"<<endl;
                continue;
            }
            int inodeIndex=datas[fatherDataIndex].dirData[name];
            datas[fatherDataIndex].dirData.erase(name);
            inodes[inodeIndex].rNum--;
            if(inodes[inodeIndex].rNum==0){
                if(inodes[inodeIndex].addr!=-1){
                    datas[inodes[inodeIndex].addr].setData('n');
                    dataBitmap[inodes[inodeIndex].addr]=0;
                }
                inodeBitmap[inodeIndex]=0;
                inodes[inodeIndex].setInode('n',0);
            }
        }
        else{
            cout<<"Invalid operation!"<<endl;
            continue;
        }
        outputInfo(inodes,datas);
    }
    return 0;
}