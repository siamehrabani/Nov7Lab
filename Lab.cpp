// Lab9: Expression Tree
// Siavash Mehrabani, Jovan Ssemakula, Dibi Barua

#include <iostream>
using namespace std;

struct Node {
    char data;
    Node* left;
    Node* right;
    Node(char val) : data(val), left(nullptr), right(nullptr) {}
};

struct VarValPair {
    char var;
    int v;
};

bool isOp(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

bool isOperand(char c) {
    return ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

Node* buildExpTree(const string& exp) {
    stack<Node*> stack;
    for (char c : exp) {
        if (!isOperand(c))
            stack.push(new Node(c));
        else {
            Node* rightOp = stack.top();
            stack.pop();
            Node* leftOp = stack.top();
            stack.pop();
            Node* newNode = new Node(c);
            newNode->left = leftOp;
            newNode->right = rightOp;
            stack.push(newNode);
        }
    }
    return stack.top();
}

int evalWithvars(Node* root, const VarValPair vars[], int size) {
    if (isOperand(root->data)) {
        if (root->data >= '0' && root->data <= '9')
            return root->data - '0';
    }
    for (int i = 0; i < size; i++) {
        if (vars[i].var == root->data) {
            return vars[i].v;
        }
    }
    int lValue = evalWithvars(root->left, vars, size);
    int rValue = evalWithvars(root->right, vars, size);
    switch (root->data) {
        case '+': return lValue + rValue;
        case '-': return lValue - rValue;
        case '*': return lValue * rValue;
        case '/': return lValue / rValue;
        default:cerr << "Wrong op: " << root->data << endl;
        return 0;
    }    
}

Node* copyExpTree(Node* root) {
    if (root == nullptr) return nullptr;
    Node* newNode = new Node(root->data);
    newNode->left = copyExpTree(root->left);
    newNode->right = copyExpTree(root->right);
    return newNode;
}    

void printExp(Node* root) {
    if (root) {
        if (isOp(root->data))
            cout << "(";
        printExp(root->left);
        cout << root->data;
        printExp(root->right);
        if (isOp(root->data))
            cout << ")";           
    }
}

void deleteTree(Node* root) {
    if (root) {
        deleteTree(root->left);
        deleteTree(root->right);
        delete root;
    }
}    


int main()
{
    // build exp tree
    Node* root = new Node('*');
    root->left = new Node('+');
    root->left->left = new Node('x');
    root->left->right = new Node('y');
    root->right = new Node('z');
    // define var
    VarValPair vars[] = { 
        { 'x', 10 },
        { 'y', 20 },
        { 'z', 30 }
    };

    int size = sizeof(vars) / sizeof(vars[0]);
    cout << "Size: " << size << endl;
    // eval exp tree with vars

    int result = evalWithvars(root, vars, size);
    cout << "Result with vars: " << result << endl;
    // copy the tree

    Node* newTree = copyExpTree(root);
    // eval the copied tree

    result = evalWithvars(newTree, vars, size);
    cout << "Result from new tree: " << result << endl;
    // clean up

    deleteTree(root);
    deleteTree(newTree);

    return 0;
}