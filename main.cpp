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

	while (true) {
		ExMessage* mess = new ExMessage;
		getmessage(mess, EM_MOUSE);
		if (mess->lbutton) {
			cout << "mess.x:" << mess->x << "，mess.y:" << mess->y << endl;
			if (PointInRect(mess->x, mess->y, tplayr)) {
				delete mess;
				return;	//直接返回到main中的循环
			}
			else if (PointInRect(mess->x, mess->y, texitr)) {
				delete mess;
				exit(0);
			}
		}
	}
}

class BK {
public:
	BK(IMAGE& img) :img(img), y(-(img.getheight() - screen_height)) {

	}

	void Show() {
		//if (y == 0) {
		//	y = -screen_height;
		//}
		//y += 5;
		putimage(0, y, &img);	//将图片的左上角放到指定坐标，然后绘制图片
	}
private:
	IMAGE& img;
	int y;	//绘制位置的 y 坐标
};

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

bool Play() {
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
	box_area.left = state_area.left + 40;
	box_area.right = state_area.right - 40;
	box_area.bottom = 600 ;
	box_area.top = 330;

	Hero hp = Hero();
	RECT hp_rect = hp.GetRect();

	vector<Turtle*>* turtle_es = CreateTurtle();
	auto turtle_it = turtle_es->begin();

	vector<Scaleph*>* scaleph_es = CreateScaleph();
	auto scaleph_it = scaleph_es->begin();

	vector<RedFish*>* redfish_es = CreateRedFish();
	auto redfish_it = redfish_es->begin();
	
	vector<GreenFish*>* greenfish_es = CreateGreenFish();
	auto greenfish_it = greenfish_es->begin();

	vector<LanternFish*>* lanternfish_es = CreateLanternFish();
	auto lanternfish_it = lanternfish_es->begin();

	vector<SmallBox*>* smallbox_es = CreateSmallBox();
	auto smallbox_it = smallbox_es->begin();
	vector<Etha*>* etha_es = CreateEtha();
	auto etha_it = etha_es->begin();
	vector<EnhancedHook*>* enhancedhook_es = CreateEnhancedHook();
	auto enhancedhook_it = enhancedhook_es->begin();
	vector<Stab*>* stab_es = CreateStab();
	auto stab_it = stab_es->begin();
	Box* box = CreateBox();

	Position W1_P1 = { 450,450 };
	Wall_1* wall_1_1 = new Wall_1(W1_P1);
	vector<Wall_1*> wall_1_es;
	auto wall_1_it = wall_1_es.begin();
	wall_1_es.push_back(wall_1_1);

	setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 5);
	setlinecolor(BLACK);

	while (playing) {
		BeginBatchDraw();

		setbkcolor(WHITE);
		cleardevice();	//这个函数使用当前背景色清空绘图设备
		bk.Show();
		hp.Attack();
		hp.Show();
		
		RECT* it_rect;	//临时存储矩形

		turtle_it = turtle_es->begin();
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
		wall_1_it = wall_1_es.begin();
		while (wall_1_it != wall_1_es.end()) {
			(*wall_1_it)->Show();
			wall_1_it++;
		}

		setfillcolor(RGB(19, 69, 71));	//设置填充颜色
		solidrectangle(state_area.left, state_area.top, state_area.right, state_area.bottom);
		outtextxy(state_area.left +0.5* text_margin_left, state_area.top + text_margin_top, SCORE);
		outtextxy(state_area.left + 0.5*text_margin_left, state_area.top + text_margin_top + text_height, lpStr_score);
		outtextxy(state_area.left + 0.5*text_margin_left, state_area.top + text_margin_top + text_height * 2, ETHER);
		outtextxy(state_area.left + 0.5*text_margin_left, state_area.top + text_margin_top + text_height * 3, lpStr_ether);

		setcolor(RGB(208, 255, 248));	//设置前景色，用以填充线条
		fillroundrect(box_area.left, box_area.top, box_area.right, box_area.bottom, 1, 1);

		smallbox_it = smallbox_es->begin();
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
		stab_it = stab_es->begin();
		while (stab_it != stab_es->end()) {
			(*stab_it)->Show();
			stab_it++;
		}
		if (box != NULL)box->Show();

		EndBatchDraw();

		hp.Control();

		if (hp.IfDead()) {
			cout << "Hero死亡" << endl;
			break;
		}

		wall_1_it = wall_1_es.begin();
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
		}

		turtle_it = turtle_es->begin();
		while (turtle_it != turtle_es->end()) {
			it_rect = &(*turtle_it)->GetRect();
			if (!(*turtle_it)->IfDying() && !(*turtle_it)->IfDead()) hp.AttackChecked(*it_rect);

			(*turtle_it)->UpdatePosition();
			(*turtle_it)->AttackedCheck(&hp);
			if ((*turtle_it)->IfDead()) {
				delete (*turtle_it);
				turtle_es->erase(turtle_it);
				break;
			}
			turtle_it++;
		}

		scaleph_it = scaleph_es->begin();
		while (scaleph_it != scaleph_es->end()) {
			it_rect = &(*scaleph_it)->GetRect();
			if (!(*scaleph_it)->IfDying() && !(*scaleph_it)->IfDead()) hp.AttackChecked(*it_rect);

			(*scaleph_it)->UpdatePosition();
			(*scaleph_it)->AttackedCheck(&hp);
			if ((*scaleph_it)->IfDead()) {
				delete (*scaleph_it);
				scaleph_es->erase(scaleph_it);
				break;
			}
			scaleph_it++;
		}

		redfish_it = redfish_es->begin();
		while (redfish_it != redfish_es->end()) {
			it_rect = &(*redfish_it)->GetRect();
			if (!(*redfish_it)->IfDying() && !(*redfish_it)->IfDead()) hp.AttackChecked(*it_rect);
			//获取屏幕上的物体的矩形
			vector<RECT*>rect_es;
			turtle_it = turtle_es->begin();
			while (turtle_it != turtle_es->end()) {
				rect_es.push_back(&(*turtle_it)->GetRect());
				turtle_it++;
			}
			scaleph_it = scaleph_es->begin();
			while (scaleph_it != scaleph_es->end()) {
				rect_es.push_back(&(*scaleph_it)->GetRect());
				scaleph_it++;
			}
			wall_1_it = wall_1_es.begin();
			while (wall_1_it != wall_1_es.end()) {
				vector<RECT*> temp_list = (*wall_1_it)->GetRects();
				auto temp_it = temp_list.begin();
				while (temp_it != temp_list.end()) {
					rect_es.push_back(*temp_it);
					temp_it++;
				}
				wall_1_it++;
			}
			//获取屏幕上的物体的位置
			vector<Position*>position_es;
			turtle_it = turtle_es->begin();
			while (turtle_it != turtle_es->end()) {
				position_es.push_back(&(*turtle_it)->GetPosition());
				turtle_it++;
			}
			scaleph_it = scaleph_es->begin();
			while (scaleph_it != scaleph_es->end()) {
				position_es.push_back(&(*scaleph_it)->GetPosition());
				scaleph_it++;
			}
			wall_1_it = wall_1_es.begin();
			while (wall_1_it != wall_1_es.end()) {
				vector<Position*> temp_list = (*wall_1_it)->GetPositions();
				auto temp_it = temp_list.begin();
				while (temp_it != temp_list.end()) {
					position_es.push_back(*temp_it);
					temp_it++;
				}
				wall_1_it++;
			}

			(*redfish_it)->UpdateRoute(&hp);
			(*redfish_it)->SpeedControl(rect_es, position_es);
			(*redfish_it)->UpdatePosition();
			(*redfish_it)->StateControl(&hp);
			(*redfish_it)->AttackedCheck(&hp);
			if ((*redfish_it)->IfDead()) {
				delete (*redfish_it);
				redfish_es->erase(redfish_it);
				break;
			}
			redfish_it++;
		}

		greenfish_it = greenfish_es->begin();
		while (greenfish_it!=greenfish_es->end())
		{
			it_rect = &(*greenfish_it)->GetRect();
			if (!(*greenfish_it)->IfDying() && !(*greenfish_it)->IfDead()) hp.AttackChecked(*it_rect);

			(*greenfish_it)->SpeedControl();
			(*greenfish_it)->UpdateRoute();
			(*greenfish_it)->UpdatePosition();
			(*greenfish_it)->AttackedCheck(&hp);
			if ((*greenfish_it)->IfDead()) {
				delete (*greenfish_it);
				greenfish_es->erase(greenfish_it);
				break;
			}
			greenfish_it++;
		}

		lanternfish_it = lanternfish_es->begin();
		while (lanternfish_it != lanternfish_es->end()) {
			it_rect = &(*lanternfish_it)->GetRect();
			if (!(*lanternfish_it)->IfDying() && !(*lanternfish_it)->IfDead()) hp.AttackChecked(*it_rect);

			(*lanternfish_it)->UpdateRoute(&hp);
			(*lanternfish_it)->SpeedControl();
			(*lanternfish_it)->UpdatePosition();
			(*lanternfish_it)->StateControl(&hp);
			(*lanternfish_it)->AttackedCheck(&hp);
			if ((*lanternfish_it)->IfDead()) {
				delete (*lanternfish_it);
				lanternfish_es->erase(lanternfish_it);
				break;
			}
			lanternfish_it++;
		}

		smallbox_it = smallbox_es->begin();
		while (smallbox_it != smallbox_es->end()) {
			Position temp_p = { game_area_width + 75  ,game_area_height * 3/ 5 -30 };
			(*smallbox_it)->IfGotten(&hp, temp_p);
			(*smallbox_it)->UpdatePosition();
			/*if ((*smallbox_it)->ReachTarget()) {
				delete (*smallbox_it);
				smallbox_es->erase(smallbox_it);
				break;
			}*/
			smallbox_it++;
		}

		etha_it = etha_es->begin();
		while (etha_it != etha_es->end()) {
			Position temp_p = { screen_width - 80,screen_height * 0.14 };
			(*etha_it)->IfGotten(&hp, temp_p);
			(*etha_it)->UpdatePosition();
			/*if ((*etha_it)->ReachTarget()) {
				delete (*etha_it);
				etha_es->erase(etha_it);
				break;
			}*/
			etha_it++;
		}

		enhancedhook_it = enhancedhook_es->begin();
		while (enhancedhook_it != enhancedhook_es->end()) {
			Position temp_p = { screen_width - 50 ,screen_height / 3 +60 };
			if ((*enhancedhook_it)->IfGotten(&hp, temp_p)) {
				hp.GetEnhancedHook();
			}
			(*enhancedhook_it)->UpdatePosition();
			/*if ((*enhancedhook_it)->ReachTarget()) {
				delete (*enhancedhook_it);
				enhancedhook_es->erase(enhancedhook_it);
				break;
			}*/
			enhancedhook_it++;
		}

		stab_it = stab_es->begin();
		while (stab_it != stab_es->end()) {
			Position temp_p = { screen_width - 60 ,screen_height / 3 };
			it_rect = &(*stab_it)->GetRect();
			hp.AttackChecked(*it_rect, 1);
			(*stab_it)->UpdatePosition();
			/*if ((*stab_it)->ReachTarget()) {
				delete (*stab_it);
				stab_es->erase(stab_it);
				break;
			}*/
			stab_it++;
		}

		if (box != NULL) {
			Position temp_p = { game_area_width + 65 ,game_area_height / 3 + 60};
			if (box->IfGotten(&hp, temp_p)) {
				
			}
			box->UpdatePosition();
		}

		Sleep(13);
	}

	//在游戏结束时要手动释放堆区数据！
	turtle_it = turtle_es->begin();
	while (turtle_it != turtle_es->end()) {
		if ((*turtle_it) != NULL)delete (*turtle_it);
		else turtle_es->erase(turtle_it);
		turtle_it++;
	}
	vector<Turtle*>().swap(*turtle_es);
	delete turtle_es;

	scaleph_it = scaleph_es->begin();
	while (scaleph_it != scaleph_es->end()) {
		if ((*scaleph_it) != NULL)delete (*scaleph_it);
		else scaleph_es->erase(scaleph_it);
		scaleph_it++;
	}
	vector<Scaleph*>().swap(*scaleph_es);
	delete scaleph_es;

	redfish_it = redfish_es->begin();
	while (redfish_it != redfish_es->end()) {
		if ((*redfish_it) != NULL)delete (*redfish_it);
		else redfish_es->erase(redfish_it);
		redfish_it++;
	}
	vector<RedFish*>().swap(*redfish_es);
	delete redfish_es;

	
	greenfish_it = greenfish_es->begin();
	while (greenfish_it != greenfish_es->end()) {
		if ((*greenfish_it) != NULL)delete (*greenfish_it);
		else greenfish_es->erase(greenfish_it);
		greenfish_it++;
	}
	vector<GreenFish*>().swap(*greenfish_es);
	delete greenfish_es;

	lanternfish_it = lanternfish_es->begin();
	while (lanternfish_it != lanternfish_es->end()) {
		if ((*lanternfish_it) != NULL)delete (*lanternfish_it);
		else lanternfish_es->erase(lanternfish_it);
		lanternfish_it++;
	}
	vector<LanternFish*>().swap(*lanternfish_es);
	delete lanternfish_es;

	return true;
}

int main() {
	initgraph(screen_width, screen_height, EX_SHOWCONSOLE);

	bool is_playing = true;

	while (is_playing) {
		Welcome();
		is_playing = Play();
	}
	system("pause");
	closegraph();
	return 0;
}
