#include "Manager.h"
#include "Page.h"
/// <summary>
/// program cycle
/// </summary>
void Manager::start() {
	Manager* m = new Manager;
	Display* d = new Display(m);
	while (true) {
		m->status_update();
		if (m->halt) {
			break;
		}
		d->render();
		d->control();
	}
	delete m;
	delete d;
};
/// <summary>
/// saves audio in wav
/// </summary>
/// <returns>if input is invalid</returns>
bool Manager::create_wav_format() {
	Audio* temp = nullptr;
	size_t l;
	for (l = 0; l < this->Audio_objects.size(); l++) {
		if (this->Audio_objects[l]->name == this->text_bufr) {
			temp = this->Audio_objects[l];
			break;
		}
	}
	if (temp != nullptr) {
		wav w = temp->convert_to_wav();
		std::fstream fout;
		fout.open(temp->name + ".wav", std::ios::out | std::ios::binary);
		if (fout.is_open()) {
			fout.write(reinterpret_cast<char*>(&w), 44);
			fout.write(reinterpret_cast<char*>(&w.Data[0]), w.Data.size());
			fout.close();
		}
		else {
			return true;
		}
		temp->remove_obj();
		this->Audio_objects.erase(this->Audio_objects.begin() + l);
		return false;
	}
	return true;
};
/// <summary>
/// read wav file
/// </summary>
/// <returns>if input is invalid</returns>
bool Manager::read_wav_format() {
	wav w;
	uint8_t riff[] = { 'R','I' ,'F' ,'F' };
	uint8_t wave[] = { 'W','A' ,'V' ,'E' };
	uint8_t fmt_[] = { 'f','m' ,'t' ,' ' };
	uint8_t data[] = { 'd','a' ,'t' ,'a' };
	uint8_t af[] = { 1,0 };
	uint8_t nc[] = {1,0};
	uint32_t samplerate = Audio::SampleRate;
	uint8_t sr[] = { (uint8_t)(samplerate >> (8 * 0)),
		(uint8_t)(samplerate >> (8 * 1)),
		(uint8_t)(samplerate >> (8 * 2)),
		(uint8_t)(samplerate >> (8 * 3)) };
	std::fstream fin;
	fin.open(this->text_bufr,std::ios::in | std::ios::binary);
	if (fin.is_open()) {
		fin.read(reinterpret_cast<char*>(&w), 44);
		uint16_t bitres= (w.BitsPerSample[1] << 8) + w.BitsPerSample[0];
		if (std::equal(std::begin(w.ChunkID), std::end(w.ChunkID), std::begin(riff)) &&
			std::equal(std::begin(w.Format), std::end(w.Format), std::begin(wave)) &&
			std::equal(std::begin(w.Subchunk1ID), std::end(w.Subchunk1ID), std::begin(fmt_)) &&
			std::equal(std::begin(w.Subchunk2ID), std::end(w.Subchunk2ID), std::begin(data)) &&
			std::equal(std::begin(w.AudioFormat), std::end(w.AudioFormat), std::begin(af)) &&
			std::equal(std::begin(w.NumChannels), std::end(w.NumChannels), std::begin(nc)) &&
			std::equal(std::begin(w.SampleRate), std::end(w.SampleRate), std::begin(sr)) &&
			(bitres & (bitres -1))==0) 
		{
			uint32_t size = (w.Subchunk2Size[3]<<(8*3))+
				(w.Subchunk2Size[2] << (8 * 2)) + 
				(w.Subchunk2Size[1] << (8 * 1)) + 
				(w.Subchunk2Size[0] << (8 * 0));
			w.Data.resize(size);
			fin.read(reinterpret_cast<char*>(&w.Data[0]), size);
			fin.close();
			this->Audio_objects.push_back(Audio::create_obj(this->text_bufr, w));
			return false;
		}
		fin.close();
		return true;
	}
	return true;
};
/// <summary>
/// save ngff file
/// </summary>
/// <returns>if input is invalid</returns>
bool Manager::create_ngff_format() {
	Audio* temp = nullptr;
	size_t l;
	for (l = 0; l < this->Audio_objects.size();l++) {
		if (this->Audio_objects[l]->name == this->text_bufr) {
			temp = this->Audio_objects[l];
			break;
		}
	}
	if (temp != nullptr) {
		ngff n = temp->convert_to_ngff();
		std::fstream fout;
		fout.open(temp->name + ".ngff", std::ios::out | std::ios::binary);
		if (fout.is_open()) {
			fout.write(reinterpret_cast<char*>(&n), 37);
			fout.write(reinterpret_cast<char*>(&n.Data[0]), n.Data.size() * sizeof(double));
			fout.close();
		}
		else {
			return true;
		}
		temp->remove_obj();
		this->Audio_objects.erase(this->Audio_objects.begin() + l);
		return false;
	}
	return true;
};
/// <summary>
/// read ngff file
/// </summary>
/// <returns>if input is invalid</returns>
bool Manager::read_ngff_format() {
	ngff n;
	uint8_t NGFF[] = { 'N','G' ,'F' ,'F' };
	uint8_t ngff[] = { 'N','G' ,'F' ,'F' };
	uint8_t fmt_[] = { 'f','m' ,'t' ,' ' };
	uint8_t data[] = { 'd','a' ,'t' ,'a' };
	uint8_t lowerfreq[] = { 0,0 ,0 ,0 };
	uint32_t upper = Audio::SampleRate / 2;
	uint8_t upperfreq[] = { uint8_t(upper >> (8*0)),
		uint8_t(upper >> (8 * 1)),
		uint8_t(upper >> (8 * 2)),
		uint8_t(upper >> (8 * 3))};

	std::fstream fin;
	fin.open(this->text_bufr, std::ios::in | std::ios::binary);
	if (fin.is_open()) {
		fin.read(reinterpret_cast<char*>(&n), 37);
		if (std::equal(std::begin(n.ChunkID), std::end(n.ChunkID), std::begin(NGFF)) &&
			std::equal(std::begin(n.Format), std::end(n.Format), std::begin(ngff)) &&
			std::equal(std::begin(n.Subchunk1ID), std::end(n.Subchunk1ID), std::begin(fmt_)) &&
			std::equal(std::begin(n.Subchunk2ID), std::end(n.Subchunk2ID), std::begin(data)) &&
			n.EncodingType[0] == 1)
		{
			uint32_t size = (n.Subchunk2Size[3] << (8 * 3)) +
				(n.Subchunk2Size[2] << (8 * 2)) +
				(n.Subchunk2Size[1] << (8 * 1)) +
				(n.Subchunk2Size[0] << (8 * 0));
			n.Data.resize(size / sizeof(double));
			fin.read(reinterpret_cast<char*>(&n.Data[0]), size);
			fin.close();
			Audio* a = Audio::create_obj(this->text_bufr, n);
			this->Audio_objects.push_back(a);
			return false;
		}
		fin.close();
		return true;
	}
	return true;
};
/// <summary>
/// some special status handler
/// </summary>
void Manager::status_update() {
	if (this->pointer->data == nullptr) {
		this->halt = true;
	}
	if (dynamic_cast<text_page*>(this->pointer->data)!=nullptr) {
		this->text_enabled = true;
	}
	else {
		this->text_enabled = false;
	}
};
/// <summary>
/// comparator to sort alphabeticaly
/// </summary>
/// <param name="a">first</param>
/// <param name="b">second</param>
/// <returns>result of comparison</returns>
bool Manager::comp_for_alphbt(Audio* a, Audio* b) {
	return a->name < b->name;
};
/// <summary>
/// comparator to sort by size
/// </summary>
/// <param name="a">first</param>
/// <param name="b">second</param>
/// <returns>result of comparison</returns>
bool Manager::comp_for_lenght(Audio* a, Audio* b) {
	return a->size < b->size;
};
/// <summary>
/// set syntax interpreter
/// </summary>
/// <returns>if input is invalid</returns>
bool Manager::interp_for_set() {
	Audio* temp = nullptr;
	std::string obj_name;
	std::string lowerfreq_str;
	double lowerfreq;
	std::string upperfreq_str;
	double upperfreq;
	std::string value_str;
	double value;
	std::stringstream ss(this->text_bufr);
	std::getline(ss, obj_name, '{');
	std::getline(ss, lowerfreq_str, ':');
	std::getline(ss, upperfreq_str, '}');
	std::getline(ss, value_str, '=');
	std::getline(ss, value_str);

	for (Audio* object : this->Audio_objects) {
		if (object->name == obj_name) {
			temp = object;
			break;
		}
	}
	try {
		lowerfreq = stod(lowerfreq_str);
		upperfreq = stod(upperfreq_str);
		value = stod(value_str);
	}
	catch (std::invalid_argument ia) {
		return true;
	};
	if (lowerfreq < upperfreq &&
		lowerfreq >=0 && lowerfreq <= 22050 &&
		upperfreq >= 0 && upperfreq <= 22050 &&
		temp != nullptr)
	{
		temp->set(lowerfreq, upperfreq, value);
		return false;
	}
	return true;
}
/// <summary>
/// mekes necesary actions by the end of the branch
/// </summary>
void Manager::end_of_branch() {
	this->execute();
	this->pointer = this->menu->children[0];
	this->obj_modif = nullptr;
	this->other_obj = nullptr;
	while (this->operations.size() != 0) {
		this->operations.pop();
    }
	this->operations.push(oper_enum::none);
	this->error = false;
};
/// <summary>
/// text entry handler
/// </summary>
void Manager::text_enter() {
	switch (this->operations.top()) {
	case oper_enum::open_ngff:
		this->error = this->read_ngff_format();
		break;
	case oper_enum::open_wav:
		this->error = this->read_wav_format();
		break;
	case oper_enum::save_ngff:
		this->error = this->create_ngff_format();;
		break;
	case oper_enum::save_wav:
		this->error = this->create_wav_format();
		break;
	case oper_enum::set:
		this->error = this->interp_for_set();
		break;
	case oper_enum::search:
		this->search_text = this->text_bufr;
		break;
	}
	this->text_bufr.clear();
};
/// <summary>
/// executes some operation
/// </summary>
void Manager::execute() {
	if (this->obj_modif != nullptr) {
		if (this->other_obj != nullptr) {
			switch (this->operations.top()) {
			case oper_enum::conncatenate:
				this->obj_modif->concatenation(this->other_obj);
				break;
			case oper_enum::add:
				this->obj_modif->addition(this->other_obj);
				break;
			case oper_enum::subtract:
				this->obj_modif->subtraction(this->other_obj);
				break;
			case oper_enum::multiply:
				this->obj_modif->multiplication(this->other_obj);
				break;
			case oper_enum::divide:
				this->obj_modif->division(this->other_obj);
				break;
			default:
				break;
			}
		}
		switch (this->operations.top()) {
		case oper_enum::Remove:
			this->Audio_objects.erase(std::find(this->Audio_objects.begin(), this->Audio_objects.begin(),this->obj_modif));
			this->obj_modif->remove_obj();
			break;
		case oper_enum::copy:
			this->Audio_objects.push_back(this->obj_modif->copy_obj());
			break;
		}
	}
};
/// <summary>
/// dynamic list for object list page render
/// </summary>
/// <returns>list of objects</returns>
std::vector<Audio*> Manager::readytouse_obj_list() {
	std::vector<Audio*> temp_obj_list = this->Audio_objects;
	std::vector<Audio*> found_objects;
	std::regex searchtext("(.*)(" + this->search_text + ")(.*)");
	switch (this->operations.top())
	{
	case oper_enum::alphabetical:
		std::sort(temp_obj_list.begin(), temp_obj_list.end(), Manager::comp_for_alphbt);
		break;
	case oper_enum::_alphabetical:
		std::sort(temp_obj_list.begin(), temp_obj_list.end(), Manager::comp_for_alphbt);
		std::reverse(temp_obj_list.begin(), temp_obj_list.end());
		break;
	case oper_enum::Time:
		//sorted
		break;
	case oper_enum::_time:
		std::reverse(temp_obj_list.begin(), temp_obj_list.end());
		break;
	case oper_enum::length:
		std::sort(temp_obj_list.begin(), temp_obj_list.end(), Manager::comp_for_lenght);
		break;
	case oper_enum::_length:
		std::sort(temp_obj_list.begin(), temp_obj_list.end(), Manager::comp_for_lenght);
		std::reverse(temp_obj_list.begin(), temp_obj_list.end());
		break;
	case oper_enum::search:
		for (Audio* object : temp_obj_list) {
			if (std::regex_match(object->name, searchtext)) {
				found_objects.push_back(object);
			}
		}
		temp_obj_list = found_objects;
		break;
	default:
		break;
	}
	return temp_obj_list;
};
/// <summary>
/// setup necessary structures
/// </summary>
void Manager::menu_setup() {
	menu_page* tools = new menu_page( "tools", oper_enum::tools );
	menu_page* conncatinate = new menu_page( "conncatenate", oper_enum::conncatenate );
	menu_page* add = new menu_page( "add", oper_enum::add );
	menu_page* subtruct = new menu_page( "subtruct", oper_enum::subtract);
	menu_page* multiply = new menu_page( "multiply", oper_enum::multiply );
	menu_page* divide = new menu_page( "divide", oper_enum::divide );
	menu_page* set = new menu_page( "set", oper_enum::set );
	menu_page* objects = new menu_page( "objects", oper_enum::objects );
	menu_page* search = new menu_page( "search", oper_enum::search );
	menu_page* sort = new menu_page( "sort", oper_enum::sort );
	menu_page* alphabetical = new menu_page( "alphabetical", oper_enum::alphabetical );
	menu_page* _alphabetical = new menu_page( "reverse alphabetical", oper_enum::_alphabetical);
	menu_page* time = new menu_page( "by time", oper_enum::Time );
	menu_page* _time = new menu_page( "by time, reverse", oper_enum::_time );
	menu_page* length = new menu_page( "by length", oper_enum::length );
	menu_page* _length = new menu_page( "by length, reverse", oper_enum::_length );
	menu_page* files = new menu_page( "files", oper_enum::files );
	menu_page* copy = new menu_page( "copy", oper_enum::copy );
	menu_page* remove = new menu_page( "remove", oper_enum::Remove );
	menu_page* rename = new menu_page( "rename", oper_enum::Rename );
	menu_page* plot = new menu_page( "plot", oper_enum::plot );
	menu_page* open_wav = new menu_page( "open .wav format", oper_enum::open_wav );
	menu_page* open_cust = new menu_page( "open .ngff format", oper_enum::open_ngff );
	menu_page* save_wav = new menu_page( "save .wav format", oper_enum::save_wav );
	menu_page* save_cust = new menu_page( "save .ngff format", oper_enum::save_ngff );

	message* mes = new message( "message" );

	text_page* text_pag = new text_page( "text interpreter" );

	obj_list* m_obj = new obj_list( "modifiable object" );
	obj_list* o_obj = new obj_list( "another object" );

	this->menu = hierarchy<Page>::newhierarchy(nullptr); 
	this->oper_map.insert({ oper_enum::none, menu});

	hierarchy<Page>* text_input_seq = hierarchy<Page>::newhierarchy(text_pag);
	text_input_seq->newchild(mes);
	text_input_seq->newchild(mes);
	hierarchy<Page>* two_obj_seq = hierarchy<Page>::newhierarchy(mes);
	two_obj_seq->newchild(m_obj);
	two_obj_seq->children[0]->newchild(o_obj);
	two_obj_seq->children[0]->children[0]->newchild(mes);
	hierarchy<Page>* search_seq = hierarchy<Page>::newhierarchy(mes);
	search_seq->newchild(text_pag);
	search_seq->children[0]->newchild(m_obj);
	search_seq->children[0]->children[0]->newchild(copy);
	this->oper_map.insert({ oper_enum::copy,search_seq->children[0]->children[0]->children[0] });
	search_seq->children[0]->children[0]->newchild(remove);
	this->oper_map.insert({ oper_enum::Remove,search_seq->children[0]->children[0]->children[1] });
	search_seq->children[0]->children[0]->newchild(rename);
	this->oper_map.insert({ oper_enum::Rename,search_seq->children[0]->children[0]->children[2] });
	search_seq->children[0]->children[0]->newchild(plot);
	this->oper_map.insert({ oper_enum::plot,search_seq->children[0]->children[0]->children[3] });

	search_seq->children[0]->children[0]->children[0]->newchild(mes);
	search_seq->children[0]->children[0]->children[1]->newchild(mes);
	search_seq->children[0]->children[0]->children[2]->connect(text_input_seq);
	search_seq->children[0]->children[0]->children[3]->newchild(mes);

	menu->newchild(tools);
	this->oper_map.insert({ oper_enum::tools, menu->children[0] }); ;
	menu->children[0]->newchild(mes);
	menu->children[0]->children[0]->newchild(conncatinate);
	this->oper_map.insert({ oper_enum::conncatenate, menu->children[0]->children[0]->children[0] }); ;
	menu->children[0]->children[0]->newchild(add);
	this->oper_map.insert({ oper_enum::add, menu->children[0]->children[0]->children[1] }); ;
	menu->children[0]->children[0]->newchild(subtruct);
	this->oper_map.insert({ oper_enum::subtract, menu->children[0]->children[0]->children[2] }); ;
	menu->children[0]->children[0]->newchild(multiply);
	this->oper_map.insert({ oper_enum::multiply, menu->children[0]->children[0]->children[3] }); ;
	menu->children[0]->children[0]->newchild(divide);
	this->oper_map.insert({ oper_enum::divide, menu->children[0]->children[0]->children[4] }); ;
	menu->children[0]->children[0]->newchild(set);
	this->oper_map.insert({ oper_enum::set, menu->children[0]->children[0]->children[5] }); ;
	menu->children[0]->children[0]->children[0]->connect(two_obj_seq);
	menu->children[0]->children[0]->children[1]->connect(two_obj_seq);
	menu->children[0]->children[0]->children[2]->connect(two_obj_seq);
	menu->children[0]->children[0]->children[3]->connect(two_obj_seq);
	menu->children[0]->children[0]->children[4]->connect(two_obj_seq);
	menu->children[0]->children[0]->children[5]->connect(text_input_seq);

	menu->newchild(objects);
	this->oper_map.insert({ oper_enum::objects, menu->children[1] }); ;
	menu->children[1]->newchild(mes);
	menu->children[1]->children[0]->newchild(search);
	this->oper_map.insert({oper_enum::search,menu->children[1]->children[0]->children[0] });
	menu->children[1]->children[0]->newchild(sort);
	this->oper_map.insert({ oper_enum::sort,menu->children[1]->children[0]->children[1] });
	menu->children[1]->children[0]->children[1]->newchild(alphabetical);
	this->oper_map.insert({ oper_enum::alphabetical,menu->children[1]->children[0]->children[1]->children[0] });
	menu->children[1]->children[0]->children[1]->newchild(_alphabetical);
	this->oper_map.insert({ oper_enum::_alphabetical,menu->children[1]->children[0]->children[1]->children[1] });
	menu->children[1]->children[0]->children[1]->newchild(time);
	this->oper_map.insert({ oper_enum::Time,menu->children[1]->children[0]->children[1]->children[2] });
	menu->children[1]->children[0]->children[1]->newchild(_time);
	this->oper_map.insert({ oper_enum::_time,menu->children[1]->children[0]->children[1]->children[3] });
	menu->children[1]->children[0]->children[1]->newchild(length);
	this->oper_map.insert({ oper_enum::length,menu->children[1]->children[0]->children[1]->children[4] });
	menu->children[1]->children[0]->children[1]->newchild(_length);
	this->oper_map.insert({ oper_enum::_length,menu->children[1]->children[0]->children[1]->children[5] });
	menu->children[1]->children[0]->children[0]->connect(search_seq);
	menu->children[1]->children[0]->children[1]->children[0]->children.push_back(search_seq->children[0]->children[0]);
	menu->children[1]->children[0]->children[1]->children[1]->children.push_back(search_seq->children[0]->children[0]);;
	menu->children[1]->children[0]->children[1]->children[2]->children.push_back(search_seq->children[0]->children[0]);;
	menu->children[1]->children[0]->children[1]->children[3]->children.push_back(search_seq->children[0]->children[0]);;
	menu->children[1]->children[0]->children[1]->children[4]->children.push_back(search_seq->children[0]->children[0]);;
	menu->children[1]->children[0]->children[1]->children[5]->children.push_back(search_seq->children[0]->children[0]);;

	menu->newchild(files);
	this->oper_map.insert({oper_enum::files,menu->children[2] });
	menu->children[2]->newchild(mes);
	menu->children[2]->children[0]->newchild(open_wav);
	this->oper_map.insert({ oper_enum::open_wav,menu->children[2]->children[0]->children[0] });
	menu->children[2]->children[0]->newchild(open_cust);
	this->oper_map.insert({ oper_enum::open_ngff,menu->children[2]->children[0]->children[1] });
	menu->children[2]->children[0]->newchild(save_wav);
	this->oper_map.insert({ oper_enum::save_wav,menu->children[2]->children[0]->children[2] });
	menu->children[2]->children[0]->newchild(save_cust);
	this->oper_map.insert({ oper_enum::save_ngff,menu->children[2]->children[0]->children[3] });
	menu->children[2]->children[0]->children[0]->connect(text_input_seq);
	menu->children[2]->children[0]->children[1]->connect(text_input_seq);
	menu->children[2]->children[0]->children[2]->connect(text_input_seq);
	menu->children[2]->children[0]->children[3]->connect(text_input_seq);
	
	this->pointer = menu->children[0];
	this->operations.push(oper_enum::none);
}
Manager::Manager() {
	menu_setup();
};
