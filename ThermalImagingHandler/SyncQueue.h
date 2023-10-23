#pragma once
#include "Item.h"

template <typename T>
class SyncQueue {
	Item<T>* head;
	Item<T>* tail;
public:
	SyncQueue() {
		head = new Item<T>(nullptr);
		tail = head;
	}

	bool Empty() {
		return head == tail;
	}

	void Enqueue(T* data) {
		Item<T>* element = new Item<T>(data);
		tail->next = element;
		tail = element;
	}

	T* Dequeue() {
		if (head == tail) return nullptr;

		Item<T>* temp = head;
		head = head->next;
		delete temp;

		return head->data;
	}
};