#include<iostream>
using namespace std;

// val树是一个 二叉搜索树
// 且无论那个节点的高度差不超过1
// 优点：搜索值极为快速 复杂度为O(log n) 即10亿个数据只要搜索30次
// 缺点：平衡稍微复杂 插入一个节点最坏都要更新平衡因子 并且反转要旋转树

//key&val 树结构
template<class K,class V>
struct TreeNode
{
	TreeNode<K, V>* left;
	TreeNode<K, V>* right;
	TreeNode<K, V>* prant;
	int _bf;

	pair<K, V> _kv;

	TreeNode(const pair<K,V>&v)
	{
		left = nullptr;
		right = nullptr;
		prant = nullptr;
		_bf = 0;
		_kv = v;
	}
};
//中序遍历val树 自然有序
void tree_b(TreeNode<int,int>*root)
{
	if (root == nullptr)
		return;
	
	tree_b(root->left);
	cout << root->_kv.first << endl;
	tree_b(root->right);
}
//VAL树类
template<class K,class V>
class VALTree
{
	
public:
	typedef TreeNode<K, V> Node;
	VALTree()
	{
		_root = nullptr;
	}
	void coutTree()
	{
		tree_b(_root);
	}

	//右旋
	void Rx(Node* prant)
	{
		Node* cur = prant->left;
		Node* Rcur = cur->right;

		prant->left = Rcur;
		if (Rcur!=nullptr)
		{
			Rcur->prant = prant;
		}
		
		Node* pp = prant->prant;

		cur->right = prant;
		prant->prant = cur;
		
		if (prant == _root)
		{
			_root = cur;
			cur->prant = nullptr;
		}
		else
		{
			if (pp->left == prant)
			{
				pp->left = cur;
			}
			else
			{
				pp->right = cur;
			}
			cur->prant = pp;
		}
		cur->_bf = 0;
		prant->_bf = 0;
	}


	//左旋
	void Lx(Node* prant)
	{
		Node* cur = prant->right;
		Node* Lcur = cur->left;


		prant->right = Lcur;
		if (Lcur != nullptr)
		{
			Lcur->prant = prant;
		}

		Node* pp = prant->prant;

		prant->prant = cur;
		cur->left = prant;

		if (prant == _root)
		{
			_root = cur;
			cur->prant = nullptr;
		}
		else
		{
			if (pp->left == prant)
			{
				pp->left = cur;
			}
			else
			{
				pp->right = cur;
			}
			cur->prant = pp;
		}
		cur->_bf = 0;
		prant->_bf = 0;
	}
	/*void Lx(Node* parent)
	{
		Node* subR = parent->right;
		Node* subRL = subR->left;

		parent->right = subRL;
		if (subRL)
			subRL->prant = parent;

		subR->left = parent;
		Node* parentParent = parent->prant;
		parent->prant = subR;

		if (_root == parent)
		{
			_root = subR;
			subR->prant = NULL;
		}
		else
		{
			if (parentParent->left == parent)
			{
				parentParent->left = subR;
			}
			else
			{
				parentParent->right = subR;
			}

			subR->prant = parentParent;
		}
		parent->_bf = 0;
		subR->_bf = 0;

	}*/
	//左旋+右旋
	void LRx(Node* prant)
	{
		Node* curL = prant->left;
		Node* curR = curL->right;
		int bf = curR->_bf;
		Lx(prant->left);
		Rx(prant);

		if (bf == -1)
		{
			prant->_bf = 1;
			curL->_bf = 0;
			curR->_bf = 0;
		}
		else if (bf == 1)
		{
			prant->_bf = 0;
			curL->_bf = -1;
			curR->_bf = 0;
		}
		else if (bf == 0)
		{
			prant->_bf = 0;
			curL->_bf = 0;
			curR->_bf = 0;
		}
		else
		{
			cout << "bf_error" << endl;
		}
	}

	//右旋+左旋
	void RLx(Node* prant)
	{
		Node* curR = prant->right;
		Node* curL = curR->left;
		int bf = curL->_bf;


		Rx(prant->right);
		Lx(prant);

		if (bf == 1)
		{
			prant->_bf = -1;
			curR->_bf = 0;
			curL->_bf = 0;
		}
		else if (bf == -1)
		{
			prant->_bf = 0;
			curR->_bf = 1;
			curL->_bf = 0;
		}
		else if(bf==0)
		{
			prant->_bf = 0;
			curR->_bf = 0;
			curL->_bf = 0;
		}
		else
		{
			cout << "bf_error" << endl;
		}
	}

	//val树插入键值
	bool insert(const pair<K,V>&val)
	{
		if (_root == nullptr)
		{
			_root = new Node(val);
			return true;
		}

		Node* prant = nullptr;
		Node* cur = _root;
		while (cur)
		{
			if (cur->_kv.first>val.first)
			{
				prant = cur;
				cur = cur->left;
			}
			else if (cur->_kv.first<val.first)
			{
				prant = cur;
				cur = cur->right;
			}
			else
			{
				cout << "相同的值" << endl;
				return false;
			}
		}
		cur = new Node(val);
		if (prant->_kv.first >cur->_kv.first)
		{
			prant->left = cur;
		}
		else
		{
			prant->right = cur;
		}
		cur->prant = prant;

		while (prant)
		{
			if (prant->left==cur)
			{
				prant->_bf--;
			}
			else
			{
				prant->_bf++;
			}
			if (prant->_bf == -1 || prant->_bf == 1)
			{
				cur = prant;
				prant = prant->prant;
			}
			else if (prant->_bf == -2 || prant->_bf == 2)
			{
				if (prant->_bf == -2 && cur->_bf == -1)
				{
					//cout << "R" << endl;
					Rx(prant);
				}
				else if (prant->_bf == 2 && cur->_bf == 1)
				{
					//cout << "L" << endl;
					Lx(prant);
				}
				else if (prant->_bf == -2 && cur->_bf == 1)
				{
					LRx(prant);
				}
				else if (prant->_bf == 2 && cur->_bf == -1)
				{
					RLx(prant);
				}
				break;
			}
			else if(prant->_bf==0)
			{
				break;
			}
			else
			{
				cout << "Tree_error" << endl;
				exit(-1);
			}
		}
		return true;
	}
	int treeH(Node* root)
	{
		if (root == nullptr)
			return  0;
		int L = treeH(root->left);
		int R = treeH(root->right);
		return L > R ? L + 1 : R + 1;
	}
	bool _isban(Node* root)
	{
		if (root == nullptr)
			return true;
		int leftH = treeH(root->left);
		int rightH = treeH(root->right);
		if (rightH-leftH!=root->_bf)
		{
			cout << root->_kv.first <<"应该是" << ":" << rightH-leftH << endl;
			return false;
		}
		return _isban(root->left) && _isban(root->right);
	}
	bool isban()
	{
		return _isban(_root);
	}
private:
	Node*_root;

	
};