#include <bits/stdc++.h>
using namespace std;

// Node structure for Huffman tree
struct Node {
    int ch;
    int freq;
    Node *left, *right;

    Node(int c, int f) {
        ch = c, freq = f;
        left = NULL, right = NULL;
    }
};

// Custom comparator for min Heap
struct Compare {
    bool operator() (Node* left, Node* right) {
        return left->freq > right->freq;
    }
};

// Function to find freq of each character
vector<int> calculateFrequency(string &text) {
    vector<int> frequency(128, 0); //as 127 ASCII characters
    for(char c : text) {
        frequency[(int)c]++;
    }
    return frequency;
}
// Build Huffman tree
Node* buildTree(string &text, vector<int> &frequency) {

    priority_queue<Node*, vector<Node*>, Compare> pq; //min Heap
    for(int i=0; i<128; i++) {
        if(frequency[i]) {
            Node* node = new Node(i, frequency[i]);
            pq.push(node);
        }
    }

    while(pq.size() > 1) {
        Node* node1 = pq.top(); pq.pop();
        Node* node2 = pq.top(); pq.pop();
        Node* merged = new Node(-1, node1->freq + node2->freq);
        merged->left = node1;
        merged->right = node2;
        pq.push(merged);
    }

    return pq.top();
}

// Generate codes for each character
void generateCodes(Node* node, vector<string> &codes, string code) {

    if(!node) return;

    if(node->ch != -1) {
        codes[node->ch] = code; 
    }

    generateCodes(node->left, codes, code + "0");
    generateCodes(node->right, codes, code + "1");
}

// Get the encoded string
string encodeText(string &text, vector<string> &codes) {
    string encodedText = "";
    for(char ch : text) {
        encodedText += codes[(int)ch];
    }
    return encodedText;
}

// Get the original string from encoded
string decodeText(Node* root, string &encodedText) {
    Node* currentNode = root;
    string decodedText = ""; //or the original text

    for (char bit : encodedText) {
        if (!currentNode->left && !currentNode->right) {
            decodedText += currentNode->ch;
            currentNode = root;
        }
        if (bit == '0') {
            currentNode = currentNode->left;
        } else {
            currentNode = currentNode->right;
        }
        
    }

    return decodedText;
}

int main() {

    // Open the file "input.txt"
    ifstream inFile("input.txt");
    if (!inFile.is_open()) {
        cerr << "Error opening file" << endl;
        return 1;
    }
    string text((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
    cout << "Text: " << text << endl;
    inFile.close();
    

    vector<int> frequency = calculateFrequency(text);

    Node* root = buildTree(text, frequency);

    vector<string> codes(128);
    string code = "";
    generateCodes(root, codes, code);

    //special case for length 1
    if(text.size() == 1) {
        codes[(int)text[0]] = "0";
    }

    string encodedText = encodeText(text, codes);
    cout << "EncodedText: " << encodedText << endl;

    // Write the encoded(compressed) text to a file
    ofstream outFile("encoded.bin", ios::binary);
    outFile << encodedText;
    outFile.close();

    // Read the encoded text from the compressed file
    ifstream encodedFile("encoded.bin");
    string encodedTextfromFile((istreambuf_iterator<char>(encodedFile)), istreambuf_iterator<char>());
    encodedFile.close();

    // Decode the text
    string decodedText = decodeText(root, encodedTextfromFile);
    cout << "DecodedText: " << decodedText << endl;

    // Write the decoded text to a file
    ofstream outDecodedFile("decoded.txt");
    outDecodedFile << decodedText;
    outDecodedFile.close();

    cout << "Encoded text has been decoded and written to decoded.txt" << endl;

    return 0;
}

