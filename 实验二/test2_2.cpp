#include<bits/stdc++.h>
#include<windows.h>
using namespace std;

class PCB{
public:
    int PID;
    string name;
    string state;
    int PC;
    PCB(){
        PID=0,name="default",state="READY",PC=0;
    }
    PCB(int pid,string n,string s,int pc){
        PID=pid,name=n,state=s,PC=pc;
    }
    PCB(const PCB& p):PID(p.PID),name(p.name),state(p.state),PC(p.PC){}
};

int main(int argc, char* argv[]){
    cout<<"\033[97m";
    if(argc!=3){
        cout<<"number of arguments is incorrect:"<<argc-1<<endl;
        cout<<"expected number:2"<<endl;
        //exit(-1);
        cout<<"using defalut arguments"<<endl<<endl;
        char defaultArgv[3][15]={"","6","sampleData"};
        argv[1]=defaultArgv[1];
        argv[2]=defaultArgv[2];
    }
    int k=atoi(argv[1]);
    string data=string(argv[2]);
    vector<vector<string>>cmd;
    vector<PCB>PCBs;
    queue<PCB>readyList,runList,waitingList,doneList;
    queue<int>timeList;
    const string TAB="\t";
    for(int i=0;i<k;i++){
        string name="P"+to_string(i+1)+".txt";
        string path="./"+data+"/"+name;
        ifstream input(path.c_str(),ios::in);
        if(!input){
            cout<<"open file error"<<endl;
            exit(0);
        }
        vector<string>c;
        string s;
        while(input>>s) c.push_back(s);
        cmd.push_back(c);
        input.close();

        PCB p(i,name,"READY",0);
        PCBs.push_back(p);
        readyList.push(p);
    }
    cout<<"Time"<<TAB;
    for(int i=0;i<k;i++) cout<<"PID:"<<i<<TAB<<TAB;
    cout<<"CPU"<<TAB<<"IOs";

    int time=0,cpu=0,ios=0,cntCPU=0,cntIOs=0;
    bool endWaiting=false;
    while(1){
        if(time-timeList.front()==5){
            timeList.pop();
            PCB tem(waitingList.front());
            waitingList.pop();
            ios--;
            endWaiting=true;
            if(tem.PC>=cmd[tem.PID].size()){
                doneList.push(tem);
                PCBs[tem.PID].state="DONE";
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
            if(tem.PC>=cmd[tem.PID].size()){
                runList.pop();
                doneList.push(tem);
                PCBs[tem.PID].state="DONE";
                cpu=0;
                continue;
            }
            if(cmd[tem.PID][tem.PC]=="cpu")
                PCBs[tem.PID].state="RUN:cpu";
            else{
                PCBs[tem.PID].state="RUN:io";
                timeList.push(time);
            }
            runList.front().PC++;
        }
        else if(!readyList.empty()){
            PCB tem=readyList.front();
            readyList.pop();
            if(cmd[tem.PID][tem.PC]=="cpu")
                PCBs[tem.PID].state="RUN:cpu";
            else{
                PCBs[tem.PID].state="RUN:io";
                timeList.push(time);
            }
            cpu=1;
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
            if(PCBs[i].state=="RUN:io")
                cout<<"\033[96m"<<PCBs[i].state<<"\033[97m";
            else if(PCBs[i].state=="READY")
                cout<<"\033[93m"<<PCBs[i].state<<"\033[97m";
            else if(PCBs[i].state=="WAITING")
                cout<<"\033[90m"<<PCBs[i].state<<"\033[97m";
            else if(PCBs[i].state=="RUN:cpu")
                cout<<"\033[94m"<<PCBs[i].state<<"\033[97m";
            else if(PCBs[i].state=="DONE")
                cout<<"\033[92m"<<PCBs[i].state<<"\033[97m";
            cout<<TAB<<TAB;
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
    cout<<"\033[0m";
    return 0;
}
