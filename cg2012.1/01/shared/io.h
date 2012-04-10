#ifndef IO_H
#define IO_H

#include <iostream>

namespace io
{
	class Input
	{
	public:
		virtual int get_int() = 0;
		virtual double get_double() = 0;
	};

	class RandomInput : public Input
	{
	public:
		RandomInput(int seed);
		int get_int();
		double get_double();

	private:
		int seed;

		static const int a = 1664525;
		static const int c = 1013904223;
	};

	class StreamInput : public Input
	{
	public:
		StreamInput(std::istream& stream);
		int get_int();
		double get_double();

	private:
		std::istream& stream;
	};

	class Output
	{
	public:
		virtual void put_result(bool result) = 0;
		virtual void finalize() = 0;
	};

	class HashOutput : public Output
	{
	public:
		HashOutput(std::ostream &);
		void put_result(bool result);
		void finalize();

	private:
		std::ostream & output;
		int hash, multiplier;
	};

	class BinaryStringOutput : public Output
	{
	public:
		BinaryStringOutput(std::ostream &);
		void put_result(bool result);
		void finalize();

	private:
		std::ostream & output;
	};
};

#endif // IO_H
