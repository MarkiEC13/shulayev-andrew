#include <cassert>
#include <limits>
#include <cmath>
#include <boost/numeric/interval.hpp>

#include "geometry.h"

// Point implementation starts

geometry::Point::Point() : x(0.0), y(0.0)
{ }

geometry::Point::Point(double x, double y) : x(x), y(y)
{ }

geometry::Point::Point(io::Input & input)
{
	x = input.get_double();
	y = input.get_double();
}

double geometry::Point::get_x() const
{
	return x;
}

double geometry::Point::get_y() const
{
	return y;
}

// Point implementation ends

// Segment implementation starts

geometry::Segment::Segment(const geometry::Point & end0, const geometry::Point & end1)
{
	ends[0] = end0;
	ends[1] = end1;
}

geometry::Segment::Segment(io::Input & input)
{
	ends[0] = Point(input);
	ends[1] = Point(input);
}

geometry::Point geometry::Segment::get_end(int index) const
{
	assert(index == 0 || index == 1);

	return ends[0];
}

bool geometry::Segment::does_intersect(const geometry::Segment &) const
{
	return false;
}

namespace
{
	int left_turn_simple(geometry::Point const& a, geometry::Point const& b, geometry::Point const& c)
	{
		double eps = 4 * std::numeric_limits<double>::epsilon() *
			(fabs((b.get_x() - a.get_x()) * (c.get_y() - a.get_y())) + fabs((b.get_y() - a.get_y()) * (c.get_x() - a.get_x())));

		double vec_prod = (b.get_x() - a.get_x()) * (c.get_y() - a.get_y()) - (b.get_y() - a.get_y()) * (c.get_x() - a.get_x());
		if (vec_prod > eps)
			return 1;
		else if (vec_prod < -eps)
			return -1;
		else
			return 0;
	}

	int left_turn_interval(geometry::Point const& a, geometry::Point const& b, geometry::Point const& c)
	{
		typedef boost::numeric::interval<double> interval;
		using boost::numeric::interval_lib::cergt;
		using boost::numeric::interval_lib::cerlt;

		interval vector_prod = (interval(b.get_x()) - interval(a.get_x())) * (interval(c.get_y()) - interval(a.get_y()))
							   - (interval(b.get_y()) - interval(a.get_y())) * (interval(c.get_x()) - interval(a.get_x()));

		if (cergt(vector_prod, 0.0))
			return 1;
		else if (cerlt(vector_prod, 0.0))
			return -1;
		else
			return 0;
	}

	/* код взят из презентации Антона Ковалёва
	 * "Устойчивая реализация алгоритмов вычислительной геометрии" */
	double sum(double a, double b, double &roundoff)
	{
		double res = a + b;
		double bv = res - a;
		double av = res - bv;
		double br = b - bv;
		double ar = a - av;
		roundoff = ar + br;
		return res;
	}

	void split(double a, size_t s, double& hi, double& lo)
	{
		double c = ((1LL << s) + 1LL) * a;
		double ab = c - a;
		hi = c - ab;
		lo = a - hi;
	}

	double mul(double a, double b, double& roundoff)
	{
		double res = a * b;
		size_t s = std::numeric_limits<double>::digits / 2
			+ std::numeric_limits<double>::digits % 2;

		double a_hi, a_lo, b_hi, b_lo;
		split(a, s, a_hi, a_lo);
		split(b, s, b_hi, b_lo);

		double e1 = res - (a_hi * b_hi);
		double e2 = e1 - (a_lo * b_hi);
		double e3 = e2 - (b_lo * a_hi);

		roundoff = (a_lo * b_lo) - e3;
		return res;
	}

	template <size_t N>
	struct grow_expansion_f
	{
		static void calc(double const *e, double b, double *r)
		{
			b = sum(*e, b, *r);
			grow_expansion_f<N - 1>::calc(e + 1, b, r + 1);
		}
	};

	template <>
	struct grow_expansion_f<0>
	{
		static void calc(double const *e, double b, double *r)
		{
			*r = b;
		}
	};

	template <size_t N1, size_t N2>
	struct expand_sum_f
	{
		static void calc(double const *e, double const *f, double *r)
		{
			grow_expansion_f<N1>::calc(e, *f, r);
			expand_sum_f<N1, N2 - 1>::calc(r + 1, f + 1, r + 1);
		}
	};

	template <size_t N1>
	struct expand_sum_f<N1, 0>
	{
		static void calc(double const *e, double const *f, double *r) { }
	};

	int left_turn_adaptive(geometry::Point const& a, geometry::Point const& b, geometry::Point const& c)
	{
		double step1[12];
		step1[1] = mul(b.get_x(), c.get_y(), step1[0]);
		step1[3] = mul(-b.get_x(), a.get_y(), step1[2]);
		step1[5] = mul(-a.get_x(), c.get_y(), step1[4]);
		step1[7] = mul(-b.get_y(), c.get_x(), step1[6]);
		step1[9] = mul(b.get_y(), a.get_x(), step1[8]);
		step1[11] = mul(a.get_y(), c.get_x(), step1[10]);

		double step2[12];
		expand_sum_f<2, 2>::calc(step1 + 0, step1 + 2, step2);
		expand_sum_f<2, 2>::calc(step1 + 4, step1 + 6, step2 + 4);
		expand_sum_f<2, 2>::calc(step1 + 8, step1 + 10, step2 + 8);

		double step3[8];
		expand_sum_f<4, 4>::calc(step2, step2 + 4, step3);

		double step4[12];
		expand_sum_f<8, 4>::calc(step2, step2 + 8, step4);

		for (int i = 11; i >= 0; i--)
		{
			if (step4[i] > 0)
				return 1;
			else if (step4[i] < 0)
				return -1;
		}

		return 0;
	}
};

int geometry::left_turn(const Point & p1, const Point & p2, const Point & p3)
{
	int result = left_turn_simple(p1, p2, p3);
	if (result != 0)
	{
		return result;
	}

	result = left_turn_interval(p1, p2, p3);
	if (result != 0)
	{
		return result;
	}

	return left_turn_interval(p1, p2, p3);
}

int geometry::left_turn(geometry::Segment const& segm, geometry::Point const& p)
{
	return geometry::left_turn(segm.get_end(0), segm.get_end(1), p);
}

// Segment implementation ends
