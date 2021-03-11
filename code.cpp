#include "bits/stdc++.h"
using namespace std;

class Node{
    private:
    public:
    char data;
    int cost;
    Node *left;
    Node *right;
    Node(char val, int freq, Node *lef=nullptr, Node *rit=nullptr){
        data = val;
        cost = freq;
        left = lef;
        right = rit;
    }
};

struct Comp {
    bool operator() ( const Node *n1, const Node *n2){
		return n1->cost > n2->cost;
	}
};

class Huffman
{
private:
    Node *decoder_tree;
    unordered_map<char,string> encoder;
    unordered_map<string,char> decoder;
    unordered_map<char,int> frequency;
    
    priority_queue<Node*, vector<Node*>, ::Comp> pq;


public:
	void preorder(Node *root,string key=""){
		if(root==nullptr)
			return;
		if(root->left==nullptr and root->right==nullptr){
			decoder[key]=root->data;
			encoder[root->data]=key;
            // cout << root->data << " "<< root->cost <<" "<<endl;		
		}
		if(root->left){
			preorder(root->left,key+"1");
		}
        if(root->right){
			preorder(root->right,key+"0");
		}	
	}

    void set_freq(string &s){
        for (char c:s)
            frequency[c]++;
        for (auto &el:frequency){
            
            pq.push( (new Node(el.first, el.second)) );
        }
        while(pq.size()>1){
			Node *one = pq.top();
			pq.pop();
			Node *two = pq.top();
			pq.pop();
			pq.push((new Node('\0',one->cost+two->cost,two, one)));		
		}
		Node* tree = pq.top();
		pq.pop();
        preorder(tree);
        this->decoder_tree = tree;
		/// parse tree to encoder and decoder...
    }

    unordered_map<string,char> get_decoder_map(){
        return decoder;
    }

    Node* get_decoder_tree(){
        return decoder_tree;
    }

    string encode_it(string s){
        set_freq(s);
        string encoded = "";
        for(auto i:s){
            encoded += encoder[i];
        }
        return encoded;
    }

    string decode_it(string cypher){
        Node *tmp=decoder_tree;
        string decrepted = "";
        for (int i: cypher){
            
            if(i-'0'){
                tmp = tmp->left;
            }
            else{
                tmp = tmp->right;
            }
            if(!tmp->left and !tmp->right){
                decrepted += tmp->data;
                tmp = decoder_tree;
            }
        }
        return decrepted;
    }

    void print(){
        cout<<"encoder : \n";
        for (auto cd:encoder){
            cout<<cd.first<<" "<<cd.second<<endl;
        }
        cout<<"decoder : \n";
        for (auto dc:decoder){
            cout<<dc.first<<" "<<dc.second<<endl;
        }
    }

};

int main(){
	string s = "pradeep";
	Huffman coder;
	string txt = coder.encode_it(s);
    string decoded = coder.decode_it(txt);
    cout<<txt<<endl;
    cout<<decoded<<endl;
    int bits = s.size()*8;
    int optimal = txt.size();
    cout<<bits<<" bits \n";
    cout<<optimal<<" used bits \n";
	// coder.print();
	
}

