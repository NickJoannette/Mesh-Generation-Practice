#pragma once
class HashTable
{
public:
	HashTable(unsigned int width, unsigned int length) : width(width), length(length),
		table(new float[width*length]) {};

private:
	unsigned int width, length;
	float * table;
};

