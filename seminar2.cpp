#include <string>
#include <iostream>
#include <vector>

template<typename T>
struct DefaultComparator {
	bool operator()(const T& l, const T& r) const {
		return l < r;
	}
};

template <typename T, typename Comparator = DefaultComparator<T>>
class Heap {
public:
	Heap(Comparator comp = Comparator());

	Heap(const Heap&) = delete;
	Heap(Heap&&) = delete;

	Heap& operator=(const Heap&) = delete;
	Heap& operator=(Heap&&) = delete;

	~Heap();

	const T& top() const;
	void pop();

	void push(const T&);

	bool is_empty() const;
	size_t size() const;

private:
	void extend();

	void sift_up();
	void sift_down();

	T* buf;
	size_t buffer_size;
	size_t heap_size;

	Comparator comp;

};



int cmp(int l, int r) {
	if (l < r) {
		return -1;
	} else if (l > r) {
		return 1;
	}
	return 0;
}

bool is_less(const int& l, const int& r) {
	return l < r;
}



struct Comp {
	int counter = 0;

	bool operator()(int l, int r) {
		counter++;
		return l < r;
	}
};

struct Point {
	int x;
	int y;

	bool operator< (const Point& r) const {
		return x < r.x;
	}

	bool operator< (const std::string& r) const {
		return x < r.size();
	}
};




Point p1;
Point p2;
// p1 < p2 -> p1.operator<(p2);


template <typename T, typename Comparator = DefaultComparator<T>>
void sort(T* arr, int l, int r, Comparator comp = Comparator()) {
	for (int i = l; i < r; ++i) {
		for (int j = l; j < r - 1 ; ++j) {
			if (comp(arr[j + 1], arr[j])) {
				std::swap(arr[j + 1], arr[j]);
			}
		}
	}
}

void sort1(int* arr, int l, int r, Comp& comp) {
	for (int i = l; i < r; ++i) {
		for (int j = l; j < r - 1 ; ++j) {
			if (comp(arr[j + 1],  arr[j])) {
				std::swap(arr[j + 1], arr[j]);
			}
		}

	}
}

template<class T>
void print(const std::vector<T>& arr, void (*func)(const T&)) {
	for (const T& i : arr) {
		func(i);
		std::cout << " ";
	}
	std::cout << std::endl;
}

int main() {
	std::vector<int> vec1 = {100, 500, 1, -10, -100, 0, 5, 6, 7, 100};

	std::vector<Point> vec_p = {
			{2, 2},
			{0, 0},
			{1, 1}
	};
	auto print_int = [](const int& i) {
		std::cout << i;
	};

	sort(&vec1[0], 0, vec1.size());//, is_less);
	print<int>(vec1, print_int);

	int counter = 0;
	sort<Point>(&vec_p[0], 0, vec_p.size(), [&counter](const Point& l, const Point& r) {
		counter++;
		return l.x < r.x;
	});
	print<Point>(vec_p,  [](const Point& i) {
		std::cout << "[" << i.x << ", " << i.y << "]";

	});


	std::cout << "lambda" << std::endl;
	sort(&vec1[0], 0, vec1.size(), [](int l, int r) {
		return r < l;
	});
	print<int>(vec1, print_int);
	/*
	std::cout << "functor" << std::endl;
	Comp comp;
	sort1(&vec1[0], 0, vec1.size(), comp);
	print(vec1);
	std::cout << "counter=" << comp.counter << std::endl;
	*/

	std::vector<std::string> vec_str = {
			"ccc", "bb", "aaa"
	};
	std::cout << "string" << std::endl;
	sort(&vec_str[0], 0, vec_str.size());
	print<std::string>(vec_str, [](const std::string& s) {
		std::cout << s;
	});

	return 0;
}
