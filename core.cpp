// ReSharper disable All
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<easyx.h>
#include<fstream>
#include<graphics.h>
#include<conio.h>
#include<list>
#include<functional>
#include <string>
#include<cmath>
#include"yNodeGUI.h"
#include "resource.h"
using namespace std;

#pragma region Defines

#define LINESIGN "Line:"
#define ENDSIGN "End"
#define FILENAME1 "course.data"
#define FILENAME2 "select.data"
#define BGFILENAME "Resources/background.jpg"
#define BUTTONCOLOR RGB(63, 92, 57)
#define LINEBOXCOLOR GREEN
#define BUTTONFONTCOLOR WHITE
#define GIRDFONTCOLOR BLACK
#define GIRDLINECOLOR BLACK
#define FONTNAME "����"

#pragma endregion

#pragma region ���ݽṹ
typedef enum
{
	Pub = 0,//����
	Nes = 1,//����
	Sel = 2,  //ѡ��
	NaN = 3
}nature;
string Index(nature nat)
{
	switch (nat)
	{
	case Pub:return "������";
	case Sel:return "ѡ�޿�";
	case Nes:return "���޿�";
	default:
		return "NULL";
	}
}
typedef struct
{
	long long id;   //�γ̱��
	char name[50];  //�γ�����
	nature nat;   //�γ�����
	float class_period; //��ѧʱ
	float teaching_time; //�ڿ�ѧʱ
	float experiental_time; //ʵ��ѧʱ
	float credit;  //ѧ��
	char term[50];

}Course;
#pragma endregion

#pragma region ȫ�ֱ���_1
//�γ�˳���
vector<Course*> courses;
int page1 = 0;
//�����ţ����ؼ���ţ�����ҳ����ţ�����ť��ţ���ѧ��GUI���
int gird_id, ret_id, next_id, last_id, id_sort, credit_sort, time_sort, alls_id;;
bool inputone = true;
int bg_id;
//��ǰGird��Ӧ�Ŀγ�˳���ָ��
vector<Course*>* pointer_courses = &courses;
#pragma endregion

#pragma region ���ߺ���
#pragma region �γ�˳������
bool exist_course(long long id)
{
	for (auto& i : courses)
	{
		if (i->id == id) return true;
	}
	return false;
}
bool exist_course(long long id, vector<Course*>* pointer)
{
	for (auto& i : *pointer)
	{
		if (i->id == id) return true;
	}
	return false;
}
void remove_course(long long id, vector<Course*>* pointer)
{
	for (int i = 0; i < pointer->size(); i++)
	{
		if ((*pointer)[i]->id == id) pointer->erase(pointer->begin() + i);
	}
}
Course* find_course(long long id)
{
	for (auto& i : courses)
	{
		if (i->id == id) return i;
	}
	return NULL;
}
#pragma endregion
#pragma region �γ�����
//��������
void swap(Course** a, Course** b)
{
	Course* c = *a;
	*a = *b;
	*b = c;
}
//����ID����ð������
void sortByid(vector<Course*>& nums)
{
	for (int i = 0; i < nums.size() - 2; i++)
	{
		for (int x = 0; x < nums.size() - i - 1; x++)
		{
			if (nums[x]->id > nums[x + 1]->id) swap(&nums[x], &nums[x + 1]);
		}
	}
}
void sortByCredit(vector<Course*>& nums)
{
	for (int i = 0; i < nums.size() - 2; i++)
	{
		for (int x = 0; x < nums.size() - i - 1; x++)
		{
			if (nums[x]->credit > nums[x + 1]->credit) swap(&nums[x], &nums[x + 1]);
		}
	}
}
void sortByTime(vector<Course*>& nums)
{
	for (int i = 0; i < nums.size() - 1; i++)
	{
		int minIndex = i;
		for (int a = i + 1; a < nums.size(); a++)
		{
			minIndex = nums[a]->class_period < nums[minIndex]->class_period ? a : minIndex;
		}
		if (minIndex != 0) swap(&nums[i], &nums[minIndex]);
	}
}
#pragma endregion
#pragma region �����ʾ��ˢ��
string ftostr(double value, int fixed)
{
	string str = to_string(value);
	return str.substr(0, str.find('.') + fixed + 1);
}
void SetRow(Gird* gird, int r, Course* course)
{
	gird->SetUnit(r, 0, to_string(course->id).c_str());
	gird->SetUnit(r, 1, course->name);
	gird->SetUnit(r, 2, Index(course->nat).c_str());
	gird->SetUnit(r, 3, ftostr(course->class_period, 1).c_str());
	gird->SetUnit(r, 4, ftostr(course->teaching_time, 1).c_str());
	gird->SetUnit(r, 5, ftostr(course->experiental_time, 1).c_str());
	gird->SetUnit(r, 6, ftostr(course->credit, 1).c_str());
	gird->SetUnit(r, 7, course->term);
}
void SetRow(Gird* gird, int r)
{
	gird->SetUnit(r, 0, " ");
	gird->SetUnit(r, 1, " ");
	gird->SetUnit(r, 2, " ");
	gird->SetUnit(r, 3, " ");
	gird->SetUnit(r, 4, " ");
	gird->SetUnit(r, 5, " ");
	gird->SetUnit(r, 6, " ");
	gird->SetUnit(r, 7, " ");
}
void showPage(Canvas& canvas, int gird_id, int page, vector<Course*>& cs)
{
	assert(page >= 0);
	Gird* gird = (Gird*)canvas.Env(1).GetGUI(gird_id);
	int start = (page) * (gird->yCount - 1);

	for (int i = 0; i < gird->yCount - 1; i++)
	{
		if (i + start < cs.size())
		{
			SetRow(gird, i + 1, cs[i + start]);
		}
		else
		{
			SetRow(gird, i + 1);
		}
	}
}
void nextPage(Canvas& canvas)
{
	int c = ((Gird*)canvas.GetGUI(gird_id))->yCount - 1;
	int allpage = pointer_courses->size() / c;
	showPage(canvas, gird_id, page1 = min(allpage, page1 + 1), *pointer_courses);
}
void lastPage(Canvas& canvas)
{
	showPage(canvas, gird_id, page1 = max(0, page1 - 1), *pointer_courses);
}
#pragma endregion
#pragma endregion

#pragma region ���ݳ־û�
//����γ�vector
void save_courses(vector<Course*>& courses, string fileName)
{
	//�����ļ������
	ofstream fout;
	fout.open(fileName);
	//���д��γ��ļ�
	for (int i = 0; i < courses.size(); i++)
	{
		//����к�
		fout << LINESIGN << i + 1 << " " << courses[i]->id << " " << courses[i]->name << " " << courses[i]->nat << " " << courses[i]->class_period << " " << courses[i]->teaching_time << " ";
		fout << courses[i]->experiental_time << " " << courses[i]->credit << " " << courses[i]->term << endl;
	}
	//β��ʶ
	fout << ENDSIGN;
	//�ر��ļ�������bufferд���ļ�
	fout.close();
}
//��ȡ�γ�vector
void read_courses(vector<Course*>& courses, string fileName)
{
	ifstream fin;
	fin.open(fileName);
	if (!fin) save_courses(courses, fileName);
	string sign;
	if (!fin.eof())
	{
		while (fin >> sign, sign != ENDSIGN)
		{
			Course* course = new Course();
			int nat;
			fin >> course->id >> course->name >> nat >> course->class_period >> course->teaching_time >> course->experiental_time >> course->credit >> course->term;
			course->nat = (nature)nat;
			courses.push_back(course);
		}
	}
	fin.close();
}

#pragma endregion

#pragma region ����ʵ��
#pragma region �γ���Ϣ¼��
//¼��γ���Ϣ
void input(Menu& menu, Canvas& canvas)
{
	//������ʾ���ڣ���տ���̨�����뻺����
	MessageBox(NULL, ("���ڿ���̨��¼��γ�"), ("��ʾ"), MB_SYSTEMMODAL | MB_ICONINFORMATION);
	rewind(stdin);
	system("cls");

	cout << "������¼��Ŀγ���Ϣ!ע��γ����ʽ�Ϊ(������=0|���޿�=1|ѡ�޿�=2),�����Կո�ָ��ַ���" << endl;
	//������ʱ���뻺����
	Course* course = (Course*)malloc(sizeof(Course));
	//����������
	InputGroup ipg{ 1 };
	int buffer = 0;
	ipg.Add("�γ̱��(int):", "%lld", &(course->id));
	ipg.Add("�γ�����(str):", "%s", &(course->name));
	ipg.Add("�γ�����(int):", "%d", &(course->nat));
	ipg.Add("�� ѧ ʱ(flo):", "%f", &(course->class_period));
	ipg.Add("�ڿ�ѧʱ(flo):", "%f", &(course->teaching_time));
	ipg.Add("ʵ��ѧʱ(flo):", "%f", &(course->experiental_time));
	ipg.Add("ѧ    ��(flo):", "%f", &(course->credit));
	ipg.Add("����ѧ��(str):", "%s", &(course->term));
	ipg.Add("ȷ��1/0)(int):", "%d", &buffer);
	//��ӡ������
	ipg.Print();
	//����ö�ٷ�Χ
	if (course->nat > NaN || course->nat < Pub) course->nat = NaN;
	//��ȡ����
	if (ipg.GetInput() && buffer)
	{
		system("cls");
		if (exist_course(course->id))
		{
			cout << "�Ѵ�����ͬ��ŵĿγ̣��������ԣ�";
		}
		else
		{
			inputone = true;
			courses.push_back(course);
			save_courses(courses, FILENAME1);
			cout << "¼��γ̳ɹ�!��ص�GUI�����������";
		}
	}
	else
	{
		system("cls");
		cout << "¼��ʧ�ܣ�";
	}
	//�����ϼ��˵�
	menu.Last();
}
//ɾ���γ�
void del_course(Menu& menu, Canvas& canvas)
{
	//������������뻺������������ʾ
	system("cls");
	rewind(stdin);
	MessageBox(NULL, ("���ڿ���̨������ɾ���Ŀγ̱��"), ("��ʾ"), MB_SYSTEMMODAL | MB_ICONINFORMATION);
	//��ȡ����
	cout << "�γ̱��:";
	long long id;
	int x = scanf("%lld", &id);
	//�����γ̽��
	bool result = true;


	system("cls");

	//����if��˵����ʽ����
	if (x == 0)
	{
		MessageBox(NULL, ("�����������벢���³���"), ("�����ʽ����"), MB_SYSTEMMODAL | MB_ICONSTOP);
		cout << "�����ʽ����!";

	}
	else for (int i = 0; i < courses.size(); i++)
	{
		//ɾ���γ̲�����
		if (courses[i]->id == id)
		{
			free(*(courses.begin() + i));
			courses.erase(courses.begin() + i);
			cout << "�Ƴ��γ̳ɹ�!";
			result = false;
			save_courses(courses, FILENAME1);
			break;
		}
	}
	//�����������
	if (result)
	{
		cout << "�Ҳ����˿γ̣������������ԣ�";
	}
	//�����ϼ��˵�
	menu.Last();
}
//�޸Ŀγ�
void set_course(Menu& menu, Canvas& canvas)
{
	system("cls");
	rewind(stdin);
	MessageBox(NULL, ("���ڿ���̨������Ҫ�޸ĵĿγ̱��"), ("��ʾ"), MB_SYSTEMMODAL | MB_ICONINFORMATION);
	cout << "�γ̱��:";
	long long id;
	int x = scanf("%lld", &id);
	bool result = true;
	system("cls");
	Course* temp;
	if (x == 0)
	{
		MessageBox(NULL, ("�����������벢���³���"), ("�����ʽ����"), MB_SYSTEMMODAL | MB_ICONSTOP);
		cout << "�����ʽ����!";

	}
	else for (int i = 0; i < courses.size(); i++)
	{
		if (courses[i]->id == id)
		{
			int size = courses.size();
			input(menu, canvas);
			if (size != courses.size())
			{
				free(*(courses.begin() + i));
				courses.erase(courses.begin() + i);
				save_courses(courses, FILENAME1);
			}
			result = false;
			break;
		}
	}
	if (result)
	{
		cout << "�Ҳ����˿γ̣������������ԣ�";
	}
	menu.Last();
}
#pragma endregion
#pragma region �γ���Ϣ���
//������
void browsing(Menu& menu, Canvas& canvas)
{
	pointer_courses = &courses;
	canvas.Env(1).Draw(gird_id);
	canvas.Env(1).Draw(ret_id);
	canvas.Env(1).Draw(last_id);
	canvas.Env(1).Draw(next_id);
	canvas.Env(1).Draw(id_sort);
	canvas.Env(1).Draw(credit_sort);
	canvas.Env(1).Draw(time_sort);
}
//ˢ�±��ִֻ��һ��
void fresh(Menu& menu)
{
	pointer_courses = &courses;
	page1 = 0;
	lastPage(*menu.canvas);
}
#pragma endregion
#pragma region �γ���Ϣ��ѯ
vector<Course*> query_courses;
//��ȡ��ѯ����
void query(Menu& menu, Canvas& canvas)
{
	MessageBox(NULL, ("���ڿ���̨����ɸѡ����"), ("��ʾ"), MB_SYSTEMMODAL | MB_ICONINFORMATION);
	rewind(stdin);
	system("cls");
	query_courses.clear();
	//MessageBox(NULL,"��ʾ", "���ڿ���̨��¼��γ���Ϣ!",MB_HELP|MB_OK);
	cout << "������ɸѡ������" << endl;

	Course* course = (Course*)malloc(sizeof(Course));

	if (course != NULL)
	{
		InputGroup ipg{ 1 ,false };
		ipg.Add("�γ̱��(int):", "%lld", &(course->id));
		ipg.Add("�γ�����(str):", "%s", &(course->name));
		ipg.Add("�γ�����(int):", "%d", &(course->nat));
		ipg.Add("�� ѧ ʱ(flo):", "%f", &(course->class_period));
		ipg.Add("�ڿ�ѧʱ(flo):", "%f", &(course->teaching_time));
		ipg.Add("ʵ��ѧʱ(flo):", "%f", &(course->experiental_time));
		ipg.Add("ѧ    ��(flo):", "%f", &(course->credit));
		ipg.Add("����ѧ��(str):", "%s", &(course->term));
		ipg.Print();

		if (course->nat > NaN || course->nat < Pub) course->nat = NaN;

		ipg.GetInput();


		for (auto& i : courses)
		{
			if (i->id == course->id || !strcmp(course->name, i->name) || i->nat == course->nat || abs(i->class_period - course->class_period) < 0.1f
				|| abs(i->experiental_time - course->experiental_time) < 0.1f || abs(i->teaching_time - course->teaching_time) < 0.1f ||
				abs(course->credit - i->credit) < 0.1f || !strcmp(i->term, course->term))
			{
				query_courses.push_back(i);
			}
		}
	}
}
//չʾ��ѯ����UI
void queryshow(Menu& menu, Canvas& canvas)
{
	canvas.Env(1).Draw(gird_id);
	canvas.Env(1).Draw(ret_id);
	canvas.Env(1).Draw(last_id);
	canvas.Env(1).Draw(next_id);
}
//ˢ�±��
void fresh_query(Menu& menu)
{
	query(menu, *menu.canvas);
	pointer_courses = &query_courses;
	page1 = 0;
	lastPage(*menu.canvas);

}
#pragma endregion
#pragma region ѧ��ѡ��
vector<Course*> select_courses;
//ѡ�ζ�ȡ
void choose(Menu& menu, Canvas& canvas)
{
	rewind(stdin);
	system("cls");
	MessageBox(NULL, ("���ڿ���̨����ѡ�α��,ѡ����ѧ�ֲ��õ���30"), ("��ʾ"), MB_SYSTEMMODAL | MB_ICONINFORMATION);
	long long id;
	InputGroup ipg = { 0 };
	ipg.Add("��������ѡ��,�����Ƴ�");
	ipg.Add("ѡ�α��:", "%lld", &id);
	ipg.Print();
	ipg.GetInput();
	if (exist_course(abs(id)))
	{
		if (id >= 0)
		{
			system("cls");
			if (!exist_course(id, &select_courses))
			{
				cout << "ѡ�γɹ�";
				select_courses.push_back(find_course(id));
				save_courses(select_courses, FILENAME2);
			}
			else
			{

				cout << "�Ѿ�ѡ����ÿγ�";
			}
		}
		else
		{
			system("cls");
			if (!exist_course(id, &select_courses))
			{
				remove_course(id, &select_courses);
				cout << "�Ƴ��ɹ�";
				save_courses(select_courses, FILENAME2);
			}
			else
			{
				cout << "��û��ѡ����ÿγ�";
			}
		}
	}
	else
	{
		system("cls");
		cout << "�����ڸÿγ̣�";
	}
	menu.Last();
}
//��ѡ�γ̿β鿴
void view_selected(Menu& menu, Canvas& canvas)
{
	canvas.Env(1).Draw(gird_id);
	canvas.Env(1).Draw(ret_id);
	canvas.Env(1).Draw(last_id);
	canvas.Env(1).Draw(next_id);
	canvas.Env(1).Draw(alls_id);
}
//ˢ�±��
void fresh_selected(Menu& menu)
{
	pointer_courses = &select_courses;
	page1 = 0;
	float allscore = 0;
	for (auto& i : select_courses)
	{
		allscore += i->credit;
	}
	((Text*)menu.canvas->Env(1).GetGUI(alls_id))->SetText("��ѧ��:" + ftostr(allscore, 1));
	lastPage(*menu.canvas);
}
#pragma endregion
#pragma region �˳��뷵��
//�˳�ϵͳ
void exit_system(Menu& menu, Canvas& canvas)
{
	save_courses(courses, FILENAME1);
	save_courses(select_courses, FILENAME2);
	//��տ���̨
	system("cls");
	printf("��л����ʹ�ã��ڴ��´μ���");
	//�رջ�����Ⱦ
	canvas.Close();
	//���ղ˵��������Լ����и��ڵ�
	delete& menu;
	//�԰�ȫ����code�˳�Ӧ�ó���
	exit(0);
}
//�����ϲ�˵�
void last_menu(Menu& menu, Canvas& canvas)
{
	menu.Last();
	menu.Last();
}
#pragma endregion
#pragma endregion

#pragma region ȫ�ֱ���_2
//�˵��ڵ������
Menu* menu;
#pragma endregion

#pragma region ������������

//��������-Start | Canvas ��Ϣ | Priority=0
void OnStart(Canvas& canvas)
{
#pragma region ������Ϣ��ʼ��
	//���ô��ڱ���
	SetWindowText(GetHWnd(), "�γ���Ϣ����ϵͳ");
	//��ȡȫ���γ���Ϣ
	read_courses(courses, FILENAME1);
	//��ȡѡ����Ϣ
	read_courses(select_courses, FILENAME2);
#pragma endregion	
#pragma region Menu�ڵ�ע��
	//�������ڵ�ͽڵ�˵�������
	Node* root = new Node();
	menu = new Menu(root, &canvas);

	//���˵��ڵ�
	Node* m1 = new Node(root, "�γ���Ϣ¼��");
	Node* m2 = new Node(root, "�γ���Ϣ���", browsing, true, fresh);
	Node* m3 = new Node(root, "�γ���Ϣ��ѯ", queryshow, true, fresh_query);
	Node* m4 = new Node(root, "ѧ������ѡ��");
	Node* m6 = new Node(root, "��ȫ�˳�ϵͳ", exit_system, true);

	//���˵�ѡ��1  һ���˵��ڵ�
	Node* x1 = new Node(m1, "¼��γ���Ϣ", input, true);
	Node* x2 = new Node(m1, "�޸Ŀγ���Ϣ", set_course, true);
	Node* x3 = new Node(m1, "ɾ���γ���Ϣ", del_course, true);
	Node* x4 = new Node(m1, "����", last_menu, true);

	//���˵�ѡ��4  һ���˵��ڵ�
	Node* n1 = new Node(m4, "����ѡ��", choose, true);
	Node* n2 = new Node(m4, "��ѡ�γ̲鿴", view_selected, true, fresh_selected);
	Node* n3 = new Node(m4, "����", last_menu, true);
#pragma endregion
#pragma region �ڵ�˵�GUIע��

	menu->RegisterMenuByRootNode( 0, 100, 100, 300, 50,10, BUTTONCOLOR, BUTTONFONTCOLOR, LINEBOXCOLOR, FONTNAME);

#pragma endregion
#pragma region ����ע��
	Image* background = new Image(createRectbyPoint(0, 0, 1000, 600),BGFILENAME);
	bg_id = background->InstanceId();
	canvas.Env(1).Register(background->InstanceId(), background);
#pragma endregion
#pragma region Gird����ע��
	//��������ʼ��
	Gird* gird = new Gird(createRectbyPoint(50, 50, 900, 500), 8, 11, GIRDLINECOLOR, FONTNAME, GIRDFONTCOLOR);
	canvas.Env(1).Register(gird->InstanceId(), gird);
	gird_id = gird->InstanceId();
#pragma endregion
#pragma region ��ͷ��ʼ��
	gird->SetUnit(0, 0, "�γ̱��");
	gird->SetUnit(0, 1, "�γ�����");
	gird->SetUnit(0, 2, "�γ�����");
	gird->SetUnit(0, 3, "��ѧʱ");
	gird->SetUnit(0, 4, "�ڿ�ѧʱ");
	gird->SetUnit(0, 5, "ʵ��ѧʱ");
	gird->SetUnit(0, 6, "ѧ��");
	gird->SetUnit(0, 7, "����ѧ��");
#pragma endregion
#pragma region ���ܰ�ťע��
	//���ذ�ťע��
	Rect rct = createRectbyCenter({ 100,550 }, 50, 20);
	Image* bg = new Image(rct, BUTTONCOLOR);
	Text* txt = new Text("����", rct, FONTNAME, BUTTONFONTCOLOR);
	LineBox* lb = new LineBox(rct, LINEBOXCOLOR);
	Button* ret = new Button(bg, txt, lb);
	ret->AddListener([&]() {menu->Last(); page1 = 0; });
	ret_id = ret->InstanceId();
	canvas.Env(1).Register(ret_id, ret);

	//��ҳ��ťע��
	Rect rc2 = createRectbyCenter({ 200,550 }, 50, 20);
	Image* bg2 = new Image(rc2, BUTTONCOLOR);
	Text* txt2 = new Text("��ҳ", rc2, FONTNAME, BUTTONFONTCOLOR);
	LineBox* lb2 = new LineBox(rc2, LINEBOXCOLOR);
	Button* ret2 = new Button(bg2, txt2, lb2);
	ret2->AddListener([&]() {lastPage(canvas); });
	last_id = ret2->InstanceId();
	canvas.Env(1).Register(last_id, ret2);

	//��ҳ��ťע��
	Rect rc3 = createRectbyCenter({ 300,550 }, 50, 20);
	Image* bg3 = new Image(rc3, BUTTONCOLOR);
	Text* txt3 = new Text("��ҳ", rc3, FONTNAME, BUTTONFONTCOLOR);
	LineBox* lb3 = new LineBox(rc3, LINEBOXCOLOR);
	Button* ret3 = new Button(bg3, txt3, lb3);
	ret3->AddListener([&]() {nextPage(canvas); });
	next_id = ret3->InstanceId();
	canvas.Env(1).Register(next_id, ret3);
#pragma endregion
#pragma region ������ťע��
	//ID����ťע��
	Rect rc4 = createRectbyCenter({ 400,550 }, 80, 20);
	Image* bg4 = new Image(rc4, BUTTONCOLOR);
	Text* txt4 = new Text("�������", rc4, FONTNAME, WHITE);
	LineBox* lb4 = new LineBox(rc4, LINEBOXCOLOR);
	Button* ret4 = new Button(bg4, txt4, lb4);
	ret4->AddListener([&]() {sortByid(courses); page1 = 0; lastPage(canvas); });
	id_sort = ret4->InstanceId();
	canvas.Env(1).Register(id_sort, ret4);

	//��ʱ����ťע��
	Rect rc5 = createRectbyCenter({ 500,550 }, 80, 20);
	Image* bg5 = new Image(rc5, BUTTONCOLOR);
	Text* txt5 = new Text("��ʱ����", rc5, FONTNAME, WHITE);
	LineBox* lb5 = new LineBox(rc5, LINEBOXCOLOR);
	Button* ret5 = new Button(bg5, txt5, lb5);
	ret5->AddListener([&]() {sortByTime(courses); page1 = 0; lastPage(canvas); });
	time_sort = ret5->InstanceId();
	canvas.Env(1).Register(time_sort, ret5);

	//ѧ������ťע��
	Rect rc6 = createRectbyCenter({ 600,550 }, 80, 20);
	Image* bg6 = new Image(rc6, BUTTONCOLOR);
	Text* txt6 = new Text("ѧ������", rc6, FONTNAME, WHITE);
	LineBox* lb6 = new LineBox(rc6, LINEBOXCOLOR);
	Button* ret6 = new Button(bg6, txt6, lb6);
	ret6->AddListener([&]() {sortByCredit(courses); page1 = 0; lastPage(canvas); });
	credit_sort = ret6->InstanceId();
	canvas.Env(1).Register(credit_sort, ret6);
#pragma endregion
#pragma region ѡ����Ϣ��ʾUIע��
	//��ѡ�γ���ѧ��ע��
	Text* all_credit = new Text("��ѧ��:", createRectbyPoint(50, 10, 200, 40), false);
	alls_id = all_credit->InstanceId();
	canvas.Env(1).Register(all_credit->InstanceId(), all_credit);
#pragma endregion
}
//��������-OnGUI | Canvas ��Ϣ | Priority=1
void OnGUI(Canvas& canvas)
{
	canvas.Env(1).Draw(bg_id); //��Ⱦ����
	//���ܽڵ����о���ʵ���߼�
	if (menu->current->funcNode)
	{
		if (menu->current->func != nullptr) menu->current->func(*menu, canvas);
	}
	//�ǹ��ܽڵ㰴λ�÷ֲ��Զ���Ⱦ,��Env0�ڽ��в���
	if (!menu->current->funcNode) for (int i = 0; i < (menu->current)->childs.size(); i++)
	{
		canvas.Env(0).Draw(menu->current->childs[i]->InstanceId());//��Ⱦ�ڵ�˵�GUI
	}
}

//��������-OnUpdate | Canvas ��Ϣ | Priority=2
void OnUpdate(Canvas& canvas)
{

}
#pragma endregion

int main()
{
	Canvas canvas = { 1000,600,90, RGB(255,255,255) }; //����1000*600�Ĵ���������ɫΪ��ɫ
	canvas.Show(OnStart, OnUpdate, OnGUI,true); //��ʼ��GUI�����������������ڣ���������,��ʾС�ڴ�
	return 0;
}