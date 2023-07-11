#pragma once
#include<fstream>
#include<vector>
#include<map>
#include<stack>
#include<algorithm>
#include<regex>
#include"Display.h"
#include"Audio.h"
#include"hierarchy.h"
class Page;
enum class oper_enum {
	none, tools,conncatenate,add,subtract,multiply,divide,set,objects,search,
	sort, alphabetical,_alphabetical,Time,_time,length,_length, files,copy,
	Remove, Rename, plot, open_wav, open_ngff, save_wav, save_ngff
};
#include"Page.h"
class Manager
{
public:
	void static start();
	bool create_ngff_format();
	bool create_wav_format();
	bool read_wav_format();
	bool read_ngff_format();
	bool interp_for_set();
	void status_update();
	void end_of_branch();
	void text_enter();
	void execute();
	bool static comp_for_alphbt(Audio* a, Audio* b);
	bool static comp_for_lenght(Audio* a, Audio* b);
	std::vector<Audio*> readytouse_obj_list();
	
	std::vector<Audio*> Audio_objects;
	hierarchy<Page>* menu;
	hierarchy<Page>* pointer;
	std::map<oper_enum, hierarchy<Page>*> oper_map;
	std::string text_bufr;
	std::string search_text;
	Audio* obj_modif;
	Audio* other_obj;
	Audio* obj_list_pointer;
	std::stack<oper_enum> operations;
	bool error;
	bool halt;
	bool text_enabled;
private:
	void menu_setup();
	Manager();
};