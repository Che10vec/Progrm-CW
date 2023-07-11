#include<string>
#include<map>
#include<vector>
#pragma once

template<typename T>
class hierarchy {
public:
	T* data = nullptr;
	std::vector<hierarchy<T>*> children;
	hierarchy<T>* parent = nullptr;

	static hierarchy<T>* newhierarchy(T* dat) {
		hierarchy<T>* newhierarchy = new hierarchy<T>;
		newhierarchy->data = dat;
		return newhierarchy;
	}
	void newchild(T* dat) {
		hierarchy<T>* newchild = new hierarchy<T>;
		newchild->parent = this;
		newchild->data = dat;
		this->children.push_back(newchild);
	};
	void connect(hierarchy<T>* node) {
		this->children.push_back(node);
		node->parent = this;
	};
	hierarchy() {};
};