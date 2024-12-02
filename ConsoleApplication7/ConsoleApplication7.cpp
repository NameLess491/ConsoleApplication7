//Однозв'язний список
#include <iostream>
#include <utility>

template <typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;
        Node(T value) : data(value), next(nullptr) {}
    };

    Node* head;
    size_t size;

    void freeMemory() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        size = 0;
    }

public:
    LinkedList() : head(nullptr), size(0) {}

    ~LinkedList() {
        freeMemory();
    }

    LinkedList(const LinkedList& other) : head(nullptr), size(0) {
        Node* current = other.head;
        while (current) {
            append(current->data);
            current = current->next;
        }
    }

    LinkedList(LinkedList&& other) noexcept : head(other.head), size(other.size) {
        other.head = nullptr;
        other.size = 0;
    }

    LinkedList& operator=(const LinkedList& other) {
        if (this == &other) return *this;

        freeMemory();

        Node* current = other.head;
        while (current) {
            append(current->data);
            current = current->next;
        }
        return *this;
    }

    LinkedList& operator=(LinkedList&& other) noexcept {
        if (this == &other) return *this;

        freeMemory();

        head = other.head;
        size = other.size;
        other.head = nullptr;
        other.size = 0;
        return *this;
    }

    void append(T value) {
        Node* newNode = new Node(value);
        if (!head) {
            head = newNode;
        }
        else {
            Node* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
        ++size;
    }

    void display() const {
        Node* temp = head;
        while (temp) {
            std::cout << temp->data << " ";
            temp = temp->next;
        }
        std::cout << "\n";
    }

    size_t getSize() const {
        return size;
    }
};

int main() {
    LinkedList<int> list1;
    list1.append(10);
    list1.append(20);
    list1.append(30);

    LinkedList<int> list2 = list1;
    LinkedList<int> list3 = std::move(list1);

    list2.display();
    list3.display();

    return 0;
}
//Бінарне дерево
#include <iostream>
#include <utility>

template <typename T>
class BinaryTree {
private:
    struct Node {
        T data;
        Node* left;
        Node* right;
        Node(T value) : data(value), left(nullptr), right(nullptr) {}
    };

    Node* root;

    Node* copyTree(Node* other) {
        if (!other) return nullptr;
        Node* newNode = new Node(other->data);
        newNode->left = copyTree(other->left);
        newNode->right = copyTree(other->right);
        return newNode;
    }

    void freeTree(Node* node) {
        if (!node) return;
        freeTree(node->left);
        freeTree(node->right);
        delete node;
    }

    void inOrderTraversal(Node* node) const {
        if (!node) return;
        inOrderTraversal(node->left);
        std::cout << node->data << " ";
        inOrderTraversal(node->right);
    }

public:
    BinaryTree() : root(nullptr) {}

    ~BinaryTree() {
        freeTree(root);
    }

    BinaryTree(const BinaryTree& other) : root(copyTree(other.root)) {}

    BinaryTree(BinaryTree&& other) noexcept : root(other.root) {
        other.root = nullptr;
    }

    BinaryTree& operator=(const BinaryTree& other) {
        if (this == &other) return *this;
        freeTree(root);
        root = copyTree(other.root);
        return *this;
    }

    BinaryTree& operator=(BinaryTree&& other) noexcept {
        if (this == &other) return *this;
        freeTree(root);
        root = other.root;
        other.root = nullptr;
        return *this;
    }

    void insert(T value) {
        Node** current = &root;
        while (*current) {
            if (value < (*current)->data) {
                current = &((*current)->left);
            }
            else {
                current = &((*current)->right);
            }
        }
        *current = new Node(value);
    }

    void display() const {
        inOrderTraversal(root);
        std::cout << "\n";
    }
};

int main() {
    BinaryTree<int> tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(5);

    BinaryTree<int> tree2 = tree;
    BinaryTree<int> tree3 = std::move(tree);

    tree2.display();
    tree3.display();

    return 0;
}
//Словник з використанням бінарного дерева
#include <iostream>
#include <string>
#include <vector>

class Dictionary {
private:
    struct Node {
        std::string word;
        std::string translation;
        int accessCount;
        Node* left;
        Node* right;

        Node(const std::string& w, const std::string& t)
            : word(w), translation(t), accessCount(0), left(nullptr), right(nullptr) {}
    };

    Node* root;

    Node* copyTree(Node* other) {
        if (!other) return nullptr;
        Node* newNode = new Node(other->word, other->translation);
        newNode->accessCount = other->accessCount;
        newNode->left = copyTree(other->left);
        newNode->right = copyTree(other->right);
        return newNode;
    }

    void freeTree(Node* node) {
        if (!node) return;
        freeTree(node->left);
        freeTree(node->right);
        delete node;
    }

    Node* add(Node* node, const std::string& word, const std::string& translation) {
        if (!node) return new Node(word, translation);
        if (word < node->word) {
            node->left = add(node->left, word, translation);
        }
        else if (word > node->word) {
            node->right = add(node->right, word, translation);
        }
        return node;
    }

    void getTopWords(Node* node, std::vector<Node*>& topWords, size_t limit, bool mostPopular) {
        if (!node) return;
        getTopWords(node->left, topWords, limit, mostPopular);
        topWords.push_back(node);
        getTopWords(node->right, topWords, limit, mostPopular);
    }

public:
    Dictionary() : root(nullptr) {}

    ~Dictionary() {
        freeTree(root);
    }

    void addWord(const std::string& word, const std::string& translation) {
        root = add(root, word, translation);
    }

    void displayTop(size_t limit, bool mostPopular) {
        std::vector<Node*> nodes;
        getTopWords(root, nodes, limit, mostPopular);
        if (!mostPopular) std::sort(nodes.begin(), nodes.end(), [](Node* a, Node* b) { return a->accessCount < b->accessCount; });
        else std::sort(nodes.begin(), nodes.end(), [](Node* a, Node* b) { return a->accessCount > b->accessCount; });
    }
};
