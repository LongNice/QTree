#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <map>
#include <vector>
#include <chrono>

#define POLYGON_NUM 1000
#define DEBUG 0

using namespace std;

class Point {
public:
	Point() { x = 0; y = 0; }
	Point* operator + (const Point &p) {
		Point* res = new Point();
		res->x = x + p.x;
		res->y = y + p.y;
		return res;
	}
	bool operator > (const Point &p) {
		if ((this->x > p.x) && (this->y > p.y))
			return true;
		return false;
	}
	bool operator < (const Point &p) {
		if ((this->x < p.x) && (this->y < p.y))
			return true;
		return false;
	}
	bool operator >= (const Point &p) {
		if ((this->x >= p.x) && (this->y >= p.y))
			return true;
		return false;
	}
	bool operator <= (const Point &p) {
		if ((this->x <= p.x) && (this->y <= p.y))
			return true;
		return false;
	}
	bool operator == (const Point &p) {
		if ((this->x = p.x) && (this->y = p.y))
			return true;
		return false;
	}
	int x;
	int y;
};

class Polygon {
public:
	Polygon() {
		leftTop = new Point();
		width = 0;
		height = 0;
	}
	Polygon(int x, int y, int w, int h) {
		leftTop = new Point();
		leftTop->x = x;
		leftTop->y = y;
		width = w;
		height = h;
	}
	Polygon(Point *p, int w, int h) {
		leftTop = new Point();
		leftTop = p;
		width = w;
		height = h;
	}
	void createRandomPoint() {
		leftTop->x = rand() % (900 + 1);
		leftTop->y = rand() % (900 + 1);
		width = rand() % (100 + 1);
		height = rand() % (100 + 1);
	}
	void setCoordinate(int x, int y) {
		leftTop->x = x;
		leftTop->y = y;
	}
	void setBoundary(int w, int h) {
		width = w;
		height = h;
	}
	Point* getLeftTop() {
		return leftTop;
	}
	Point* getLeftBot() {
		Point *p = new Point();
		p->x = leftTop->x; 
		p->y = leftTop->y + height;
		return p;
	}
	Point* getRightTop() {
		Point *p = new Point();
		p->x = leftTop->x + width;
		p->y = leftTop->y;
		return p;
	}
	Point* getRightBot() {
		Point *p = new Point();
		p->x = leftTop->x + width;
		p->y = leftTop->y + height;
		return p;
	}
	int getX() {
		return leftTop->x;
	}
	int getY() {
		return leftTop->y;
	}
	int getWidth() { 
		return width;
	}
	int getHeight() {
		return height;
	}
	void setX(int x) {
		leftTop->x = x;
	}
	void setY(int y) {
		leftTop->y = y;
	}
	void setWidth(int w) {
		width = w;
	}
	void setHeight(int h) {
		height = h;
	}
private:
	Point *leftTop;
	int width;
	int height;
};

class QTree {
public:
	void QTreeInit() {
		maxObj = 4;
		objNum = 0;
		level = 0;
		obj = new vector<Point*>;
		area = new vector<QTree*>;
		boundary = new Polygon;
	}
	QTree(Polygon *poly) {
		QTreeInit();
		boundary->setCoordinate(poly->getX(), poly->getY());
		boundary->setBoundary(poly->getWidth(), poly->getHeight());
	}
	QTree(int x, int y, int w, int h) {
		QTreeInit();
		boundary->setCoordinate(x, y);
		boundary->setBoundary(w, h);
	}
	int getMaxObj() {
		return maxObj;
	}
	int getObjNum() {
		return objNum;
	}
	void objNumIncr() {
		objNum++;
	}
	Polygon* getBoundary() {
		return boundary;
	}
	bool checkCollision(Point *p) {
		Polygon* boundary = getBoundary();
		if (*(boundary->getLeftTop()) <= *p && *(boundary->getRightBot()) >= *p)
			return true;
		return false;
	}
	bool checkCollision(Polygon *range) {
		if (*(boundary->getLeftTop()) <= *(range->getLeftTop()) && *(boundary->getRightBot()) >= *(range->getLeftTop()) ||
			*(boundary->getLeftTop()) <= *(range->getLeftBot()) && *(boundary->getRightBot()) >= *(range->getLeftBot()) ||
			*(boundary->getLeftTop()) <= *(range->getRightTop()) && *(boundary->getRightBot()) >= *(range->getRightTop()) ||
			*(boundary->getLeftTop()) <= *(range->getRightBot()) && *(boundary->getRightBot()) >= *(range->getRightBot()))
			return true;
		if (*(range->getLeftTop()) <= *(boundary->getLeftTop()) && *(range->getRightBot()) >= *(boundary->getLeftTop()) ||
			*(range->getLeftTop()) <= *(boundary->getLeftBot()) && *(range->getRightBot()) >= *(boundary->getLeftBot()) ||
			*(range->getLeftTop()) <= *(boundary->getRightTop()) && *(range->getRightBot()) >= *(boundary->getRightTop()) ||
			*(range->getLeftTop()) <= *(boundary->getRightBot()) && *(range->getRightBot()) >= *(boundary->getRightBot()))
			return true;

		return false;
	}
	bool checkCollision(Polygon* range, Point *p) {
		if (*(range->getLeftTop()) <= *p && *(range->getRightBot()) >=*p) 
			return true;

		return false;
	}
	bool checkAllCover(Polygon *range) {
		if (*(range->getLeftTop()) <= *(boundary->getLeftTop()) && *(range->getRightBot()) >= *(boundary->getLeftTop()) &&
			*(range->getLeftTop()) <= *(boundary->getLeftBot()) && *(range->getRightBot()) >= *(boundary->getLeftBot()) &&
			*(range->getLeftTop()) <= *(boundary->getRightTop()) && *(range->getRightBot()) >= *(boundary->getRightTop()) &&
			*(range->getLeftTop()) <= *(boundary->getRightBot()) && *(range->getRightBot()) >= *(boundary->getRightBot()))
			return true;
		return false;
	}
	bool insert(Point *p) {
		if (!checkCollision(p))
			return false;

		if (getObjNum() < getMaxObj()) {
			obj->push_back(p);
			objNumIncr();
			return true;
		}

		Polygon* boundary = getBoundary();
		int x = boundary->getX();
		int y = boundary->getY();
		int w = boundary->getWidth();
		int h = boundary->getHeight();

		QTree* qt = new QTree(x, y, w / 2, h / 2);
		QTree* qt2 = new QTree(x, y + (h / 2), w / 2, h / 2);
		QTree* qt3 = new QTree(x + (w / 2), y, w / 2, h / 2);
		QTree* qt4 = new QTree(x + (w / 2), y + (h / 2), w / 2, h / 2);
		area->push_back(qt);
		area->push_back(qt2);
		area->push_back(qt3);
		area->push_back(qt4);
		level++;

		vector<QTree*>::iterator QtIt = area->begin();
		for (; QtIt != area->end(); QtIt++) {
			QTree *qt = *QtIt;
			if (qt->insert(p)) return true;
		}	
	}
	void query(Polygon* range, multimap<int, int> *res, bool cover) {
		if (!cover && !checkCollision(range))
			return;
		if (checkAllCover(range))
			cover = true;
		
		vector<Point*>::iterator pvIt = obj->begin();
		for (; pvIt != obj->end() ; pvIt++ ) {
			Point* p = *pvIt;
			if (cover || checkCollision(range, p))
				res->insert(pair<int, int> {p->x, p->y});
		}
		vector<QTree*>::iterator qvIt = area->begin();
		for (; qvIt != area->end(); qvIt++) {
			QTree* qt = *qvIt;
			qt->query(range, res, cover);
		}
	}
private:
	int maxObj;
	int objNum;
	int level;
	vector<Point*> *obj;
	vector<QTree*> *area;
	Polygon *boundary;
};

void createRandomPolygon(vector<Polygon*> *vp) {
	for (int i = 0; i < POLYGON_NUM; i++) {
		Polygon* p = new Polygon();
		p->createRandomPoint();
		vp->push_back(p);
#if DEBUG
		cout << "Left Top x: " << p->getLeftTop()->x << " Left Top y:" << p->getLeftTop()->y << endl;
		cout << "Left Bot x: " << p->getLeftBot()->x << " Left Bot y:" << p->getLeftBot()->y << endl;
		cout << "Right Top x: " << p->getRightTop()->x << " Right Top y:" << p->getRightTop()->y << endl;
		cout << "Right Bot x: " << p->getRightBot()->x << " Right Bot y:" << p->getRightBot()->y << endl;
#endif
		//p = nullptr;
	}
	cout << "==============================" << endl;
	cout << "Create Random Polygon is Done!" << endl;
	cout << "==============================" << endl;
	cout << endl;
}

void insertPoly2QTree(QTree *qt, vector<Polygon*> *pv) {
	vector<Polygon*>::iterator vpIt = pv->begin();
	for (; vpIt != pv->end(); vpIt++) {
		Polygon* poly = *vpIt;
		qt->insert(poly->getLeftTop());
		qt->insert(poly->getLeftBot());
		qt->insert(poly->getRightTop());
		qt->insert(poly->getRightBot());
	}
	cout << "================================" << endl;
	cout << "Insert Polygon to QTree is Done!" << endl;
	cout << "================================" << endl;
	cout << endl;
}

void insertPoly2Map(multimap<int, Polygon*> *mm, vector<Polygon*> *pv) {
	vector<Polygon*>::iterator vpIt = pv->begin();
	for (; vpIt != pv->end(); vpIt++) {
		Polygon* poly = *vpIt;
		mm->insert(pair<int, Polygon*>{poly->getLeftTop()->x, poly});
	}
	cout << "================================" << endl;
	cout << "Insert Polygon to Map is Done!" << endl;
	cout << "================================" << endl;
	cout << endl;
}

bool checkCollision(Polygon* range, Point* p) {
	if (*(range->getLeftTop()) <= *p && *(range->getRightBot()) >= *p)
		return true;
	return false;
}

void mapQuery(Polygon* range, multimap<int, Polygon*> *normalMap, multimap<int, int>* res) {
	multimap<int, Polygon*>::iterator mapIt = normalMap->begin();
	for (; mapIt != normalMap->end(); mapIt++) {
		Polygon* poly = mapIt->second;
		if (poly->getLeftTop()->x > range->getRightTop()->x)
			break;

		if (checkCollision(range, poly->getLeftTop()))
			res->insert(pair<int, int> {poly->getLeftTop()->x, poly->getLeftTop()->y});
		if (checkCollision(range, poly->getLeftBot()))
			res->insert(pair<int, int> {poly->getLeftBot()->x, poly->getLeftBot()->y});
		if (checkCollision(range, poly->getRightTop()))
			res->insert(pair<int, int> {poly->getRightTop()->x, poly->getRightTop()->y});
		if (checkCollision(range, poly->getRightBot()))
			res->insert(pair<int, int> {poly->getRightBot()->x, poly->getRightBot()->y});
	}
}

void showResults( multimap <int, int> *res) {
	multimap<int, int>::iterator mit = res->begin();
	for (; mit != res->end(); mit++)
		cout << "Point( " << mit->first << " " << mit->second << " ) in range." << endl;
}

int main()
{
	QTree *mainQTree = new QTree(0, 0, 1000, 1000);
	Polygon *range = new Polygon(750, 750, 250, 250);
	vector<Polygon*> *pv = new vector<Polygon*>;
	multimap<int, Polygon*> *normalMap = new multimap<int, Polygon*>;
	multimap<int, int> *res = new multimap<int, int>;
	multimap<int, int>* res2 = new multimap<int, int>;
	srand(time(0));
	createRandomPolygon(pv);
	insertPoly2QTree(mainQTree, pv);
	insertPoly2Map(normalMap, pv);

	auto begin = std::chrono::high_resolution_clock::now();

	mainQTree->query(range, res, false);

	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
	printf("Time measured: %.3f seconds.\n", elapsed.count() * 1e-9);
	showResults(res);

	cout << endl;
	cout << "============================" << endl;
	cout << endl;

	begin = std::chrono::high_resolution_clock::now();

	mapQuery(range, normalMap, res2);

	end = std::chrono::high_resolution_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
	printf("Time measured: %.3f seconds.\n\n", elapsed.count() * 1e-9);
	showResults(res2);

	cout << "============================" << endl;
	cout << "Collision Detection is Done!" << endl;
	cout << "============================" << endl;
}

