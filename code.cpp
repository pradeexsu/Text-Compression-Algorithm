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


class Huffman{
private:
    Node *decoder_tree;
    unordered_map<char,string> encoder;
    int frequency[1<<7];
    priority_queue<Node*, vector<Node*>, ::Comp> pq;
    int edge;

public:
    int *get_freq_arr(){
        return frequency;
    }
	void preorder(Node *root,string key=""){
		if(root==nullptr)
			return;
		if(root->left==nullptr and root->right==nullptr){
			encoder[root->data]=key;
		}
		if(root->left){
			preorder(root->left,key+"1");
		}
        if(root->right){
			preorder(root->right,key+"0");
		}	
	}

    void set_freq(string &s){
        memset(frequency,-1,sizeof(frequency));
        for (char c:s){
            if(frequency[c]==-1)
                frequency[c]=0;
            frequency[c]++;
        }
        for (int i=0; i<(1<<7); i++){
            if(frequency[i]>0)
                pq.push( (new Node((char)i, frequency[i])) );
        }
        while(pq.size()>1){
			Node *one = pq.top();
			pq.pop();
			Node *two = pq.top();
			pq.pop();
			pq.push((new Node('\0', one->cost+two->cost, two, one)));		
		}
		Node* tree = pq.top();
        preorder(tree);
        this->decoder_tree = tree;
		/// parse tree to encoder and decoder...
    }

    Node* get_decoder_tree(){
        return decoder_tree;
    }

    string encode_it(string s){
        set_freq(s);
        string encoded = "";
        for(auto i:s){
            encoded.append(encoder[i]);
        }
        edge = 8- encoded.size()%8 ;
        
        return encoded;
    }

    string decode_it(string binary_string){
        Node *tmp=decoder_tree;
        binary_string.erase(binary_string.end()-edge, binary_string.end());
        string decrepted = "";
        for (int i: binary_string){
            
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
    }

};


string read_from_file(string file_name){
    string s="";
    ifstream input(file_name, ios::binary|ios::in);
    char c;
    while((c=input.get())!=EOF){
        s+=c;
    }
    input.close();
    return s;
}


void write_bits_to_file(string file_name, string &txt){
    unsigned byte=0;
    ofstream outs( file_name.c_str(), ios::binary|ios::out);
    size_t i;
    bool term = false;
    for (i=0; i<txt.size(); i+=8){
        if(i+8>=txt.size()){
            term=true;
            break;
        }
        char byte = (char)stoi( txt.substr(i,8),nullptr, 2 );
        outs.put(byte);
    }
    int edge_bits = 0;
    if(term){
        string terminal = txt.substr(i,string::npos);
        edge_bits = 8 - terminal.size();
        while(terminal.size()<8)
            terminal +='0';
        unsigned char edge = stoi(terminal , nullptr, 2);
        outs.put(edge);
    }
    outs.close();
}


string load_bits_from_file(string file_name){
    ifstream input(file_name, ios::binary|ios::in);
    string bit_stream="";
    char c;
    while((c=input.get())!=EOF){
        bit_stream.append(bitset<8>(c).to_string());
    }
    return bit_stream;
}



void test_case(){
    Huffman engin;
    string s ="welcome to the cpp world";
    string file_name = "data.bin";

    string encoded = engin.encode_it(s);

    write_bits_to_file(file_name, encoded);
    
    string loaded_bits = load_bits_from_file(file_name);

    string decoded_ouput = engin.decode_it(loaded_bits);
    cout<< decoded_ouput << endl ;
}

int main(){
    test_case();
    return 0;
}


// testing on pending

struct MetaData{
    int const size = 1<<7;

    int freq[1<<7];
    int edge;
    priority_queue<Node*,vector<Node*>,::Comp> *pq;
    MetaData(){
        memset(freq,-1, sizeof(freq));
        pq = nullptr;
    }
    MetaData(int freq_[],int edge_){
        copy(freq_, freq_+(size),freq);
        edge = edge_;
        pq = nullptr;
    }
    void build_tree(){
        for (int i=0;i< size; i++){
            if(freq[i]>0)
                pq->push( ( new Node((char)i, freq[i]) ) );
        }
        while (pq->size()>1){
            
            Node* one = pq->top();
            pq->pop();
            Node* two = pq->top();
            pq->pop();
            pq->push((new Node('\0', one->cost+two->cost, two, one)));            
        }
    }

    string decode(string binary_string){
        string decoded_string="";
        Node *pointer = pq->top();
        for (auto c: binary_string){
            if(c-'0'){
                pointer = pointer->left;
            }
            else{
                pointer = pointer->right;
            }
            if(pointer->right==nullptr and pointer->left == nullptr){
                decoded_string += pointer->data;
                pointer = pq->top();
            }
        }
        return decoded_string;
    }
};

