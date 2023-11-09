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
            dirData["."]=now;
            dirData[".."]=father;
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
    cout<<"inode bitmap\t";
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
    cout<<endl;
}

bool checkPath(string path,vector<Inode> &inodes,vector<Data> &datas,string &name,int &fatherInodeIndex,int &fatherDataIndex){
    smatch result;
    int dataIndex=0,inodeIndex=0;
    while(regex_search(path,result,regex("/([^/\"]*)"))){
        name=result[1];
        path=result.suffix();
        if(path.empty()) break;
        if(datas[dataIndex].dirData.find(name)==datas[dataIndex].dirData.end()){
            cout<<"No such file or directory!"<<endl;
            return false;
        }
        inodeIndex=datas[dataIndex].dirData[name];
        if(inodes[inodeIndex].type=='f'){
            cout<<"Not a directory!"<<endl;
            return false;
        }
        dataIndex=inodes[inodeIndex].addr;
    }
    fatherInodeIndex=inodeIndex;
    fatherDataIndex=dataIndex;
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

void mkdir(string path,vector<Inode> &inodes,vector<Data> &datas,int n,int m){
    string name;
    int fatherInodeIndex=0,fatherDataIndex=0,inodeIndex=-1,dataIndex=-1;
    if(!checkPath(path,inodes,datas,name,fatherInodeIndex,fatherDataIndex)) return;
    if(!findFreeInode(inodeIndex,n)||!findFreeData(dataIndex,m)) return;
    if(datas[fatherDataIndex].dirData.find(name)!=datas[fatherDataIndex].dirData.end()){
        cout<<"Directory already exists!"<<endl;
        return;
    }
    inodeBitmap[inodeIndex]=1;
    inodes[inodeIndex].setInode('d',dataIndex);
    dataBitmap[dataIndex]=1;
    datas[dataIndex].setData('d',inodeIndex,fatherInodeIndex);
    datas[fatherDataIndex].dirData[name]=inodeIndex;
    outputInfo(inodes,datas);
}

void creat(string path,vector<Inode> &inodes,vector<Data> &datas,int n,int m){
    string name;
    int fatherInodeIndex=0,fatherDataIndex=0,inodeIndex=-1;
    if(!checkPath(path,inodes,datas,name,fatherInodeIndex,fatherDataIndex)) return;
    if(!findFreeInode(inodeIndex,n)) return;
    if(datas[fatherDataIndex].dirData.find(name)!=datas[fatherDataIndex].dirData.end()){
        cout<<"File already exists!"<<endl;
        return;
    }
    inodeBitmap[inodeIndex]=1;
    inodes[inodeIndex].setInode('f',-1,1);
    datas[fatherDataIndex].dirData[name]=inodeIndex;
    outputInfo(inodes,datas);
}

void write(string path,vector<Inode> &inodes,vector<Data> &datas,int n,int m){
    string name;
    int fatherInodeIndex=0,fatherDataIndex=0,dataIndex=-1;
    if(!checkPath(path,inodes,datas,name,fatherInodeIndex,fatherDataIndex)) return;
    if(!findFreeData(dataIndex,m)) return;
    if(datas[fatherDataIndex].dirData.find(name)==datas[fatherDataIndex].dirData.end()){
        cout<<"No such file!"<<endl;
        return;
    }
    int inodeIndex=datas[fatherDataIndex].dirData[name];
    if(inodes[inodeIndex].type=='d'){
        cout<<"Not a file!"<<endl;
        return;
    }
    inodes[inodeIndex].addr=dataIndex;
    dataBitmap[dataIndex]=1;
    datas[dataIndex].setData('f',"u");
    outputInfo(inodes,datas);
}

void link(string path1,string path2,vector<Inode> &inodes,vector<Data> &datas,int n,int m){
    string name;
    int fatherInodeIndex=0,fatherDataIndex=0;
    if(!checkPath(path1,inodes,datas,name,fatherInodeIndex,fatherDataIndex)) return;
    if(datas[fatherDataIndex].dirData.find(name)==datas[fatherDataIndex].dirData.end()){
        cout<<"No such file!"<<endl;
        return;
    }
    int inodeIndex=datas[fatherDataIndex].dirData[name];
    if(!checkPath(path2,inodes,datas,name,fatherInodeIndex,fatherDataIndex)) return;
    datas[fatherDataIndex].dirData[name]=inodeIndex;
    inodes[inodeIndex].rNum++;
    outputInfo(inodes,datas);
}

void unlink(string path,vector<Inode> &inodes,vector<Data> &datas,int n,int m){
    string name;
    int fatherInodeIndex=0,fatherDataIndex=0;
    if(!checkPath(path,inodes,datas,name,fatherInodeIndex,fatherDataIndex)) return;
    if(datas[fatherDataIndex].dirData.find(name)==datas[fatherDataIndex].dirData.end()){
        cout<<"No such file!"<<endl;
        return;
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
    outputInfo(inodes,datas);
}

int main(int argc, char const *argv[]){
    string name=string(argv[1]);
    int n=atoi(argv[2]);
    int m=atoi(argv[3]);
    vector<Inode> inodes(n);
    vector<Data> datas(m);

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
        cout<<endl<<op<<endl;
        smatch result;
        if(regex_match(op,result,regex("mkdir\\(\"(.*)\"\\)")))
            mkdir(result[1],inodes,datas,n,m);
        else if(regex_match(op,result,regex("creat\\(\"(.*)\"\\)")))
            creat(result[1],inodes,datas,n,m);
        else if(regex_match(op,result,regex("fd=open\\(\"(.*)\"\\);\\s*write\\(fd\\);\\s*close\\(fd\\);")))
            write(result[1],inodes,datas,n,m);
        else if(regex_match(op,result,regex("link\\(\"(.*)\",\\s*\"(.*)\"\\)")))
            link(result[1],result[2],inodes,datas,n,m);
        else if(regex_match(op,result,regex("unlink\\(\"(.*)\"\\)")))
            unlink(result[1],inodes,datas,n,m);
        else{
            cout<<"Invalid operation!"<<endl;
            continue;
        }
        //outputInfo(inodes,datas);
    }
    return 0;
}