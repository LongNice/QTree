#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <map>
#include <vector>

#define POLYGON_NUM 20
#define DEBUG 1

using namespace std;

class Point {
public:
	Point() { x = 0; y = 0; }
	Point* operator + (Point* p) {
		Point* res = new Point();
		res->x = x + p->x;
		res->y = y + p->y;
		return res;
	}
	bool operator > (Point* p) {
		if ((x > p->x) && (y > p->y))
			return true;
		return false;
	}
	bool operator < (Point* p) {
		if ((x < p->x) && (y < p->y))
			return true;
		return false;
	}
	bool operator >= (Point* p) {
		if ((x >= p->x) && (y >= p->y))
			return true;
		return false;
	}
	bool operator <= (Point* p) {
		if ((x <= p->x) && (y <= p->y))
			return true;
		return false;
	}
	bool operator == (Point* p) {
		if ((x = p->x) && (y = p->y))
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
		Polygon* poly = getBoundary();
		if (poly->getX() <= p->x && poly->getX() + poly->getWidth() >= p->x && poly->getY() <= p->y && poly->getY() + poly->getHeight() >= p->y)
			return true;
		return false;
	}
	bool checkCollision(Polygon *range) {
		if (boundary->getLeftTop() <= range->getLeftTop() ||
			(boundary->getX() + boundary->getWidth()) >= range->getLeftBot()->x ||
			(boundary->getY() + boundary->getHeight()) >= range->getRightTop()->y ||
			boundary->getRightBot() >= range->getRightBot())
			return true;
		if (range->getLeftTop() <= boundary->getLeftTop() ||
			(range->getX() + range->getWidth()) >= boundary->getLeftBot()->x ||
			(range->getY() + range->getHeight()) >= boundary->getRightTop()->y ||
			range->getRightBot() >= boundary->getRightBot())
			return true;

		return false;
	}
	bool checkCollision(Polygon* range, Point *p) {
		if (range->getX() <= p->x && range->getX() + range->getWidth() >= p->x && range->getY() <= p->y && range->getY() + range->getHeight() >= p->y)
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
		else if (getObjNum() == getMaxObj() && level == 0) {
			Polygon *boundary = getBoundary();
			int x = boundary->getX();
			int y = boundary->getY();
			int w = boundary->getWidth();
			int h = boundary->getHeight();

			QTree *qt = new QTree(x, y, w/2, h/2);
			QTree* qt2 = new QTree(x, y + (h / 2), w / 2, h / 2);
			QTree* qt3 = new QTree(x + (w / 2), y, w / 2, h / 2);
			QTree* qt4 = new QTree(x + (w / 2), y + (h / 2), w / 2, h / 2);
			area->push_back(qt);
			area->push_back(qt2);
			area->push_back(qt3);
			area->push_back(qt4);
			level++;
			return true;
		}
		else {
			vector<QTree*>::iterator QtIt = area->begin();
			for (; QtIt != area->end(); QtIt++) {
				QTree *qt = *QtIt;
				if (qt->insert(p)) return true;
			}	
		}
	}
	void query(Polygon* range, multimap<int, int> *res) {
		if (!checkCollision(range))
			return;
		vector<Point*>::iterator pvIt = obj->begin();
		for (; pvIt != obj->end() ; pvIt++ ) {
			Point* p = *pvIt;
			if (checkCollision(range, p))
				res->insert(pair<int, int> {p->x, p->y});
		}
		vector<QTree*>::iterator qvIt = area->begin();
		for (; qvIt != area->end(); qvIt++) {
			QTree* qt = *qvIt;
			qt->query(range, res);
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
}

void showResults( multimap <int, int> *res) {
	multimap<int, int>::iterator mit = res->begin();
	for (; mit != res->end(); mit++)
		cout << "Point( " << mit->first << " " << mit->second << " ) in range." << endl;
}

int main()
{
	QTree *mainQTree = new QTree(0, 0, 1000, 1000);
	Polygon *range = new Polygon(250, 250, 500, 500);
	vector<Polygon*> *pv = new vector<Polygon*>;
	multimap<int, int> *res = new multimap<int, int>;
	srand(time(0));
	createRandomPolygon(pv);
	insertPoly2QTree(mainQTree, pv);
	mainQTree->query(range, res);
	showResults(res);
	cout << "============================" << endl;
	cout << "Collision Detection is Done!" << endl;
	cout << "============================" << endl;
}

