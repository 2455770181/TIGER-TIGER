#pragma once
#include<graphics.h>
#include<vector>
#include<math.h>
#include<iostream>
#include"GlobalSettings.h"
using namespace std;

struct StageProperty {
	unsigned int box;
	unsigned int smallbox;
	unsigned int ether;
	bool invincible_time;
	bool enhanced_hook;
};

class Hook {
public:
	Hook() {
		loadimage(&hook_img, _T("./image/hook.png"), 21, 27, true);
		loadimage(&hook_mask, _T("./image/hook_mask.png"), 21, 27, true);
		rect_length = 20;
		position.x = 0;
		position.y = 0;

		hook_direction = 0;
		rect.left = position.x - rect_length / 2;
		rect.right = position.x + rect_length / 2;
		rect.bottom = position.y + rect_length / 2;
		rect.top = position.y - rect_length / 2;
	}

	void Rotate(int direction) {
		hook_direction = direction;
		switch (direction)
		{
			//��
		case(1): {
			rotateimage(&hook_i_rot, &hook_img, PI / 2, BLACK, true);
			rotateimage(&hook_m_rot, &hook_mask, PI / 2, WHITE, true);
			break;
		}
			   //��
		case(2): {
			rotateimage(&hook_i_rot, &hook_img, -PI / 2, BLACK, true);
			rotateimage(&hook_m_rot, &hook_mask, -PI / 2, WHITE, true);
			break;
		}
			   //��
		case(3): {
			rotateimage(&hook_i_rot, &hook_img, PI, BLACK, true);
			rotateimage(&hook_m_rot, &hook_mask, PI, WHITE, true);
			break;
		}
			   //��
		case(4): {
			rotateimage(&hook_i_rot, &hook_img, 0, BLACK, true);
			rotateimage(&hook_m_rot, &hook_mask, 0, WHITE, true);
			break;
		}
			   //����
		case(5): {
			rotateimage(&hook_i_rot, &hook_img, PI / 4, BLACK, true);
			rotateimage(&hook_m_rot, &hook_mask, PI / 4, WHITE, true);
			break;
		}
			   //����
		case(6): {
			rotateimage(&hook_i_rot, &hook_img, -PI / 4, BLACK, true);
			rotateimage(&hook_m_rot, &hook_mask, -PI / 4, WHITE, true);
			break;
		}
			   //����
		case(7): {
			rotateimage(&hook_i_rot, &hook_img, -PI * 3 / 4, BLACK, true);
			rotateimage(&hook_m_rot, &hook_mask, -PI * 3 / 4, WHITE, true);
			break;
		}
			   //����
		case(8): {
			rotateimage(&hook_i_rot, &hook_img, PI * 3 / 4, BLACK, true);
			rotateimage(&hook_m_rot, &hook_mask, PI * 3 / 4, WHITE, true);
			break;
		}
		default:
			break;
		}
	}

	void UpdatePosition(int x, int y) {
		position.x = x;
		position.y = y;
		//x,yΪͼƬ��������
		rect.left = x - rect_length / 2;
		rect.right = x + rect_length / 2;
		rect.bottom = y + rect_length / 2;
		rect.top = y - rect_length / 2;
	}

	IMAGE hook_img, hook_mask;
	IMAGE hook_i_rot, hook_m_rot;
	RECT rect;
	int hook_direction;
	Position position;	//hook���ĵ�����
	int rect_length;	//hook�ľ��η���߳�
};

class Hero {
public:
	Hero() :HP(hero_HP) {
		for (int i = 0;i < 4;i++) {
			direction_check[i] = 0;
		}
		//��
		loadimage(&img_list[0], _T("./image/hero/down1_i.png"), 60, 60);
		loadimage(&img_list[1], _T("./image/hero/down2_i.png"), 60, 60);
		loadimage(&img_mask[0], _T("./image/hero/down1_m.png"), 60, 60);
		loadimage(&img_mask[1], _T("./image/hero/down2_m.png"), 60, 60);
		//��
		loadimage(&img_list[2], _T("./image/hero/up1_i.png"), 60, 60);
		loadimage(&img_list[3], _T("./image/hero/up2_i.png"), 60, 60);
		loadimage(&img_mask[2], _T("./image/hero/up1_m.png"), 60, 60);
		loadimage(&img_mask[3], _T("./image/hero/up2_m.png"), 60, 60);
		//��
		loadimage(&img_list[4], _T("./image/hero/right1_i.png"), 60, 60);
		loadimage(&img_list[5], _T("./image/hero/right2_i.png"), 60, 60);
		loadimage(&img_mask[4], _T("./image/hero/right1_m.png"), 60, 60);
		loadimage(&img_mask[5], _T("./image/hero/right2_m.png"), 60, 60);
		//��
		loadimage(&img_list[6], _T("./image/hero/left1_i.png"), 60, 60);
		loadimage(&img_list[7], _T("./image/hero/left2_i.png"), 60, 60);
		loadimage(&img_mask[6], _T("./image/hero/left1_m.png"), 60, 60);
		loadimage(&img_mask[7], _T("./image/hero/left2_m.png"), 60, 60);
		//����
		loadimage(&img_list[8], _T("./image/hero/rightup1_i.png"), 60, 60);
		loadimage(&img_list[9], _T("./image/hero/rightup2_i.png"), 60, 60);
		loadimage(&img_mask[8], _T("./image/hero/rightup1_m.png"), 60, 60);
		loadimage(&img_mask[9], _T("./image/hero/rightup2_m.png"), 60, 60);
		//����
		loadimage(&img_list[10], _T("./image/hero/rightdown1_i.png"), 60, 60);
		loadimage(&img_list[11], _T("./image/hero/rightdown2_i.png"), 60, 60);
		loadimage(&img_mask[10], _T("./image/hero/rightdown1_m.png"), 60, 60);
		loadimage(&img_mask[11], _T("./image/hero/rightdown2_m.png"), 60, 60);
		//����
		loadimage(&img_list[12], _T("./image/hero/leftdown1_i.png"), 60, 60);
		loadimage(&img_list[13], _T("./image/hero/leftdown2_i.png"), 60, 60);
		loadimage(&img_mask[12], _T("./image/hero/leftdown1_m.png"), 60, 60);
		loadimage(&img_mask[13], _T("./image/hero/leftdown2_m.png"), 60, 60);
		//����
		loadimage(&img_list[14], _T("./image/hero/leftup1_i.png"), 60, 60);
		loadimage(&img_list[15], _T("./image/hero/leftup2_i.png"), 60, 60);
		loadimage(&img_mask[14], _T("./image/hero/leftup1_m.png"), 60, 60);
		loadimage(&img_mask[15], _T("./image/hero/leftup2_m.png"), 60, 60);
		//�ϸ�
		loadimage(&img_list[16], _T("./image/hero/float1_i.png"), 60, 60);
		loadimage(&img_list[17], _T("./image/hero/float2_i.png"), 60, 60);
		loadimage(&img_mask[16], _T("./image/hero/float1_m.png"), 60, 60);
		loadimage(&img_mask[17], _T("./image/hero/float2_m.png"), 60, 60);

		current_img = &img_list[0];
		current_mask = &img_mask[0];

		SP = { 0,0,0,0,0 };

		speed = 3;

		position.x = game_area_width / 2;
		position.y = game_area_height / 4;

		rect.left = position.x - img_list[0].getwidth() / 2;
		rect.top = position.y - img_list[0].getheight() / 2;
		rect.right = position.x + img_list[0].getwidth() / 2;
		rect.bottom = position.y + img_list[0].getheight() / 2;

		line_target_length[0] = position.x;
		line_target_length[1] = position.y;

		line_current_length[0] = position.x;
		line_current_length[1] = position.y;

		attack_flag = 0;
		step[0] = step[1] = 0;

		hurt_flash = 0;
		hurt_interval = 2000;
		hurt_time = clock();
		flashing_flag = 0;
		flash_interval = 150;
		flash_timer_time = clock();
		is_dead = 0;

		carousel_time = clock();
		carousel_interval = 500;
		current_index = 0;
		current_delta_index = 0;

		invicible_period = 9000;
		invicible_start = clock();

		complete_flag = 0;
		if_float = 0;
	}
	void Show() {
		//����ͼƬ
		clock_t current_time = clock();
		if (current_time - carousel_time >= carousel_interval) {
			// ʱ�����Ѵﵽ���л�����һ��ͼƬ
			current_delta_index = (current_delta_index + 1) % 2;  // ����ͼƬ��������
			carousel_time = current_time;
		}
		if (if_float) {
			// ֻ���ϸ�����
			current_img = &img_list[16 + current_delta_index];
			current_mask = &img_mask[16 + current_delta_index];
		}
		else if (SP.enhanced_hook) {
			//���ڹ���ʱ���ܸı�ͼƬ
			//��
			if (direction_check[0] && !direction_check[1] && !direction_check[2] && !direction_check[3] && !attack_flag) {
				current_index = 2;
				current_img = &img_list[2 + current_delta_index];
				current_mask = &img_mask[2 + current_delta_index];
			}
			//��
			else if (!direction_check[0] && direction_check[1] && !direction_check[2] && !direction_check[3] && !attack_flag) {
				current_index = 0;
				current_img = &img_list[0 + current_delta_index];
				current_mask = &img_mask[0 + current_delta_index];
			}
			//��
			else if (!direction_check[0] && !direction_check[1] && direction_check[2] && !direction_check[3] && !attack_flag) {
				current_index = 6;
				current_img = &img_list[6 + current_delta_index];
				current_mask = &img_mask[6 + current_delta_index];
			}
			//��
			else if (!direction_check[0] && !direction_check[1] && !direction_check[2] && direction_check[3] && !attack_flag) {
				current_index = 4;
				current_img = &img_list[4 + current_delta_index];
				current_mask = &img_mask[4 + current_delta_index];
			}
			//����
			else if (direction_check[0] && !direction_check[1] && !direction_check[2] && direction_check[3] && !attack_flag) {
				current_index = 8;
				current_img = &img_list[8 + current_delta_index];
				current_mask = &img_mask[8 + current_delta_index];
			}
			//����
			else if (!direction_check[0] && direction_check[1] && !direction_check[2] && direction_check[3] && !attack_flag) {
				current_index = 10;
				current_img = &img_list[10 + current_delta_index];
				current_mask = &img_mask[10 + current_delta_index];
			}
			//����
			else if (!direction_check[0] && direction_check[1] && direction_check[2] && !direction_check[3] && !attack_flag) {
				current_index = 12;
				current_img = &img_list[12 + current_delta_index];
				current_mask = &img_mask[12 + current_delta_index];
			}
			//����
			else if (direction_check[0] && !direction_check[1] && direction_check[2] && !direction_check[3] && !attack_flag) {
				current_index = 14;
				current_img = &img_list[14 + current_delta_index];
				current_mask = &img_mask[14 + current_delta_index];
			}
			else {
				current_img = &img_list[current_index + current_delta_index];
				current_mask = &img_mask[current_index + current_delta_index];
			}
		}
		else {
			//�����Ϻ����Ϲ�Ϊ���ϣ����º����¹�Ϊ����
			if (direction_check[0] && !direction_check[1] && !attack_flag) {
				current_index = 2;
				current_img = &img_list[2 + current_delta_index];
				current_mask = &img_mask[2 + current_delta_index];
			}
			else if (!direction_check[0] && direction_check[1] && !attack_flag) {
				current_index = 0;
				current_img = &img_list[0 + current_delta_index];
				current_mask = &img_mask[0 + current_delta_index];
			}
			else if (direction_check[2] && !direction_check[3] && !attack_flag) {
				current_index = 6;
				current_img = &img_list[6 + current_delta_index];
				current_mask = &img_mask[6 + current_delta_index];
			}
			else if (!direction_check[2] && direction_check[3] && !attack_flag) {
				current_index = 4;
				current_img = &img_list[4 + current_delta_index];
				current_mask = &img_mask[4 + current_delta_index];
			}
			else {
				current_img = &img_list[current_index + current_delta_index];
				current_mask = &img_mask[current_index + current_delta_index];
			}
		}
		
		if (!flashing_flag) {
			putimage(position.x - current_mask->getwidth() / 2, position.y - current_mask->getheight() / 2, current_mask, SRCAND);
			putimage(position.x - current_img->getwidth() / 2, position.y - current_img->getheight() / 2, current_img, SRCPAINT);
		}
		else {
			
		}
	}
	void HurtFlash() {
		if (clock() - invicible_start >= invicible_period) {
			SP.invincible_time = 0;
		}
		if (hurt_flash == 0) {
			hurt_time = clock();
			flash_timer_time = clock();
			flashing_flag = 0;
		}
		else {
			int hurt_last = clock() - hurt_time;
			if ((hurt_last >= hurt_interval) && (SP.invincible_time != 1)) {
				flashing_flag = 0;
				hurt_flash = 0;
			}
			if ((HP == 0) && (hurt_flash == 0)) is_dead = 1;
		}
		if (hurt_flash) {
			if ((clock() - flash_timer_time) >= flash_interval) {
				flashing_flag = !flashing_flag;
				flash_timer_time = clock();
			}
		}
	}
	void AttackChecked(RECT rect2, bool ifstab = 0) {
		HurtFlash();
		if (ifstab) {
			if (RectCrush(rect, rect2) && !hurt_flash) {
				HP = 0;
				hurt_flash = 1;
				//cout << "HP:" << HP << endl;
			}
		}
		else {
			if (RectCrush(rect, rect2) && !hurt_flash) {
				HP = (HP == 0) ? 0 : HP - 1;
				SP.smallbox = (SP.smallbox == 0) ? 0 : SP.smallbox - 1;
				SP.enhanced_hook = 0;
				hurt_flash = 1;
			}
		}
	}
	void Control() {
		if (complete_flag) {
			position.y += max_scroll_speed * 4;
			rect.left = position.x - current_img->getwidth() / 2;
			rect.top = position.y - current_img->getheight() / 2;
			rect.right = rect.left + current_img->getwidth() / 2;
			rect.bottom = rect.top + current_img->getheight() / 2;
			return;
		}
		HurtFlash();
		if (HP == 0)return;
		/*for (int i = 0;i < 4;i++) {
			direction_check[i] = 0;
		}*/
		if (GetAsyncKeyState(0x57)) {	//�����ƶ�,W
			rect.top = ((rect.top - speed) < 0) ? 0 : (rect.top - speed);
			rect.bottom = rect.top + current_img->getheight();
			direction_check[0] = 1;
		}
		else direction_check[0] = 0;
		if (GetAsyncKeyState(0x53)) {	//�����ƶ�,S
			rect.top = ((rect.bottom + speed) > game_area_height) ? (game_area_height - current_img->getwidth()) : (rect.top + speed);
			rect.bottom = rect.top + current_img->getheight();
			direction_check[1] = 1;
		}
		else direction_check[1] = 0;
		if (GetAsyncKeyState(0x41)) {	//�����ƶ�,A
			rect.left = ((rect.left - speed) < 0) ? 0 : (rect.left - speed);
			rect.right = rect.left + current_img->getwidth();
			direction_check[2] = 1;
		}
		else direction_check[2] = 0;
		if (GetAsyncKeyState(0x44)) {	//�����ƶ�,D
			rect.left = ((rect.right + speed) > game_area_width) ? (game_area_width - current_img->getwidth()) : (rect.left + speed);
			rect.right = rect.left + current_img->getwidth();
			direction_check[3] = 1;
		}
		else direction_check[3] = 0;
		position.x = rect.left + current_img->getwidth() / 2;
		position.y = rect.top + current_img->getheight() / 2;
		if (position.x<0 || position.x>game_area_width || position.y<0 || position.y>game_area_height) {
			position.x = game_area_width / 2;
			position.y = game_area_height / 2;
			//cout << "Hero�ܳ�����" << endl;
		}
	}
	void Attack() {
		if (HP == 0)return;
		//��J�����������ﲻ��direction_check�ж�����Ϊ����һ��ʼֻ�����ĸ�����Ĺ���
		if (GetAsyncKeyState(0x4A) && !attack_flag) {
			attack_flag = 1;
			//��
			if (current_img == &img_list[2] || current_img == &img_list[3]) {
				line_target_length[0] = 0;
				line_target_length[1] = -140;
				hook.Rotate(1);
			}
			//��
			else if (current_img == &img_list[0] || current_img == &img_list[1]) {
				line_target_length[0] = 0;
				line_target_length[1] = 140;
				hook.Rotate(2);
			}
			//��
			else if (current_img == &img_list[6] || current_img == &img_list[7]) {
				line_target_length[0] = -140;
				line_target_length[1] = 0;
				hook.Rotate(3);
			}
			//��
			else if (current_img == &img_list[4] || current_img == &img_list[5]) {
				line_target_length[0] = 140;
				line_target_length[1] = 0;
				hook.Rotate(4);
			}
			//����
			else if (current_img == &img_list[8] || current_img == &img_list[9]) {
				line_target_length[0] = 100;
				line_target_length[1] = -100;
				hook.Rotate(5);
			}
			//����
			else if (current_img == &img_list[10] || current_img == &img_list[11]) {
				line_target_length[0] = 100;
				line_target_length[1] = 100;
				hook.Rotate(6);
			}
			//����
			else if (current_img == &img_list[12] || current_img == &img_list[13]) {
				line_target_length[0] = -100;
				line_target_length[1] = 100;
				hook.Rotate(7);
			}
			//����
			else if (current_img == &img_list[14] || current_img == &img_list[15]) {
				line_target_length[0] = -100;
				line_target_length[1] = -100;
				hook.Rotate(8);
			}
			else {
				line_target_length[0] = 0;
				line_target_length[1] = 0;
			}

			line_current_length[0] = 0;
			line_current_length[1] = 0;

			step[0] = step[1] = 0;
		}
		else if (attack_flag) {
			//���㲽����ÿ��Ϊ10���أ���ʱҪȷ��line_target_lengthΪ10�ı���
			int sign = ((line_target_length[0] - line_current_length[0]) == 0) ? 0 : (((line_target_length[0] - line_current_length[0]) > 0) ? 1 : -1);
			step[0] = 10 * sign;
			sign = ((line_target_length[1] - line_current_length[1]) == 0) ? 0 : (((line_target_length[1] - line_current_length[1]) > 0) ? 1 : -1);
			step[1] = 10 * sign;

			if (attack_flag == 1) {
				if ((abs(line_current_length[0]) >= abs(line_target_length[0])) && (abs(line_current_length[1]) >= abs(line_target_length[1]))) {
					attack_flag = 2;
					line_target_length[0] = 0;
					line_target_length[1] = 0;
				}
				else {
					line_current_length[0] += step[0];
					line_current_length[1] += step[1];
				}
			}
			else if (attack_flag == 2) {
				if ((abs(line_current_length[0]) <= abs(line_target_length[0])) && (abs(line_current_length[1]) <= abs(line_target_length[1]))) {
					attack_flag = 0;
					line_current_length[0] = line_target_length[0] = 0;
					line_current_length[0] = line_target_length[1] = 0;
				}
				else {
					line_current_length[0] += step[0];
					line_current_length[1] += step[1];
				}
			}
			else {
				//cout << "attack_flag��Ϊ0��1��2��" << endl;
			}

			//����
			int hook_i_positionX = position.x + line_current_length[0] - hook.hook_i_rot.getwidth() / 2;
			int hook_i_positionY = position.y + line_current_length[1] - hook.hook_i_rot.getheight() / 2;
			hook.UpdatePosition(position.x + line_current_length[0], position.y + line_current_length[1]);
			if (!flashing_flag) {
				line(position.x, position.y, position.x + line_current_length[0], position.y + line_current_length[1]);
				putimage(hook_i_positionX, hook_i_positionY, &hook.hook_m_rot, SRCAND);
				putimage(hook_i_positionX, hook_i_positionY, &hook.hook_i_rot, SRCPAINT);
			}
			else {

			}
		}
	}
	void SetLineTarget(int targetX, int targetY) {
		line_target_length[0] = targetX - position.x;
		line_target_length[1] = targetY - position.y;
	}
	void CollisionDetect(int rebound_direction) {
		if (!rebound_direction) return;
		else if (complete_flag)return;	//�ж�ͨ�غ��ټ���Ƿ�ײǽ

		HurtFlash();

		int distance = 15;	//0��û����ײ��1�����Ϸ�����2�����ҷ�����3�����·�����4���󷴵�
		switch (rebound_direction)
		{
		case(1): {
			if (rect.top - distance >= 0) {
				position.y -= distance;
			}
			else {
				position.y = current_img->getheight() / 2;
			}
			break;
		}
		case(2): {
			if (rect.right + distance <= game_area_width) {
				position.x += distance;
			}
			else {
				position.x = game_area_width - current_img->getwidth() / 2;
			}
			break;
		}
		case(3): {
			if (rect.bottom + distance <= game_area_height) {
				position.y += distance;
			}
			else {
				position.y = game_area_height - current_img->getheight() / 2;
			}
			break;
		}
		case(4): {
			if (rect.left - distance >= 0) {
				position.x -= distance;
			}
			else {
				position.x = current_img->getwidth() / 2;
			}
			break;
		}
		default:
			break;
		}
		if (!hurt_flash) {
			HP = (HP == 0) ? 0 : HP - 1;
			SP.smallbox = (SP.smallbox == 0) ? 0 : SP.smallbox - 1;
			SP.enhanced_hook = 0;
			hurt_flash = 1;
			//cout << "HP:" << HP << endl;
		}
		rect.top = position.y - current_img->getheight() / 2;
		rect.bottom = position.y + current_img->getheight() / 2;
		rect.right = position.x + current_img->getwidth() / 2;
		rect.left = position.x - current_img->getwidth() / 2;
	}
	void GetEnhancedHook() {
		SP.enhanced_hook = 1;
	}
	void GetInvicibleTime() {
		//cout << "����޵�ʱ��" << endl;
		HurtFlash();
		hurt_flash = 1;
		SP.invincible_time = 1;
		invicible_start = clock();
	}
	void GetOxygenBottle() {
		//cout << "���OxygenBottle" << endl;
		HP = (HP >= hero_HP) ? hero_HP : HP + 1;
	}
	void GetBox() {
		//cout << "���Box" << endl;
		SP.box++;
	}
	void GetEther() {
		SP.ether++;
	}
	void GetSmallBox() {
		SP.smallbox++;
	}
	int GetCurrentHP() {
		return HP;
	}
	StageProperty& GetStageProp() {
		return SP;
	}

	RECT& GetRect() {
		return rect;
	}

	Hook& GetHook() {
		return hook;
	}

	int GetPositionX() {
		return position.x;
	}
	int GetPositionY() {
		return position.y;
	}
	int GetAttackState() {
		return attack_flag;
	}
	bool IfDead() {
		return is_dead;
	}
	bool IfDying() {
		return !HP && !is_dead;
	}
	void Startfloat() {
		if_float = 1;
	}
	void CompleteFloat() {
		complete_flag = 1;
	}
	~Hero() {
		//cout << "Hero������" << endl;
	}
private:
	IMAGE img_list[18];
	IMAGE img_mask[18];
	IMAGE* current_img;
	IMAGE* current_mask;
	RECT rect;
	StageProperty SP;
	unsigned int HP;
	int speed;

	bool hurt_flash;	//��1ʱ��ʾ���˺���˸����0ʱ����˸
	int hurt_interval;	//�ܻ��ļ����HP���㵽���ٵ�ʱ�� ����λΪms
	clock_t hurt_time;	//��¼�ܻ���ʱ��
	bool flashing_flag;	//������˸��־λ
	int flash_interval;	//�ܻ���˸�ļ��
	int flash_timer_time;	//��¼flashing_flag�ı��ʱ��
	bool is_dead;	//��HPΪ0��hurt_flashΪ0ʱ��1

	bool direction_check[4];	//[�ϣ��£�����]
	Position position;	//��¼ͼƬ�е����꣬��Control()�н��и���

	clock_t carousel_time;	//�ֲ�ͼƬ��һ�θı��ʱ��
	int carousel_interval;	//�ֲ�ͼƬ�ļ��
	int current_index;  // ��ǰ���ŵ�ͼƬ��������
	int current_delta_index;

	Hook hook;
	IMAGE hook_img, hook_mask;
	int attack_flag;	//����������ʼ���һ�׶�Ϊ1���ڶ��׶�Ϊ2����������������Ϊ0
	int line_target_length[2];	//[�յ�x,�յ�y]�������ΪͼƬ�е�
	int line_current_length[2];	//[x�����ĳ���,y�����ĳ���]�������ΪͼƬ�е�
	int step[2];	//[��x,��y]

	clock_t invicible_start;
	int invicible_period;

	bool if_float;	// �Ƿ����ϸ�����״̬
	bool complete_flag;	//�ϸ���һ������ʱ�ж�ͨ�أ���ʱ���ǲ��ܿ��ƣ��Զ����ϸ�����Ļ����
};