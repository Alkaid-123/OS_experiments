#include<bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]){
    srand((unsigned)time(NULL));
    if(argc!=5){
        cout<<"������������:"<<argc<<endl;
        exit(-1);
    }
    int k=atoi(argv[1]);
    int p=atoi(argv[2]);
    int mi=atoi(argv[3]);
    int mx=atoi(argv[4]);
    cout<<"����Ϊ"<<endl
        <<"k="<<k<<endl
        <<"persent="<<p<<endl
        <<"min="<<mi<<endl
        <<"max="<<mx<<endl;
/*
    if(k<0){
        cout<<"k��������"<<endl;
        exit(-1);
    }
    if(p<0||p>100){
        cout<<"percent��������"<<endl;
        exit(-1);
    }
    if(mi<0){
        cout<<"min��������"<<endl;
        exit(-1);
    }
    if(mx<0||mx<mi){
        cout<<"max��������"<<endl;
        exit(-1);
    }
*/

    for(int i=1;i<=k;i++){
        string name="P"+to_string(i)+".txt";
        string path="./data/"+name;
        freopen(path.c_str(), "w", stdout);

        int num=mi+rand()%(mx-mi+1);
        for(int j=1;j<=num;j++){
            if(rand()%101<=p) cout<<"cpu"<<endl;
            else cout<<"io"<<endl;
        }
    }
    return 0;
}
