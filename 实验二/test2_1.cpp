#include<bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]){
    srand((unsigned)time(NULL));
    if(argc!=12){
        cout<<"number of arguments is incorrect:"<<argc-1<<endl;
        cout<<"expected number:11"<<endl;
        //exit(-1);
        cout<<"using defalut arguments"<<endl<<endl;
        char defaultArgv[15][15]={"","3","80","5","10","12","16","18","3","7","0.8","0.85"};
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
    cout<<"²ÎÊýÎª"<<endl
        <<"k="<<k<<endl
        <<"persent="<<p<<endl
        <<"min="<<mi<<endl
        <<"max="<<mx<<endl
        <<"x="<<x<<endl
        <<"a="<<a<<endl
        <<"b="<<b<<endl
        <<"min_size="<<min_size<<endl
        <<"max_size="<<max_size<<endl
        <<"p_valid="<<p_valid<<endl
        <<"p_available="<<p_available<<endl;

    for(int i=1;i<=k;i++){
        string name="P"+to_string(i)+".txt";
        string path="./data/"+name;
        freopen(path.c_str(), "w", stdout);

        int y=rand()%(max_size-min_size+1)+min_size;
        int pages=pow(2,a-x);
        vector<int>valid,invalid;
        set<int>valid_set;
        for(int i=1;i<=y;i++){
            int page=rand()%pages;
            if(valid_set.find(page)==valid_set.end())
                valid_set.insert(page);
            else i--;
        }
        for(int i=0;i<pages;i++){
            if(valid_set.find(i)==valid_set.end())
                invalid.push_back(i);
            else valid.push_back(i);
        }

        cout<<y;
        for(auto it:valid) cout<<" "<<it;
        cout<<endl;
        int num=mi+rand()%(mx-mi+1);
        int max_offset=pow(2,x);
        for(int j=1;j<=num;j++){
            if(rand()%101<=p) cout<<"cpu\t";
            else cout<<"io\t";

            int num_o=(a-x)/4+((a-x)%4!=0);
            int num_x=x/4+(x%4!=0);
            if((double)rand()/RAND_MAX<=p_valid)
                cout<<hex<<uppercase<<setw(num_o)<<setfill('0')<<valid[rand()%valid.size()];
            else cout<<hex<<uppercase<<setw(num_o)<<setfill('0')<<invalid[rand()%invalid.size()];
            cout<<uppercase<<setw(num_x)<<setfill('0')<<rand()%max_offset<<dec<<endl;
        }
    }
    return 0;
}
