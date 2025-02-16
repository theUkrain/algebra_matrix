#include <iostream>
#include <cmath>
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

    void fout_element(FILE* fr){
        if(denominator!=1){
            string s=to_string(numerator)+"/"+to_string(denominator);
            string t;
            /*for(int i=0;i<6-s.length();++i){
                t+=" ";
            }*/
            s=t+s;
            fprintf(fr,"%s",s);
        }else fprintf(fr,"%d",numerator);
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

    int& get_n(){
        return n;
    }
    int& get_m(){
        return m;
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
    matrix_node*& curent(){
        return cur;
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
    void tout(){
        printf("\n");
        for(int i=0;i<n;++i){
            for(int j=0;j<m;++j){
                cur->a[i][j].out_element();
                printf(" ");
            }
            printf("\n");
        }
    }
    void save(char filename[]){
        FILE *fr = fopen(filename, "w");
        fprintf(fr,"%d %d\n",n,m);
        for(int i=0;i<n;++i){
            for(int j=0;j<m;++j){
                cur->a[i][j].fout_element(fr);
                fprintf(fr," ");
            }
            fprintf(fr,"\n");
        }
        fprintf(fr,"%d",kr);
        fclose(fr);
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
    element& elem(int i, int j){
        return cur->a[i][j];
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
            n=cur->prev->get_n();
            m=cur->prev->get_m();
            //cout<<n<<" "<<m<<endl;
            cur=cur->prev;
        }
    }
    void revert(){
        if(cur->next!=NULL){
            n=cur->next->get_n();
            m=cur->next->get_m();
            //cout<<n<<" "<<m<<endl;
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

    int det(){
        if(n==1 && m==1){
            return cur->a[0][0].get_n()/cur->a[0][0].get_d();
        }else return det(cur->a, n, m);
    }
    int det(element** t, int t_n, int t_m){
        if(t_n==2 && t_m==2){
            element y_1;
            y_1.set(t[0][0].get_n()*t[1][1].get_n(),t[0][0].get_d()*t[1][1].get_d());

            element y_2;
            y_2.set(t[0][1].get_n()*t[1][0].get_n(),t[0][1].get_d()*t[1][0].get_d());

            y_1.set(
                y_1.get_n()*y_2.get_d() - y_2.get_n()*y_1.get_d()
                ,y_1.get_d()*y_2.get_d());

            return y_1.get_n()/y_2.get_d();
        }else if(t_n>2 && t_m>2){
            int t_i, t_j;
            int nul_i=evaluate_row(t, t_n, t_m, t_i);
            int nul_j=evaluate_col(t, t_n, t_m, t_j);
            int s=0;
            if(nul_i>=nul_j){
                for(int k=0;k<t_m;++k){
                    if(t[t_i][k].get_n()!=0){
                        element** t_det = new element *[t_n-1];
                        for(int i=0;i<t_n-1;++i){
                            t_det[i] = new element[t_m-1];
                        }
                        int v=0;
                        for(int j=0;j<t_m;++j){
                            if(j!=k){
                                int u=0;
                                for(int i=0;i<t_n;++i){
                                    if(i!=t_i){
                                        t_det[u][v]=t[i][j];
                                        ++u;
                                    }
                                }
                                ++v;
                            }
                        }

                        s+=det(t_det, t_n-1, t_m-1) * pow(-1,t_i+k+2) * t[t_i][k].get_n() / t[t_i][k].get_d();

                        for(int i=0;i<t_n-1;++i){
                            delete[] t_det[i];
                        }
                        delete[] t_det;
                    }
                }
            }else{
                for(int k=0;k<t_n;++k){
                    if(t[k][t_j].get_n()!=0){
                        element** t_det = new element *[t_n-1];
                        int u=0;
                        for(int i=0;i<t_n;++i){
                            if(i!=k){
                                t_det[u] = new element[t_m-1];
                                int v=0;
                                for(int j=0;j<t_m;++j){
                                    if(j!=t_j){
                                        t_det[u][v]=t[i][j];
                                        ++v;
                                    }
                                }
                                ++u;
                            }
                        }
                        
                        s+=det(t_det, t_n-1, t_m-1) * pow(-1,t_j+k+2) * t[k][t_j].get_n() / t[k][t_j].get_d();

                        for(int i=0;i<t_n-1;++i){
                            delete[] t_det[i];
                        }
                        delete[] t_det;
                    }
                }
            }

            return s;
        }else return -1;
    }

    int evaluate_row(element** t, int t_n, int t_m, int& t_i){
        int max=-1;
        t_i=0;
        for(int i=0;i<t_n;++i){
            int kol=0;
            for(int j=0;j<t_m;++j){
                kol+=t[i][j].get_n()==0;
            }
            if(kol>max){
                max=kol;
                t_i=i;
            }
        }
        return max;
    }

    int evaluate_col(element** t, int t_n, int t_m, int& t_j){
        int max=-1;
        t_j=0;
        for(int j=0;j<t_m;++j){
            int kol=0;
            for(int i=0;i<t_n;++i){
                kol+=t[i][j].get_n()==0;
            }
            if(kol>max){
                max=kol;
                t_j=j;
            }
        }
        return max;
    }

    void T(){
        element** t = new element*[m];
        for(int i=0;i<m;++i){
            t[i] = new element[n];
        }
        for(int j=0;j<m;++j){
            for(int i=0;i<n;++i){
                t[j][i] = cur->a[i][j];
            }
        }
        
        for(int i=0;i<n;++i){
            delete[] cur->a[i];
        }
        delete[] cur->a;

        int h=n;
        n=m;
        m=h;

        cur->get_n()=n;
        cur->get_m()=m;

        cur->a=t;
    }

    void matrix_mult(matrix* t){
        element** r = new element*[n];
        for(int i=0;i<n;++i){
            r[i] = new element[t->get_m()];
        }
        
        for(int i=0;i<n;++i){
            for(int j=0;j<t->get_m();++j){
                element res;
                res.set(0,1);
                for(int k=0;k<m;++k){
                    if(kr==-1){
                        int num = cur->a[i][k].get_n() * t->cur->a[k][j].get_n();
                        int den = cur->a[i][k].get_d() * t->cur->a[k][j].get_d();
                        num = num * res.get_d() + res.get_n() * den;
                        den = den * res.get_d();
                        res.set(num,den);
                    }else{
                        int num = cur->a[i][k].get_n() * t->cur->a[k][j].get_n();
                        num%=kr;
                        res.set(num,1);
                    }
                }
                r[i][j]=res;
            }
        }
        for(int i=0;i<n;++i){
            delete[] cur->a[i];
        }
        delete[] cur->a;
        cur->a=r;
        m=t->get_m();
        cur->get_m()=m;
    }

    void matrix_add(matrix* t){
        for(int i=0;i<n;++i){
            for(int j=0;j<m;++j){
                if(kr==-1){
                    int num = cur->a[i][j].get_n() * t->cur->a[i][j].get_d() + t->cur->a[i][j].get_n() * cur->a[i][j].get_d();
                    int den = cur->a[i][j].get_d() * t->cur->a[i][j].get_d();
                    cur->a[i][j].set(num,den);
                }else{
                    int num = cur->a[i][j].get_n() + t->cur->a[i][j].get_n();
                    num%=kr;
                    cur->a[i][j].set(num,cur->a[i][j].get_d());
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

//matrix* ma;

void matrix_input(matrix*& ma){
    char command[101];
    printf("Choose input f/h: ");
    scanf("%s",command);
    if(strcmp(command, "h") == 0){
        printf("Please enter matrix size (n x m): ");
        int n,m;
        scanf("%d %s %d", &n, command, &m);
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
            int t = fscanf(fr,"%d %d", &n, &m);
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
                matrix_input(ma);
            }
        }else{
            printf("File does not exist\n\n");
            matrix_input(ma);
        }
    }else{
        printf("Input unidentified\n\n");
        matrix_input(ma);
    }
}

void calculation(matrix*& ma){
    bool f=true;
    while(f){
        char command[101];
        printf("> ");
        scanf("%s", command);
        
        if (strcmp(command, "new") == 0) {
            delete ma;
            printf("\n");
            matrix_input(ma);
            ma->out();
            printf("\n");
        } else if (strcmp(command, "help") == 0){
            FILE* fr = fopen("guide.txt","r");
            if(fr=nullptr){
                char buffer[256];
                while (fgets(buffer, sizeof(buffer), fr)) {
                    cout << buffer;
                }
                cout<<endl<<endl<<endl;
                fclose(fr);
            }else cout<<"Unavailable\n\n";
        } else if (strcmp(command, "size") == 0){
            printf("n: %d  m: %d\nsaved:\nn: %d  m: %d\n\n",ma->get_n(),ma->get_m(),ma->curent()->get_n(),ma->curent()->get_m());
        } else if (strcmp(command, "matrix") == 0){
            ma->out();
            printf("\n");
        } else if (strcmp(command, "save") == 0){
            printf("Please enter filename: ");
            char filename[101];
            scanf("%s",filename);
            if(string(filename).compare(strlen(filename)-4,4,".txt")==0){
                ma->save(filename);
                printf("Saved\n\n");
            }else cout<<"Unavailable filename\n\n";
        } else if (strcmp(command, "rtm") == 0){
            ma->copy();
            ma->rtm();
            ma->out();
            printf("\n");
        } else if (strcmp(command, "det") == 0){
            if(ma->get_n()==ma->get_m()){
                printf("Determinant: %d\n\n", ma->det());
            }else printf("Possible only with square matrix\n\n");
        } else if (strcmp(command, "T") == 0){
            ma->copy();
            ma->T();
            ma->out();
            printf("\n");
        } else if (strcmp(command, "*matrix") == 0){
            matrix* t;
            char c='n';
            while(c!='y'){
                matrix_input(t);
                t->tout();
                printf("\nConfirm matrix B (y/n): ");
                cin>>c;
                if(c=='n'){
                    delete t;
                }
                printf("\n");
            }
            if(ma->get_m()==t->get_n()){
                if(ma->krat()==t->krat()){
                    ma->copy();
                    cout<<"Result";
                    ma->matrix_mult(t);
                    ma->tout();
                }else cout<<"Incomparable krat\n";
            }else cout<<"Incomparable size\n";
            printf("\n");
            delete t;
        } else if (strcmp(command, "+matrix") == 0){
            matrix* t;
            char c='n';
            while(c=='n'){
                matrix_input(t);
                t->tout();
                printf("\nConfirm matrix B (y/n): ");
                cin>>c;
                if(c=='n'){
                    delete t;
                }
                printf("\n");
            }
            if(ma->get_n()==t->get_n()&&ma->get_m()==t->get_m()){
                if(ma->krat()==t->krat()){
                    ma->copy();
                    cout<<"Result";
                    ma->matrix_add(t);
                    ma->tout();
                }else cout<<"Incomparable krat\n";
            }else cout<<"Incomparable size\n";
            printf("\n");
            delete t;
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
        } else if (strcmp(command, "/") == 0) {
            char t[8];
            int y;
            scanf("%s %d",t,&y);
            element m(t);
            if(m.get_n()==0){
                printf("Cannot devide by 0\n\n");
            }else if(y>=1&&y<=(ma->get_n())){
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
        } else if (strcmp(command, "-") == 0) {
            int x,y;
            char t[8];
            scanf("%d %d", &x, &y);
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
                }
            }
        } else if (strcmp(command, "swap") == 0) {
            int x,y;
            scanf("%d %d", &x, &y);
            if((x<1||x>(ma->get_n()))||(y<1||y>(ma->get_n()))){
                printf("Invalid line number\n\n");
            }else{
                ma->copy();
                ma->swap(x-1,y-1);
                printf("R%d <-> R%d",x ,y);
                ma->out();
                printf("\n");
            }
        } else if (strcmp(command, "krat") == 0) {
            printf("Krat: %d\n\n", ma->krat());
        } else if (strcmp(command, "undo") == 0) {
            ma->undo();
            ma->out();
            printf("\n");
        } else if (strcmp(command, "revert") == 0) {
            ma->revert();
            ma->out();
            printf("\n");
        } else if (strcmp(command, "change") == 0) {
            ma->copy();
            printf("Enter line number: ");
            int x;
            scanf("%d",&x);
            if(x<1||x>(ma->get_n())){
                printf("Invalid line number\n\n");
            }else{
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
            }
        } else if (strcmp(command, "chelem") == 0) {
            ma->copy();
            int i,j;
            printf("Enter line number: ");
            scanf("%d",&i);
            printf("Enter row number: ");
            scanf("%d",&j);
            if((i<1||i>(ma->get_n()))||(j<1||j>(ma->get_m()))){
                printf("Invalid coordinates\n\n");
            }else{
                printf("Enter element: ");
                char st[8];
                scanf("%s",st);
                element t(st);
                ma->elem(i-1, j-1)=t;
                ma->out();
                printf("\n");
            }
        } else if (strcmp(command, "n*") == 0) {
            int x;
            scanf("%d",&x);
            printf("For %d invert(*): %d\n\n",x ,ma->nm(x));
        } else if (strcmp(command, "n+") == 0) {
            int x;
            scanf("%d",&x);
            printf("For %d invert(+): %d\n\n",x ,ma->na(x));
        } else if (strcmp(command, "setk") == 0) {
            int x;
            scanf("%d",&x);
            if(x<-1||x==0){
                printf("Invalid krat option\n\n");
            }else{
                ma->krat()=x;
                printf("Krat set to: %d\n\n",x);
                if(!ma->chek_matrix()){
                    printf("Matrix and krat contradiction\n");
                    ma->out();
                    printf("\n");
                }
            }
        } else if (strcmp(command, "end") == 0) {
            f=false;
        } else {
            printf("bad input\n\n");
        }
    }
}

int main(){
    matrix* ma;
    matrix_input(ma);

    ma->out();
    printf("\n");
    
    calculation(ma);

    delete ma;
}
