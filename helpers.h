#pragma once
#include <map>
#include <string>
#include <iostream>
#include <time.h>

template <typename T>
T input(std::string message)
{
	std::cout << message;
	T v;
	std::cin >> v;
	return v;
}

template <typename T>
void println(T message)
{
	std::cout << message << "\n";
}

template<typename K, typename V>
bool does_map_key_exists(std::map<K, V>& map, K key)
{
	if (map.find(key) == map.end()) {
		return false;
	}
	else {
		return true;
	}
}

int get_gold_exchange_rate()
{
	srand((unsigned)time(NULL));
	int lowest = 1, highest = 10;
	int range = (highest - lowest) + 1;
	return lowest + rand() % range;
}