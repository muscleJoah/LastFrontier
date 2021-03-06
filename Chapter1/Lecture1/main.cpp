#include "Game2D.h"
#include "fmod.hpp"
#include <iostream>
#include <conio.h>
#include<memory>
#include<iostream>
#include<Math.h>
#include<stdlib.h>
#include<ctime>
#include<windows.h>
#define randomize() srand((int) time(NULL))

using namespace std;

float enemy_speed = 0.05f;
int enemy_health_point = 1;
const int MAX_BULLET = 10;
int MAX_ENEMY = 10;
const float MAX_X_POS = 2.5f; 
const float MAX_X_NEG = -2.5f;
const float MAX_Y_POS = 0.9f;
const float MAX_Y_NEG = -0.9f;
int current_kill_count;
int kill_count = 0;
int grenade_count = 3;
FMOD::System     *system_g(nullptr);
FMOD::Sound      *sound_g(nullptr);
FMOD::Channel    *channel_g(nullptr);
FMOD_RESULT       result_g;
unsigned int      version_g;
void             *extradriverdata_g(nullptr);

FMOD::System     *system_e(nullptr);
FMOD::Sound      *sound_e(nullptr);
FMOD::Channel    *channel_e(nullptr);
FMOD_RESULT       result_e;
unsigned int      version_e;
void             *extradriverdata_e(nullptr);

//gun shot

FMOD::System     *system_a(nullptr);
FMOD::Sound      *sound_a(nullptr);
FMOD::Channel    *channel_a(nullptr);
FMOD_RESULT       result_a;
unsigned int      version_a;
void             *extradriverdata_a(nullptr);
//airplane goes

FMOD::System     *system_s(nullptr);
FMOD::Sound      *sound_s(nullptr);
FMOD::Channel    *channel_s(nullptr);
FMOD_RESULT       result_s;
unsigned int      version_s;
void             *extradriverdata_s(nullptr);
//special attack explosion

void delay(clock_t n)

{
	clock_t start = clock();
	while (clock() - start < n);
}

float random_y()
{
	float random = (double) rand() / RAND_MAX ;
	return (random * 1.6f) - 0.8f;
}
float random_x()
{
	float random = (double)rand() / RAND_MAX;
	return (random * 5.0f)+2.5f;
}

namespace jm
{

	class special_attack : public Game2D
	{
	public:
		vec2 center = vec2(-3.0f, 0.9f);
		vec2 velocity = vec2(0.0f, 0.0f);
		bool is_move = false;
		void draw()
		{
			if (is_move == true)
				center.x += 0.3f * getTimeStep();
			beginTransformation();
			
			translate(center);
			drawFilledBox(Colors::silver, 0.5f, 0.1f);
			translate(0.25f, 0.0f);
			drawFilledCircle(Colors::silver, 0.05f);
			translate(-0.2f, -0.05f);
			rotate(-30.0f);
			drawFilledBox(Colors::silver, 0.1f, 0.15f);
			rotate(30.0f);
			translate(0.0f, +0.1f);
			rotate(30.0f);
			drawFilledBox(Colors::silver, 0.1f, 0.15f);
			rotate(-30.0f);
			translate(-0.238f, 0.0f );
			drawFilledTriangle(Colors::silver, 0.1f);
			endTransformation();
			
			
			beginTransformation();
			translate(center);
			translate(0.23f, 0.01f);
			drawFilledBox(Colors::blue, 0.06f, 0.04f);
			endTransformation();
		}
		
	};
	class Item_grenade  // 폭탄 아이템 
	{
	public:
		vec2 center = vec2(0.0f, 0.0f);
		bool flag = false;
		void draw()
		{
			beginTransformation();
			
			translate(center);
			drawFilledBox(Colors::white, 0.11f, 0.11f);
			drawFilledBox(Colors::black, 0.1f , 0.1f);
			translate(0.0f, -0.01f);
			drawFilledCircle(Colors::yellow, 0.03f);
			translate(0.01f, 0.04f);
			drawFilledBox(Colors::yellow, 0.05f, 0.02f);
			endTransformation();
		}
	};

	class Item_HP
	{
	public:
		vec2 center = vec2(0.0f, 0.0f);
		bool flag = false;
		void draw()
		{
			beginTransformation();
			translate(center);
			drawFilledBox(Colors::white, 0.11f, 0.11f);
			drawFilledBox(Colors::black, 0.1f, 0.1f);
			drawFilledBox(Colors::black, 0.06f, 0.06f);
			drawFilledPentagon(Colors::red, 0.04f);
			endTransformation();
		}
	};
	class Background
	{
	public:
		vec2 center = vec2(0.0f, 0.0f);
		void draw() {
			beginTransformation();
			drawFilledBox(Colors::olive, 5.0f, 2.0f);
			endTransformation();
			beginTransformation();
			translate(0.0f, 1.0f);
			drawFilledBox(Colors::black, 5.0f, 0.7f);
			translate(0.2f, -0.1f);
			drawFilledStar(Colors::yellow, 0.025f , 0.0125f);
			translate(0.6f, -0.03f);
			drawFilledStar(Colors::yellow, 0.025f, 0.0125f);
			

			translate(1.5f, 0.0f);
			drawFilledCircle(Colors::yellow, 0.04f);
			
			translate(-0.02f, 0.0f);
			drawFilledCircle(Colors::black, 0.04f);
			translate(-4.0f, +0.02f);
			drawFilledStar(Colors::yellow, 0.025f, 0.0125f);
			translate(1.0f, +0.02f);
			drawFilledStar(Colors::yellow, 0.020f, 0.01f);
			endTransformation();	
		}
	};
	class Explosion
	{
	public:
		vec2 center = vec2(0.0f, 0.0f);
		bool is_explode = false;
		void draw_explosion()
		{
			beginTransformation();
			translate(center);
			drawFilledCircle(Colors::red, 0.4f);
			drawFilledCircle(Colors::yellow, 0.3f);
			drawFilledCircle(Colors::white, 0.1f);
			endTransformation();
		}
	};
	class mybullet
	{
	public:
		vec2 center = vec2(0.0f, 0.0f);
		vec2 velocity = vec2(0.0f, 0.0f);
		void draw()
		{
			beginTransformation();
		
			translate(center);
			drawFilledRegularConvexPolygon(Colors::yellow, 0.015f, 8);
			drawWiredRegularConvexPolygon(Colors::black, 0.015f, 8);
			endTransformation();
		}

		void update(const float& dt)
		{
			center += velocity * dt;
		}
	};
	class Enemy : public Game2D
	{
	public:
		vec2 center = vec2(3.0f , 0.0f);
		float time = 0.0f;
		int health_point = enemy_health_point;
		mybullet *bullet;
		float speed = 0.03f;
		void draw()
		{
			beginTransformation();
			translate(center);
			translate(0.0f, 0.15f);
			drawFilledCircle(Colors::gold, 0.05f); //face
			translate(-0.025f, 0.01f);
			drawFilledCircle(Colors::white, 0.005f); //eye
			translate(-0.002f, 0.0f);
			drawFilledCircle(Colors::black, 0.0005f);//pupil
			translate(0.002f, 0.0f);
			translate(0.0f, -0.04f);
			drawFilledBox(Colors::red, 0.03f, 0.01f); // mouth
			translate(0.0f, 0.04f);
			translate(-0.02f, -0.01f);
			drawFilledTriangle(Colors::gold, 0.025f); // nose
			translate(0.045f, 0.045f);
			drawFilledBox(Colors::red, 0.1f, 0.05f);
			translate(-0.03f, -0.0f);
			drawFilledStar(Colors::black, 0.02f, 0.01f);
			
			
			endTransformation();
			// head and helmet

			beginTransformation();
			translate(center);
				translate(0.0f, 0.06f);
				rotate((sinf(time*1.0f + 3.141592f)) * 50.0f);
				translate(0.0f, -0.06f);
				drawFilledBox(Colors::red, 0.03f, 0.12f);
			translate(0.0f, -0.06f);
			drawFilledCircle(Colors::gold, 0.01f);
			endTransformation();
			//leftarm

			beginTransformation();
			translate(center);
			translate(0.0f, 0.017f);
			drawFilledBox(Colors::red, 0.08f, 0.16f);
			endTransformation();
			//body


			beginTransformation();
			translate(center);
			translate(-0.06f, 0.048f);
			drawFilledBox(Colors::red, 0.12f, 0.03f);
			translate(-0.06f, 0.0f);
			drawFilledBox(Colors::silver, 0.022f, 0.05f);
			translate(-0.023f, 0.025f);
			drawFilledBox(Colors::silver, 0.1f, 0.02f);
			endTransformation();
			//gun and right arm



			beginTransformation();
			translate(center);
				rotate(sinf(time*1.0f + 3.141592f) * 20.0f);
			translate(0.0f, -0.13f);
			drawFilledBox(Colors::red, 0.04f, 0.15f);
			translate(-0.01f, -0.095f);
			drawFilledBox(Colors::black, 0.08f, 0.04f);
			endTransformation();
			//rightleg;

			beginTransformation();
			translate(center);
			rotate(-(sinf(time*1.0f + 3.141592f)) * 20.0f);
			translate(0.0f, -0.13f);
			drawFilledBox(Colors::red, 0.04f, 0.15f);
			translate(-0.01f, -0.095f);
			drawFilledBox(Colors::black, 0.08f, 0.04f);
			endTransformation();
			//leftleg

			time += this->getTimeStep();
		}
		void draw_rightarm()
		{

		}
	};
	class Grenade // 수류탄 class
	{
	public:
		vec2 center = vec2(0.0f, 0.0f); // 던진 위치
		vec2 thorw_pos = vec2(0.0f, 0.0f); //터질 위치
		vec2 velocity = vec2(0.0f, 0.0f); // 속도
		void draw_grenade() // 수류탄그리기
			{
			beginTransformation();
			translate(center);
			drawFilledCircle(Colors::black, 0.03f);
			translate(0.01f,0.04f);
			drawFilledBox(Colors::black, 0.05f, 0.02f);
			endTransformation();
			}
		
		void update(const float& dt) // 실시간으로 수류탄 움직이기
		{
			center += velocity * dt;
		}
		
	};

	class MySoldier :public Game2D //플레이어
	{
	public:
		vec2 center = vec2(0.0f, 0.0f);
		float time = 0.0f;
		bool is_moving = false; // 움직이고 있는지에 대한 flag
		int health_point = 3; // 체력
		vec2 rightarm_center = vec2(0.0f, 0.0f);
		vec2 mouse_pos = vec2(0.0f, 0.0f);
		
		
		void draw() 

		{
			beginTransformation();
			translate(center);
			translate(0.0f, 0.15f);
			drawFilledCircle(Colors::gold, 0.05f); //face
			translate(0.025f, 0.01f);
			drawFilledCircle(Colors::white, 0.005f); //eye
			translate(0.002f, 0.0f);
			drawFilledCircle(Colors::black, 0.0005f);//pupil
			translate(-0.002f, 0.0f);
			translate(0.0f, -0.04f);
			drawFilledBox(Colors::red, 0.03f, 0.01f); // mouth
			translate(0.0f, 0.04f);
			translate(0.02f, -0.01f);
			drawFilledTriangle(Colors::gold , 0.025f); // nose
			translate(-0.045f, 0.045f);
			drawFilledBox(Colors::green, 0.1f, 0.05f);
			drawFilledBox(Colors::black, 0.02f, 0.02f);
			translate(-0.04f, -0.017f);
			drawFilledBox(Colors::black, 0.02f, 0.02f);
			translate(0.08f, 0.0f);
			drawFilledBox(Colors::black, 0.02f, 0.02f);
			translate(0.0f, 0.03f);
			drawFilledBox(Colors::black, 0.02f, 0.02f);
			endTransformation();
			// head and helmet

			beginTransformation();
			translate(center);
			if (is_moving == true)
			{
				translate(0.0f, 0.06f);
				rotate((sinf(time*1.0f + 3.141592f)) * 50.0f);
				translate(0.0f, -0.06f);
			}
			drawFilledBox(Colors::green, 0.03f, 0.12f);
			translate(0.0f, -0.06f);
			drawFilledCircle(Colors::gold, 0.01f);
			endTransformation();
			//leftarm

			beginTransformation();
			translate(center);
			translate(0.0f, 0.017f);
			drawFilledBox(Colors::green, 0.08f, 0.16f);
			endTransformation();
			//body
			beginTransformation();
			translate(center);
			if(is_moving == true)
			rotate(sinf(time*1.0f + 3.141592f) * 20.0f);
			translate(0.0f, -0.13f);
			drawFilledBox(Colors::green, 0.04f, 0.15f);
			translate(0.01f, -0.095f);
			drawFilledBox(Colors::black, 0.08f, 0.04f);
			endTransformation();
			//rightleg;

			beginTransformation();
			translate(center);
			if (is_moving == true)
				rotate(-(sinf(time*1.0f + 3.141592f)) * 20.0f);
			translate(0.0f, -0.13f);
			drawFilledBox(Colors::green, 0.04f, 0.15f);
			translate(0.01f, -0.095f);
			drawFilledBox(Colors::black, 0.08f, 0.04f);
			endTransformation();
			//leftleg

			time += this->getTimeStep();
		}
		void draw_rightarm()
		{
			beginTransformation();
				translate(rightarm_center);
				translate(0.01f, 0.05f);
				rotate(atan2(mouse_pos.y - rightarm_center.y, mouse_pos.x - rightarm_center.x) * 180 / 3.1415f);
				translate(-0.01f, -0.05f);
				translate(0.06f, 0.048f);
				drawFilledBox(Colors::green, 0.12f, 0.03f);
				translate(0.06f, 0.0f);
				drawFilledBox(Colors::black, 0.022f, 0.05f);
				translate(0.023f, 0.025f);
				drawFilledBox(Colors::black, 0.1f, 0.02f);
				endTransformation();

				//gun and right arm

		}
		
	};

	
	class Aim  //조준점을 위한 class
	{
	public:
		vec2 center = vec2(0.0f, 0.0f);
		void draw() // 조준점 그리기
		{
			beginTransformation();
			translate(center);

			drawWiredCircle(Colors::black, 0.04f);
			drawFilledBox(Colors::black, 0.12f, 0.005f);
			drawFilledBox(Colors::black, 0.005f, 0.12f);
			drawFilledCircle(Colors::red, 0.01f);
			endTransformation();

		}
	};

	class MysoldierExample : public Game2D
	{
	public:
		MySoldier soldier;
		Enemy *enemies[15] = { nullptr, nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr }; // 적 개체 초기화 포인터 배열사용
		mybullet *bullet[MAX_BULLET] = { nullptr, nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr }; // 수류탄 개체 초기화 포인터 배열 사용
		Aim aim;
		int count = 0;
		bool SA_FLAG = false;
		Background background;
		Explosion *explosion = nullptr;
		Explosion *SA_Explosion[8] = { nullptr , nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr};
		Grenade *grenade = nullptr;
		Item_grenade *i_grenade = nullptr;
		Item_HP *i_hp = nullptr;
		special_attack SA;
		MysoldierExample()
			: Game2D("Last - FrontLine", 2000, 768, false, 2)
		{
			
			FMOD::System_Create(&system_g);

			system_g->getVersion(&version_g);

			system_g->init(32, FMOD_INIT_NORMAL, extradriverdata_g);

			system_g->createSound("gunshot.wav", FMOD_LOOP_OFF, 0, &sound_g); // .mp3 files work!
			//총소리 만들기

			FMOD::System_Create(&system_e);

			system_e->getVersion(&version_e);

			system_e->init(32, FMOD_INIT_NORMAL, extradriverdata_e);

			system_e->createSound("explosion.mp3", FMOD_LOOP_OFF, 0, &sound_e); // .mp3 files work!
			//수류탄 터지는 소리

			FMOD::System_Create(&system_a);

			system_a->getVersion(&version_a);

			system_a->init(32, FMOD_INIT_NORMAL, extradriverdata_a);

			system_a->createSound("dropping.wav", FMOD_LOOP_OFF, 0, &sound_a); // .mp3 files work!

			FMOD::System_Create(&system_s);

			system_s->getVersion(&version_s);

			system_s->init(32, FMOD_INIT_NORMAL, extradriverdata_s);

			system_s->createSound("SA_Explosion.wav", FMOD_LOOP_OFF, 0, &sound_s); // .mp3 files work!
		} // 폭탄떨어지는소리

		~MysoldierExample()
		{
			for (int i = 0; i < MAX_ENEMY; i++)
			{
				if (enemies[i] != nullptr)
					delete enemies[i];
			}
			for (int i = 0; i < MAX_BULLET; i++)
			{
				if (bullet[i] != nullptr)
					delete bullet[i];
			}
			delete explosion;
			delete grenade;
			cout << "delete! " << endl;
		}

		void update() override
		{
			const vec2 mouse_pos = getCursorPos(true);
			background.draw();
			// move tank
			soldier.is_moving = false;

			switch (soldier.health_point)
			{
			case 3:
				beginTransformation();
				translate(-2.2f, -0.8f);
				drawFilledPentagon(Colors::red, 0.05f);
				translate(0.2f, 0.0f);
				drawFilledPentagon(Colors::red, 0.05f);
				translate(0.2f, 0.0f);
				drawFilledPentagon(Colors::red, 0.05f);
				endTransformation();
				break;

			case 2:
				beginTransformation();
				translate(-2.2f, -0.8f);
				drawFilledPentagon(Colors::red, 0.05f);
				translate(0.2f, 0.0f);
				drawFilledPentagon(Colors::red, 0.05f);
				translate(0.2f, 0.0f);
				drawFilledPentagon(Colors::olive, 0.05f);
				endTransformation();
				break;
			case 1:
				beginTransformation();
				translate(-2.2f, -0.8f);
				drawFilledPentagon(Colors::red, 0.05f);
				translate(0.2f, 0.0f);
				drawFilledPentagon(Colors::olive, 0.05f);
				translate(0.2f, 0.0f);
				drawFilledPentagon(Colors::olive, 0.05f);
				endTransformation();
				break;
			default:
				beginTransformation();
				translate(-2.2f, -0.8f);
				drawFilledPentagon(Colors::olive, 0.05f);
				translate(0.2f, 0.0f);
				drawFilledPentagon(Colors::olive, 0.05f);
				translate(0.2f, 0.0f);
				drawFilledPentagon(Colors::olive, 0.05f);
				endTransformation();

				break;
			}
			//체력 잔여 갯수 

			switch (grenade_count)
			{
			case 3:
				beginTransformation();
				translate(-1.5f, -0.81f);
				drawFilledCircle(Colors::black, 0.03f);
				translate(0.01f, 0.04f);
				drawFilledBox(Colors::black, 0.05f, 0.02f);
				translate(0.2f, -0.04f);
				drawFilledCircle(Colors::black, 0.03f);
				translate(0.01f, 0.04f);
				drawFilledBox(Colors::black, 0.05f, 0.02f);
				translate(0.2f, -0.04f);
				drawFilledCircle(Colors::black, 0.03f);
				translate(0.01f, 0.04f);
				drawFilledBox(Colors::black, 0.05f, 0.02f);
				endTransformation();
				break;
			case 2:
				beginTransformation();
				translate(-1.5f, -0.81f);
				drawFilledCircle(Colors::black, 0.03f);
				translate(0.01f, 0.04f);
				drawFilledBox(Colors::black, 0.05f, 0.02f);
				translate(0.2f, -0.04f);
				drawFilledCircle(Colors::black, 0.03f);
				translate(0.01f, 0.04f);
				drawFilledBox(Colors::black, 0.05f, 0.02f);
				endTransformation();
				break;
			case 1:
				beginTransformation();
				translate(-1.5f, -0.81f);
				drawFilledCircle(Colors::black, 0.03f);
				translate(0.01f, 0.04f);
				drawFilledBox(Colors::black, 0.05f, 0.02f);
				endTransformation();
				break;
			default:
				break;
			}
			//수류탄 잔여 갯수 

			if (isKeyPressed(GLFW_KEY_A)) { soldier.center.x -= 0.1f * getTimeStep(); soldier.rightarm_center.x -= 0.1f * getTimeStep(); soldier.is_moving = true; }
			if (isKeyPressed(GLFW_KEY_D)) { soldier.center.x += 0.1f * getTimeStep(); soldier.rightarm_center.x += 0.1f * getTimeStep(); soldier.is_moving = true; }
			if (isKeyPressed(GLFW_KEY_W)) { soldier.center.y += 0.1f * getTimeStep(); soldier.rightarm_center.y += 0.1f * getTimeStep(); soldier.is_moving = true; }
			if (isKeyPressed(GLFW_KEY_S)) { soldier.center.y -= 0.1f * getTimeStep(); soldier.rightarm_center.y -= 0.1f * getTimeStep();  soldier.is_moving = true; }
			//조작키에 따른 움직임
			if (isKeyPressedAndReleased(GLFW_KEY_F)) // 필살기썻을떄
			{
				system_a->playSound(sound_a, 0, false, &channel_a);
				SA.is_move = true;
			}
			if (SA.center.x > 3.0f) // 비행기가 완전히 지나갔을때 explosion 생성
			{
				for (int i = 0; i < 8; i++)
				{
					if (SA_Explosion[i] == nullptr)
					{
						SA_Explosion[i] = new Explosion;
					}
					else break;
				}
			}
		
			
		
			

			if (isMouseButtonPressedAndReleased(GLFW_MOUSE_BUTTON_1)) // 마우스 왼쪽클릭 (총알발사)
			{
				system_g->playSound(sound_g, 0, false, &channel_g);
				for (int i = 0; i < MAX_BULLET; i++)
				{			
					if ((count % 10) == i)
					{
						bullet[i] = new mybullet;
						bullet[i]->center = soldier.rightarm_center;
						bullet[i]->center.y = soldier.rightarm_center.y + 0.043f;
						// 총알 각도
						float x = mouse_pos.x - soldier.rightarm_center.x;
						float y = mouse_pos.y - soldier.rightarm_center.y;
						bullet[i]->velocity = vec2(x / abs(sqrt(x*x + y * y)), y / abs(sqrt(x*x + y * y))) * 0.8f;
						SA_FLAG = true;
						count++;
						break;
					}
				}
			}
			if (isMouseButtonPressedAndReleased(GLFW_MOUSE_BUTTON_2)) //마우스 오른쪽 클릭 (수류탄 발사)
			{
				if (grenade_count > 0) // 수류탄이 남아있을떄
				{
					grenade = new Grenade;
					explosion = new Explosion;
					grenade->center = soldier.rightarm_center;
					grenade->center.y = soldier.rightarm_center.y + 0.043f;

					float x = mouse_pos.x - soldier.rightarm_center.x;
					float y = mouse_pos.y - soldier.rightarm_center.y;
					grenade->velocity = vec2(x / abs(sqrt(x*x + y * y)), y / abs(sqrt(x*x + y * y))) * 0.5f;
					grenade->thorw_pos = mouse_pos; //수류탄이 터지는 곳은 발사했을시에 마우스포인터 위치
					grenade_count--; // 수류탄 잔여갯수 줄이기
				}
				
			}
			
			
			if (grenade != nullptr) // 수류탄이 발사되었을때
			{
				grenade->update(getTimeStep());
				explosion->center = grenade->center;
				grenade->draw_grenade(); // 수류탄그리기
			}//수류탄 발사


			if(grenade != nullptr )
			if (grenade->center.x < grenade->thorw_pos.x +0.1f && grenade->center.x > grenade->thorw_pos.x - 0.1f) // 수류탄이 던진 위치에 다달았을때
			{
				grenade->update(0.0f); // 수류탄의 움직임을 멈춘다.
				explosion -> is_explode = true; // 폭발 flag true;
				for (int i = 0; i < MAX_ENEMY; i++)
				{
					if (enemies[i] != nullptr)
					{
						if (enemies[i]->center.x <grenade->center.x+0.6f && enemies[i]->center.x  > grenade->center.x-0.6f &&
							enemies[i]->center.y < grenade->center.y + 0.6f  && enemies[i]->center.y  > grenade->center.y - 0.6f) // 수류탄 터졌을때 적죽이기
						{
							system_e -> playSound(sound_e, 0, false, &channel_e);
							kill_count++;
							enemies[i] = nullptr;
							current_kill_count = kill_count;
						}
					}
				}
				grenade = nullptr;
			} // 수류탄이 위치에 도달하였을때 터트리기


			for (int i = 0; i < MAX_BULLET; i++)
			{
				if (bullet[i] != nullptr) { bullet[i]->update(getTimeStep()); }
			}
			for (int i = 0; i < MAX_BULLET; i++)
			{
				if (bullet[i] != nullptr) { bullet[i]->draw(); }
			}
	
			//총알 발사 했을시에 bullet객체 그리기
			
			for (int i = 0; i < MAX_ENEMY; i++)
			{
				if (enemies[i] == nullptr)
				{
					enemies[i] = new Enemy;
					enemies[i] -> center.y = random_y();
					enemies[i]-> center.x = random_x() + 0.2f;
				
				}
			}
			//적 생성

			for (int i = 0; i < MAX_ENEMY; i++)
			{
				if (enemies[i] != nullptr) {
					enemies[i]->center.x -=  enemy_speed *getTimeStep();
					enemies[i]->draw();
				}
			}
			//적 움직이기

			for (int i = 0; i < MAX_ENEMY; i++)
			{
				if (enemies[i] != nullptr)
				{
					if (enemies[i]->health_point == 0) // 적의 체력이 0이되었을떄
					{
						kill_count += 1;
						if (kill_count % 25 == 0)
						{
							i_grenade = new Item_grenade;
							i_grenade->center = enemies[i]->center;
							i_grenade->center.y = enemies[i]->center.y - 0.1f;
							i_grenade->flag  = true;
						}
						else if (kill_count % 35 == 0)
						{

							i_hp = new Item_HP;
							i_hp->center = enemies[i]->center;
							i_hp->center.y = enemies[i]->center.y - 0.1f;
							i_hp->flag = true;

						}
						enemies[i] = nullptr;
						
						

					}
					else if(enemies[i] -> center.x < -2.5f ) 
						// 전선에 도다르면 플레이어의 체력을 깎는다.
					{
						soldier.health_point--;
						enemies[i] = nullptr;
					}
				}
			}
			//적 죽이기

			for (int i = 0; i < MAX_BULLET; i++)
			{
				if (bullet[i] != nullptr)
				{
					for (int j = 0; j < MAX_ENEMY; j++)
					{
						if (enemies[j] != nullptr)
							if (bullet[i]->center.x > enemies[j]->center.x - 0.04f && bullet[i]->center.x < enemies[j]->center.x + 0.04f &&
								bullet[i]->center.y > enemies[j]->center.y - 0.2f && bullet[i]->center.y < enemies[j]->center.y + 0.22f)
								// enemy hitbox
							{
								enemies[j]->health_point--;

								delete bullet[i];
								break;
								//총알이 적에게 맞았을때
							}
					}
					if (bullet[i]->center.x > 2.5f || bullet[i]->center.x < -2.5f || bullet[i]->center.y > 1.0f || bullet[i]->center.y < -1.0f)
					{
						bullet[i] = nullptr;
						break;
						//총알이 범위를 나갔을때 총알을 지운다.
						// 총알속도 에러도 고쳐졌다.
					}
				}
			}
			//총알 삭제 반복문
			

			if (kill_count == 20)
			{
				enemy_health_point = 2 ; //적 체력 증가
				enemy_speed = 0.07f; // 적 속도 증가
			}
			if (kill_count == 40)
			{
				MAX_ENEMY = 13; //적 개체 증가
				enemy_speed = 0.09f;
			}
			if (kill_count == 60)
			{
				enemy_speed = 0.1f;
				enemy_health_point = 3;
			}
			if (kill_count == 80)
			{
				enemy_speed = 0.12f;
				MAX_ENEMY = 15;
			}
		 // 죽일수록 난이도를 상승시킨다.



			if (soldier.health_point == 0) // 플레이어의 체력이 0이 되었을때
			{
				cout << "당신의 점수는 : " << kill_count << endl;
				exit(1);
			}
			
			soldier.mouse_pos = getCursorPos(true);
			soldier.draw();
			soldier.draw_rightarm();

			if (explosion != nullptr)
			{
				if (explosion->is_explode == true)
				{
					explosion->draw_explosion();
					if (current_kill_count < kill_count) // 적을 더 죽이면 explosion 효과 사라지게
						explosion = nullptr;
				}
			}
			aim.center = mouse_pos;
			aim.draw(); // 조준점 그리기
			SA.draw();
			
			int explosion_count = 0;
			for (int i = 0; i < 8; i++) // explosion 그리기
			{
				if (SA_Explosion[i] != nullptr && SA.center.x < 7.0f) // 폭발잔상남아있는 조건
				{
					SA_Explosion[0]->center = vec2(0.0f, 0.0f);
					SA_Explosion[1]->center = vec2(-2.0f, 0.0f);
					SA_Explosion[2]->center = vec2(2.0f, 0.0f);
					SA_Explosion[3]->center = vec2(1.0f, 0.5f);
					SA_Explosion[4]->center = vec2(-1.0f, 0.5f);
					SA_Explosion[5]->center = vec2(1.5f, -0.5f);
					SA_Explosion[6]->center = vec2(-1.0f, -0.5f);
					SA_Explosion[7]->center = vec2(0.5f, -0.4f);
					SA_Explosion[i]->draw_explosion();
					if (explosion_count == 0)
						system_s->playSound(sound_s, 0, false, &channel_s);
						explosion_count++;
					SA_FLAG = true;
				}
				else
					SA_FLAG = false;
			}

			for (int i = 0; i < MAX_ENEMY; i++) // 스페셜 어택을 썻을때 모든 적을 죽인다.
			{
				if (enemies[i] != nullptr && SA_FLAG == true)
				{
					enemies[i] = nullptr;
				}
			}

			if (i_grenade != nullptr) // 폭탄아이템 생성및 먹기
			{
				if (i_grenade->flag == true)
				{
					i_grenade->draw();
					if(i_grenade ->center.x + 0.05f > soldier.center.x  && i_grenade->center.x - 0.05f < soldier.center.x 
						&& i_grenade -> center.y > soldier.center.y  - 0.15f && i_grenade -> center.y < soldier.center.y  + 0.15f)
						{
						if(grenade_count < 3)
						grenade_count++;
						delete i_grenade;
						}
				}
			}

			if (i_hp != nullptr)
				if (i_hp ->flag == true)
				{
					i_hp->draw();
					if (i_hp->center.x + 0.05f > soldier.center.x  && i_hp->center.x - 0.05f < soldier.center.x
						&& i_hp->center.y > soldier.center.y - 0.15f && i_hp->center.y < soldier.center.y + 0.15f)
					{
						if (soldier.health_point < 3)
							soldier.health_point++;
						delete i_hp;
					}
				}
			
		
		}
	};


}





int main(void)
{
	FMOD::System     *system(nullptr);
	FMOD::Sound      *sound(nullptr);
	FMOD::Channel    *channel(nullptr);
	FMOD_RESULT       result;
	unsigned int      version;
	void             *extradriverdata(nullptr);

	FMOD::System_Create(&system);

	system->getVersion(&version);

	system->init(32, FMOD_INIT_NORMAL, extradriverdata);

	system->createSound("backgroundmusic.mp3", FMOD_LOOP_NORMAL, 0, &sound); // .mp3 files work!
	//배경음악 재생
	system->playSound(sound, 0, false, &channel);

	srand(time(NULL));

	jm::MysoldierExample().init("this is battle field", 2000, 1400, false).run();


	return 0;
}