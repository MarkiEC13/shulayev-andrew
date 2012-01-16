#include "geometry.h"

point::point() { }
point::point(double x, double y) : x(x), y(y) { }
point::point(RandomInput& rand) : x(rand.get_double()), y(rand.get_double()) { }

std::istream& operator>>(std::istream& in, point& p)
{
	in >> p.x >> p.y;
}

std::ostream& operator<<(std::ostream& out, const point& p)
{
	out << '(' << p.x << "; " << p.y << ')';
}

segment::segment() { }
segment::segment(RandomInput& rand)
{
	end[0] = point(rand);
	end[1] = point(rand);
}

std::istream& operator>>(std::istream& in, segment& s)
{
	in >> s.end[0] >> s.end[1];
}

std::ostream& operator<<(std::ostream& out, const segment& s)
{
	out << '(' << s.end[0] << ", " << s.end[1] << ')';
}

inline boost::numeric::interval<double> make_interval(double start, double end)
{
	return boost::numeric::interval<double>(std::min(start, end), std::max(start, end));
}

bool overlap(const boost::numeric::interval<double>& int1, const boost::numeric::interval<double>& int2)
{
	using boost::numeric::interval_lib::cergt;
	using boost::numeric::interval_lib::cerlt;
	return !(cergt(int1, int2) || cerlt(int1, int2));
}

bool segments_intersect(const segment& segm1, const segment& segm2)
{
	typedef boost::numeric::interval<double> interval;
	
	interval segm1_x = make_interval(segm1.end[0].x, segm1.end[1].x);
	interval segm1_y = make_interval(segm1.end[0].y, segm1.end[1].y);
	
	interval segm2_x = make_interval(segm2.end[0].x, segm2.end[1].x);
	interval segm2_y = make_interval(segm2.end[0].y, segm2.end[1].y);
	
	if (!overlap(segm1_x, segm2_x) || !overlap(segm1_y, segm2_y))
		return false;
	else if (left_turn(segm1, segm2.end[0]) * left_turn(segm1, segm2.end[1]) > 0)
		return false;
	else if (left_turn(segm2, segm1.end[0]) * left_turn(segm2, segm1.end[1]) > 0)
		return false;
	else
		return true;
}

int left_turn(const segment& segm, const point& p)
{
	int result = left_turn_simple(segm, p);
	if (result != 0)
	{
		return result;
	}
	
	result = left_turn_interval(segm, p);
	if (result != 0)
	{
		return result;
	}
	
	return left_turn_adaptive(segm, p);
}

int left_turn_simple(const segment& segm, const point& p)
{
	double eps = 8 * std::numeric_limits<double>::epsilon() *
		(std::abs((segm.end[1].x - segm.end[0].x) * (p.y - segm.end[0].y)) + std::abs((segm.end[1].y - segm.end[0].y) * (p.x - segm.end[0].x)));
	
	double vec_prod = (segm.end[1].x - segm.end[0].x) * (p.y - segm.end[0].y) - (segm.end[1].y - segm.end[0].y) * (p.x - segm.end[0].x);
	if (vec_prod > eps)
		return 1;
	else if (vec_prod < -eps)
		return -1;
	else
		return 0;
}

int left_turn_interval(const segment& segm, const point& p)
{
	typedef boost::numeric::interval<double> interval;
	using boost::numeric::interval_lib::cergt;
	using boost::numeric::interval_lib::cerlt;
	
	interval vector_prod = (interval(segm.end[1].x) - interval(segm.end[0].x)) * (interval(p.y) - interval(segm.end[0].y)) - (interval(segm.end[1].y) - interval(segm.end[0].y)) * (interval(p.x) - interval(segm.end[0].x));
	
	if (cergt(vector_prod, 0.0))
		return 1;
	else if (cerlt(vector_prod, 0.0))
		return -1;
	else
		return 0;
}

namespace adaptive_details
{
	/* код adaptive_details взят из презентации Антона Ковалёва
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
		double ab = c - a ;
		hi = c - ab ;
		lo = a - hi ;
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

};

int left_turn_adaptive(const segment& segm, const point& p)
{
	using namespace adaptive_details;
	
	double step1[12];
	step1[1] = mul(segm.end[1].x, p.y, step1[0]);
	step1[3] = mul(-segm.end[1].x, segm.end[0].y, step1[2]);
	step1[5] = mul(-segm.end[0].x, p.y, step1[4]);
	step1[7] = mul(-segm.end[1].y, p.x, step1[6]);
	step1[9] = mul(segm.end[1].y, segm.end[0].x, step1[8]);
	step1[11] = mul(segm.end[0].y, p.x, step1[10]);
	
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
