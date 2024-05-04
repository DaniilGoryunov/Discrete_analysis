#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;

struct Data {
    char key[257];
    unsigned long long value;

    Data() {
        memset(key,255,257);
        value = 0;
    }
};

class TTree {
        enum TNodeColor {black, red};

        struct TTreeNode {
            TTreeNode* left;
            TTreeNode* right;
            TTreeNode* parent;
            TNodeColor node_color;
            Data node_data;

            TTreeNode(Data data, TTreeNode* pparent, TNodeColor color, TTreeNode* nil = 0) {
                parent = pparent;
                node_data = data;
                left = nil;
                right = nil;
                node_color = color;
            }
            
            ~TTreeNode() {
            }
        };
        
    
    public:
        TTreeNode* nil = 0;
        TTreeNode* root = 0;

        void Save(ofstream& output_file) {
            if (root == nil) {
                return;
            }

            int key_length = strlen(root->node_data.key);

            output_file.write((char*)(&key_length), sizeof(int));
            output_file.write(root->node_data.key, sizeof(char) * key_length);
            output_file.write((char*)(&root->node_data.value), sizeof(unsigned long long));

            if (root->left != nil) {
                SaveNodes(root->left, output_file);
            }

            if (root->right != nil) {
                SaveNodes(root->right, output_file);
            }

        }

        void SaveNodes(TTreeNode* current_node, ofstream& output_file) {
            if (current_node == nil) {
                return;
            }

            int key_length = strlen(current_node->node_data.key);

            output_file.write((char*)(&key_length), sizeof(int));
            output_file.write(current_node->node_data.key, sizeof(char) * key_length);
            output_file.write((char*)(&current_node->node_data.value), sizeof(unsigned long long));

            if (current_node->left != nil) {
                SaveNodes(current_node->left, output_file);
            }

            if (current_node->right != nil) {
                SaveNodes(current_node->right, output_file);
            }

        }

        void Load(ifstream& input_file) {
            int key_length;
            Data data;

            while (input_file.read((char*)&key_length, sizeof(int))) {
                char input_key[257];
                unsigned long long input_value;

                input_file.read(input_key, key_length);
                input_file.read((char*)&input_value, sizeof(unsigned long long));

                input_key[key_length]='\0';

                strcpy(data.key, input_key);
                data.value = input_value;

                Insert(data);
            }
        }

        TTree() {
            nil = new TTreeNode(Data(), 0, black);
            root = nil;
            root->left = nil;
            root->right = nil;      
            nil->parent = nil;
        }

        ~TTree() {
            DeleteMem(root);
            delete nil;
        }

        void DeleteMem(TTreeNode* z) {
            if(z->left != nil) {
                DeleteMem( z->left);
            }

            if(z->right != nil) {
                DeleteMem( z->right);
            }

            delete z;
        }

        void LeftRotate(TTreeNode* x) {
            if( x == nil ) {
                return;
            }
            
            TTreeNode* y = x->right;
            x->right = y->left;

            if( y->left != nil) {
                y->left->parent = x;
            }

            y->parent = x->parent;

            if(x->parent == nil) {
                root = y;
            }
            else if(x == x->parent->left) {
                x->parent->left = y;
            }
            else {
                x->parent->right = y;
            }

            y->left = x;
            x->parent = y;
        }
        
        void RightRotate(TTreeNode* x) {
            if( x == nil ) {
                return;
            }

            TTreeNode* y = x->left;
            x->left = y->right;

            if( y->right != nil) {
                y->right->parent = x;
            }

            y->parent = x->parent;

            if(x->parent == nil) {
                root = y;
            }
            else if(x == x->parent->right) {
                x->parent->right = y;
            }
            else {
                x->parent->left = y;
            }

            y->right = x;
            x->parent = y;
        }

        void FixUp(TTreeNode* z) {
            while((z != root) && z->parent->node_color == red) {
                if(z->parent == z->parent->parent->left) {         
                    TTreeNode* y = z->parent->parent->right;

                    // case 1: father and uncle both red
                    if(y->node_color == red) {
                        z->parent->node_color = black;
                        y->node_color = black;
                        z->parent->parent->node_color = red;
                        z = z->parent->parent;
                    }
                    // case 2: father red, uncle black, z is right son
                    else if (z == z->parent->right) {
                        z = z->parent;
                        LeftRotate(z);
                    }
                    // case 3: father red, uncle black, z is left son
                    else {
                        z->parent->node_color = black;
                        z->parent->parent->node_color = red;
                        RightRotate(z->parent->parent);
                    }
                }
                else {
                    TTreeNode* y = z->parent->parent->left;

                    if( y->node_color == red) {
                        z->parent->node_color = black;
                        y->node_color = black;
                        z->parent->parent->node_color = red;
                        z = z->parent->parent;
                    }
                    else if (z == z->parent->left) {
                        z = z->parent;
                        RightRotate(z);
                    }
                    else {
                        z->parent->node_color = black;
                        z->parent->parent->node_color = red;
                        LeftRotate(z->parent->parent);
                    }
                }
            }

            nil->node_color = black;
            root->node_color = black;
        }

        void Insert(Data data) {
            if(root == nil) {
                root = new TTreeNode(data, nil, black, nil);
                return;
            }

            TTreeNode* z = root;
            TTreeNode* y = nil;

            while(z != nil) {
                y = z;
                int cmp = strcmp(data.key, z->node_data.key);

                if(cmp < 0) {
                    z = z->left;
                }
                else {
                    z = z->right;
                }
            }

            int cmp = strcmp(data.key, y->node_data.key);

            if(cmp < 0) {
                y->left =  new TTreeNode(data, y, red, nil);
                FixUp(y->left);
            }
            else {
                y->right =  new TTreeNode(data, y, red, nil);
                FixUp(y->right);
            }
        };
        
        void Transplant(TTreeNode* u, TTreeNode* v) {
            if (u->parent == nil) {
                root = v;
            }
            else if( u == u->parent->left) {
                u->parent->left = v;
            }
            else {
                u->parent->right = v;
            }

            v->parent = u->parent;
        }

        TTreeNode* Find(Data data) {
            TTreeNode* z = root;

            while(z != nil) {
                int cmp = strcmp(data.key, z->node_data.key);

                if(cmp == 0) {
                    return z;
                }
                if(cmp < 0) {
                    z = z->left;
                }
                else {
                    z = z->right;
                }
            }

            return 0;
        }

        TTreeNode* Minimum(TTreeNode* z) {
            TTreeNode* y = z->parent;

            while(z != nil) {
                y = z;
                z = z->left;
            }

            return y;
        }

        void DeleteFixUp(TTreeNode* x) {
            while (x != root && x->node_color == black) {
                if(x == x->parent->left) {
                    TTreeNode* w = x->parent->right;

                    // case 1 x black, brother red -> come to case 2/3/4
                    if (w->node_color == red) {
                        w->node_color = black;
                        x->parent->node_color = red;
                        LeftRotate(x->parent);
                        w = x->parent->right;
                    }

                    // case 2 x black, brother black, and both of his son are black
                    if (w->left->node_color == black && w->right->node_color == black) {
                        w->node_color = red;
                        x = x->parent;
                    }

                    // case 3 x black, brother black, and right son of brother is black, left son is red
                    else if (w->right->node_color == black) {
                        w->left->node_color = black;
                        w->node_color = red;
                        RightRotate(w);
                        w = x->parent->right;
                    }

                    // case 4 x is black, brother black, and right son of brother is red
                    else {
                        w->node_color = x->parent->node_color;
                        x->parent->node_color = black;
                        w->right->node_color = black;
                        LeftRotate(x->parent);
                        x = root;
                    }
                }
                else {
                    TTreeNode* w = x->parent->left;

                    if (w->node_color == red) {
                        w->node_color = black;
                        x->parent->node_color = red;
                        RightRotate(x->parent);
                        w = x->parent->left;
                    }
                    if (w->right->node_color == black && w->left->node_color == black) {
                        w->node_color = red;
                        x = x->parent;
                    }
                    else if (w->left->node_color == black) {
                        w->right->node_color = black;
                        w->node_color = red;
                        LeftRotate(w);
                        w = x->parent->left;
                    }
                    else {
                        w->node_color = x->parent->node_color;
                        x->parent->node_color = black;
                        w->left->node_color = black;
                        RightRotate(x->parent);
                        x = root;
                    }
                }
            }

            x->node_color = black;
        }
        
        void Delete(Data data) {
            TTreeNode* z = Find(data);

            if(z == 0) {
                return;
            }
            TTreeNode* y = z;
            TTreeNode* x = nil;
            int y_original_color = y->node_color;

            if(z->left == nil) {
                x = z->right;
                Transplant(z, z->right);
            }
            else if(z->right == nil) {
                x = z->left;
                Transplant(z, z->left);
            }
            else {
                y = Minimum(z->right);
                y_original_color = y->node_color;
                x = y->right;
                if(y->parent == z) {
                    x->parent = y;
                }
                else {
                    Transplant(y, y->right);
                    y->right = z->right;
                    y->right->parent = y;
                }
                Transplant(z, y);
                y->left = z->left;
                y->left->parent = y;
                y->node_color = z->node_color;
            }

            delete z;

            if(y_original_color == black) {
                DeleteFixUp(x);
            }
        }
    };

int main() {
    ios:: sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    TTree* RBtree = new TTree();
    ofstream out_dictionary;
    ifstream in_dictionary;
    char command[257];

    while (cin >> command) {
        Data data;

        switch(command[0]){
            case '+':
                cin >> data.key;

                for (int i = 0; i < 257; ++i) {
                    data.key[i] = tolower(data.key[i]);
                }

                cin >> data.value;

                if(RBtree->Find(data) == 0 ){
                    RBtree->Insert(data);

                    cout << "OK" << endl;
                }
                else {
                    cout << "Exist" << endl;
                }

                break;
            case '-':
                cin >> data.key;

                for (int i = 0; i < 257; ++i) {
                    data.key[i] = tolower(data.key[i]);
                }

                if(RBtree->Find(data) != 0 ){
                    RBtree->Delete(data);

                    cout << "OK" << endl;
                }
                else {
                    cout << "NoSuchWord" << endl;
                }

                break;
            case '!':
                char comm_file[5];

                cin >> comm_file;

                if(strcmp(comm_file, "Save") == 0) {
                    char path[256];

                    cin >> path;

                    out_dictionary.open(path, ios_base::binary);
                    RBtree->Save(out_dictionary);
                    out_dictionary.close();

                    cout << "OK" << endl;
                }
                else if (strcmp(comm_file, "Load") == 0) {
                    char path[256];

                    cin >> path;

                    in_dictionary.open(path, ios_base::binary);
                    RBtree = new TTree();
                    RBtree->Load(in_dictionary);
                    in_dictionary.close();

                    cout << "OK" << endl;
                }

                break;

            default:
                strcpy(data.key, command);

                for (int i = 0; i < 257; ++i) {
                    data.key[i] = tolower(data.key[i]);
                }

                auto node = RBtree->Find(data);

                if(node != 0 ){
                    cout << "OK: " << node->node_data.value << endl;
                }
                else {
                    cout << "NoSuchWord" << endl;
                }
                break;
        }
    }

    return 0;
}
