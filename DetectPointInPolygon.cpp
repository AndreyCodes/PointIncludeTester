#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
struct point
{
    double x;
    double y;

    point(double arg_x = 0, double arg_y = 0) : x(arg_x), y(arg_y) {}
    bool operator==(const point &p) const
    {
        return x == p.x and y == p.y;
    }
    bool operator!=(const point &p) const
    {
        return !(*this == p);
    }
};
struct vec
{
    double x;
    double y;
    vec(const point &a, const point &b) : x(b.x - a.x), y(b.y - a.y) {}
    double abs()
    {
        return hypot(x,y);
    }
};
class PolyGon
{
public:
    std::vector<point> p;
    PolyGon(const std::vector<point> a) : p(a) {}
};

class PointInclusionTester //ТОЛЬКО СТАТИЧЕСКИЕ МНОГОУГОЛЬНИКИ
{
    static double TriangleArea(const point &a1, const point &a2, const point &a3)
    {
        double res = ((a1.x - a3.x) * (a2.y - a3.y) - (a2.x - a3.x) * (a1.y - a3.y)) / 2.0;
        if (res < 0)
            return -res;
        return res;
    }
    static bool IsPointInTriangle(const point &a1, const point &a2, const point &a3, const point &p)
    {
        double s1 = TriangleArea(a1, a2, p);
        double s2 = TriangleArea(a2, a3, p);
        double s3 = TriangleArea(a1, a3, p);
        double s = TriangleArea(a1, a2, a3);
        if (s1 + s2 + s3 <= s) //!нет! погрешности eps
        {
            return true;
        }
        return false;
    }

    static std::pair<point, point> findPos(const std::vector<std::pair<point, double>> &a, double cos)
    {
        int fs = 0;
        int ls = a.size() - 1;
        int mid = (fs + ls) / 2;
        while (fs != ls)
        {
            if (cos > a[mid].second and cos < a[mid + 1].second)
            {
                return {a[mid].first, a[mid + 1].first};
            }
            if (cos > a[mid].second and cos > a[mid + 1].second)
            {
                mid = (mid + ls) / 2;
                continue;
            }
            if (cos < a[mid].second and cos < a[mid + 1].second)
            {
                mid = (mid + fs) / 2;
                continue;
            }
        }
    }

    const std::vector<point> &polyGon;
    point Z = polyGon[0];
    std::vector<std::pair<point, double>> edited;

public:
    PointInclusionTester(const PolyGon &nGon) : polyGon(nGon.p), Z(polyGon[0])
    {
        if (polyGon.size() <= 2)
        {
            throw std::invalid_argument("Number of Points is less than 3");
        }
        for (auto &el : polyGon)
        {
            if (el.x < Z.x)
            {
                Z = el;
            }
            if (el.x == Z.x)
            {
                if (el.y < Z.y)
                {
                    Z = el;
                }
            }
        }
        for (auto &el : polyGon)
        {
            if (el != Z)
            {
                edited.push_back({el, 0});
            }
        }
        std::sort(edited.begin(), edited.end(), [&](std::pair<point, double> &a, std::pair<point, double> &b)
                  {
                      vec v_a(Z, a.first);
                      vec v_b(Z, b.first);
                      a.second = v_a.y / v_a.abs(); //косинус
                      b.second = v_b.y / v_b.abs(); //косинус
                      return a.second < b.second;   //отсортировать по косинусу угла
                  });
        //ready to process.
    }
    bool isPointInPolygon(const point &p)
    {
        //можно отвечать на запросы

        vec v_this(Z, p);
        double cos = v_this.y / v_this.abs();

        point L, R; //
        auto tempRes = findPos(edited, cos);
        L = tempRes.first;
        R = tempRes.second;

        return IsPointInTriangle(Z, L, R, p);
    }
};
int main()
{
    std::vector<point> t{{5.5,3.5},{7,-2},{2,-5},{-4,-2},{-5,2},{-1,5}};
    PolyGon p(t);
    PointInclusionTester a(p);
    std::cout << a.isPointInPolygon({0,0});
}