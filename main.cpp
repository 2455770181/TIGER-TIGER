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
vector<SmallBox*>* CreateSmallBox() {
	Position P = { 10,game_area_height - 50 };
	SmallBox* SB = new SmallBox(P);
	vector<SmallBox*>* smallbox_es = new vector<SmallBox*>();
	smallbox_es->push_back(SB);
	return smallbox_es;
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

//控制敌人的死亡，内存回收，以及可能掉落的道具（无敌时间道具或氧气瓶）
template<typename T>
void ControlEnemyDeath(vector<T*>* es, vector<RouteNode*>* rnl, vector<InvincibleProp*>* invincibleprop_es, vector<OxygenBottle*>* oxygenbottle_es) {
	auto it = es->begin();
	while (it != es->end()) {
		if ((*it)->IfDead()) {
			Position P = (*it)->GetPosition();
			srand((unsigned)time(NULL));
			int temp_rand = rand() % 2;
			if (temp_rand == 0) {
				InvincibleProp* IP = new InvincibleProp(P);
				invincibleprop_es->push_back(IP);
			}
			else if (temp_rand == 1) {
				OxygenBottle* OB = new OxygenBottle(P);
				oxygenbottle_es->push_back(OB);
			}
			
			auto rnl_it = rnl->begin();
			while (rnl_it != rnl->end()) {
				if ((*rnl_it)->id == (*it)->GetID()) {
					delete (*rnl_it);
					rnl->erase(rnl_it);

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

//这个函数用于InvicibleProp和OxygenBottle
template<typename T>
void RemoveObject(vector<T*>* es, bool upordown) {
	int pre_distance = 50;
	auto it = es->begin();
	while (it != es->end()) {
		if ((upordown == 1) && ((*it)->GetRect().bottom <= -pre_distance)) {
			delete (*it);
			es->erase(it);
			return;
		}
		else if ((upordown == 0) && ((*it)->GetRect().top >= game_area_height + pre_distance)) {
			delete (*it);
			es->erase(it);
			return;
		}
		it++;
	}
}

//获取屏幕上的物体的矩形及其对应坐标
template<typename T>
void AddRectAndPosition(vector<T*>* es, vector<RECT*>& rect_es, vector<Position*>& position_es) {
	auto it = es->begin();
	while (it != es->end()) {
		rect_es.push_back(&(*it)->GetRect());
		position_es.push_back(&(*it)->GetPosition());
		it++;
	}
}

//upordown：滚动方向，向上为0，向下为1
template<typename T>
void GenerateObject(vector<T*>* es, vector<RouteNode*>* rnl, int offsetY, bool upordown) {
	auto rnl_it = rnl->begin();
	int pre_distance = 50;
	while (rnl_it != rnl->end()) {
		//offsetY + game_area_height + pre_distance：提前生成，使其缓慢进入视野
		if (((*rnl_it)->offsetY <= offsetY + game_area_height + pre_distance) && ((*rnl_it)->offsetY >= offsetY) && ((*rnl_it)->flag == 0) && (upordown == 1)) {
			(*rnl_it)->SP.y = game_area_height + pre_distance;
			(*rnl_it)->END.y = game_area_height + pre_distance;
			Position node1_P[2] = { (*rnl_it)->SP, (*rnl_it)->END };
			T* node1 = new T(node1_P, (*rnl_it)->id);
			es->push_back(node1);
			(*rnl_it)->flag = 1;
			break;
		}
		else if (((*rnl_it)->offsetY >= offsetY - pre_distance) && ((*rnl_it)->offsetY <= offsetY) && ((*rnl_it)->flag == 0) && (upordown == 0)) {
			(*rnl_it)->SP.y = -pre_distance;
			(*rnl_it)->END.y = -pre_distance;
			Position node2_P[2] = { (*rnl_it)->SP, (*rnl_it)->END };
			T* node2 = new T(node2_P, (*rnl_it)->id);
			es->push_back(node2);
			(*rnl_it)->flag = 1;
			break;
		}
		rnl_it++;
	}
}

template<typename T>
void GenerateProp(vector<T*>* es, vector<RouteNode*>* rnl, int offsetY, bool upordown) {
	auto rnl_it = rnl->begin();
	int pre_distance = 50;
	while (rnl_it != rnl->end()) {
		//offsetY + game_area_height + pre_distance：提前生成，使其缓慢进入视野
		if (((*rnl_it)->offsetY <= offsetY + game_area_height + pre_distance) && ((*rnl_it)->offsetY >= offsetY) && ((*rnl_it)->flag == 0) && (upordown == 1)) {
			(*rnl_it)->SP.y = game_area_height + pre_distance;
			Position node1_P = (*rnl_it)->SP;
			T* node1 = new T(node1_P, (*rnl_it)->id);
			es->push_back(node1);
			(*rnl_it)->flag = 1;
			break;
		}
		else if (((*rnl_it)->offsetY >= offsetY - pre_distance) && ((*rnl_it)->offsetY <= offsetY) && ((*rnl_it)->flag == 0) && (upordown == 0)) {
			cout << "GenerateProp2" << endl;
			(*rnl_it)->SP.y = -pre_distance;
			Position node2_P = (*rnl_it)->SP;
			T* node2 = new T(node2_P, (*rnl_it)->id);
			es->push_back(node2);
			(*rnl_it)->flag = 1;
			break;
		}
		rnl_it++;
	}
}

void GenerateObject(vector<Lobster*>* es, vector<RouteNode*>* rnl, int offsetY, bool upordown) {
	auto rnl_it = rnl->begin();
	int pre_distance = 50;
	while (rnl_it != rnl->end()) {
		//offsetY + game_area_height + pre_distance：提前生成，使其缓慢进入视野
		if (((*rnl_it)->offsetY <= offsetY + game_area_height + pre_distance) && ((*rnl_it)->offsetY >= offsetY) && ((*rnl_it)->flag == 0) && (upordown == 1)) {
			(*rnl_it)->SP.y = game_area_height + pre_distance;
			(*rnl_it)->END.y = game_area_height + pre_distance;
			Position L_P = (*rnl_it)->SP;
			Lobster* L1 = new Lobster(L_P, (*rnl_it)->id);
			es->push_back(L1);
			(*rnl_it)->flag = 1;
			break;
		}
		else if (((*rnl_it)->offsetY >= offsetY - pre_distance) && ((*rnl_it)->offsetY <= offsetY) && ((*rnl_it)->flag == 0) && (upordown == 0)) {
			(*rnl_it)->SP.y = -pre_distance;
			(*rnl_it)->END.y = -pre_distance;
			Position LP = (*rnl_it)->SP;
			Lobster* L2 = new Lobster(LP, (*rnl_it)->id);
			es->push_back(L2);
			(*rnl_it)->flag = 1;
			break;
		}
		rnl_it++;
	}
}

void GenerateObject(vector<Trichiurus*>* es, vector<RouteNode*>* rnl, int offsetY, bool upordown) {
		auto rnl_it = rnl->begin();
		int pre_distance = 50;
		while (rnl_it != rnl->end()) {
			//offsetY + game_area_height + pre_distance：提前生成，使其缓慢进入视野
			if (((*rnl_it)->offsetY <= offsetY + game_area_height + pre_distance) && ((*rnl_it)->offsetY >= offsetY) && ((*rnl_it)->flag == 0) && (upordown == 1)) {
				(*rnl_it)->SP.y = game_area_height + pre_distance;
				(*rnl_it)->END.y = game_area_height + pre_distance;
				Position Tr_P = (*rnl_it)->SP;
				Trichiurus* Tr1 = new Trichiurus(Tr_P, (*rnl_it)->id);
				es->push_back(Tr1);
				(*rnl_it)->flag = 1;
				break;
			}
			else if (((*rnl_it)->offsetY >= offsetY - pre_distance) && ((*rnl_it)->offsetY <= offsetY) && ((*rnl_it)->flag == 0) && (upordown == 0)) {
				(*rnl_it)->SP.y = -pre_distance;
				(*rnl_it)->END.y = -pre_distance;
				Position Tr_P = (*rnl_it)->SP;
				Trichiurus* Tr2 = new Trichiurus(Tr_P, (*rnl_it)->id);
				es->push_back(Tr2);
				(*rnl_it)->flag = 1;
				break;
			}
			rnl_it++;
		}
}

void GenerateObject(vector<LanternFish*>* es, vector<RouteNode*>* rnl, int offsetY, bool upordown) {
	auto rnl_it = rnl->begin();
	int pre_distance = 50;
	while (rnl_it != rnl->end()) {
		//offsetY + game_area_height + pre_distance：提前生成，使其缓慢进入视野
		if (((*rnl_it)->offsetY <= offsetY + game_area_height + pre_distance) && ((*rnl_it)->offsetY >= offsetY) && ((*rnl_it)->flag == 0) && (upordown == 1)) {
			(*rnl_it)->SP.y = game_area_height + pre_distance;
			(*rnl_it)->END.y = game_area_height + pre_distance;
			Position LF_P = (*rnl_it)->SP;
			LanternFish* L1 = new LanternFish(LF_P, (*rnl_it)->id, (*rnl_it)->kind);
			es->push_back(L1);
			(*rnl_it)->flag = 1;
			break;
		}
		else if (((*rnl_it)->offsetY >= offsetY - pre_distance) && ((*rnl_it)->offsetY <= offsetY) && ((*rnl_it)->flag == 0) && (upordown == 0)) {
			(*rnl_it)->SP.y = -pre_distance;
			(*rnl_it)->END.y = -pre_distance;
			Position LF_P = (*rnl_it)->SP;
			LanternFish* L2 = new LanternFish(LF_P, (*rnl_it)->id, (*rnl_it)->kind);
			es->push_back(L2);
			(*rnl_it)->flag = 1;
			break;
		}
		rnl_it++;
	}
}

void ControlObject(vector<Turtle*>* turtle_es, vector<RouteNode*>* rnl, Hero& hp) {
	auto turtle_it = turtle_es->begin();
	RECT* it_rect;
	while (turtle_it != turtle_es->end()) {
		it_rect = &(*turtle_it)->GetRect();
		if (!(*turtle_it)->IfDying() && !(*turtle_it)->IfDead()) hp.AttackChecked(*it_rect);

		(*turtle_it)->UpdatePosition();
		(*turtle_it)->AttackedCheck(&hp);
		turtle_it++;
	}
}

void ControlObject(vector<Scaleph*>* scaleph_es, vector<RouteNode*>* rnl, Hero& hp) {
	auto scaleph_it = scaleph_es->begin();
	RECT* it_rect;
	while (scaleph_it != scaleph_es->end()) {
		it_rect = &(*scaleph_it)->GetRect();
		if (!(*scaleph_it)->IfDying() && !(*scaleph_it)->IfDead()) hp.AttackChecked(*it_rect);

		(*scaleph_it)->UpdatePosition();
		(*scaleph_it)->AttackedCheck(&hp);
		scaleph_it++;
	}
}

void ControlObject(vector<GreenFish*>* greenfish_es, vector<RouteNode*>* rnl, Hero& hp) {
	auto greenfish_it = greenfish_es->begin();
	while (greenfish_it != greenfish_es->end())
	{
		RECT it_rect = (*greenfish_it)->GetRect();
		if (!(*greenfish_it)->IfDying() && !(*greenfish_it)->IfDead()) hp.AttackChecked(it_rect);

		(*greenfish_it)->SpeedControl();
		(*greenfish_it)->UpdateRoute();
		(*greenfish_it)->UpdatePosition();
		(*greenfish_it)->AttackedCheck(&hp);
		greenfish_it++;
	}
}

void ControlObject(vector<RedFish*>* redfish_es, vector<RouteNode*>* rnl, vector<RECT*> rect_es, vector<Position*> position_es, Hero& hp) {
	auto redfish_it = redfish_es->begin();
	while (redfish_it != redfish_es->end()) {
		RECT it_rect = (*redfish_it)->GetRect();
		if (!(*redfish_it)->IfDying() && !(*redfish_it)->IfDead()) hp.AttackChecked(it_rect);

		(*redfish_it)->UpdateRoute(&hp);
		(*redfish_it)->SpeedControl(rect_es, position_es);
		(*redfish_it)->UpdatePosition();
		(*redfish_it)->StateControl(&hp);
		(*redfish_it)->AttackedCheck(&hp);

		redfish_it++;
	}
}

void ControlObject(vector<Lobster*>* lobster_es, vector<RouteNode*>* rnl, Hero& hp) {
	auto lobster_it = lobster_es->begin();
	while (lobster_it != lobster_es->end()) {
		RECT it_rect = (*lobster_it)->GetRect();
		if (!(*lobster_it)->IfDying() && !(*lobster_it)->IfDead()) hp.AttackChecked(it_rect);

		(*lobster_it)->UpdateRoute(&hp);
		(*lobster_it)->SpeedControl();
		(*lobster_it)->UpdatePosition();
		(*lobster_it)->StateControl(&hp);
		(*lobster_it)->AttackedCheck(&hp);
		lobster_it++;
	}
}

void ControlObject(vector<Trichiurus*>* trichiurus_es, vector<RouteNode*>* rnl, Hero& hp) {
	auto trichiurus_it = trichiurus_es->begin();
	while (trichiurus_it != trichiurus_es->end()) {
		RECT it_rect = (*trichiurus_it)->GetRect();
		if (!(*trichiurus_it)->IfDying() && !(*trichiurus_it)->IfDead()) hp.AttackChecked(it_rect);

		(*trichiurus_it)->UpdateRoute(&hp);
		(*trichiurus_it)->SpeedControl();
		(*trichiurus_it)->UpdatePosition();
		(*trichiurus_it)->StateControl(&hp);
		(*trichiurus_it)->AttackedCheck(&hp);
		trichiurus_it++;
	}
}

void ControlObject(vector<LanternFish*>* lanternfish_es, vector<RouteNode*>* rnl, Hero& hp) {
	auto lanternfish_it = lanternfish_es->begin();
	while (lanternfish_it != lanternfish_es->end()) {
		RECT it_rect = (*lanternfish_it)->GetRect();
		if (!(*lanternfish_it)->IfDying() && !(*lanternfish_it)->IfDead()) hp.AttackChecked(it_rect);

		(*lanternfish_it)->UpdateRoute(&hp);
		(*lanternfish_it)->SpeedControl();
		(*lanternfish_it)->UpdatePosition();
		(*lanternfish_it)->StateControl(&hp);
		(*lanternfish_it)->AttackedCheck(&hp);
		lanternfish_it++;
	}

}

void ControlObject(vector<InvincibleProp*>* invincibleprop_es, Hero& hp) {
	auto invincibleprop_it = invincibleprop_es->begin();
	while (invincibleprop_it != invincibleprop_es->end()) {
		Position temp_p = (*invincibleprop_it)->GetPosition();
		if ((*invincibleprop_it)->IfGotten(&hp, temp_p)) {
			hp.GetInvicibleTime();
		}
		(*invincibleprop_it)->UpdatePosition();
		if ((*invincibleprop_it)->ReachTarget()) {
			delete (*invincibleprop_it);
			invincibleprop_es->erase(invincibleprop_it);
			break;
		}
		invincibleprop_it++;
	}
}

void ControlObject(vector<OxygenBottle*>* oxygenbottle_es, Hero& hp) {
	auto oxygenbottle_it = oxygenbottle_es->begin();
	while (oxygenbottle_it != oxygenbottle_es->end()) {
		Position temp_p = (*oxygenbottle_it)->GetPosition();
		if ((*oxygenbottle_it)->IfGotten(&hp, temp_p)) {
			hp.GetOxygenBottle();
		}
		(*oxygenbottle_it)->UpdatePosition();
		if ((*oxygenbottle_it)->ReachTarget()) {
			delete (*oxygenbottle_it);
			oxygenbottle_es->erase(oxygenbottle_it);
			break;
		}
		oxygenbottle_it++;
	}
}

void ControlObject(vector<Box*>* box_es, Hero& hp) {
	Position temp_p = { game_area_width + 65 ,game_area_height / 3 + 60 };
	auto box_it = box_es->begin();
	while (box_it != box_es->end()) {
		(*box_it)->IfGotten(&hp, temp_p);
		(*box_it)->UpdatePosition();
		if ((*box_it)->ReachTarget()) {
			hp.GetBox();
			delete (*box_it);
			box_es->erase(box_it);
			break;
		}
		box_it++;
	}
}

void ControlObject(vector<Ether*>* ether_es, Hero& hp) {
	Position temp_p = { screen_width - 80 ,screen_height * 0.14 };
	auto ether_it = ether_es->begin();
	while (ether_it != ether_es->end()) {
		(*ether_it)->IfGotten(&hp, temp_p);
		(*ether_it)->UpdatePosition();
		if ((*ether_it)->ReachTarget()) {
			hp.GetEther();
			delete (*ether_it);
			ether_es->erase(ether_it);
			break;
		}
		ether_it++;
	}
}

void ControlObject(vector<SmallBox*>* smallbox_es, Hero& hp) {
	auto smallbox_it = smallbox_es->begin();
	Position P0 = { game_area_width + 75 ,game_area_height * 3 / 5 - 30 };
	Position P;
	unsigned int n = 0;
	while (smallbox_it != smallbox_es->end()) {
		n = hp.GetStageProp().smallbox + 1;	//计算获得的小箱子是hp的第几个箱子，用以设置其移动的目标位置
		P.x = (n % 2 == 0) ? P0.x + 50 : P0.x;
		P.y = P0.y + floor((n - 1) / 2) * 50;
		(*smallbox_it)->IfGotten(&hp, P);
		(*smallbox_it)->UpdatePosition();
		if ((*smallbox_it)->ReachTarget()) {
			hp.GetSmallBox();
			delete (*smallbox_it);
			smallbox_es->erase(smallbox_it);
			break;
		}
		smallbox_it++;
		n++;
	}
}

void ControlObject(vector<EnhancedHook*>* enhancedhook_es, Hero& hp) {
	Position temp_p = { screen_width - 50 ,screen_height / 3 + 60 };
	auto enhancedhook_it = enhancedhook_es->begin();
	while (enhancedhook_it != enhancedhook_es->end()) {
		(*enhancedhook_it)->IfGotten(&hp, temp_p);
		(*enhancedhook_it)->UpdatePosition();
		if ((*enhancedhook_it)->ReachTarget()) {
			hp.GetEnhancedHook();
			delete (*enhancedhook_it);
			enhancedhook_es->erase(enhancedhook_it);
			break;
		}
		enhancedhook_it++;
	}
}

void ControlObject(vector<Stab*>* stab_es, Hero& hp) {
	auto stab_it = stab_es->begin();
	while (stab_it != stab_es->end()) {
		RECT it_rect = (*stab_it)->GetRect();
		hp.AttackChecked(it_rect, 1);
		(*stab_it)->UpdatePosition();
		//由于碰到水雷后hp就死亡，所以这里不再对stab_es做处理
		stab_it++;
	}
}

//es的0、1为smallbox的原图和掩码图，2、3为enhancedbox的原图和掩码图，4、5为box的原图和掩码图
void DrawStateArea(IMAGE* imglist, Hero& hp) {
	StageProperty SP = hp.GetStageProp();
	Position P0 = { game_area_width + 75 ,game_area_height * 3 / 5 - 30 };
	Position P;
	for (int i = 1;i <= SP.smallbox;i++) {
		P.x = (i % 2 == 0) ? P0.x + 50 : P0.x;
		P.y = P0.y + floor((i - 1) / 2) * 50;
		putimage(P.x - imglist[1].getwidth() / 2, P.y - imglist[1].getheight() / 2, &imglist[1], SRCAND);
		putimage(P.x - imglist[0].getwidth() / 2, P.y - imglist[0].getheight() / 2, &imglist[0], SRCPAINT);
	}
	if (SP.enhanced_hook) {
		P = { screen_width - 50 ,screen_height / 3 + 60 };
		putimage(P.x - imglist[3].getwidth() / 2, P.y - imglist[3].getheight() / 2, &imglist[3], SRCAND);
		putimage(P.x - imglist[2].getwidth() / 2, P.y - imglist[2].getheight() / 2, &imglist[2], SRCPAINT);
	}
	if (SP.box) {
		P = { game_area_width + 65 ,game_area_height / 3 + 60 };
		putimage(P.x - imglist[5].getwidth() / 2, P.y - imglist[5].getheight() / 2, &imglist[5], SRCAND);
		putimage(P.x - imglist[4].getwidth() / 2, P.y - imglist[4].getheight() / 2, &imglist[4], SRCPAINT);
	}
	int linelength = 100 * hp.GetCurrentHP() / hero_HP;
	setcolor(RGB(189, 227, 212));	//设置前景色，用以填充线条
	fillroundrect(game_area_width + 20, 170, game_area_width + 185, 180, 1, 1);
	setcolor(RGB(57, 219, 216));	//设置前景色，用以填充线条
	line(game_area_width + 22  , 175, game_area_width + 83 + linelength, 175);
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
	box_area.bottom = state_area.bottom - 50;
	box_area.top = state_area.top + 330;
	//为了减少内存使用，状态区的物体只需要显示图片，不需要创建相应的对象
	IMAGE state_area_img[6];
	IMAGE smallbox_image;
	IMAGE smallbox_mask;
	loadimage(&smallbox_image, _T("./image/prop/smallbox_i.png"));
	loadimage(&smallbox_mask, _T("./image/prop/smallbox_m.png"));
	state_area_img[0] = smallbox_image;
	state_area_img[1] = smallbox_mask;
	IMAGE enhancedhook_image;
	IMAGE enhancedhook_mask;
	loadimage(&enhancedhook_image, _T("./image/prop/enhanced_hook_i.png"));
	loadimage(&enhancedhook_mask, _T("./image/prop/enhanced_hook_m.png"));
	state_area_img[2] = enhancedhook_image;
	state_area_img[3] = enhancedhook_mask;
	IMAGE box_image;
	IMAGE box_mask;
	loadimage(&box_image, _T("./image/prop/box_i.png"));
	loadimage(&box_mask, _T("./image/prop/box_m.png"));
	state_area_img[4] = box_image;
	state_area_img[5] = box_mask;
	
	const int MaxOffsetY = 6500;	//地图总长度
	int OffsetY = 0;	//用于记录背景图已经滚动的总路程

	bool upordown = 1;	//向上为0，向下为1

	Hero hp = Hero();
	RECT hp_rect = hp.GetRect();

	//存储当前窗口内需要显示的敌人
	vector<Turtle*>* turtle_es = new vector<Turtle*>();
	vector<RouteNode*>* turtle_rnl = ReadRouteNodeList("./enemy_data/Turtle.txt");

	vector<Scaleph*>* scaleph_es = new vector<Scaleph*>();
	vector<RouteNode*>* scaleph_rnl = ReadRouteNodeList("./enemy_data/Scaleph.txt");

	vector<GreenFish*>* greenfish_es = new vector<GreenFish*>();
	vector<RouteNode*>* greenfish_rnl = ReadRouteNodeList("./enemy_data/GreenFish.txt");

	vector<RedFish*>* redfish_es = new vector<RedFish*>();
	vector<RouteNode*>* redfish_rnl = ReadRouteNodeList("./enemy_data/RedFish.txt");

	vector<Lobster*>* lobster_es = new vector<Lobster*>();
	vector<RouteNode*>* lobster_rnl = ReadRouteNodeList("./enemy_data/Lobster.txt");

	vector<Trichiurus*>* trichiurus_es = new vector<Trichiurus*>();
	vector<RouteNode*>* trichiurus_rnl = ReadRouteNodeList("./enemy_data/Trichiurus.txt");

	vector<LanternFish*>* lanternFish_es = new vector<LanternFish*>();
	vector<RouteNode*>* lanternFish_rnl = ReadRouteNodeList("./enemy_data/LanternFish.txt");

	//存储当前窗口内需要显示的道具
	vector<InvincibleProp*>* invincibleprop_es = new vector<InvincibleProp*>();	//无敌道具在敌人被消灭后概率掉落，该概率设为10%

	vector<OxygenBottle*>* oxygenbottle_es = new vector<OxygenBottle*>();	//氧气瓶在敌人被消灭后概率掉落，该概率设为10%

	vector<Box*>* box_es = new vector<Box*>();
	vector<RouteNode*>* box_rnl = ReadRouteNodeList("./prop_data/Box.txt");

	vector<Ether*>* ether_es = new vector<Ether*>();
	vector<RouteNode*>* ether_rnl = ReadRouteNodeList("./prop_data/Ether.txt");

	vector<SmallBox*>* smallbox_es = new vector<SmallBox*>();
	vector<RouteNode*>* smallbox_rnl = ReadRouteNodeList("./prop_data/SmallBox.txt");

	vector<Stab*>* stab_es = new vector<Stab*>();
	vector<RouteNode*>* stab_rnl = ReadRouteNodeList("./prop_data/Stab.txt");

	vector<EnhancedHook*>* enhancedhook_es = new vector<EnhancedHook*>();
	vector<RouteNode*>* enhancedhook_rnl = ReadRouteNodeList("./prop_data/EnhancedHook.txt");

	/*
	Position W1_P1 = { 450,450 };
	Wall_1* wall_1_1 = new Wall_1(W1_P1);
	vector<Wall_1*> wall_1_es;
	auto wall_1_it = wall_1_es.begin();
	wall_1_es.push_back(wall_1_1);
	*/

	setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 5);
	setlinecolor(BLACK);

	clock_t start_time = clock();
	int preparation_period = 3000;

	while (playing) {
		GenerateObject(turtle_es, turtle_rnl, OffsetY, upordown);
		GenerateObject(scaleph_es, scaleph_rnl, OffsetY, upordown);
		GenerateObject(greenfish_es, greenfish_rnl, OffsetY, upordown);
		GenerateObject(redfish_es, redfish_rnl, OffsetY, upordown);
		GenerateObject(lobster_es, lobster_rnl, OffsetY, upordown);
		GenerateObject(trichiurus_es, trichiurus_rnl, OffsetY, upordown);
		GenerateObject(lanternFish_es, lanternFish_rnl, OffsetY, upordown);
		GenerateProp(ether_es, ether_rnl, OffsetY, upordown);
		GenerateProp(smallbox_es, smallbox_rnl, OffsetY, upordown);
		GenerateProp(enhancedhook_es, enhancedhook_rnl, OffsetY, upordown);
		GenerateProp(stab_es, stab_rnl, OffsetY, upordown);
		GenerateProp(box_es, box_rnl, OffsetY, upordown);

		BeginBatchDraw();

		setbkcolor(WHITE);
		cleardevice();	//这个函数使用当前背景色清空绘图设备
		bk.Show(upordown);
		OffsetY -= scroll_speed;
		hp.Attack();
		hp.Show();

		/*
		wall_1_it = wall_1_es.begin();
		while (wall_1_it != wall_1_es.end()) {
			(*wall_1_it)->Show();
			wall_1_it++;
		}*/
		Show(turtle_es);
		Show(scaleph_es);
		Show(greenfish_es);
		Show(lobster_es);
		Show(trichiurus_es);
		Show(lanternFish_es);
		Show(redfish_es);

		setfillcolor(RGB(19, 69, 71));	//设置填充颜色
		solidrectangle(state_area.left, state_area.top, state_area.right, state_area.bottom);
		setcolor(RGB(189, 227, 212));
		outtextxy(state_area.left + 0.5 * text_margin_left, state_area.top + text_margin_top, SCORE);
		outtextxy(state_area.left + 0.5 * text_margin_left, state_area.top + text_margin_top + text_height, lpStr_score);
		outtextxy(state_area.left + 0.5 * text_margin_left, state_area.top + text_margin_top + text_height * 2, ETHER);
		outtextxy(state_area.left + 0.5 * text_margin_left, state_area.top + text_margin_top + text_height * 3, lpStr_ether);
		outtextxy(state_area.left + 0.5 * text_margin_left, state_area.top + text_margin_top + text_height * 4, AIR);

		setcolor(RGB(208, 255, 248));	//设置前景色，用以填充线条
		fillroundrect(box_area.left, box_area.top, box_area.right, box_area.bottom, 1, 1);

		Show(invincibleprop_es);
		Show(oxygenbottle_es);
		Show(ether_es);
		Show(smallbox_es);
		Show(enhancedhook_es);
		Show(stab_es);
		Show(box_es);

		DrawStateArea(state_area_img, hp);

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
		//存储当前窗口内需要检测碰撞的物体的矩形
		vector<RECT*>rect_es;
		//存储当前窗口内需要检测碰撞的物体的位置
		vector<Position*>position_es;

		ControlObject(turtle_es, turtle_rnl, hp);
		ControlObject(scaleph_es, scaleph_rnl, hp);
		ControlObject(greenfish_es, greenfish_rnl, hp);
		ControlObject(lobster_es, lobster_rnl, hp);
		ControlObject(trichiurus_es, trichiurus_rnl, hp);
		ControlObject(lanternFish_es, lanternFish_rnl, hp);
		AddRectAndPosition(turtle_es, rect_es, position_es);
		AddRectAndPosition(scaleph_es, rect_es, position_es);
		AddRectAndPosition(greenfish_es, rect_es, position_es);
		AddRectAndPosition(lobster_es, rect_es, position_es);
		AddRectAndPosition(trichiurus_es, rect_es, position_es);
		AddRectAndPosition(lanternFish_es, rect_es, position_es);
		ControlObject(redfish_es, redfish_rnl, rect_es, position_es, hp);

		ControlEnemyDeath(turtle_es, turtle_rnl, invincibleprop_es, oxygenbottle_es);
		ControlEnemyDeath(scaleph_es, scaleph_rnl, invincibleprop_es, oxygenbottle_es);
		ControlEnemyDeath(greenfish_es, greenfish_rnl, invincibleprop_es, oxygenbottle_es);
		ControlEnemyDeath(redfish_es, redfish_rnl, invincibleprop_es, oxygenbottle_es);
		ControlEnemyDeath(lobster_es, lobster_rnl, invincibleprop_es, oxygenbottle_es);
		ControlEnemyDeath(trichiurus_es, trichiurus_rnl, invincibleprop_es, oxygenbottle_es);
		ControlEnemyDeath(lanternFish_es, lanternFish_rnl, invincibleprop_es, oxygenbottle_es);
		ControlObject(invincibleprop_es, hp);
		ControlObject(oxygenbottle_es, hp);
		ControlObject(ether_es, hp);
		ControlObject(smallbox_es, hp);
		ControlObject(enhancedhook_es, hp);
		ControlObject(stab_es, hp);
		ControlObject(box_es, hp);

		RemoveObject(turtle_es, turtle_rnl, upordown);
		RemoveObject(scaleph_es, scaleph_rnl, upordown);
		RemoveObject(greenfish_es, greenfish_rnl, upordown);
		RemoveObject(redfish_es, redfish_rnl, upordown);
		RemoveObject(lobster_es, lobster_rnl, upordown);
		RemoveObject(trichiurus_es, trichiurus_rnl, upordown);
		RemoveObject(lanternFish_es, lanternFish_rnl, upordown);
		RemoveObject(invincibleprop_es, upordown);
		RemoveObject(oxygenbottle_es, upordown);
		RemoveObject(ether_es, ether_rnl, upordown);
		RemoveObject(smallbox_es, smallbox_rnl, upordown);
		RemoveObject(enhancedhook_es, enhancedhook_rnl, upordown);
		RemoveObject(stab_es, stab_rnl, upordown);
		RemoveObject(box_es, box_rnl, upordown);

		/*
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
		*/

		if ((OffsetY >= MaxOffsetY - game_area_height) && (upordown == 1)) {
			if (clock() - start_time < preparation_period) scroll_speed = 0;
			else scroll_speed = max_scroll_speed;
			if ((hp.GetStageProp().box > 0) && (box_es->begin() == box_es->end())) {
				start_time = clock();
				if (clock() - start_time < preparation_period) {
					upordown = 0;
					scroll_speed = -max_scroll_speed;
				}
				else scroll_speed = 0;
			}
			else {
				scroll_speed = 0;
			}
		}
		else if ((OffsetY <= 0) && (upordown == 0)) {
			scroll_speed = 0;
		}
		if (hp.IfDying()) scroll_speed = 0;
		Sleep(13);
	}

	//在游戏结束时要手动释放堆区数据！
	ReleaseVector(turtle_es);
	ReleaseRouteNodeList(turtle_rnl);
	ReleaseVector(scaleph_es);
	ReleaseRouteNodeList(scaleph_rnl);
	ReleaseVector(greenfish_es);
	ReleaseRouteNodeList(greenfish_rnl);
	ReleaseVector(redfish_es);
	ReleaseRouteNodeList(redfish_rnl);
	ReleaseVector(lobster_es);
	ReleaseRouteNodeList(lobster_rnl);
	ReleaseVector(trichiurus_es);
	ReleaseRouteNodeList(trichiurus_rnl);
	ReleaseVector(lanternFish_es);
	ReleaseRouteNodeList(lanternFish_rnl);
	ReleaseVector(invincibleprop_es);
	ReleaseVector(oxygenbottle_es);
	ReleaseVector(ether_es);
	ReleaseRouteNodeList(ether_rnl);
	ReleaseVector(smallbox_es);
	ReleaseRouteNodeList(smallbox_rnl);
	ReleaseVector(enhancedhook_es);
	ReleaseRouteNodeList(enhancedhook_rnl);
	ReleaseVector(stab_es);
	ReleaseRouteNodeList(stab_rnl);
	ReleaseVector(box_es);
	ReleaseRouteNodeList(box_rnl);

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
