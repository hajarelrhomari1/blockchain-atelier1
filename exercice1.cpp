#include <iostream>
#include <vector>
#include <string>

std::string sha256(const std::string& s) {
    unsigned long h = 0;
    for (char c : s) h = h * 31 + c;
    return "h" + std::to_string(h % 100000000);
}

struct MerkleNode {
    std::string hash;
    MerkleNode* left;
    MerkleNode* right;

    MerkleNode(std::string h) : hash(h), left(nullptr), right(nullptr) {}
    MerkleNode(std::string h, MerkleNode* l, MerkleNode* r) : hash(h), left(l), right(r) {}
};

class MerkleTree {
public:
    MerkleNode* root;

    MerkleTree(const std::vector<std::string>& data) {
        root = buildTree(data);
    }

    ~MerkleTree() {
        clear(root);
    }

    std::string getRoot() const {
        return root ? root->hash : "";
    }

private:
    MerkleNode* buildTree(const std::vector<std::string>& data) {
        if (data.empty()) return nullptr;

        std::vector<MerkleNode*> nodes;
        for (const auto& d : data)
            nodes.push_back(new MerkleNode(sha256(d)));

        if (nodes.size() == 1) return nodes[0];

        while (nodes.size() > 1) {
            if (nodes.size() % 2 == 1) {
                nodes.push_back(new MerkleNode(nodes.back()->hash));
            }

            std::vector<MerkleNode*> next;
            for (size_t i = 0; i < nodes.size(); i += 2) {
                std::string combined = nodes[i]->hash + nodes[i+1]->hash;
                next.push_back(new MerkleNode(sha256(combined), nodes[i], nodes[i+1]));
            }
            nodes = next;
        }
        return nodes[0];
    }

    void clear(MerkleNode* n) {
        if (!n) return;
        clear(n->left);
        clear(n->right);
        delete n;
    }
};

int main() {
    std::vector<std::string> tx = {"A", "B", "C", "D", "E"};
    MerkleTree tree(tx);
    std::cout << "Racine originale : " << tree.getRoot() << "\n";

    tx[0] = "A_modif";
    MerkleTree tree2(tx);
    std::cout << "Racine modifiée : " << tree2.getRoot() << "\n";

    return 0;
}