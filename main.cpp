#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::min;
using std::max;
using std::sort;
using std::unordered_map;
using std::unordered_set;
using std::vector;

// Данной погрешности вычислений обычно хватает, но в теории могут возникать проблемы
const double eps = 1e-9;

// Это тип координат точек. Его можно поменять на целочисленный тип, убрав вычисления с погрешностями
typedef double dataT;

// Вектор в двумерном пространстве
struct CVec2 {
    CVec2() = default;
    CVec2( dataT _x, dataT _y, int _left = 0, int _right = 0 ):
            x( _x ), y( _y ), left( _left ), right( _right ) {}
    dataT x, y;
    int left, right;    // Эти поля отвечают за соседей слева и справа в МВО, можно их убрать и хранить двусвязный список отдельно

    CVec2 operator+( const CVec2& other ) const;
    CVec2 operator-( const CVec2& other ) const;
    CVec2 operator-() const;
};

CVec2 CVec2::operator+( const CVec2 &other ) const
{
    return CVec2( x + other.x, y + other.y );
}

CVec2 CVec2::operator-( const CVec2 &other ) const
{
    return CVec2( x - other.x, y - other.y );
}

CVec2 CVec2::operator-() const
{
    return CVec2( -x, -y );
}

// Модуль векторного произведения со знаком
dataT CrossProduct( const CVec2& fst, const CVec2& snd )
{
    return fst.x * snd.y - fst.y * snd.x;
}

// Тип ребро, фактически работает как std::pair<int, int>
struct Edge {
    Edge( int _v1 = -1, int _v2 = -1 ):
            v1( _v1 ), v2( _v2 ) {}
    int v1;    // Вершины, которые соединяет ребро
    int v2;
    bool operator==( const Edge& other ) const {
        return v1 == other.v1 && v2 == other.v2;
    }
};

// Определение хэш-функции для ребра, как комбинация хэшей вершин
struct EdgeHasher {
    std::hash<int> hasher;
    size_t operator() ( const Edge& edge ) const {
        size_t lhs = hasher( edge.v1 );
        size_t rhs = hasher( edge.v2 );
        lhs ^= rhs + 0x9e3779b9 + ( lhs << 6 ) + ( lhs >> 2 );
        return lhs;
    }
};

// Множество из 2 вершин, нужна для удобства 
struct TwoVertices {
    TwoVertices( int v1 = -1, int v2 = -1 ): outer1( v1 ), outer2( v2 ) {}
    int outer1;
    int outer2;
    void insert( int v ) {
        if( outer1 == v || outer2 == v ) return;
        assert( this->size() < 2 );
        ( outer1 == -1 ? outer1 : outer2 ) = v;
    }
    void erase( int v ) {
        if( outer1 == v ) {
            outer1 = -1;
        } else if( outer2 == v ) {
            outer2 = -1;
        }
    }
    int GetMax() {
        assert( outer1 != -1 || outer2 != -1 );
        return max( outer1, outer2 );
    }
    int GetMin() {
        if( outer1 != -1 && outer2 != -1 ) {
            return min( outer1, outer2 );
        }
        assert( outer1 != -1 || outer2 != -1 );
        return outer1 != -1 ? outer1 : outer2;
    }
    int size() { return ( outer1 != -1 ) + ( outer2 != -1 ); }
};

class Solver {

public:
    Solver() = default;
    void Solve();
private:
	// Функция, которая рекурсивно перестраивает триангуляцию (шаг 4)
    void restructure( int left, int right, int cur );

	// Проверяет, нужно ли перестраивать четырехугольник, заданный индексами точек в аргументах
    bool check( int left, int right, int cur, int innerPt );

    vector<CVec2> points;    // Храним точки как радиус-вектора
    vector<Edge> recursionStack;    // Моделируем рекурсию с помощью стека, чтобы не было stack overflow
    unordered_map<Edge, TwoVertices, EdgeHasher> triangulation;  // Структура для хранения триангуляции: ребро -> пара вершин
};

// Компаратор для сортировки (в данном случае по координате х)
bool cmpPoints( const CVec2& fst, const CVec2& snd )
{
    return fst.x < snd.x;
}

void Solver::Solve()
{
    dataT x, y;
	// Считывание пар точек до EOF
    while( ( cin >> x ) && ( cin >> y ) ) {
        points.emplace_back( x, y );
    }
	// Размер стека рекурсии никогда не превышает количества точек
    recursionStack.resize( points.size() );
	// Сортировка по направлению
    sort( points.begin(), points.end(), cmpPoints );

    if( points.size() < 3 ) {
		// Если менее трех точек, то триангуляция пустая
        return;
    }
	
	// Инициализация первого треугольника
    if( CrossProduct( points[1] - points[0], points[2] - points[0] ) < 0 ) {
        points[0].left = 2; points[0].right = 1;
        points[1].left = 0; points[1].right = 2;
        points[2].left = 1; points[2].right = 0;
    } else {
        points[0].left = 1; points[0].right = 2;
        points[1].left = 2; points[1].right = 0;
        points[2].left = 0; points[2].right = 1;
    }
    triangulation[{ 1, 2 }].insert( 0 );
    triangulation[{ 0, 1 }].insert( 2 );
    triangulation[{ 0, 2 }].insert( 1 );


    for( int i = 3; i < points.size(); ++i ) {
        int currentPt = i - 1;    // Последняя добавленная точка
		// Пока, двигаясь вправо от точки, ребра видимые, выполняем рекурсивное перестроение
        while( CrossProduct( points[currentPt] - points[i], points[points[currentPt].right] - points[i] ) > -eps ) {
            restructure( currentPt, points[currentPt].right, i );
            currentPt = points[currentPt].right;
        }
        points[i].right = currentPt;    // Добавляем правого соседа МВО текущей точки

		// Аналогично для движения влево, пока ребра видимые
        currentPt = i - 1;

        while( CrossProduct( points[currentPt] - points[i], points[points[currentPt].left] - points[i] ) < eps ) {
            restructure( points[currentPt].left, currentPt, i );
            currentPt = points[currentPt].left;
        }
        points[i].left = currentPt;    // Левый сосед МВО текущей точки

        points[points[i].right].left = i;    // Обновление соседей
        points[currentPt].right = i;
    }

	// Последовательно собираем точки, находящиеся в выпуклой оболочке
	// Знаем, что 0-ая точка, так как самая дальняя при сортировке уже в МВО
	// Остальные получаем через двусвязный список
    unordered_set<int> convexHull;
    convexHull.insert( 0 );
    int hullPoint = 0;
    while( points[hullPoint].right != 0 ) {
        hullPoint = points[hullPoint].right;
        convexHull.insert( hullPoint );
    }

    // Тут в triangulation находится вся триангуляция в формате ребро -> две вершины для всех ребер, теперь можно с ней делать что угодно
}

// cur - всегда добавляемая точка, left и right - смежные с ней в треугольнике
// Четвертую точку для четырехугольника innerPt узнаем из ребра {left, right}
void Solver::restructure( int left, int right, int cur )
{
	// Инициализация стека рекурсии
	// хранить достаточно left и right
    recursionStack[0] = { left, right };
    int stackSize = 1;
    while( stackSize > 0 ) {
        left = recursionStack[stackSize - 1].v1; right = recursionStack[stackSize - 1].v2;
        --stackSize;

		// Берем минимум в множестве, потому что cur > индекс любой уже добавленной точки
        int innerPt = triangulation[{ min( left, right ), max( left, right ) }].GetMin();
        if( check( left, right, cur, innerPt ) ) {
			// Если менять ничего в четырехугольнике не надо, просто добавляем недостающие ребра и выходим
            triangulation[{ right, cur }].insert( left );
            triangulation[{ left, cur }].insert( right );
            triangulation[{ min( left, right ), max( left, right ) }].insert( cur );
            continue;
        }

		// Иначе перестраиваем триангуляцию в четырехугольнике
        triangulation[{ right, cur }].erase( left );
        triangulation[{ right, cur }].insert( innerPt );
        triangulation[{ left, cur }].erase( right );
        triangulation[{ left, cur }].insert( innerPt );
        triangulation[{ min( innerPt, left ), max( innerPt, left ) }].erase( right );
        triangulation[{ min( innerPt, left ), max( innerPt, left ) }].insert( cur );
        triangulation[{ min( innerPt, right ), max( innerPt, right ) }].erase( left );
        triangulation[{ min( innerPt, right ), max( innerPt, right ) }].insert( cur );
        triangulation.erase( { min( left, right ), max( left, right ) } );

		// И добавляем 2 новых рекурсивных вызова
        recursionStack[stackSize++] = { left, innerPt };
        recursionStack[stackSize++] = { innerPt, right };
    }
}

bool Solver::check( int left, int right, int cur, int innerPt )
{
	// Проверка на то, что это вообще четырехугольник и выпуклость
    if( innerPt == cur || CrossProduct( points[left] - points[cur], points[innerPt] - points[cur] ) < 0 ||
        CrossProduct( points[right] - points[cur], points[innerPt] - points[cur] ) > 0 )
    {
        return true;
    }
	// Проверка условия Делоне, как в книжке из статьи
    dataT Sa = ( points[cur].x - points[right].x ) * ( points[cur].x - points[left].x ) +
               ( points[cur].y - points[right].y ) * ( points[cur].y - points[left].y );
    dataT Sb = ( points[innerPt].x - points[right].x ) * ( points[innerPt].x - points[left].x ) +
               ( points[innerPt].y - points[right].y ) * ( points[innerPt].y - points[left].y );
    if( Sa > -eps && Sb > -eps ) {
        return true;
    } else if( !( Sa < 0 && Sb < 0 ) ) {
        dataT Sc = ( points[cur].x - points[right].x ) * ( points[cur].y - points[left].y ) -
                   ( points[cur].y - points[right].y ) * ( points[cur].x - points[left].x );
        dataT Sd = ( points[innerPt].x - points[right].x ) * ( points[innerPt].y - points[left].y ) -
                   ( points[innerPt].y - points[right].y ) * ( points[innerPt].x - points[left].x );
        if( Sc < 0 ) Sc = -Sc; if( Sd < 0 ) Sd = -Sd;
        if( Sc * Sb + Sa * Sd > -eps ) {
            return true;
        }
    }
    return false;
}

int main() {
    std::ios_base::sync_with_stdio( false );
    cin.tie( nullptr );
    cout.tie( nullptr );

    Solver my_solver;
    my_solver.Solve();

    return 0;
}
