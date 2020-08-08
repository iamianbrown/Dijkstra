#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <list>
using namespace std;
float prob();
int randWeight();
template <class T>
bool vecContains(vector<T> vec, T elem);
const float DENSITY = 0.19;

class Node{
public:
    explicit Node(int i):index(i), distToThis(-1){}
    void addEdge(Node* node){adjacent.push_back(node);};
    list<Node*> getAdjacent(){return adjacent;}
    int getIndex() const{return index;}
    list<Node*> getPathTo(){return pathToThis;}
    void setPathTo(list<Node*> path);
    int getDist(){return distToThis;}
    void setDist(int dist){distToThis = dist;}
    void addToPath(Node* node);
private:
    int index; list<Node*> adjacent; list<Node*> pathToThis; int distToThis;//nEdges number of edges connecting this to another node
};
void Node::setPathTo(list<Node*> path){
    pathToThis = path;
}
void Node::addToPath(Node* node){
    pathToThis.push_back(node);
}
bool compareDist(Node* a, Node* b);
vector<Node*> listToVec(list<Node*> lst);
bool listContains(list<Node*> lst, Node* elem);

class Edge{
public:
    explicit Edge(Node *a = nullptr, Node* b = nullptr, int wt = 0);
    int getWeight(){return wt;}
    Node* getNode1(){return node1;};
    Node* getNode2(){return node2;};
private:
    int wt; Node* node1; Node* node2;
};
Edge::Edge(Node* a, Node* b, int w) : node1(a), node2(b), wt(w){
    a->addEdge(b); b->addEdge(a); //increase the number of edges a and b recognize
}

class Graph{
public:
    explicit Graph(int n = 1);
    void addEdge(Node* a, Node* b, int wt);//creates edge between a and b with weight w
    int getSize(){return static_cast<int>(nodes.size());}
    int getEdgeWeight(Node* a, Node* b){
        Edge* edge = getEdgePointer(a, b);
        if(edge != nullptr){
            return edge->getWeight();
        }
        return NULL;
    }
    void addEdge(int a, int b, int wt);
    //returns a pointer to an edge connecting a and b. returns nullptr if no edge connects a and b
    Edge* getEdgePointer(Node* a, Node* b){
        for(auto & edge : edges){
            if((edge->getNode1() == a && edge->getNode2() == b)||(edge->getNode2() == a && edge->getNode1() == b)){
                return edge;
            }
        }
        return nullptr;
    }
    vector<Node*> getNodes(){return nodes;}
    bool nodesConnected(Node* a, Node* b);
    list<Node*> dijkstra(Node* a, Node* b);
private:
    vector<Node*> nodes; vector<Edge*> edges;
};
ostream& operator<< (ostream& out, list<Node*> nodes){
    for(list<Node*>::iterator it = nodes.begin(); it != nodes.end(); it++){
        out << (*it)->getIndex() << ' ' << endl;
    }
    return out;
}
list<Node*> Graph::dijkstra(Node* a, Node* b){
    Node* temp; Node* previous = a; int distToPrevious = 0, edgeWeight;
    a->setDist(0);
    list<Node*> open, closed, adj;
    if(!nodesConnected(a, b))
        return closed;
    previous = a;
    a->setDist(0);
    while(!listContains(closed, b)){
        adj = previous->getAdjacent();
        /*for(list<Node*>::iterator node = adj.begin(); node != adj.end(); node++){
            edgeWeight = getEdgeWeight(*node, previous);
            if((*node)->getDist() == -1 || (*node)->getDist() > distToPrevious + edgeWeight){
                (*node)->setDist(distToPrevious + edgeWeight);
            }
        }*/
        //open.sort(compareDist);
        for(list<Node*>::iterator node = adj.begin(); node != adj.end(); node++){
            if(!(listContains(open, *node)||listContains(closed, *node))){
                open.push_back(*node);
                edgeWeight = getEdgeWeight(*node, previous);
                //if path thru current node is shorter than old path, update
                if((*node)->getDist() == -1 || (*node)->getDist() > distToPrevious + edgeWeight){
                    //set the path of the node in focus (*node) to the path to the previous node plus the dist between the previous node and the node in focus
                    (*node)->setPathTo(previous->getPathTo());
                    (*node)->addToPath(*node);
                    (*node)->setDist(distToPrevious + edgeWeight);
                    cout << closed << endl;
                }
            }
        }
        closed.push_back(previous);
        open.erase(open.begin());
        temp = previous;
        previous = *open.begin();
        adj.clear();
        distToPrevious += getEdgeWeight(temp, previous);
    }
    for(int i = 0; i < nodes.size(); i++){
        nodes.at(i)->setDist(-1);
    }
    return b->getPathTo();
}
bool Graph::nodesConnected(Node* a, Node* b){
    list<Node*> open = a -> getAdjacent(), closed, adj;
    Node* current;
    closed.push_back(a);
    while(open.size()!= 0){
        current = *open.begin();
        adj = current->getAdjacent();
        open.erase(open.begin());
        closed.push_back(current);
        for(list<Node*>::iterator it; it != adj.end(); it++){
            if(!(listContains(open, *it) || listContains(closed, *it)))
                open.push_back(*it);
        }
        if(listContains(closed, b))
            return true;
    }
    return false;
}
Graph::Graph(int n){
    for(int index = 0; index < n; index++){
        nodes.push_back(nullptr);
        nodes.at(index) = new Node(index);
    }
}
void Graph::addEdge(Node *a, Node *b, int wt) {
    Edge* edg;
    edg = new Edge(a, b, wt);
    edges.push_back(edg);
}
void Graph::addEdge(int a, int b, int wt) {
    Edge* edg;
    edg = new Edge(nodes.at(a), nodes.at(b), wt);
    edges.push_back(edg);
}
Graph* randGraph(int n);

ostream& operator<< (ostream& out, Node* n){
    out << n->getIndex();
    return out;
}
ostream& operator<< (ostream& out, Graph* g){
    vector<Node*> nodes = g->getNodes(), adj;
    Node* current; //the current node in focus
    Edge* edge;       //pointer to edge to be printed
    out << endl;
    for(int i = 0; i < g->getSize(); i++){
        current = nodes.at(i);
        adj = listToVec(current->getAdjacent());
        out << "Node " << current << ":" << endl;
        vector<Edge*> used; //stores edge values which have already been printed
        for(int j = 0; j < adj.size(); j++){
            edge = g->getEdgePointer(nodes.at(i), adj.at(j));
            if(edge != nullptr && find(used.begin(), used.end(), edge) == used.end())
                    out << "\t" << current << " ---> " << adj.at(j) << " (" << edge->getWeight() << ")" << endl;
            used.push_back(edge);
        }
    }
    return out;
}

int main() {
    //srand(0);
    Graph* graph = new Graph(8);
    //vector<Edge*> edges = graph->getEdges();
    vector<Node*> nodes = graph->getNodes();
    graph->addEdge(0, 6, 7);
    graph->addEdge(1, 2, 3);
    graph->addEdge(1, 6, 4);
    graph->addEdge(2, 3, 8);
    graph->addEdge(2, 5, 9);
    graph->addEdge(2, 7, 1);
    graph->addEdge(4, 5, 5);
    //graph->addEdge(nodes.at(0),nodes.at(1),7);
    //graph->addEdge(nodes.at(0),nodes.at(2),3);
    cout << graph << endl;
    //cout << graph->nodesConnected(nodes.at(0), nodes.at(7)) << endl;
    //cout << graph->nodesConnected(nodes.at(0), nodes.at(1));
    cout << graph->dijkstra(nodes.at(0), nodes.at(7));
    return 0;
}

bool edgeExists(Node* a, Node* b){
    vector<Node*> adj = listToVec(a->getAdjacent());
    return (find(adj.begin(), adj.end(), b) != adj.end());
}
float prob(){
    return (rand() % 100) / 100.0;
}
int randWeight(){
    return (rand() % 10) + 1;
}
Graph* randGraph(int n){
    Graph* g = new Graph(n);
    vector<Node*> nodes = g->getNodes();
    for(int i = 0; i < n; i++){
        for(int j = 0; j < i; j++){
            if(prob() <= DENSITY){
                g->addEdge(nodes.at(i),nodes.at(j), randWeight());
            }
        }
    }
    return g;
}
template <class T>
bool vecContains(vector<T> vec, T elem){
    return(find(vec.begin(), vec.end(), elem) != vec.end());
}
bool listContains(list<Node*> lst, Node* elem){
    for(list<Node*>::iterator it = lst.begin(); it != lst.end(); it++){
        if(elem == *it){
            return true;
        }
    }
    return false;
}
bool compareDist(Node* a, Node* b){
    return(a->getDist() < b->getDist());
}
vector<Node*> listToVec(list<Node*> lst){
    vector<Node*> vec;
    for(list<Node*>::iterator it = lst.begin(); it != lst.end(); it++){
        vec.push_back(*it);
    }
    return vec;
}
