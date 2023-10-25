#include<bits/stdc++.h>
#include<windows.h>
using namespace std;
class PCB{
public:
    int PID;
    string name;
    string state;
    int PC;
    int ptbr;
    PCB(){
        PID=0,name="default",state="READY",PC=0,ptbr=0;
    }
    PCB(int pid,string n,string s,int pc,int p){
        PID=pid,name=n,state=s,PC=pc,ptbr=p;
    }
    PCB(const PCB& p):PID(p.PID),name(p.name),state(p.state),PC(p.PC),ptbr(p.ptbr){}
};
class pageTableItem{
public:
    int pNum;//physical page frame number
    bool isValid;
    bool isExist;
    pageTableItem(){
        pNum=-1,isValid=false,isExist=false;
    }
    pageTableItem(int p,bool v,bool e){
        pNum=p,isValid=v,isExist=e;
    }
    pageTableItem(const pageTableItem& p):pNum(p.pNum),isValid(p.isValid),isExist(p.isExist){}
};

bitset<100>bitmap;
int PTBR=-1;

void generateProgress(int k,int p,int mi,int mx,int x,int a,int b,int min_size,int max_size,double p_valid,double p_available){
    for(int i=0;i<k;i++){
        string name="P"+to_string(i)+".txt";
        string path="./data/"+name;
        freopen(path.c_str(), "w", stdout);

        int y=rand()%(max_size-min_size+1)+min_size;
        int num_v=pow(2,a-x);
        vector<int>valid,invalid;
        set<int>valid_set;
        for(int j=1;j<=y;j++){
            int page=rand()%num_v;
            if(valid_set.find(page)==valid_set.end())
                valid_set.insert(page);
            else j--;
        }
        for(int j=0;j<num_v;j++){
            if(valid_set.find(j)==valid_set.end())
                invalid.push_back(j);
            else valid.push_back(j);
        }

        cout<<y;
        for(auto it:valid) cout<<" "<<it;
        cout<<endl;
        int num=mi+rand()%(mx-mi+1);
        int max_offset=pow(2,x);
        for(int j=1;j<=num;j++){
            if(rand()%101<=p) cout<<"cpu\t";
            else cout<<"io\t";

            int num_a=a/4+(a%4!=0);
            int addr=0;
            if((double)rand()/RAND_MAX<=p_valid)
                addr+=valid[rand()%valid.size()]*pow(2,x);
            else addr+=invalid[rand()%invalid.size()]*pow(2,x);
            addr+=rand()%max_offset;
            cout<<hex<<uppercase<<setw(num_a)<<setfill('0')<<addr<<dec<<endl;
        }
        freopen("CONOUT$", "w", stdout);
    }
}

//bitmap释放
void freeBitmap(int ptbr,vector<vector<pageTableItem>>&pageTables){
    int num_v=pageTables[ptbr].size();
    for(int i=0;i<num_v;i++){
        if(pageTables[ptbr][i].isValid==true){
            bitmap[pageTables[ptbr][i].pNum]=0;
            pageTables[ptbr][i].isValid=false;
        }
    }
}
// //function of virtual address to physical address
// void vAddr2pAddr(int vAddr,int& pAddr,int& offset,int num_o,vector<vector<pageTableItem>>&pageTables,bool &error){
//     offset=vAddr%num_o;
//     if(pageTables[PTBR][vAddr/num_o].isValid==true)
//         pAddr=pageTables[PTBR][vAddr/num_o].pNum*num_o+offset;
//     else error=true;
// }

//function of virtual address to physical address use bit opeartion
void vAddr2pAddr(int vAddr,int& pAddr,int& offset,int x,vector<vector<pageTableItem>>&pageTables,bool &error){
    offset=vAddr&((1<<x)-1);
    if(pageTables[PTBR][vAddr>>x].isValid==true)
        pAddr=(pageTables[PTBR][vAddr>>x].pNum<<x)|offset;
    else error=true;
}

void scheduleProgress(int k,int x,int a,int b,int max_alloc,string mode){
    vector<vector<string>>cmds;//commands
    vector<vector<int>>vAddrs;//virtual addresses
    vector<PCB>PCBs;
    vector<vector<pageTableItem>>pageTables;
    queue<PCB>readyList,runList,waitingList,doneList;
    queue<int>timeList;
    int vAddr,pAddr,offset;
    const string TAB="\t";
    int num_v=pow(2,a-x);//number of virtual pages
    int num_p=pow(2,b-x);//number of physical pages
    int num_o=pow(2,x);//number of offsets
    int num_a=a/4+(a%4!=0);
    int num_b=b/4+(b%4!=0);
    for(int i=0;i<k;i++){
        string name="P"+to_string(i)+".txt";
        string path="./data/"+name;
        ifstream input(path.c_str(),ios::in);
        if(!input){
            cout<<"open file error"<<endl;
            exit(0);
        }
        vector<pageTableItem>pageTable;
        int y;
        input>>y;

        for(int j=0;j<num_v;j++){
            pageTableItem pti(-1,false,false);
            pageTable.push_back(pti);
        }
        for(int j=0;j<y;j++){
            int p;
            input>>p;
            pageTable[p].isValid=true;
        }
        pageTables.push_back(pageTable);
        vector<string>c;
        vector<int>addr;
        string s1,s2;
        while(input>>s1>>s2){
            c.push_back(s1);
            addr.push_back(stoi(s2,0,16));
        }
        cmds.push_back(c);
        vAddrs.push_back(addr);
        input.close();

        PCB p(i,name,"READY",0,i);
        PCBs.push_back(p);
        readyList.push(p);
    }

    cout<<endl<<"Time"<<TAB;
    for(int i=0;i<k;i++) cout<<"PID:"<<i<<TAB<<TAB<<TAB;
    cout<<"CPU"<<TAB<<"IOs";

    int time=0,cpu=0,ios=0,cntCPU=0,cntIOs=0;
    bool endWaiting=false;
    bool error=false;
    while(1){
        if(error){
            if(PCBs[runList.front().PID].state=="RUN:io")
                timeList.pop();
            PCB tem=runList.front();
            runList.pop();
            doneList.push(tem);
            PCBs[tem.PID].state="DONE";
            cpu=0;
            freeBitmap(tem.ptbr,pageTables);
            error=false;
        }
        if(time-timeList.front()==5){
            timeList.pop();
            PCB tem(waitingList.front());
            waitingList.pop();
            ios--;
            endWaiting=true;
            if(tem.PC>=cmds[tem.PID].size()){
                doneList.push(tem);
                PCBs[tem.PID].state="DONE";
                freeBitmap(tem.ptbr,pageTables);
                continue;
            }
            else{
                readyList.push(tem);
                PCBs[tem.PID].state="READY";
            }
        }
        if(!runList.empty()){
            PCB tem=runList.front();
            if(PCBs[tem.PID].state=="RUN:io"){
                runList.pop();
                waitingList.push(tem);
                PCBs[tem.PID].state="WAITING";
                cpu=0;
                ios++;
                continue;
            }
            if(tem.PC>=cmds[tem.PID].size()){
                runList.pop();
                doneList.push(tem);
                PCBs[tem.PID].state="DONE";
                cpu=0;
                freeBitmap(tem.ptbr,pageTables);
                continue;
            }
            if(cmds[tem.PID][tem.PC]=="cpu")
                PCBs[tem.PID].state="RUN:cpu";
            else{
                PCBs[tem.PID].state="RUN:io";
                timeList.push(time);
            }
            vAddr=vAddrs[tem.PID][tem.PC];
            vAddr2pAddr(vAddr,pAddr,offset,x,pageTables,error);
            runList.front().PC++;
        }
        else if(!readyList.empty()){
            PCB tem=readyList.front();
            readyList.pop();
            if(cmds[tem.PID][tem.PC]=="cpu")
                PCBs[tem.PID].state="RUN:cpu";
            else{
                PCBs[tem.PID].state="RUN:io";
                timeList.push(time);
            }
            cpu=1;
            PTBR=tem.ptbr;
            vAddr=vAddrs[tem.PID][tem.PC];
            vAddr2pAddr(vAddr,pAddr,offset,x,pageTables,error);
            tem.PC++;
            runList.push(tem);
        }

        cout<<endl<<++time;
        if(endWaiting){
            cout<<"*";
            endWaiting=false;
        }
        cout<<TAB;
        for(int i=0;i<k;i++){
            if(PCBs[i].state=="RUN:io"){
                cout<<"\033[96m"<<PCBs[i].state<<"  \033[97m";
                if(error) cout<<hex<<uppercase<<setfill('0')<<"\033[91m"<<setw(num_a)<<vAddr<<" error\033[97m"<<dec;
                else cout<<hex<<uppercase<<setfill('0')<<"\033[95m["<<setw(num_a)<<vAddr<<";"<<setw(num_b)<<pAddr<<"]\033[97m"<<dec;
            }
            else if(PCBs[i].state=="RUN:cpu"){
                cout<<"\033[94m"<<PCBs[i].state<<" \033[97m"; 
                if(error) cout<<hex<<uppercase<<setfill('0')<<"\033[91m"<<setw(num_a)<<vAddr<<" error\033[97m"<<dec;
                else cout<<hex<<uppercase<<setfill('0')<<"\033[95m["<<setw(num_a)<<vAddr<<";"<<setw(num_b)<<pAddr<<"]\033[97m"<<dec;
            }          
            else if(PCBs[i].state=="READY")
                cout<<"\033[93m"<<PCBs[i].state<<"\033[97m"<<TAB<<TAB;
            else if(PCBs[i].state=="WAITING")
                cout<<"\033[90m"<<PCBs[i].state<<"\033[97m"<<TAB<<TAB;
            else if(PCBs[i].state=="DONE")
                cout<<"\033[92m"<<PCBs[i].state<<"\033[97m"<<TAB<<TAB;
            cout<<TAB;
        }
        if(cpu){
            cntCPU++;
            cout<<cpu;
        }
        cout<<TAB;
        if(ios){
            cntIOs++;
            cout<<ios;
        }
        if(doneList.size()==k) break;
    }
    printf("\n\nStatus: Total Time %d\n",time);
    printf("Status: CPU Busy %d (%.2f\%)\n",cntCPU,cntCPU*100.0/time);
    printf("Status: IO Busy %d (%.2f\%)\n",cntIOs,cntIOs*100.0/time);
    
    cout<<endl<<"bitmap when completed"<<endl;
    for(int i=0;i<num_p;i++)
        cout<<bitmap[i];
}

int main(int argc, char* argv[]){
    srand((unsigned)time(NULL));
    cout<<"\033[97m";
    if(argc!=13){
        cout<<"number of arguments is incorrect:"<<argc-1<<endl;
        cout<<"expected number:11"<<endl;
        cout<<"use default arguments"<<endl;
        //use default arguments
        //char* defaultArgv[12]={"","6","80","5","10","12","16","18","3","7","0.9","0.7"};
    }
    int k=atoi(argv[1]);
    int p=atoi(argv[2]);
    int mi=atoi(argv[3]);
    int mx=atoi(argv[4]);
    int x=atoi(argv[5]);
    int a=atoi(argv[6]);
    int b=atoi(argv[7]);
    int min_size=atoi(argv[8]);
    int max_size=atoi(argv[9]);
    double p_valid=atof(argv[10]);
    double p_available=atof(argv[11]);
    int max_alloc=atoi(argv[12]);
    string mode=string(argv[13]);
    // cout<<"arguments are"<<endl
    //     <<"k="<<k<<endl
    //     <<"persent="<<p<<endl
    //     <<"min="<<mi<<endl
    //     <<"max="<<mx<<endl
    //     <<"x="<<x<<endl
    //     <<"a="<<a<<endl
    //     <<"b="<<b<<endl
    //     <<"min_size="<<min_size<<endl
    //     <<"max_size="<<max_size<<endl
    //     <<"p_valid="<<p_valid<<endl
    //     <<"p_available="<<p_available<<endl
    //     <<"max_alloc="<<max_alloc<<endl
    //     <<"mode="<<mode<<endl;

    cout<<endl<<"initial bitmap"<<endl;
    int num=pow(2,b-x);
    for(int i=0;i<num;i++){
        if((double)rand()/RAND_MAX<=p_available)
            bitmap[i]=0;
        else bitmap[i]=1;
        cout<<bitmap[i];
    }
    cout<<endl;
    //generateProgress(k,p,mi,mx,x,a,b,min_size,max_size,p_valid,p_available);
    scheduleProgress(k,x,a,b,max_alloc,mode);
    cout<<"\033[0m";
    return 0;
}