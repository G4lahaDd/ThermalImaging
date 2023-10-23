#pragma once

template <typename T>
class Item
{
public:
	Item(T* data) {
		this->data = data;
		this->next = nullptr;
	}

	T* data;
	Item<T>* next;
};