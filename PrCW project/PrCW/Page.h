#pragma once
#include<string>
#include<typeinfo>
#include"Manager.h"
class Manager;
class Page {
public:
	std::string name;
	virtual void render(Manager* m) =0;
	virtual void do_up(Manager* m)  =0;
	virtual void do_down(Manager* m) =0;
	virtual void do_forward(Manager* m) =0;
	virtual void do_back(Manager* m) =0;
	virtual void render_controls(Manager* m) =0;
	Page(std::string name) {
		this->name = name;
	}
	Page() {};
};
class obj_list : public Page {
public:
	void render(Manager* m);
	void do_up(Manager* m);
	void do_down(Manager* m);
	void do_forward(Manager* m);
	void do_back(Manager* m);
	void render_controls(Manager* m);
	obj_list(std::string name) {
		this->name = name;
	}
	obj_list() {};
};
class message : public Page {
public:
	void render(Manager* m);
	void do_up(Manager* m);
	void do_down(Manager* m);
	void do_forward(Manager* m);
	void do_back(Manager* m);
	void render_controls(Manager* m);
	message(std::string name){
		this->name = name;
	}
	message() {};
};
class text_page : public Page {
public:
	void render(Manager* m);
	void do_up(Manager* m);
	void do_down(Manager* m);
	void do_forward(Manager* m);
	void do_back(Manager* m);
	void render_controls(Manager* m);
	text_page(std::string name) {
		this->name = name;
	}
	text_page() {};
};
class menu_page : public Page {
public:
	oper_enum operatr;
	void render(Manager* m);
	void do_up(Manager* m);
	void do_down(Manager* m);
	void do_forward(Manager* m);
	void do_back(Manager* m);
	void render_controls(Manager* m);
	menu_page(std::string name, oper_enum operatr) {
		this->name = name;
		this->operatr = operatr;
	};
	menu_page() {
		this->operatr = oper_enum::none;
	};
};


