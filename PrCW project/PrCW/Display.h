#pragma once
#include<conio.h>
#include<string>
#include<vector>
#include<iostream>
#include<algorithm>
#include"Manager.h"
class Manager;
class Display
{
public:
	void render();
	void control();
	Display(Manager* m);
	Manager* m;
};