#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<map>
#include<set>
#include<deque>
#include<algorithm>

using namespace std;

typedef vector<float> Vector;
typedef vector<int>Vector_int;
typedef vector<Vector> Matrix;

class Excentricite{
    private:
        int n;          //dimension de la matrice
        Matrix A;
        Vector D;       //pour stocker les éléments diagonaux de la matrice L
        Vector APi;     //Valeur de la matrice
        Vector li;      //nombre de valeur avant la valeur du diagonal
        Vector pi;      //i - li
        Vector nDiag;
        int max_length;
    public:
        map<int,vector<int>>graph;
        map<int,int>length_neighbor;
        Excentricite();
        ~Excentricite();
        void displayMatrix(Matrix M);
        void initializeData();
        void profil();
        void displayGraph();
        Matrix get_matrix();
        void set_maxL(int noeud);
        int min_path(int noeud,int dest);
        Vector get_APi();
        Vector get_solution();
        Vector get_diag();
        Vector get_num_diag();
        Vector get_li();
        Vector get_pi();
        int stockage(Matrix A);
        int get_first_node();
        Matrix numerotation(int first_node);
        void generateDOT(Matrix adjMat,const string& dotfilename);
        void renderGraph(const string& dotFilename,const string& outputFilename);
};

Excentricite::Excentricite(){
    this->initializeData();
}

Excentricite::~Excentricite(){
    cout<<endl;
}

Matrix Excentricite::get_matrix(){
    return this->A;
}

void Excentricite::set_maxL(int sommet) {
    this->max_length = sommet;
}

Vector Excentricite::get_diag(){
    return this->D;
}

Vector Excentricite::get_APi(){
    return this->APi;
}

Vector Excentricite::get_num_diag(){
    return this->nDiag;
}

Vector Excentricite::get_li(){
    return this->li;
}

Vector Excentricite::get_pi(){
    return this->pi;
}

void Excentricite::displayMatrix(Matrix M){
    for(int i=0;i<int(M.size());i++){
        for(int j=0;j<int(M[i].size());j++){
            cout<<M[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

void Excentricite::initializeData(){
    ifstream file("data2.txt");      //Ouvre le fichier data.txt

    if(file){
        file>>this->n;

        for(int i=0;i<this->n;i++){
            Vector temp;
            for(int j=0;j<this->n;j++){
                float element;
                file>>element;
                temp.push_back(element);
            }
            A.push_back(temp);
        }

        cout<<"The matrix :"<<endl;

        file.close();
    }

    else{
        cout<<"Error opening file"<<endl;
    }
}

void Excentricite::profil(){
    for(int i=0;i<this->n;i++){
        for(int j=0;j<this->n;j++){
            if(A[i][j] != 0 && i != j){
                this->graph[i+1].push_back(j+1);
            }
        }
    }
}

void Excentricite::displayGraph(){
    for(int i=1;i<=this->n;i++){
        vector<int>& neighbor = this->graph[i];
        cout<<"Node s neighbors "<<i<<" : ";

        for(int n:neighbor){
            cout<<n<<" ";
        }
        cout<<endl;
        length_neighbor.insert(make_pair(i,neighbor.size()));
        // cout<<"Length of "<<i<<" : "<<neighbor.size()<<endl;
    }
}

int Excentricite::stockage(Matrix A){
    int count = 1;
    int npf = 0;
    for(int i=0;i<this->n;i++){
        int indice = 0;
        bool passage = false;
        for(int j=0;j<i+1;j++){ 
            auto element = this->A[i][j]; 
            //Numerotation du diagonal 
            if(i==j){
                this->nDiag.push_back(count);
            }

            if(element != 0 || A[i][i] == 0){
                indice = j;
                passage = true;
                this->APi.push_back(element);
                count++;
            }
            else{
                if(j>indice && passage){
                    this->APi.push_back(element);
                    count++;
                }
            }
        }

        //Calcul de l[i] = nDiag[i] - nDiag[i-1] - 1
        this->li.push_back(this->nDiag[i] - this->nDiag[i-1] - 1);

        //Calcul de p[i] = i - l[i]
        this->pi.push_back(i + 1 - this->li[i]);
    }

    for(auto i:li){
        npf += i + 1;
    }
    

    return npf;
}

//BFS
int Excentricite::min_path(int noeud,int dest){
    deque<map<int,vector<int>>>file;
    set<int> visited;
    map<int,vector<int>> t;
    t[noeud].push_back(noeud);
    file.push_back(t);
    //Visiter le noeud
    visited.insert(noeud);
    vector<int>N;

    while(!file.empty()){
        map<int,vector<int>> queue = file.front();    //Accéder au premier élément
        file.pop_front();
        vector<int>path;
        int noeud = 0;
        for(auto s:queue){
            noeud = s.first;
            path = s.second;
        }
        
        if(noeud==dest){
            return path.size()-1;
        }
        vector<int>neighbors = this->graph[noeud];

        for(auto neighbor:neighbors){
            if(visited.insert(neighbor).second){
                map<int,vector<int>> next;
                next[neighbor].push_back(neighbor);
                for(auto pt:path){
                    next[neighbor].push_back(pt);
                }
                file.push_back(next);
            }
        }
    }

    return 0;
}

int Excentricite::get_first_node(){
    int first_node = 0;
    int temp_ecc = -1;
    int temp_neigh = 999;

    for(auto g:this->graph){
        vector<int> list;
        int neigh_size = int(g.second.size());

        int max;
        cout<<"\nNode "<<g.first;
       
        for(int i=1;i<=int(this->graph.size());i++){
            list.push_back(min_path(g.first,i));
        }
        
        max = *max_element(list.begin(),list.end());
        cout<<"\nThe eccentricity of each node "<<g.first<<" is "<<max<<endl;
        cout<<"------------------------------"<<endl;
        //cout<<endl<<max<<" "<<neigh_size<<" "<<g.first<<endl;
        if(temp_ecc <= max){
            if(temp_neigh>neigh_size){
                temp_ecc = max;
                temp_neigh = neigh_size;
                first_node = g.first;
            }
        }
        //cout<<temp_ecc<<" "<<temp_neigh<<" "<<first_node<<endl;
    }

    cout<<"Eccentricity max : "<<temp_ecc<<endl;
    cout<<"Length  : "<<temp_neigh<<endl;
    cout<<"Node : "<<first_node<<endl;
    
    return first_node;
}

Matrix Excentricite::numerotation(int first_node){
    deque<int>file;
    set<int> visited;
    file.push_back(first_node);
    //Visiter le noeud
    visited.insert(first_node);
    vector<int>N;
    int counter = 1;
    map<int,map<int,int>> numerotation;
    map<int,int> cuthill;
    cuthill.insert(make_pair(counter,(this->n+1)-counter));
    numerotation.insert(make_pair(first_node,cuthill));

    while(!file.empty()){
        int node = file.front();    //Accéder au premier élément
        file.pop_front();

        vector<int>neighbors = this->graph[node];
        

        for(int i=0;i<int(neighbors.size());i++){
            int nb_neighbors_i = int(this->graph[neighbors[i]].size());
            for(int j=i+1;j<int(neighbors.size());j++){
                int nb_neighbors_j = int(this->graph[neighbors[j]].size());
                if(nb_neighbors_i > nb_neighbors_j){
                    swap(neighbors[i],neighbors[j]);
                    swap(nb_neighbors_i,nb_neighbors_j);
                }
                else if(nb_neighbors_i == nb_neighbors_j){
                    if(neighbors[i]  > neighbors[j]){
                        swap(neighbors[i],neighbors[j]);
                    }
                }
            }
        }

        for(auto neighbor:neighbors){
            if(visited.insert(neighbor).second){
                file.push_back(neighbor);
                counter++;
                map<int,int> cuthill;
                cuthill.insert(make_pair(counter,(this->n+1)-counter));
                numerotation.insert(make_pair(neighbor,cuthill));
            }
        }
    }

    cout<<"\nNumerotation : "<<endl;
    cout<<"i     CM     CMI"<<endl;
    cout<<"----------------"<<endl;
    for(auto key:numerotation){
        int index = key.first;
        cout<<index<<"  ->  ";
        for(auto m:key.second){
            int cmk = m.first;
            int cmki = m.second;
            cout<<cmk<<"  ->  "<<cmki;
        }
        cout<<endl;
    }

    int numNodes = this->n;
    vector<int> cmki(numNodes);
    for(const auto& entry : numerotation){
        int node = entry.first;
        int cmk = entry.second.begin()->first;
        int cmki_val = entry.second.begin()->second;
        cmki[cmk - 1] = cmki_val; // -1 pour l'indexation 0-based
    }

    // Création de Mat_prim à partir de Mat en utilisant cmki
    Matrix Mat_prim(this->n, Vector (this->n, 0));
    for(int i = 0; i < this->n; i++){
        for(int j=0;j<this->n;j++){
            Mat_prim[i][j] = this->A[cmki[i] - 1][cmki[j] - 1]; // -1 pour l'indexation 0-based
        }
    }

    // Afficher Mat_prim
    cout << "\nMat_prim:" << endl;
    for(const auto& row : Mat_prim){
        for(const auto& val : row){
            cout << val << " ";
    }
    cout << endl;
}


    // int numNodes = this->n;
    // Matrix P;
    // P.resize(numNodes,Vector(numNodes, 0));

    // for(const auto& entry : numerotation){
    //     int node = entry.first;
    //     if (!entry.second.empty()){
    //         int newPosition = entry.second.begin()->second;
    //         P[node][newPosition] = 1;
    //     }
    //     else {
    //         cout<<"The map is empty"<<endl;
    //     }
    // }


    // // Print the permutation matrix P
    // cout << "\nPermutation Matrix P:" << endl;
    // for(const auto& row : P){
    //     for(const auto& val : row){
    //         cout << val << " ";
    //     }
    //     cout << endl;
    // }
    return Mat_prim;
}

void Excentricite::generateDOT(Matrix adjacencyMatrix,const string& dotfilename){
    ofstream dotFile(dotfilename);
    if(!dotFile){
        cerr<<"Error opening file :"<<dotfilename<<endl;
        return;
    }

    dotFile << "graph G {\n";
    const int n = adjacencyMatrix.size();
    for(int i=0;i<n;i++) {
        for(int j=i+1;j<n;j++) {
            if(adjacencyMatrix[i][j] != 0) {
                dotFile << i << " -- " << j << " [label=\"" << adjacencyMatrix[i][j] << "\"];\n";
            }
        }
    }
    dotFile << "}\n";

    dotFile.close();
}

//Fonction qui rend le fichier dot en image
void Excentricite::renderGraph(const string& dotFilename,const string& outputFilename){
    string command = "dot -Tpng " + dotFilename + " -o " + outputFilename;
    int result = system(command.c_str());
    if(result != 0){
        cerr<<"\nError rendering graph"<<endl;
    }
}




int main(){
    Excentricite a;
    a.displayMatrix(a.get_matrix());
    a.profil();
    a.displayGraph();
    cout<<endl;
    int npf = a.stockage(a.get_matrix());
    cout<<"=> NPF = "<<npf<<endl;
    int first_node = a.get_first_node();
    cout<<"--------------------"<<endl;
    Matrix B = a.numerotation(first_node);
    //a.displayMatrix(B);
    int npf1 = a.stockage(B);
    cout<<"=> NPF = "<<npf1<<endl;
    a.generateDOT(a.get_matrix(),"test.dot");
    a.renderGraph("test.dot","output.png");

    return(0);
}