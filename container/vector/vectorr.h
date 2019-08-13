#include <stdlib.h>
#include <malloc.h>
#include <typeinfo>

template<class T>
class vectorr
{
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef value_type* iterator;
	typedef value_type& reference;
	typedef size_t      size_type;
	typedef ptrdiff_t   different_type;
protected:
	iterator start;
	iterator finish;
	iterator end_of_storage;
public:
	std::allocator<value_type> allocat;
	iterator begin() { return start; }
	iterator end() { return finish; }
	size_type size() { return size_type(end() - begin()); }
	size_type capacity() { return size_type(end_of_storage - begin()); }
	T& back() { return *(finish-1); }
	bool empty() { if (size() == 0)return true; else return false; }
	T& front() { return *start; }
	iterator insert(iterator position, const T& x) {
		if (finish != end_of_storage) {
			for (iterator iter = finish; iter != position; iter--) {
				*iter = *(iter - 1);
			}
			*position = x;
			finish++;
			return position;
		}
		else {
			size_type n = capacity();
			pointer iter = reinterpret_cast<T*>(malloc(sizeof(value_type) * n * 2));
			pointer s = iter;
			for (iterator it = start; it < finish; it++) {
				*iter++ = *it;
			}
			*iter = x;
			start = s;
			finish = iter + 1;
			end_of_storage = start + 2 * n;
			return insert(position, x);
		}
	}
	//构造函数
	vectorr() :start(NULL), finish(NULL), end_of_storage(NULL) {};
	vectorr(int n, T x) {
		pointer iter = reinterpret_cast<T*>( allocat.allocate(sizeof(value_type) * n));
		start = iter;
		for (int i = 0; i < n; i++) {
			*iter++ = x;
		}	
		finish = iter;
		end_of_storage = iter;
	}
	//析构函数
	~vectorr()
	{
		iterator end = end_of_storage;
		iterator s = start;
		size_type n = capacity();
		allocat.deallocate(start,n);
	}
	vectorr& operator=( const vectorr &v) {//赋值构造函数
		if (this == &v) {
			return *this;
		}
		start = v.start;
		finish = v.finish;
		end_of_storage = v.end_of_storage;
		return *this;
	}
	void push_back(T x) {
		if (finish != end_of_storage) *finish++ = x;
		else {
			size_type n = capacity();
			pointer iter = reinterpret_cast<pointer>(allocat.allocate(sizeof(value_type) * n * 2));
			iterator s = iter;
			for (iterator it = start; it < finish; it++) {
				*iter++ = *it;
				allocat.destroy(it);
			}
			allocat.deallocate(start, capacity());
			*iter = x;
			start = s;
			finish = iter+1;
			end_of_storage = start + 2 * n;
		}
	}
	iterator erase(iterator pos) {
		if (pos == finish - 1) {
			allocat.destroy(pos);
			--finish;
			return pos - 1;
		}
		for (iterator it = pos; it < finish-1; it++) {
			*it = *(it + 1);
		}
		--finish;
		allocat.destroy(finish);
		return pos;
	}
	void clear() {
		for (iterator it = finish-1; it >=start; it--) {
			allocat.destroy(it);
		}
		finish = start;
   }
	reference operator[](size_type n) { return *(begin() + n); }
};

