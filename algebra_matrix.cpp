#include <iostream>
#include <cstdio>
#include <cassert>
#include <cstring>

using namespace std;

class element{
    private:
        int numerator, denominator;
    public:

    element(int numerator, int denominator):numerator(numerator), denominator(denominator){
        normalize();
    }
    element(char c[]){
        if(check_fraction(c)){
            analyze_fraction(numerator, denominator, c);
            normalize();
        }else{
            numerator=atoi(c);
            denominator=1;
        }
    }
    element(){}

    ~element(){}

    bool check_fraction(char c[]){
        int n=strlen(c);
        for(int i=0;i<n;++i){
            if(c[i]=='/'){
                return true;
            }
        }
        return false;
    }

    void analyze_fraction(int& n, int& d, char c[]){
        n=0;
        d=0;
        int k=1, i=strlen(c)-1;
        while(c[i]!='/'){
            if(c[i]=='-'){
                d*=-1;
            }else if(c[i]>='0'&&c[i]<='9'){
                d+=(c[i]-48)*k; 
                k*=10;
            }
            --i;
        }
        --i;
        k=1;
        while(i>=0){
            if(c[i]=='-'){
                n*=-1;
            }else if(c[i]>='0'&&c[i]<='9'){
                n+=(c[i]-48)*k;
                k*=10;
            }
            --i;
        }
    }

    void normalize(){
        int k=gcd(numerator, denominator);
        numerator/=k;
        denominator/=k;

        if(denominator<0){
            numerator*=-1;
            denominator*=-1;
        }
    }

    void set(int n, int d){
        numerator=n;
        denominator=d;
        normalize();
    }

    int& get_n(){
        return numerator;
    }

    int& get_d(){
        return denominator;
    }
    
    void set(char c[]){
        if(check_fraction(c)){
            analyze_fraction(numerator, denominator, c);
            normalize();
        }else{
            numerator=atoi(c);
            denominator=1;
        }
    }

    int gcd(int a, int b){
        if (b == 0) return a;
        else return gcd(b, a % b);
    }

    void out_element(){
        if(denominator!=1){
            string s=to_string(numerator)+"/"+to_string(denominator);
            string t;
            for(int i=0;i<6-s.length();++i){
                t+=" ";
            }
            s=t+s;
            cout<<s;
        }else printf("%6d",numerator);
    }
};

class matrix_node{
    private:
        int n,m;
    public:

    element** a;
    matrix_node* next;
    matrix_node* prev;

    matrix_node(element** a, int n, int m):a(a),n(n),m(m){
        next=NULL;
        prev=NULL;
    }

    matrix_node(element**a, int n, int m, matrix_node* prev):a(a),n(n),m(m),prev(prev){
        next=NULL;
    }

    ~matrix_node(){
        for(int i=0;i<n;++i){
            delete[] a[i];
        }
        delete[] a;
    }
};

class matrix{
    private:
        int n,m;
        element multiplicator;
        int kr;
        matrix_node* cur;
    public:
    
    matrix(){}
    matrix(int n, int m):n(n), m(m){
        init();
    }
    ~matrix() {
        delete_nodes(cur->next);
        while(cur!=NULL){
            matrix_node* del=cur;
            cur=cur->prev;
            delete del;
        }
    }
    void init(){
        element**a;
        a=new element *[n];
        for(int i=0;i<n;++i){
            a[i] = new element[m];
        }
        cur = new matrix_node(a,n,m);
    }
    void delete_nodes(matrix_node* node){
        while(node!=NULL){
            matrix_node* del = node;
            node=node->next;
            delete del;
        }
    }
    void read(){
        for(int i=0;i<n;++i){
            printf("    Enter line %d:\n", i+1);
            printf("    ");
            for(int j=0;j<m;++j){
                char t[8];
                scanf("%s",t);
                cur->a[i][j].set(t);
            }
        }
    }
    bool fread(FILE *fr){
        for(int i=0;i<n;++i){
            for(int j=0;j<m;++j){
                char t[8];
                int ret = fscanf(fr,"%s",t);
                if(ret!=1){
                    return false;
                }
                cur->a[i][j].set(t);
            }
        }
        return true;
    }
    void out(){
        printf("\nCurrent matrix\n");
        for(int i=0;i<n;++i){
            for(int j=0;j<m;++j){
                //printf("%4d ",a[i][j]);
                cur->a[i][j].out_element();
                printf(" ");
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
                if(cur->a[i][j].get_n()>=kr){
                    return false;
                }
            }
        }
        return true;
    }
    int& krat(){
        return kr;
    }
    element& mul(){
        return multiplicator;
    }
    int get_n(){
        return n;
    }
    int get_m(){
        return m;
    }

    void copy(){
        element** c = new element*[n];
        for(int i=0;i<n;++i){
            c[i]=new element[m];
            for(int j=0;j<m;++j){
                c[i][j] = cur->a[i][j];
            }
        }
        delete_nodes(cur->next);
        matrix_node* t = new matrix_node(c,n,m,cur);
        cur->next=t;
        cur=t;
    }
    void undo(){
        if(cur->prev!=NULL){
            cur=cur->prev;
        }
    }
    void revert(){
        if(cur->next!=NULL){
            cur=cur->next;
        }
    }

    void rtm(){
        for(int i=0;i<n;++i){
            int t=-1;
            for(int j=0;j<m&&t==-1;++j){
                if(cur->a[i][j].get_n()!=0){
                    t=j;
                }
            }
            dev(cur->a[i][t],i);
            for(int t1=0;t1<n;++t1){
                if(t1!=i){
                    multiplicator=cur->a[t1][t];
                    min(t1,i);
                }
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
        element *t = cur->a[i1];
        cur->a[i1] = cur->a[i2];
        cur->a[i2] = t;
    }
    void mult(element t, int i){
        if(kr!=-1){
            for(int j=0;j<m;++j){
                int s = cur->a[i][j].get_n() * t.get_n();
                s%=kr;
                cur->a[i][j].set(s, cur->a[i][j].get_d());
            }
        }else{
            for(int j=0;j<m;++j){
                int n = cur->a[i][j].get_n() * t.get_n();
                int d = cur->a[i][j].get_d() * t.get_d();
                cur->a[i][j].set(n,d);
            }
        }
    }
    void dev(element t, int i){
        if(kr!=-1){
            for(int j=0;j<m;++j){
                int s = cur->a[i][j].get_n() * nm(t.get_n());
                s%=kr;
                cur->a[i][j].set(s, cur->a[i][j].get_d());
            }
        }else{
            for(int j=0;j<m;++j){
                int n = cur->a[i][j].get_n() * t.get_d();
                int d = cur->a[i][j].get_d() * t.get_n();
                cur->a[i][j].set(n,d);
            }
        }
    }
    void add(int i1, int i2){
        for(int j=0;j<m;++j){
            if(kr!=-1){
                int s = cur->a[i2][j].get_n() * multiplicator.get_n();
                s%=kr;
                s+=cur->a[i1][j].get_n();
                s%=kr;
                cur->a[i1][j].get_n()=s;
            }else{
                int n = (cur->a[i1][j].get_n()*
                        cur->a[i2][j].get_d()*
                        multiplicator.get_d()) +
                        (cur->a[i1][j].get_d()*
                        cur->a[i2][j].get_n()*
                        multiplicator.get_n());
                int d = cur->a[i1][j].get_d()*
                        cur->a[i2][j].get_d()*
                        multiplicator.get_d();
                cur->a[i1][j].set(n,d);
            }
        }
    }
    void min(int i1, int i2){
        for(int j=0;j<m;++j){
            if(kr!=-1){
                int s = cur->a[i2][j].get_n() * multiplicator.get_n();
                s%=kr;
                s = na(s);
                s+=cur->a[i1][j].get_n();
                s%=kr;
                cur->a[i1][j].get_n()=s;
            }else{
                int n = (cur->a[i1][j].get_n()*
                        cur->a[i2][j].get_d()*
                        multiplicator.get_d()) -
                        (cur->a[i1][j].get_d()*
                        cur->a[i2][j].get_n()*
                        multiplicator.get_n());
                int d = cur->a[i1][j].get_d()*
                        cur->a[i2][j].get_d()*
                        multiplicator.get_d();
                cur->a[i1][j].set(n,d);
            }
        }
    }
    void change_line(int i, element* t){
        delete[] cur->a[i];
        cur->a[i]=t;
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
        printf("> ");
        scanf("%s", command);
        if (strcmp(command, "new") == 0) {
            delete ma;
            printf("\n");
            matrix_input();
            ma->out();
        } else if (strcmp(command, "*") == 0) {
            char t[8];
            int y;
            scanf("%s %d",t,&y);
            element m(t);
            if(y<1||y>(ma->get_n())){
                printf("Invalid line number\n\n");
            }else if(m.get_n()==0){
                printf("Cannot multiply by 0\n\n");
            }else{
                ma->copy();
                ma->mult(m,y-1);
                if(m.get_d()==1){
                    printf("R%d <- %d * R%d", y, m.get_n(), y);
                }else{
                    printf("R%d <- %d/%d * R%d", y, m.get_n(), m.get_d(), y);
                }
                ma->out();
                printf("\n");
            }
        } else if (strcmp(command, "+") == 0) {
            int x,y;
            char t[8];
            scanf("%d %d",&x,&y);
            if((x<1||x>(ma->get_n()))||(y<1||y>(ma->get_n()))){
                printf("Invalid line number\n\n");
            }else{
                printf("Choose multiplicator: ");
                scanf("%s", t);
                element m(t);
                if(m.get_n()<=0){
                    printf("Multiplicator must be strictly positive\n\n");
                }else{
                    ma->copy();
                    ma->mul()=m;
                    ma->add(x-1,y-1);
                    if(m.get_n()==1&&m.get_d()==1){
                        printf("R%d <- R%d + R%d", x, x, y);
                    }else{        
                        if(m.get_d()==1){
                            printf("R%d <- R%d + %d * R%d", x, x, m.get_n(), y);
                        }else{
                            printf("R%d <- R%d + %d/%d * R%d", x, x, m.get_n(), m.get_d(), y);
                        }
                    }
                    ma->out();
                    printf("\n");
                }
            }
        } else if (strcmp(command, "/") == 0) {
            char t[8];
            int y;
            scanf("%s %d",t,&y);
            element m(t);
            if(y>=1&&y<=(ma->get_n())){
                ma->copy();
                ma->dev(m, y-1);
                if(m.get_d()==1){
                    printf("R%d <- 1/%d * R%d", y, m.get_n(), y);
                }else{
                    printf("R%d <- %d/%d * R%d", y, m.get_d(), m.get_n(), y);
                }
                ma->out();
                printf("\n");
            }else{
                printf("Invalid line number\n");
            }
        } else if (strcmp(command, "-") == 0) {
            int x,y;
            char t[8];
            scanf("%d %d", &x, &y);
            printf("Choose multiplicator: ");
            scanf("%s", t);
            element m(t);
            ma->copy();
            ma->mul()=m;
            ma->min(x-1, y-1);
            if(m.get_n()==1&&m.get_d()==1){
                printf("R%d <- R%d + R%d", x, x, y);
            }else{        
                if(m.get_d()==1){
                    printf("R%d <- R%d - %d * R%d", x, x, m.get_n(), y);
                }else{
                    printf("R%d <- R%d - %d/%d * R%d", x, x, m.get_n(), m.get_d(), y);
                }
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
        } else if (strcmp(command, "undo") == 0) {
            ma->undo();
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
            printf("Enter line number: ");
            int x;
            scanf("%d",&x);
            printf("Enter line: ");
            element *t = new element[ma->get_m()];
            for(int i=0;i<ma->get_m();++i){
                char st[8];
                scanf("%s",st);
                t[i].set(st);
            }
            ma->change_line(x-1, t);
            ma->out();
            printf("\n");
        } else if (strcmp(command, "help") == 0){
            FILE* fr = fopen("guide.txt","r");
            char buffer[256];
            while (fgets(buffer, sizeof(buffer), fr)) {
                cout << buffer;
            }
            cout<<endl<<endl<<endl;
            fclose(fr);
        } else if (strcmp(command, "matrix") == 0){
            ma->out();
            printf("\n");
        } else if (strcmp(command, "rtm") == 0){
            ma->copy();
            ma->rtm();
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