#include "Page.h"
/// <summary>
/// renders object list page
/// </summary>
/// <param name="m"> for Manager</param>
void obj_list::render(Manager* m) {
    std::string header_name = m->oper_map[m->operations.top()]->data->name;
    std::vector<Audio*> obj_list= m->readytouse_obj_list();
    //head
    //upper border
    std::cout << (char)(201);
    for (size_t l = 0; l < header_name.size(); l++) {
        std::cout << (char)(205);
    }
    std::cout << (char)(187) << "\n";
    //centr
    std::cout << (char)(186);
    std::cout << header_name;
    std::cout << (char)(186) << "\n";
    //lower border
    std::cout << (char)(200);
    for (size_t l = 0; l < header_name.size(); l++) {
        std::cout << (char)(205);
    }
    std::cout << (char)(188) << "\n";
    if (!obj_list.empty()) {
        if (m->obj_list_pointer == nullptr) {
            m->obj_list_pointer = obj_list[0];
        };
        //elements
        for (size_t l = 0; l < obj_list.size(); l++) {
            if (m->obj_list_pointer == obj_list[l]) {
                std::cout << "\033[31m";
            }
            //upper border
            std::cout << (char)(218);
            for (size_t l1 = 0; l1 < obj_list[l]->name.size(); l1++) {
                std::cout << (char)(196);
            }
            std::cout << (char)(191) << "\n";
            //centr
            std::cout << (char)(179);
            std::cout << obj_list[l]->name;
            std::cout << (char)(179) << "\n";
            //lower
            std::cout << (char)(192);
            for (size_t l1 = 0; l1 < obj_list[l]->name.size(); l1++) {
                std::cout << (char)(196);
            }
            std::cout << (char)(217) << "\n";
            if (m->obj_list_pointer == obj_list[l]) {
                std::cout << "\033[m";
            }
        }
    }
    else {
        std::cout << "\n there are no objects \n \n";
    }
};
/// <summary>
/// control for arrow up
/// </summary>
/// <param name="m"> for Manager</param>
void obj_list::do_up(Manager* m) {
    std::vector<Audio*> obj_list = m->readytouse_obj_list();
    if (!obj_list.empty()) {
        std::vector<Audio*>::iterator it = std::find(obj_list.begin(), obj_list.end(), m->obj_list_pointer);
        if (it != obj_list.begin()) {
            m->obj_list_pointer = *--it;
        }
    }
};
/// <summary>
/// control for arrow down
/// </summary>
/// <param name="m"> for Manager</param>
void obj_list::do_down(Manager* m) {
    std::vector<Audio*> obj_list = m->readytouse_obj_list();
    if (!obj_list.empty()) {
        std::vector<Audio*>::iterator it = std::find(obj_list.begin(), obj_list.end(), m->obj_list_pointer);
        if (++it != obj_list.end()) {
            m->obj_list_pointer = *it;
        }
    }
};
/// <summary>
/// control for arrow right
/// </summary>
/// <param name="m"> for Manager</param>
void obj_list::do_forward(Manager* m) {
    //pointing to the next element of menu
    if (m->pointer->children.empty()) {
        m->end_of_branch();
    }
    else {
        m->pointer = m->pointer->children[0];
    }
    //set obj
    if (m->obj_modif==nullptr) {
        m->obj_modif = m->obj_list_pointer;
    }
    else if (m->other_obj == nullptr) {
        m->other_obj = m->obj_list_pointer;
    }
    m->obj_list_pointer = nullptr;
};
/// <summary>
/// control for arrow left
/// </summary>
/// <param name="m"></param>
void obj_list::do_back(Manager* m) {
    m->pointer = m->oper_map[m->operations.top()];
    m->operations.pop();
    m->obj_list_pointer = nullptr;
};
/// <summary>
/// render controls prompt
/// </summary>
/// <param name="m"> for Manager</param>
void obj_list::render_controls(Manager*) {
    std::cout << "^ (up)\n";
    std::cout << "> (choose object)\n";
    std::cout << "< (go back)\n";
    std::cout << "v (down)\n";
};
/// <summary>
/// renders message page
/// </summary>
/// <param name="m"> for Manager</param>
void message::render(Manager* m) {
    switch (m->operations.top())
    {
    case oper_enum::tools:
        std::cout << "You are provided with some basic tools:\n\n";
        std::cout << "concatenation\n";
        std::cout << "addition\n";
        std::cout << "subtraction\n";
        std::cout << "multiplication\n";
        std::cout << "division\n";
        std::cout << "setting the frequancies *\n\n";
        std::cout << "choose two objects: the one that would be modified and the second one\n\n";
        std::cout << "* this one will require the text input with the special rules\n";
        std::cout << "<object name>{<lowwer frequency>:<upper frequency>}=<value to set>\n";
        break;
    case oper_enum::conncatenate:
        if (m->obj_modif == nullptr || m->other_obj == nullptr) {
            std::cout << "concatenation\n";
            std::cout << "choose the modifiable and then the second object\n";
        }
        else {
            std::cout << "concatenation\n";
            std::cout << "press left to complete\n";
        };
        break;
    case oper_enum::add:
        if (m->obj_modif == nullptr || m->other_obj == nullptr) {
            std::cout << "addition\n";
            std::cout << "choose the modifiable and then the second object\n";
        }
        else {
            std::cout << "addition\n";
            std::cout << "press left to completee\n";
        };
        break;
    case oper_enum::subtract:
        if (m->obj_modif == nullptr || m->other_obj == nullptr) {
            std::cout << "subtraction\n";
            std::cout << "choose the modifiable and then the second object\n";
        }
        else {
            std::cout << "subtraction\n";
            std::cout << "press left to complete\n";
        };
        break;
    case oper_enum::multiply:
        if (m->obj_modif == nullptr || m->other_obj == nullptr) {
            std::cout << "multiplication\n";
            std::cout << "choose the modifiable and then the second object\n";
        }
        else {
            std::cout << "multiplication\n";
            std::cout << "press left to complete\n";
        };
        break;
    case oper_enum::divide:
        if (m->obj_modif == nullptr || m->other_obj == nullptr) {
            std::cout << "division\n";
            std::cout << "choose the modifiable and then the second object\n";
        }
        else {
            std::cout << "division\n";
            std::cout << "press left to complete\n";
        };
        break;
    case oper_enum::set:
        if (m->error) {
            std::cout << "setting the frequencies\n";
            std::cout << "incorrect syntax, try again\n";
        }
        else {
            std::cout << "setting the frequencies\n";
            std::cout << "operation is complete\n";
        };
        break;
    case oper_enum::objects:
        std::cout << "Your given choice to traverse the list of object\n";
        std::cout << "either via search or filters\n\n";
        std::cout << "sort has following option:\n\n";
        std::cout << "alphabetical sort and in reverse\n";
        std::cout << "sort by creation time and in reverse\n";
        std::cout << "sort sample length and in reverse\n";
        break;
    case oper_enum::search:
        std::cout << "there are what were found\n";
        break;
    case oper_enum::files:
        std::cout << "Find the files on your computer\n";
        std::cout << "supports a WAVE or a custom NGFF format\n\n";
        std::cout << "note 1: don't forget to add extention like .wav or .ngff\n";
        std::cout << "note 2: make sure you put the right name\n";
        std::cout << "you could find correct name in objects\n";
        break;
    case oper_enum::copy:
        if (m->obj_modif == nullptr) {
            std::cout << "nothing was chosen\n";
        }
        else {
            std::cout << "press left to copy created\n";
        }
        break;
    case oper_enum::Remove:
        if (m->obj_modif == nullptr) {
            std::cout << "nothing was chosen\n";
        }
        else {
            std::cout << "press left to remove object\n";
        }
        break;
    case oper_enum::Rename:
        if (m->obj_modif == nullptr) {
            std::cout << "nothing was chosen\n";
        }
        else {
            std::cout << "press left to rename object\n";
        }
        break;
    case oper_enum::plot:
        std::cout << "unfortunatly the function is not supported\n";
        std::cout << ":(\n";
        break;
    case oper_enum::open_wav:
        if (m->error) {
            std::cout << ".wav wasn't opened \n";
        }
        else {
            std::cout << ".wav is opened \n";
        };
        break;
    case oper_enum::open_ngff:
        if (m->error) {
            std::cout << ".ngff wasn't opened \n";
        }
        else {
            std::cout << ".ngff is opened \n";
        };
        break;
    case oper_enum::save_wav:
        if (m->error) {
            std::cout << ".wav wasn't saved\n";
        }
        else {
            std::cout << ".wav is saved \n";
        };
        break;
    case oper_enum::save_ngff:
        if (m->error) {
            std::cout << ".ngff wasn't saved\n";
        }
        else {
            std::cout << ".ngff is saved \n";
        };
        break;
    default:
        break;
    }
};
/// <summary>
/// control for arrow up
/// </summary>
/// <param name="m"> for Manager</param>
void message::do_up(Manager*) {};
/// <summary>
/// control for arrow down
/// </summary>
/// <param name="m"> for Manager</param>
void message::do_down(Manager*) {};
/// <summary>
/// control for arrow right
/// </summary>
/// <param name="m"> for Manager</param>
void message::do_forward(Manager* m) {
    if (m->pointer->children.empty()) {
        m->end_of_branch();
    }
    else {
        m->pointer = m->pointer->children[0];
    }
};
/// <summary>
/// control for arrow left
/// </summary>
/// <param name="m"> for Manager</param>
void message::do_back(Manager* m) {
    m->pointer = m->oper_map[m->operations.top()];
    m->operations.pop();
};
/// <summary>
/// render controls prompt
/// </summary>
/// <param name="m"> for Manager</param>
void message::render_controls(Manager*) {
    std::cout << "> (skip message)\n";
    std::cout << "< (go back)\n";
};
/// <summary>
/// renders text input page
/// </summary>
/// <param name="m"> for Manager</param>
void text_page::render(Manager* m) {
    //upper
    std::cout << (char)(201);
    for (size_t l = 0; l < m->text_bufr.size(); l++) {
        std::cout << (char)(205);
    }
    std::cout << (char)(187) << "\n";
    //centr
    std::cout << (char)(186);
    std::cout << m->text_bufr;
    std::cout << (char)(186) << "\n";
    //lower
    std::cout << (char)(200);
    for (size_t l = 0; l < m->text_bufr.size(); l++) {
        std::cout << (char)(205);
    }
    std::cout << (char)(188) << "\n";
};
/// <summary>
/// control for arrow up
/// </summary>
/// <param name="m"> for Manager</param>
void text_page::do_up(Manager*) {};
/// <summary>
/// control for arrow down
/// </summary>
/// <param name="m"> for Manager</param>
void text_page::do_down(Manager*) {};
/// <summary>
/// control for arrow right
/// </summary>
/// <param name="m"> for Manager</param>
void text_page::do_forward(Manager* m) {
    if (m->pointer->children.empty()) {
        m->end_of_branch();
    }
    else {
        m->pointer = m->pointer->children[0];
    }
    m->text_enter();
};
/// <summary>
/// control for arrow left
/// </summary>
/// <param name="m"> for Manager</param>
void text_page::do_back(Manager* m) {
    m->pointer = m->oper_map[m->operations.top()];
    m->operations.pop();
    m->text_bufr.clear();
};
/// <summary>
/// render controls prompt
/// </summary>
/// <param name="m"> for Manager</param>
void text_page::render_controls(Manager*) {
    std::cout << "> (enter text)\n";
    std::cout << "< (go back)\n";
};
/// <summary>
/// renders menu page
/// </summary>
/// <param name="m"> for Manager</param>
void menu_page::render(Manager* m) {
    std::string header_name;
    std::vector<std::string> elements_names;
    size_t highlighted = 0;
    if (m->oper_map[m->operations.top()]->data==nullptr) {
        header_name = "menu";
    }
    else {
        header_name = m->oper_map[m->operations.top()]->data->name;
    }

    hierarchy<Page>* temp_parent = m->pointer->parent;
    for (size_t l = 0; l < temp_parent->children.size(); l++) {
        if (temp_parent->children[l] == m->pointer) {
            highlighted = l;
        }
        elements_names.push_back(temp_parent->children[l]->data->name);
    }
    //head
    //upper
    std::cout << (char)(201);
    for (size_t l = 0; l < header_name.size(); l++) {
        std::cout << (char)(205);
    }
    std::cout << (char)(187) << "\n";
    //centr
    std::cout << (char)(186);
    std::cout << header_name;
    std::cout << (char)(186) << "\n";
    //lower
    std::cout << (char)(200);
    for (size_t l = 0; l < header_name.size(); l++) {
        std::cout << (char)(205);
    }
    std::cout << (char)(188) << "\n";
    //elements
    for (size_t l = 0; l < elements_names.size(); l++) {
        if (highlighted == l) {
            std::cout << "\033[31m";
        }
        //upper
        std::cout << (char)(218);
        for (size_t l1 = 0; l1 < elements_names[l].size(); l1++) {
            std::cout << (char)(196);
        }
        std::cout << (char)(191) << "\n";
        //centr
        std::cout << (char)(179);
        std::cout << elements_names[l];
        std::cout << (char)(179) << "\n";
        //lower
        std::cout << (char)(192);
        for (size_t l1 = 0; l1 < elements_names[l].size(); l1++) {
            std::cout << (char)(196);
        }
        std::cout << (char)(217) << "\n";
        if (highlighted == l) {
            std::cout << "\033[m";
        }
    }
};
/// <summary>
/// control for arrow up
/// </summary>
/// <param name="m"> for Manager</param>
void menu_page::do_up(Manager* m) {
    hierarchy<Page>* temp_parent = m->pointer->parent;
    std::vector<hierarchy<Page>*>::iterator it = std::find(temp_parent->children.begin(), temp_parent->children.end(), m->pointer);
    if (it != temp_parent->children.begin()) {
        m->pointer = *--it;
    }
};
/// <summary>
/// control for arrow down
/// </summary>
/// <param name="m"> for Manager</param>
void menu_page::do_down(Manager* m) {
    hierarchy<Page>* temp_parent = m->pointer->parent;
    std::vector<hierarchy<Page>*>::iterator it = std::find(temp_parent->children.begin(), temp_parent->children.end(), m->pointer);
    if (++it != temp_parent->children.end()) {
        m->pointer = *it;
    }
};
/// <summary>
/// control for arrow right
/// </summary>
/// <param name="m"> for Manager</param>
void menu_page::do_forward(Manager* m) {
    for (auto elem : m->oper_map) {
        if (elem.second == m->pointer) {
            m->operations.push(elem.first);
        }
    }
    if (m->pointer->children.empty()) {
        m->end_of_branch();
    }
    else {
        m->pointer = m->pointer->children[0];
    }
};
/// <summary>
/// control for arrow left
/// </summary>
/// <param name="m"> for Manager</param>
void menu_page::do_back(Manager* m) {
    m->pointer = m->oper_map[m->operations.top()];
    m->operations.pop();
};
/// <summary>
/// render controls prompt
/// </summary>
/// <param name="m"> for Manager</param>
void menu_page::render_controls(Manager*) {
    std::cout << "^ (up)\n";
    std::cout << "> (proceed with option)\n";
    std::cout << "< (go back)\n";
    std::cout << "v (down)\n";
};