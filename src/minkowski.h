#pragma once
#include <boost/polygon/polygon.hpp>
#include <boost/polygon/polygon_traits.hpp>
#include <boost/polygon/polygon_with_holes_data.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <limits>

typedef boost::polygon::point_data<int> point;
typedef boost::polygon::polygon_set_data<int> polygon_set;
typedef boost::polygon::polygon_with_holes_data<int> polygon;
typedef boost::polygon::rectangle_data<int> Rectangle;
typedef std::pair<point, point> edge;
using namespace boost::polygon::operators;

void convolve_two_segments(std::vector<point>& figure, const edge& a, const edge& b)
{
	using namespace boost::polygon;
	figure.clear();
	figure.push_back(point(a.first));
	figure.push_back(point(a.first));
	figure.push_back(point(a.second));
	figure.push_back(point(a.second));
	convolve(figure[0], b.second);
	convolve(figure[1], b.second);
	convolve(figure[2], b.first);
	convolve(figure[3], b.second);
}

template<typename itrT1,typename itrT2>
void convolve_two_points_sequences(polygon_set& result, itrT1 ab, itrT1 ae, itrT2 bb, itrT2 be)
{
    using namespace boost::polygon;
    if (ab == ae || bb == be)
        return;
    point first_a = *ab;
    point prev_a = *ab;
    std::vector<point> vec;
    polygon poly;
    ++ab;
    for (; ab != ae; ++ab) {
        point first_b = *bb;
        point prev_b = *bb;
        itrT2 tmpb = bb;
        ++tmpb;
        for (; tmpb != be; ++tmpb) {
            convolve_two_segments(vec, std::make_pair(prev_b, *tmpb), std::make_pair(prev_a, *ab));
            set_points(poly, vec.begin(), vec.end());
            result.insert(poly);
            prev_b = *tmpb;
        }
        prev_a = *ab;
    }
}

template<typename itrT>
void convolve_point_sequence_with_polygons(polygon_set& results, itrT b, itrT e, const std::vector<polygon>& polygons)
{
    using namespace boost::polygon;
    for (std::size_t i = 0; i < polygons.size(), ++i)
    {
        convolve_two_points_sequences(result, b, e, begin_points(polygons[i]), end_points(polygons[i]));
        for (polygon_with_holes_traits<polygon>::iterator_holes_type itrh = begin_holes(polygons[i]);
            itrh != end_holes(polygons[i]); ++itrh)
        {
            convolve_two_point_sequences(result, b, e, begin_points(*itrh), end_points(itrh));
        }
    }
}

void convolve_two_polygon_sets(polygon_set& result, const polygon_set& a, const polygon_set& b)
{
    using namespace boost::polygon;
    result.clear();
    std::vector<polygon> a_polygons;
    std::vector<polygon> b_polygons;
    a.get(a_polygons);
    b.get(b_polygons);
    for (std::size_t ai = 0; ai < a_polygons.size(); ++ai) {
        convolve_point_sequence_with_polygons(result, begin_points(a_polygons[ai]),
            end_points(a_polygons[ai]), b_polygons);
        for (polygon_with_holes_traits<polygon>::iterator_holes_type itrh = begin_holes(a_polygons[ai]);
            itrh != end_holes(a_polygons[ai]); ++itrh) {
            convolve_point_sequence_with_polygons(result, begin_points(*itrh),
                end_points(*itrh), b_polygons);
        }
        for (std::size_t bi = 0; bi < b_polygons.size(); ++bi) {
            polygon tmp_poly = a_polygons[ai];
            result.insert(convolve(tmp_poly, *(begin_points(b_polygons[bi]))));
            tmp_poly = b_polygons[bi];
            result.insert(convolve(tmp_poly, *(begin_points(a_polygons[ai]))));
        }
    }
}

double inputscale;

polygon_set calculateNFP(polygon& A,bool isolate, polygon& B,std::vector<polygon>& children)
{
    polygon_set a, b, c;
    std::vector<polygon> polys;
    std::vector<point> pts;

    unsigned int len = A.size();
    Rectangle boundingBoxA;
    boost::polygon::extents(boundingBoxA, A);
    double Aminx = boost::polygon::xl(boundingBoxA);
    double Aminy = boost::polygon::yl(boundingBoxA);
    double Amaxx = boost::polygon::xh(boundingBoxA);
    double Amaxy = boost::polygon::yh(boundingBoxA);

    Rectangle boundingBoxB;
    boost::polygon::extents(boundingBoxB, B);
    double Bminx = boost::polygon::xl(boundingBoxB);
    double Bminy = boost::polygon::yl(boundingBoxB);
    double Bmaxx = boost::polygon::xh(boundingBoxB);
    double Bmaxy = boost::polygon::yh(boundingBoxB);

    double Cmaxx = Amaxx + Bmaxx;
    double Cminx = Aminx + Bminx;
    double Cmaxy = Amaxy + Bmaxy;
    double Cminy = Aminy + Bminy;

    double maxxAbs = (std::max)(Cmaxx, std::fabs(Cminx));
    double maxyAbs = (std::max)(Cmaxy, std::fabs(Cminy));

    double maxda = (std::max)(maxxAbs, maxyAbs);
    int maxi = std::numeric_limits<int>::max();

    if (maxda > 1) {
        maxda = 1;
    }

    inputscale = (0.1f * (double)(maxi)) / maxda;

    for (const auto& pt : A)
    {
        int x = (int)(inputscale * (double)pt.x());
        int y = (int)(inputscale * (double)pt.y());

        pts.push_back(point(x, y));
    }
    polygon poly;
    boost::polygon::set_points(poly, pts.begin(), pts.end());
    a += poly;

    //substract holes from a here
    for (const auto& hole : children)
    {
        pts.clear();
        for (const auto& pt : hole)
        {
            int x = (int)(inputscale * pt.x());
            int y = (int)(inputscale * pt.y());
            pts.push_back(point(x, y));
        }
        boost::polygon::set_points(poly, pts.begin(), pts.end());
        a -= poly;
    }

    pts.clear();
    len = B.size();

    double xshift = 0;
    double yshift = 0;

    for (unsigned int i = 0; i < len; i++)
    {
        auto pt = *(B.begin() + i);
        int x = -(int)(inputscale * (double)pt.x());
        int y = -(int)(inputscale * (double)pt.y());
        pts.push_back(point(x, y));

        if (i == 0) {
            xshift = pt.x();
            yshift = pt.y();
        }
    }

    boost::polygon::set_points(poly, pts.begin(), pts.end());
    b += poly;

    polys.clear();

    convolve_two_polygon_sets(c, a, b);
    c.get(polys);


    std::vector<polygon> result_list;

    for (unsigned int i = 0; i < polys.size(); ++i)
    {
        std::vector<point> pointlist;
        int j = 0;

        for (auto itr = polys[i].begin(); itr != polys[i].end(); ++itr)
        {
            pointlist.push_back(point((*itr).get(boost::polygon::HORIZONTAL)/inputscale + xshift,(*itr).get(boost::polygon::VERTICAL)/inputscale + yshift));
            j++;
        }
        namespace bp = boost::polygon;
        std::vector<polygon> children;
        int k = 0;
        for (auto itrh = begin_holes(polys[i]); itrh != end_holes(polys[i]); ++itrh)
        {
            polygon child;
            int z = 0;

            std::vector<point> points;
            for (auto itr2 = (*itrh).begin(); itr2 != (*itrh).end(); ++itr2)
            {
                points.push_back(point((*itr2).get(boost::polygon::HORIZONTAL) / inputscale + xshift, (*itr2).get(boost::polygon::VERTICAL) / inputscale + yshift));
                z++;
            }
            bp::set_points(child,points.begin(),points.end());
            children.push_back(child);
        }
    }
    c.insert(children, true);

    return c;
}