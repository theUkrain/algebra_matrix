#include <iostream>
#include <cstdio>
#include <cassert>
#include <cstring>

using namespace std;

class matrix{
    private:
        int n,m;
        int multiplicator;
        int kr;
        int **a;
        int **c;
    public:
    
    matrix(){}
    matrix(int n, int m):n(n), m(m){
        init();
    }
    ~matrix() {
        for(int i=0;i<n;++i){
            delete[] a[i];
            delete[] c[i];
        }
        delete[] a;
        delete[] c;
    }
    void init(){
        a=new int *[n];
        c=new int *[n];
        for(int i=0;i<n;++i){
            a[i] = new int[m];
            c[i] = new int[m];
        }
    }
    void read(){
        for(int i=0;i<n;++i){
            printf("    Enter line %d:\n", i+1);
            printf("    ");
            for(int j=0;j<m;++j){
                scanf("%d",&a[i][j]);
                c[i][j] = a[i][j];
            }
        }
    }
    bool fread(FILE *fr){
        for(int i=0;i<n;++i){
            for(int j=0;j<m;++j){
                int ret = fscanf(fr,"%d",&a[i][j]);
                if(ret!=1){
                    return false;
                }
            }
        }
        return true;
    }
    void out(){
        printf("\nCurrent matrix\n");
        for(int i=0;i<n;++i){
            for(int j=0;j<m;++j){
                printf("%4d ",a[i][j]);
            }
            printf("\n");
        }
    }
    bool chek_matrix(){
        if(kr==-1){
            return true;
        }
        for(int i=0;i<n;++i){
            for(int j=0;j<m;++j){
                if(a[i][j]>=kr){
                    return false;
                }
            }
        }
        return true;
    }
    int& krat(){
        return kr;
    }
    int& mul(){
        return multiplicator;
    }
    int get_n(){
        return n;
    }
    int get_m(){
        return m;
    }

    void copy(){
        for(int i=0;i<n;++i){
            for(int j=0;j<m;++j){
                c[i][j] = a[i][j];
            }
        }
    }
    void revert(){
        for(int i=0;i<n;++i){
            for(int j=0;j<m;++j){
                a[i][j] = c[i][j];
            }
        }
    }

    int nm(int x){
        x=abs(x);
        for(int i=0;i<kr;++i){
            if((x*i)%kr==1){
                return i;
            }
        }
        return -1;
    }

    int na(int x){
        x=abs(x);
        for(int i=0;i<kr;++i){
            if((x+i)%kr==0){
                return i;
            }
        }
        return -1;
    }

    void swap(int i1, int i2){
        int *t = a[i1];
        a[i1] = a[i2];
        a[i2] = t;
    }
    void mult(int t, int i){
        if(kr!=-1&&t<0){
            t=na(t);
        }
        for(int j=0;j<m;++j){
            int s=a[i][j] * t;
            if(kr!=-1){
                s%=kr;
            }
            a[i][j]=s;
        }
    }
    void dev(int t, int i){
        if(kr!=-1){
            if(t<0){
                t=na(t);
            }
            t=nm(t);
        }
        mult(t,i);
    }
    void add(int i1, int i2){
        for(int j=0;j<m;++j){
            int s = a[i2][j] * multiplicator;
            if(kr!=-1){
                s%=kr;
            }
            s+=a[i1][j];
            if(kr!=-1){
                s%=kr;
            }
            a[i1][j]=s;
        }
    }
    void min(int i1, int i2){
        for(int j=0;j<m;++j){
            int s = a[i2][j] * multiplicator;
            if(kr!=-1){
                s%=kr;
                s = na(s);
            }
            s+=a[i1][j];
            if(kr!=-1){
                s%=kr;
            }
            a[i1][j]=s;
        }
    }
    void change_line(int i, int* t){
        delete[] a[i];
        a[i]=t;
    }
};

matrix* ma;

void matrix_input(){
    char command[101];
    printf("Choose input f/h: ");
    scanf("%s",command);
    if(strcmp(command, "h") == 0){
        printf("Please enter matrix size (n x m): ");
        int n,m;
        scanf("%d %s %d", &m, command, &n);
        ma = new matrix(n,m);
        ma->init();
        
        printf("\nPlease enter matrix\n");
        ma->read();

        int t;
        printf("\nChoose krat: ");
        scanf("%d",&t);
        ma->krat() = t;
        
        if(!ma->chek_matrix()){
            printf("Matrix and krat contradiction\n\n");
        }
    }else if(strcmp(command, "f") == 0){
        printf("Please enter file name with type: ");

        scanf("%s",command);
        FILE *fr = fopen(command, "r");

        bool f=false;

        if(fr!=NULL){
            int n,m;
            int t = fscanf(fr,"%d %d", &m, &n);
            f=(t==2 && m>=0 && n>=0);
            if(f){
                ma = new matrix(n,m);
                ma->init();
                f=ma->fread(fr);
                if(f){
                    int t;
                    int ret = fscanf(fr,"%d",&t);
                    f=(ret==1);
                    if(f){
                        ma->krat() = t;
                    }else{
                        strcpy(command, "bad krat");
                    }
                }else{
                    strcpy(command, "bad matrix");
                }
            }else{
                strcpy(command, "bad size parametrs");
            }
            if(f){
                printf("Reading complete\n\n");
                
                if(!ma->chek_matrix()){
                    printf("Matrix and krat contradiction\n");
                }
            }else{
                printf("File corrupted: %s\n\n", command);
                matrix_input();
            }
        }else{
            printf("File does not exist\n\n");
            matrix_input();
        }
    }else{
        printf("Input unidentified\n\n");
        matrix_input();
    }
}

void calculation(){
    bool f=true;
    while(f){
        char command[101];
        scanf("%s", command);
        if (strcmp(command, "new") == 0) {
            delete ma;
            printf("\n");
            matrix_input();
            ma->out();
        } else if (strcmp(command, "*") == 0) {
            int x, y;
            scanf("%d %d", &x, &y);
            if(y<1||y>(ma->get_n())){
                printf("Invalid line number\n\n");
            }else if(x==0){
                printf("Cannot multiply by 0\n\n");
            }else{
                ma->copy();
                ma->mult(x, y-1);
                printf("R%d <- %d * R%d", y, x, y);
                ma->out();
                printf("\n");
            }
        } else if (strcmp(command, "+") == 0) {
            int x, y, t;
            scanf("%d %d", &x, &y);
            if((x<1||x>(ma->get_n()))||(y<1||y>(ma->get_n()))){
                printf("Invalid line number\n\n");
            }else{
                printf("Choose multiplicator: ");
                scanf("%d", &t);
                if(t<=0){
                    printf("Multiplicator must be strictly positive\n\n");
                }else{
                    ma->copy();
                    ma->mul()=t;
                    ma->add(x-1, y-1);
                    if(t!=1){
                        printf("R%d <- R%d + %d * R%d", x, x, t, y);
                    }else{
                        printf("R%d <- R%d + R%d", x, x, y);
                    }
                    ma->out();
                    printf("\n");
                }
            }
        } else if (strcmp(command, "/") == 0) {
            int x, y;
            scanf("%d %d", &x, &y);
            if(y>=1&&y<=(ma->get_n())){
                ma->copy();
                ma->dev(x, y-1);
                printf("R%d <- 1/%d R%d", y, x, y);
                ma->out();
                printf("\n");
            }else{
                printf("Invalid line number\n");
            } 
        } else if (strcmp(command, "-") == 0) {
            int x, y, t;
            scanf("%d %d", &x, &y);
            printf("Choose multiplicator: ");
            scanf("%d", &t);
            ma->copy();
            ma->mul()=t;
            ma->min(x-1, y-1);
            if(t!=1){
                printf("R%d <- R%d - %d * R%d", x, x, t, y);
            }else{
                printf("R%d <- R%d - R%d", x, x, y);
            }
            ma->out();
            printf("\n");
        } else if (strcmp(command, "swap") == 0) {
            int x,y;
            scanf("%d %d", &x, &y);
            ma->copy();
            ma->swap(x-1,y-1);
            printf("R%d <-> R%d",x ,y);
            ma->out();
            printf("\n");
        } else if (strcmp(command, "krat") == 0) {
            printf("Krat: %d\n\n", ma->krat());
        } else if (strcmp(command, "revert") == 0) {
            ma->revert();
            ma->out();
            printf("\n");
        }  else if (strcmp(command, "n*") == 0) {
            int x;
            scanf("%d",&x);
            printf("For %d invert(*): %d\n\n",x ,ma->nm(x));
        } else if (strcmp(command, "n+") == 0) {
            int x;
            scanf("%d",&x);
            printf("For %d invert(+): %d\n\n",x ,ma->na(x));
        }  else if (strcmp(command, "setk") == 0) {
            int x;
            scanf("%d",&x);
            ma->krat()=x;
            printf("Krat set to: %d\n\n",x);
            if(!ma->chek_matrix()){
                printf("Matrix and krat contradiction\n");
                ma->out();
                printf("\n");
            }
        }else if (strcmp(command, "end") == 0) {
            f=false;
        } else if (strcmp(command, "change") == 0) {
            ma->copy();
            printf("Eneter line number: ");
            int x;
            scanf("%d",&x);
            printf("Enter line: ");
            int *t = new int[ma->get_m()];
            for(int i=0;i<ma->get_m();++i){
                scanf("%d",&t[i]);
            }
            ma->change_line(x-1, t);
            ma->out();
            printf("\n");
        } else {
            printf("bad input\n");
        }
    }
}

int main(){
    matrix_input();

    ma->out();
    printf("\n");
    
    calculation();

    delete ma;
}