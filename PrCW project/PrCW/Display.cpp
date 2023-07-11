#include "Display.h"
/// <summary>
/// full render of the page
/// </summary>
void Display::render() {
    system("cls");
    m->pointer->data->render(m);
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    m->pointer->data->render_controls(m);
};
/// <summary>
/// response to the keybord input 
/// </summary>
void Display::control() {
    int c, c1;
    c = _getch();
    if (c == 224)
    {
        switch (c1 = _getch())
        {
        case 72:
            this->m->pointer->data->do_up(m);
            break;
        case 80:
            this->m->pointer->data->do_down(m);
            break;
        case 75:
            this->m->pointer->data->do_back(m);
            break;
        case 77:
            this->m->pointer->data->do_forward(m);
            break;
        };
    }
    else if(this->m->text_enabled){
        this->m->text_bufr += (char)c;
    }
};
Display::Display(Manager* m) {
    this->m = m;
};