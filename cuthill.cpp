#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<map>
#include<set>
#include<deque>
#include<typeinfo>

using namespace std;

typedef vector<float> Vector;
typedef vector<int>Vector_int;
typedef vector<Vector> Matrix;

class Excentricite{
    private:
        int n;  //dimension de la matrice
        Matrix A;
        int max_lengh;

    public:
        Matrix B;
        map<int,vector<int>>graph;
        Excentricite();
        ~Excentricite();
        void displayMatrix(Matrix M);
        void initializeData();
        void profil();
        void displayGraph();
        Matrix get_matrix();
        void set_maxL(int noeud);
        int min_path(int noeud,int dest);
        int research_max(Vector_int list);
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
    this->max_lengh = sommet;
}

void Excentricite::displayMatrix(Matrix M){
    for(int i=0;i<this->n;i++){
        for(int j=0;j<this->n;j++){
            cout<<M[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

void Excentricite::initializeData(){
    ifstream file("data1.txt");      //Ouvre le fichier data.txt

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

        cout<<"Voici la matrice donnee :"<<endl;

        file.close();
    }

    else{
        cout<<"Erreur d'ouverture du fichier"<<endl;
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
        cout<<"Les voisins du noeud "<<i<<" : ";

        for(int n:neighbor){
            cout<<n<<" ";
        }
        cout<<endl;
    }
}

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

int Excentricite::research_max(Vector_int list){
    int max = list[0];
    int temp;

    for(int i=1; i<int(list.size());i++) {
        temp = list[i];
        if(temp>max) {
            max = temp; 
        }
    }

    return max;
}

void Excentricite::generateDOT(Matrix adjacencyMatrix,const string& dotfilename){
    ofstream dotFile(dotfilename);
    if(!dotFile){
        cerr<<"Error opening file :"<<dotfilename<<endl;
        return;
    }

    dotFile << "graph G {\n";
    const int n = adjacencyMatrix.size();
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (adjacencyMatrix[i][j] != 0) {
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

    for(int sommet=1;sommet<=int(a.get_matrix().size());sommet++){
        Vector_int list;
        int max;

        cout<<"\nNoeud "<<sommet<<endl;
        for(int i=1;i<=int(a.graph.size());i++){
            cout<<"\nLa distance entre "<<sommet<<"  et "<< i << " : "<< a.min_path(sommet,i);
            //cout<<typeid(a.min_path(sommet,i)).name();
            list.push_back(a.min_path(sommet,i));
        }

        cout<<endl;
        cout<<"\nLa liste des distances obtenue est : [";
        for(auto l:list){
            cout<<l<<" ";
            max = a.research_max(list);
        }
        cout<<"]"<<endl;
        cout<<"\nL excentricite du sommet "<<sommet<<" est "<<max<<endl;
        cout<<"------------------------------";
    }
    cout<<endl;
    
    a.generateDOT(a.get_matrix(),"test.dot");
    a.renderGraph("test.dot","output.png");

    return(0);
}
