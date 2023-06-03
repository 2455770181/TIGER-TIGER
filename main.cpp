#include<iostream>
#include<vector>
#include<math.h>
#include"GlobalSettings.h"
#include"Enemy.h"
#include"Barrier.h"
#include"Prop.h"
using namespace std;

void Welcome() {
	LPCTSTR title = _T("TIGER!TIGER!");	//_T()函数用于转换字符类型
	LPCTSTR tplay = _T("开始游戏");
	LPCTSTR texit = _T("退出游戏");

	RECT tplayr, texitr;	//设置鼠标点击的矩形区域

	BeginBatchDraw();	//防止图像闪烁

	setbkcolor(WHITE);
	cleardevice();
	settextstyle(60, 0, _T("黑体"));
	settextcolor(BLACK);
	outtextxy(screen_width / 2 - textwidth(title) / 2, screen_height / 5, title);	//绘制标题

	settextstyle(40, 0, _T("黑体"));
	tplayr.left = screen_width / 2 - textwidth(tplay) / 2;
	tplayr.right = tplayr.left + textwidth(tplay);
	tplayr.top = screen_height / 5 * 2.5;
	tplayr.bottom = tplayr.top + textheight(tplay);

	texitr.left = screen_width / 2 - textwidth(texit) / 2;
	texitr.right = texitr.left + textwidth(texit);
	texitr.top = screen_height / 5 * 3;
	texitr.bottom = texitr.top + textheight(texit);

	outtextxy(tplayr.left, tplayr.top, tplay);	//绘制开始游戏文字
	outtextxy(texitr.left, texitr.top, texit);	//绘制退出游戏文字

	EndBatchDraw();

	//while (true) {
	//	//如果观察到在进入Welcome函数后，在点击“开始游戏”区域之前就进入了Play函数，这可能是因为绘图库（graphics.h）
	//	//中的getmessage函数是一个阻塞函数。在调用getmessage函数时，它会等待并接收用户的输入消息，直到用户点击鼠标或执行其他操作。
	//	//如果用户在点击“开始游戏”区域之前进行了其他操作，例如移动鼠标或点击其他区域，getmessage函数可能会在接收到相关消息后返回，
	//	//导致程序进入下一轮循环并调用Play函数。
	//	if (MouseHit()) {
	//		ExMessage* mess = new ExMessage;
	//		getmessage(mess, EM_MOUSE);
	//		if (mess->lbutton) {
	//			cout << "mess.x:" << mess->x << "，mess.y:" << mess->y << endl;
	//			if (PointInRect(mess->x, mess->y, tplayr)) {
	//				delete mess; // 释放内存
	//				return;	//直接返回到main中的循环
	//			}
	//			else if (PointInRect(mess->x, mess->y, texitr)) {
	//				delete mess; // 释放内存
	//				exit(0);
	//			}
	//		}
	//		delete mess; // 释放内存
	//	}
	//}

	bool startGameClicked = false;

	while (!startGameClicked) {
		if (MouseHit()) {
			ExMessage* mess = new ExMessage;
			getmessage(mess, EM_MOUSE);
			if (mess->lbutton) {
				cout << "mess.x:" << mess->x << "，mess.y:" << mess->y << endl;
				if (PointInRect(mess->x, mess->y, tplayr)) {
					startGameClicked = true;
				}
				else if (PointInRect(mess->x, mess->y, texitr)) {
					exit(0);
				}
			}
			delete mess; // 释放内存
		}
	}
}

class BK {
public:
	BK(IMAGE& img) :img(img), y(0) {

	}

	void Show(bool upordown) {
		if (upordown) {
			if (y <= -(img.getheight() - screen_height)) {
				y = 0;
			}
		}
		else {
			if (y >= 0) {
				y = -(img.getheight() - screen_height);
			}
		}
		
		y += scroll_speed;
		putimage(0, y, &img);	//将图片的左上角放到指定坐标，然后绘制图片
	}
private:
	IMAGE& img;
	int y;	//绘制位置的 y 坐标
};

/*
vector<GreenFish*>* CreateGreenFish() {
	Position GF_P[2] = { {100,100},{400,100} };
	GreenFish* GF = new GreenFish(GF_P);
	vector<GreenFish*> *greenfish_es = new vector<GreenFish*>();
	greenfish_es->push_back(GF);
	return greenfish_es;
}
vector<RedFish*>* CreateRedFish() {
	Position RF_P[2] = { {50,500},{400,500} };
	RedFish* RF = new RedFish(RF_P);
	vector<RedFish*>* greenfish_es = new vector<RedFish*>();
	greenfish_es->push_back(RF);
	return greenfish_es;
}
vector<Scaleph*>* CreateScaleph() {
	Position S_P[2] = { {200,200},{200,200} };
	Scaleph* S = new Scaleph(S_P);
	vector<Scaleph*>* scaleph_es = new vector<Scaleph*>();
	scaleph_es->push_back(S);
	return scaleph_es;
}
vector<Turtle*>* CreateTurtle() {
	Position T_P[2] = { {100,300},{400,300} };
	Turtle* T = new Turtle(T_P);
	vector<Turtle*>* turtle_es = new vector<Turtle*>();
	turtle_es->push_back(T);
	return turtle_es;
}
Box* CreateBox() {
	Position P = { game_area_width / 2,game_area_height - 85 };
	Box* box = new Box(P);
	return box;
}
vector<SmallBox*>* CreateSmallBox() {
	Position P = { 10,game_area_height - 50 };
	SmallBox* SB = new SmallBox(P);
	vector<SmallBox*>* smallbox_es = new vector<SmallBox*>();
	smallbox_es->push_back(SB);
	return smallbox_es;
}
vector<Etha*>* CreateEtha() {
	Position P = { 80,game_area_height - 50 };
	Etha* E = new Etha(P);
	vector<Etha*>* etha_es = new vector<Etha*>();
	etha_es->push_back(E);
	return etha_es;
}
vector<EnhancedHook*>* CreateEnhancedHook() {
	Position P = { 120,game_area_height - 50 };
	EnhancedHook* EH = new EnhancedHook(P);
	vector<EnhancedHook*>* enhancedbook_es = new vector<EnhancedHook*>();
	enhancedbook_es->push_back(EH);
	return enhancedbook_es;
}
vector<OxygenBottle*>* CreateOxygenBottle() {
	Position P = { 120,game_area_height - 90 };
	OxygenBottle* OB = new OxygenBottle(P);
	vector<OxygenBottle*>* oxygenbottle_es = new vector<OxygenBottle*>();
	oxygenbottle_es->push_back(OB);
	return oxygenbottle_es;
}
vector<Stab*>* CreateStab() {
	Position P = { 200,game_area_height - 50 };
	Stab* S = new Stab(P);
	vector<Stab*>* stab_es = new vector<Stab*>();
	stab_es->push_back(S);
	return stab_es;
}
vector<LanternFish*>* CreateLanternFish() {
	Position LF_P = {100,400};
	LanternFish* LF = new LanternFish(LF_P, 1);
	vector<LanternFish*>* lanternfish_es = new vector<LanternFish*>();
	lanternfish_es->push_back(LF);
	return lanternfish_es;
}
vector<Lobster*>* CreateLobster() {
	Position L_P = { 600,100 };
	Lobster* L = new Lobster(L_P);
	vector<Lobster*>* lobster_es = new vector<Lobster*>();
	lobster_es->push_back(L);
	return lobster_es;
}
vector<Trichiurus*>* CreateTrichiurus() {
	Position Tr_P = { 600,500 };
	Trichiurus* Tr = new Trichiurus(Tr_P);
	vector<Trichiurus*>* trichiurus_es = new vector<Trichiurus*>();
	trichiurus_es->push_back(Tr);
	return trichiurus_es;
}
*/

template<typename T>
void Show(vector<T*>* es) {
	auto it = es->begin();
	while (it != es->end()) {
		(*it)->Show();
		it++;
	}
}

template<typename T>
void ReleaseVector(vector<T*>* es) {
	// 遍历容器并删除每个元素指向的内存块
	for (auto& it : *es) {
		delete it;
	}

	// 清空容器
	es->clear();

	// 释放指向容器本身的内存块
	delete es;
}

void ControlObject(vector<Turtle*>* turtle_es, vector<RouteNode*>* rnl, Hero& hp) {
	auto turtle_it = turtle_es->begin();
	RECT* it_rect;
	while (turtle_it != turtle_es->end()) {
		it_rect = &(*turtle_it)->GetRect();
		if (!(*turtle_it)->IfDying() && !(*turtle_it)->IfDead()) hp.AttackChecked(*it_rect);

		(*turtle_it)->UpdatePosition();
		(*turtle_it)->AttackedCheck(&hp);
		if ((*turtle_it)->IfDead()) {
			auto rnl_it = rnl->begin();
			while (rnl_it != rnl->end()) {
				if ((*rnl_it)->id == (*turtle_it)->GetID()) {
					delete (*rnl_it);
					rnl->erase(rnl_it);

					delete (*turtle_it);
					turtle_es->erase(turtle_it);
					return;
				}
				rnl_it++;
			}
		}
		turtle_it++;
	}
}

template<typename T>
void RemoveObject(vector<T*>* es, vector<RouteNode*>* rnl, bool upordown) {
	int pre_distance = 50;
	auto it = es->begin();
	while (it != es->end()) {
		if ((upordown == 1) && ((*it)->GetRect().bottom <= -pre_distance)) {
			auto rnl_it = rnl->begin();
			while (rnl_it != rnl->end()) {
				if ((*rnl_it)->id == (*it)->GetID()) {
					(*rnl_it)->flag = 0;

					delete (*it);
					es->erase(it);
					return;
				}
				rnl_it++;
			}
		}
		else if ((upordown == 0) && ((*it)->GetRect().top >= game_area_height + pre_distance)) {
			auto rnl_it = rnl->begin();
			while (rnl_it != rnl->end()) {
				if ((*rnl_it)->id == (*it)->GetID()) {
					(*rnl_it)->flag = 0;

					delete (*it);
					es->erase(it);
					return;
				}
				rnl_it++;
			}
		}
		it++;
	}
}

//upordown：滚动方向，向上为0，向下为1
void GenerateObject(vector<Turtle*>* turtle_es, vector<RouteNode*>* rnl, int offsetY, bool upordown) {
	auto rnl_it = rnl->begin();
	int pre_distance = 50;
	while (rnl_it != rnl->end()) {
		//offsetY + game_area_height + pre_distance：提前生成，使其缓慢进入视野
		if (((*rnl_it)->offsetY <= offsetY + game_area_height + pre_distance) && ((*rnl_it)->offsetY >= offsetY) && ((*rnl_it)->flag == 0) && (upordown == 1)) {
			(*rnl_it)->SP.y = game_area_height + pre_distance;
			(*rnl_it)->END.y = game_area_height + pre_distance;
			Position T_P[2] = { (*rnl_it)->SP, (*rnl_it)->END };
			Turtle* T = new Turtle(T_P, (*rnl_it)->id);
			turtle_es->push_back(T);
			(*rnl_it)->flag = 1;
			break;
		}
		else if (((*rnl_it)->offsetY >= offsetY - pre_distance) && ((*rnl_it)->offsetY <= offsetY) && ((*rnl_it)->flag == 0) && (upordown == 0)) {
			cout << "(*rnl_it)->offsetY:" << (*rnl_it)->offsetY << "，offsetY:" << offsetY << endl;
			(*rnl_it)->SP.y = -pre_distance;
			(*rnl_it)->END.y = -pre_distance;
			Position T_P[2] = { (*rnl_it)->SP, (*rnl_it)->END };
			Turtle* T = new Turtle(T_P, (*rnl_it)->id);
			turtle_es->push_back(T);
			(*rnl_it)->flag = 1;
			break;
		}
		rnl_it++;
	}
}


bool Play(int map1) {
	setbkcolor(WHITE);
	cleardevice();	//这个函数使用当前背景色清空绘图设备
	bool playing = true;

	IMAGE bkimg;

	loadimage(&bkimg, _T("./image/Background/background-final.png"));

	BK bk = BK(bkimg);

	LPCTSTR SCORE = _T("SCORE");
	LPCTSTR ETHER = _T("ETHER");
	LPCTSTR AIR = _T("AIR");

	//设置文字边距
	int text_margin_left = state_area_width / 4;
	int text_margin_top = state_area_height / 40;
	int text_height = 30;

	settextstyle(text_height, 0, _T("Arial"));
	settextcolor(RGB(233, 255, 255));
	setbkmode(TRANSPARENT);

	int score = 0;
	LPCTSTR lpStr_score = IntToLPCTSTR(score);
	int ether = 0;
	LPCTSTR lpStr_ether = IntToLPCTSTR(ether);

	//记录分数、以太数、空气剩余量、特殊道具、拾取到的宝箱
	RECT state_area;
	state_area.left = game_area_width;
	state_area.right = state_area.left + state_area_width;
	state_area.bottom = state_area_height;
	state_area.top = 0;
	
	//拾取到的宝箱在该区域显示
	RECT box_area;
	box_area.left = state_area.left + 4;
	box_area.right = state_area.right - 4;
	box_area.bottom = state_area.bottom - 50;
	box_area.top = state_area.top + 250;

	const int MaxOffsetY = 800;	//地图总长度
	int OffsetY = 0;	//用于记录背景图已经滚动的总路程

	bool upordown = 1;	//向上为0，向下为1

	Hero hp = Hero();
	RECT hp_rect = hp.GetRect();

	//存储当前窗口内需要显示的敌人
	vector<Turtle*>* turtle_es = new vector<Turtle*>();
	auto turtle_it = turtle_es->begin();
	vector<RouteNode*>* turtle_rnl = ReadRouteNodeList("./enemy_data/Turtle.txt");

	/*vector<Scaleph*>* scaleph_es = CreateScaleph();
	auto scaleph_it = scaleph_es->begin();

	vector<RedFish*>* redfish_es = CreateRedFish();
	auto redfish_it = redfish_es->begin();
	
	vector<GreenFish*>* greenfish_es = CreateGreenFish();
	auto greenfish_it = greenfish_es->begin();

	vector<LanternFish*>* lanternfish_es = CreateLanternFish();
	auto lanternfish_it = lanternfish_es->begin();

	vector<Lobster*>* lobster_es = CreateLobster();
	auto lobster_it = lobster_es->begin();

	vector<Trichiurus*>* trichiurus_es = CreateTrichiurus();
	auto trichiurus_it = trichiurus_es->begin();

	vector<SmallBox*>* smallbox_es = CreateSmallBox();
	auto smallbox_it = smallbox_es->begin();
	vector<Etha*>* etha_es = CreateEtha();
	auto etha_it = etha_es->begin();
	vector<EnhancedHook*>* enhancedhook_es = CreateEnhancedHook();
	auto enhancedhook_it = enhancedhook_es->begin();
	vector<OxygenBottle*>* oxygenbottle_es = CreateOxygenBottle();
	auto oxygenbottle_it = oxygenbottle_es->begin();
	vector<Stab*>* stab_es = CreateStab();
	auto stab_it = stab_es->begin();
	Box* box = CreateBox();

	Position W1_P1 = { 450,450 };
	Wall_1* wall_1_1 = new Wall_1(W1_P1);
	vector<Wall_1*> wall_1_es;
	auto wall_1_it = wall_1_es.begin();
	wall_1_es.push_back(wall_1_1);*/

	setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 5);
	setlinecolor(BLACK);

	while (playing) {
		GenerateObject(turtle_es, turtle_rnl, OffsetY, upordown);

		BeginBatchDraw();

		setbkcolor(WHITE);
		cleardevice();	//这个函数使用当前背景色清空绘图设备
		bk.Show(upordown);
		OffsetY -= scroll_speed;
		hp.Attack();
		hp.Show();

		/*turtle_it = turtle_es->begin();
		while (turtle_it != turtle_es->end()) {
			(*turtle_it)->Show();
			turtle_it++;
		}
		scaleph_it = scaleph_es->begin();
		while (scaleph_it != scaleph_es->end()) {
			(*scaleph_it)->Show();
			scaleph_it++;
		}
		redfish_it = redfish_es->begin();
		while (redfish_it != redfish_es->end()) {
			(*redfish_it)->Show();
			redfish_it++;
		}
		greenfish_it = greenfish_es->begin();
		while (greenfish_it != greenfish_es->end()) {
			(*greenfish_it)->Show();
			greenfish_it++;
		}
		lanternfish_it = lanternfish_es->begin();
		while (lanternfish_it != lanternfish_es->end()) {
			(*lanternfish_it)->Show();
			lanternfish_it++;
		}
		lobster_it = lobster_es->begin();
		while (lobster_it != lobster_es->end()) {
			(*lobster_it)->Show();
			lobster_it++;
		}
		trichiurus_it = trichiurus_es->begin();
		while (trichiurus_it != trichiurus_es->end()) {
			(*trichiurus_it)->Show();
			trichiurus_it++;
		}
		wall_1_it = wall_1_es.begin();
		while (wall_1_it != wall_1_es.end()) {
			(*wall_1_it)->Show();
			wall_1_it++;
		}*/

		setfillcolor(RGB(19, 69, 71));	//设置填充颜色
		solidrectangle(state_area.left, state_area.top, state_area.right, state_area.bottom);
		outtextxy(state_area.left + text_margin_left, state_area.top + text_margin_top, SCORE);
		outtextxy(state_area.left + text_margin_left, state_area.top + text_margin_top + text_height, lpStr_score);
		outtextxy(state_area.left + text_margin_left, state_area.top + text_margin_top + text_height * 2, ETHER);
		outtextxy(state_area.left + text_margin_left, state_area.top + text_margin_top + text_height * 3, lpStr_ether);

		setcolor(RGB(208, 255, 248));	//设置前景色，用以填充线条
		fillroundrect(box_area.left, box_area.top, box_area.right, box_area.bottom, 1, 1);

		Show(turtle_es);

		/*smallbox_it = smallbox_es->begin();
		while (smallbox_it != smallbox_es->end()) {
			(*smallbox_it)->Show();
			smallbox_it++;
		}
		etha_it = etha_es->begin();
		while (etha_it != etha_es->end()) {
			(*etha_it)->Show();
			etha_it++;
		}
		enhancedhook_it = enhancedhook_es->begin();
		while (enhancedhook_it != enhancedhook_es->end()) {
			(*enhancedhook_it)->Show();
			enhancedhook_it++;
		}
		oxygenbottle_it = oxygenbottle_es->begin();
		while (oxygenbottle_it != oxygenbottle_es->end()) {
			(*oxygenbottle_it)->Show();
			oxygenbottle_it++;
		}
		stab_it = stab_es->begin();
		while (stab_it != stab_es->end()) {
			(*stab_it)->Show();
			stab_it++;
		}
		if (box != NULL)box->Show();*/

		EndBatchDraw();

		hp.Control();

		if (hp.IfDead()) {
			cout << "Hero死亡" << endl;
			break;
		}

		/*wall_1_it = wall_1_es.begin();
		while (wall_1_it != wall_1_es.end()) {
			it_rect = &(*wall_1_it)->GetRect();
			(*wall_1_it)->CollisitionDetect(&hp);
			(*wall_1_it)->UpdatePosition();
			if (it_rect->bottom <= 0) {
				delete (*wall_1_it);
				wall_1_es.erase(wall_1_it);
				break;
			}
			wall_1_it++;
		}*/

		ControlObject(turtle_es, turtle_rnl, hp);

		RemoveObject(turtle_es, turtle_rnl, upordown);

		//scaleph_it = scaleph_es->begin();
		//while (scaleph_it != scaleph_es->end()) {
		//	it_rect = &(*scaleph_it)->GetRect();
		//	if (!(*scaleph_it)->IfDying() && !(*scaleph_it)->IfDead()) hp.AttackChecked(*it_rect);

		//	(*scaleph_it)->UpdatePosition();
		//	(*scaleph_it)->AttackedCheck(&hp);
		//	if ((*scaleph_it)->IfDead()) {
		//		delete (*scaleph_it);
		//		scaleph_es->erase(scaleph_it);
		//		break;
		//	}
		//	scaleph_it++;
		//}

		//redfish_it = redfish_es->begin();
		//while (redfish_it != redfish_es->end()) {
		//	it_rect = &(*redfish_it)->GetRect();
		//	if (!(*redfish_it)->IfDying() && !(*redfish_it)->IfDead()) hp.AttackChecked(*it_rect);
		//	//获取屏幕上的物体的矩形
		//	vector<RECT*>rect_es;
		//	turtle_it = turtle_es->begin();
		//	while (turtle_it != turtle_es->end()) {
		//		rect_es.push_back(&(*turtle_it)->GetRect());
		//		turtle_it++;
		//	}
		//	scaleph_it = scaleph_es->begin();
		//	while (scaleph_it != scaleph_es->end()) {
		//		rect_es.push_back(&(*scaleph_it)->GetRect());
		//		scaleph_it++;
		//	}
		//	wall_1_it = wall_1_es.begin();
		//	while (wall_1_it != wall_1_es.end()) {
		//		vector<RECT*> temp_list = (*wall_1_it)->GetRects();
		//		auto temp_it = temp_list.begin();
		//		while (temp_it != temp_list.end()) {
		//			rect_es.push_back(*temp_it);
		//			temp_it++;
		//		}
		//		wall_1_it++;
		//	}
		//	//获取屏幕上的物体的位置
		//	vector<Position*>position_es;
		//	turtle_it = turtle_es->begin();
		//	while (turtle_it != turtle_es->end()) {
		//		position_es.push_back(&(*turtle_it)->GetPosition());
		//		turtle_it++;
		//	}
		//	scaleph_it = scaleph_es->begin();
		//	while (scaleph_it != scaleph_es->end()) {
		//		position_es.push_back(&(*scaleph_it)->GetPosition());
		//		scaleph_it++;
		//	}
		//	wall_1_it = wall_1_es.begin();
		//	while (wall_1_it != wall_1_es.end()) {
		//		vector<Position*> temp_list = (*wall_1_it)->GetPositions();
		//		auto temp_it = temp_list.begin();
		//		while (temp_it != temp_list.end()) {
		//			position_es.push_back(*temp_it);
		//			temp_it++;
		//		}
		//		wall_1_it++;
		//	}

		//	(*redfish_it)->UpdateRoute(&hp);
		//	(*redfish_it)->SpeedControl(rect_es, position_es);
		//	(*redfish_it)->UpdatePosition();
		//	(*redfish_it)->StateControl(&hp);
		//	(*redfish_it)->AttackedCheck(&hp);
		//	if ((*redfish_it)->IfDead()) {
		//		delete (*redfish_it);
		//		redfish_es->erase(redfish_it);
		//		break;
		//	}
		//	redfish_it++;
		//}

		//greenfish_it = greenfish_es->begin();
		//while (greenfish_it!=greenfish_es->end())
		//{
		//	it_rect = &(*greenfish_it)->GetRect();
		//	if (!(*greenfish_it)->IfDying() && !(*greenfish_it)->IfDead()) hp.AttackChecked(*it_rect);

		//	(*greenfish_it)->SpeedControl();
		//	(*greenfish_it)->UpdateRoute();
		//	(*greenfish_it)->UpdatePosition();
		//	(*greenfish_it)->AttackedCheck(&hp);
		//	if ((*greenfish_it)->IfDead()) {
		//		delete (*greenfish_it);
		//		greenfish_es->erase(greenfish_it);
		//		break;
		//	}
		//	greenfish_it++;
		//}

		//lanternfish_it = lanternfish_es->begin();
		//while (lanternfish_it != lanternfish_es->end()) {
		//	it_rect = &(*lanternfish_it)->GetRect();
		//	if (!(*lanternfish_it)->IfDying() && !(*lanternfish_it)->IfDead()) hp.AttackChecked(*it_rect);

		//	(*lanternfish_it)->UpdateRoute(&hp);
		//	(*lanternfish_it)->SpeedControl();
		//	(*lanternfish_it)->UpdatePosition();
		//	(*lanternfish_it)->StateControl(&hp);
		//	(*lanternfish_it)->AttackedCheck(&hp);
		//	if ((*lanternfish_it)->IfDead()) {
		//		delete (*lanternfish_it);
		//		lanternfish_es->erase(lanternfish_it);
		//		break;
		//	}
		//	lanternfish_it++;
		//}

		//lobster_it = lobster_es->begin();
		//while (lobster_it != lobster_es->end()) {
		//	it_rect = &(*lobster_it)->GetRect();
		//	if (!(*lobster_it)->IfDying() && !(*lobster_it)->IfDead()) hp.AttackChecked(*it_rect);

		//	(*lobster_it)->UpdateRoute(&hp);
		//	(*lobster_it)->SpeedControl();
		//	(*lobster_it)->UpdatePosition();
		//	(*lobster_it)->StateControl(&hp);
		//	(*lobster_it)->AttackedCheck(&hp);
		//	if ((*lobster_it)->IfDead()) {
		//		delete (*lobster_it);
		//		lobster_es->erase(lobster_it);
		//		break;
		//	}
		//	lobster_it++;
		//}

		//trichiurus_it = trichiurus_es->begin();
		//while (trichiurus_it != trichiurus_es->end()) {
		//	it_rect = &(*trichiurus_it)->GetRect();
		//	if (!(*trichiurus_it)->IfDying() && !(*trichiurus_it)->IfDead()) hp.AttackChecked(*it_rect);

		//	(*trichiurus_it)->UpdateRoute(&hp);
		//	(*trichiurus_it)->SpeedControl();
		//	(*trichiurus_it)->UpdatePosition();
		//	(*trichiurus_it)->StateControl(&hp);
		//	(*trichiurus_it)->AttackedCheck(&hp);
		//	if ((*trichiurus_it)->IfDead()) {
		//		delete (*trichiurus_it);
		//		trichiurus_es->erase(trichiurus_it);
		//		break;
		//	}
		//	trichiurus_it++;
		//}

		//smallbox_it = smallbox_es->begin();
		//while (smallbox_it != smallbox_es->end()) {
		//	Position temp_p = { game_area_width + 20 ,game_area_height * 3 / 5 };
		//	(*smallbox_it)->IfGotten(&hp, temp_p);
		//	(*smallbox_it)->UpdatePosition();
		//	/*if ((*smallbox_it)->ReachTarget()) {
		//		delete (*smallbox_it);
		//		smallbox_es->erase(smallbox_it);
		//		break;
		//	}*/
		//	smallbox_it++;
		//}

		//etha_it = etha_es->begin();
		//while (etha_it != etha_es->end()) {
		//	Position temp_p = { screen_width - 40 ,screen_height / 5 };
		//	(*etha_it)->IfGotten(&hp, temp_p);
		//	(*etha_it)->UpdatePosition();
		//	/*if ((*etha_it)->ReachTarget()) {
		//		delete (*etha_it);
		//		etha_es->erase(etha_it);
		//		break;
		//	}*/
		//	etha_it++;
		//}

		//enhancedhook_it = enhancedhook_es->begin();
		//while (enhancedhook_it != enhancedhook_es->end()) {
		//	Position temp_p = { screen_width - 60 ,screen_height / 3 };
		//	if ((*enhancedhook_it)->IfGotten(&hp, temp_p)) {
		//		hp.GetEnhancedHook();
		//	}
		//	(*enhancedhook_it)->UpdatePosition();
		//	/*if ((*enhancedhook_it)->ReachTarget()) {
		//		delete (*enhancedhook_it);
		//		enhancedhook_es->erase(enhancedhook_it);
		//		break;
		//	}*/
		//	enhancedhook_it++;
		//}

		//oxygenbottle_it = oxygenbottle_es->begin();
		//while (oxygenbottle_it != oxygenbottle_es->end()) {
		//	Position temp_p = { screen_width - 60 ,2 * screen_height / 3 };
		//	(*oxygenbottle_it)->IfGotten(&hp, temp_p);
		//	(*oxygenbottle_it)->UpdatePosition();
		//	/*if ((*oxygenbottle_it)->ReachTarget()) {
		//		delete (*oxygenbottle_it);
		//		oxygenbottle_es->erase(oxygenbottle_it);
		//		break;
		//	}*/
		//	oxygenbottle_it++;
		//}

		//stab_it = stab_es->begin();
		//while (stab_it != stab_es->end()) {
		//	Position temp_p = { screen_width - 60 ,screen_height / 3 };
		//	it_rect = &(*stab_it)->GetRect();
		//	hp.AttackChecked(*it_rect, 1);
		//	(*stab_it)->UpdatePosition();
		//	/*if ((*stab_it)->ReachTarget()) {
		//		delete (*stab_it);
		//		stab_es->erase(stab_it);
		//		break;
		//	}*/
		//	stab_it++;
		//}

		//if (box != NULL) {
		//	Position temp_p = { game_area_width + 80 ,game_area_height / 3 };
		//	if (box->IfGotten(&hp, temp_p)) {
		//		
		//	}
		//	box->UpdatePosition();
		//}

		if ((OffsetY >= MaxOffsetY) && (upordown == 1)) {
			upordown = 0;
			scroll_speed = -scroll_speed;
		}
		else if ((OffsetY <= 0) && (upordown == 0)) {
			scroll_speed = 0;
		}
		Sleep(13);
	}

	////在游戏结束时要手动释放堆区数据！
	//turtle_it = turtle_es->begin();
	//while (turtle_it != turtle_es->end()) {
	//	if ((*turtle_it) != NULL)delete (*turtle_it);
	//	else turtle_es->erase(turtle_it);
	//	turtle_it++;
	//}
	//vector<Turtle*>().swap(*turtle_es);
	//delete turtle_es;

	//scaleph_it = scaleph_es->begin();
	//while (scaleph_it != scaleph_es->end()) {
	//	if ((*scaleph_it) != NULL)delete (*scaleph_it);
	//	else scaleph_es->erase(scaleph_it);
	//	scaleph_it++;
	//}
	//vector<Scaleph*>().swap(*scaleph_es);
	//delete scaleph_es;

	//redfish_it = redfish_es->begin();
	//while (redfish_it != redfish_es->end()) {
	//	if ((*redfish_it) != NULL)delete (*redfish_it);
	//	else redfish_es->erase(redfish_it);
	//	redfish_it++;
	//}
	//vector<RedFish*>().swap(*redfish_es);
	//delete redfish_es;

	//
	//greenfish_it = greenfish_es->begin();
	//while (greenfish_it != greenfish_es->end()) {
	//	if ((*greenfish_it) != NULL)delete (*greenfish_it);
	//	else greenfish_es->erase(greenfish_it);
	//	greenfish_it++;
	//}
	//vector<GreenFish*>().swap(*greenfish_es);
	//delete greenfish_es;

	//lanternfish_it = lanternfish_es->begin();
	//while (lanternfish_it != lanternfish_es->end()) {
	//	if ((*lanternfish_it) != NULL)delete (*lanternfish_it);
	//	else lanternfish_es->erase(lanternfish_it);
	//	lanternfish_it++;
	//}
	//vector<LanternFish*>().swap(*lanternfish_es);
	//delete lanternfish_es;

	//lobster_it = lobster_es->begin();
	//while (lobster_it != lobster_es->end()) {
	//	if ((*lobster_it) != NULL)delete (*lobster_it);
	//	else lobster_es->erase(lobster_it);
	//	lobster_it++;
	//}
	//vector<Lobster*>().swap(*lobster_es);
	//delete lobster_es;

	//trichiurus_it = trichiurus_es->begin();
	//while (trichiurus_it != trichiurus_es->end()) {
	//	if ((*trichiurus_it) != NULL)delete (*trichiurus_it);
	//	else trichiurus_es->erase(trichiurus_it);
	//	trichiurus_it++;
	//}
	//vector<Trichiurus*>().swap(*trichiurus_es);
	//delete trichiurus_es;

	ReleaseVector(turtle_es);
	ReleaseRouteNodeList(turtle_rnl);

	return true;
}

int main() {
	initgraph(screen_width, screen_height, EX_SHOWCONSOLE);

	bool is_playing = true;

	while (is_playing) {
		Welcome();
		is_playing = Play(1);
	}
	system("pause");
	closegraph();
	return 0;
}
