#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class Tree;
string op;
long long TotalWeight;
int TowerNumber;
bool NullRoot;

class Node
{
public:
    Node() : data(' '), parent(NULL), left(NULL), right(NULL), tower(false), leaf(false), defence(false){};
    Node(int c) : data(c), parent(NULL), left(NULL), right(NULL), tower(false), leaf(false), defence(false)  {};

    friend class Tree;
    friend Node *build(int min, int max);
    friend void preorder(Node *root);
    friend void inorder(Node *root);
    friend void postorder(Node *root);
    friend int height(Node *root, int max);
    friend long long MaxPathSum(Node *root, long long sum);
    friend void Delete(Node *node);
    friend void DeleteLeaf(Node *root);
    friend bool Foldable(Node *node_1, Node *node_2);
    friend void FindLeaf(Node *root);
    friend void BinaryTower(Node *root);

private:
    int data;
    Node *parent;
    Node *left;
    Node *right;
    int weight;
    bool tower;
    bool leaf;
    bool defence;
};

class Tree
{
private:
    Node *root;

public:
    Tree() : root(0){};
    ~Tree(){};
    Node *get_root() { return root; }
    void set_root(Node *node) { root = node; }
    int get_data(Node *node) { return node->data; }
    friend Node *build(int min, int max);
    friend void preorder(Node *root);
    friend void inorder(Node *root);
    friend void postorder(Node *root);
    friend int height(Node *root, int max);
    friend long long MaxPathSum(Node *root, long long sum);
    friend void Delete(Node *root);
    friend void DeleteLeaf(Node *root);
    friend bool Foldable(Node *node_1, Node *node_2);
    friend void BinaryTower(Node *root);
    friend void FindLeaf(Node *root);
};

Node *build(int min, int max)
{
    if (op[min] == '(' && op[min + 1] == ')')
        return NULL;

    int pos = min + 1;
    stringstream ss;
    for (; op[pos] != '('; pos++)
    {
        ss << op[pos];
    }
    int num;
    ss >> num;
    ss.str("");
    ss.clear();
    TotalWeight += num;
    Node *new_node = new Node(num);
    int idx;
    int var = 0;
    for (int i = pos; i < max; i++)
    {
        if (op[i] == '(')
            var++;
        else if (op[i] == ')')
            var--;
        if (var == 0)
        {
            idx = i;
            break;
        }
    }
    new_node->left = build(pos, idx);
    if (new_node->left != NULL)
    {
        new_node->left->parent = new_node;
    }
    new_node->right = build(idx + 1, max - 1);
    if (new_node->right != NULL)
    {
        new_node->right->parent = new_node;
    }
    return new_node;
}

void preorder(Node *root)
{
    if (root == NULL)
    {
        return;
    }
    cout << root->data << " ";
    preorder(root->left);
    preorder(root->right);
}
void inorder(Node *root)
{
    if (root == NULL)
    {
        return;
    }
    inorder(root->left);
    cout << root->data << " ";
    inorder(root->right);
}
void postorder(Node *root)
{
    if (root == NULL)
    {
        return;
    }
    postorder(root->left);
    postorder(root->right);
    cout << root->data << " ";
}

int height(Node *root, int max)
{
    if (root == NULL)
    {
        return max - 1;
    }

    if ((root->right == NULL) && (root->left == NULL))
    {
        return max;
    }
    int max_l = height(root->left, max + 1);
    int max_r = height(root->right, max + 1);
    if (max_l > max)
    {
        max = max_l;
    }
    if (max_r > max)
    {
        max = max_r;
    }
    return max;
}

long long MaxPathSum(Node *root, long long sum)
{
    if ((root->right == NULL) && (root->left == NULL))
    {
        return sum;
    }
    long long sum_l = -100000000;
    long long sum_r = -100000000;
    if (root->left != NULL)
    {
        sum_l = MaxPathSum(root->left, sum + root->left->data);
    }
    if (root->right != NULL)
    {
        sum_r = MaxPathSum(root->right, sum + root->right->data);
    }
        if(sum_l>sum_r){
            return sum_l;
        }
        else{
            return sum_r;
        }
}

void BinaryTower(Node *root)
{
    if ((root->left == NULL) && (root->right == NULL))
    {
        if ((root->parent == NULL))
        { //if the root of the tree is also a leaf
            TowerNumber = 1;
            return;
        }
        else
        {
            if (root->parent->tower == false)
            {
                root->parent->tower = true;
                root->parent->defence = true;
                if (root->parent->left)
                    root->parent->left->defence = true;
                if (root->parent->right)
                    root->parent->right->defence = true;
                if (root->parent->parent)
                    root->parent->parent->defence = true;
                TowerNumber++;
            }
            return;
        }
    }
    if (root->left)
        BinaryTower(root->left);
    if (root->right)
        BinaryTower(root->right);
    bool build = false;
    if (root->left)
    {
        if (!root->left->defence)
            build = true;
    }
    if (root->right)
    {
        if (!root->right->defence)
            build = true;
    }
    if (!root->parent)
    {
        if (!root->defence)
            build = true;
    }
    if (build)
    {
        root->tower = true;
        root->defence = true;
        if (root->left)
            root->left->defence = true;
        if (root->right)
            root->right->defence = true;
        if (root->parent)
            root->parent->defence = true;
        TowerNumber++;
        return;
    }
}
void DeleteLeaf(Node *root)
{
    if (root == NULL)
    {
        return;
    }
    if ((root->left == NULL) && (root->right == NULL))
    {
        if (root->parent != NULL)
        {
            if (root->parent->left == root)
            {
                root->parent->left = NULL;
            }
            if (root->parent->right == root)
            {
                root->parent->right = NULL;
            }
            root->parent = NULL;
            TotalWeight -= root->data;
            delete root;
        }
        else
        {
            TotalWeight -= root->data;
            NullRoot = true;
        }
        return;
    }
    else
    {
        root->defence = false;
        root->tower = false;
    }
    DeleteLeaf(root->left);
    DeleteLeaf(root->right);
}

bool Foldable(Node *node_1, Node *node_2)
{
    if ((node_1 == NULL) && (node_2 != NULL))
    {
        return false;
    }
    else if ((node_2 == NULL) && (node_1 != NULL))
    {
        return false;
    }
    else if ((node_2 == NULL) && (node_1 == NULL))
    {
        return true;
    }
    bool left = Foldable(node_1->left, node_2->right);
    bool right = Foldable(node_1->right, node_2->left);
    if ((!left) || (!right))
    {
        return false;
    }
    else
    {
        return true;
    }
}

void Delete(Node *root)
{
    if (root == NULL)
    {
        return;
    }
    Delete(root->left);
    Delete(root->right);
    delete root;
}

int main()
{
    while (cin >> op)
    {
        TotalWeight = 0;
        //TotalHeight=0;
        Tree tree;
        tree.set_root(build(0, op.length() - 1));
        string com;
        while (cin >> com)
        {
            Node *root = tree.get_root();

            if (com == "Traverse")
            {
                if (root != NULL)
                {
                    preorder(root);
                    cout << endl;
                    inorder(root);
                    cout << endl;
                    postorder(root);
                    cout << endl;
                }
                else
                {
                    cout << endl;
                    cout << endl;
                    cout << endl;
                }
            }
            else if (com == "Height")
            {
                if (root != NULL)
                {
                    int ans = height(root, 1);
                    cout << ans << endl;
                }
                else
                {
                    cout << 0 << endl;
                }
            }
            else if (com == "WeightSum")
            {
                if (root == NULL)
                {
                    cout << 0 << endl;
                    continue;
                }
                cout << TotalWeight << endl;
            }
            else if (com == "MaximumPathSum")
            {
                if (root == NULL)
                {
                    cout << 0 << endl;
                    continue;
                }
                long long ans = MaxPathSum(root, tree.get_data(root));
                cout << ans << endl;
            }
            else if (com == "BinaryTower")
            {
                if (root == NULL)
                {
                    cout << 0 << endl;
                    continue;
                }
                TowerNumber = 0;
                BinaryTower(root);
                cout << TowerNumber << endl;
            }
            else if (com == "DeleteLeaf")
            {
                if (root == NULL)
                {
                    continue;
                }
                NullRoot = false;
                DeleteLeaf(root);
                if (NullRoot)
                {
                    tree.set_root(NULL);
                }
            }
            else if (com == "Foldable")
            {
                if (root == NULL)
                {
                    cout << "Yes" << endl;
                    continue;
                }
                bool ans = Foldable(root, root);
                if (ans)
                {
                    cout << "Yes" << endl;
                }
                else
                {
                    cout << "No" << endl;
                }
            }
            else if (com == "End")
            {
                if (root == NULL)
                {
                    break;
                }
                Delete(root);
                break;
            }
        }
    }

    return 0;
}