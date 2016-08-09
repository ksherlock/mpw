#ifndef __unique_resource_h__
#define __unique_resource_h__

#include <utility>

template <class T, class D>
class unique_resource {
public:
	typedef T element_type;
	typedef D deleter_type;

	unique_resource() = default;
	unique_resource(const unique_resource &) = delete;
	unique_resource(unique_resource &&rhs) {
		swap(rhs);
	}

	unique_resource(T t, D d): _pair(t,d), _active(true)
	{}

	~unique_resource() {
		reset();
	}

	unique_resource &operator=(const unique_resource &) = delete;
	unique_resource &operator=(unique_resource &&rhs) {
		if (this != std::addressof(rhs)) {
			reset();
			swap(rhs);
		}
		return *this;
	}

	void swap(unique_resource & rhs) {
		if (this != std::addressof(rhs)) {
			std::swap(_active, rhs._active);
			std::swap(_pair, rhs._pair);
		}
	}

	void reset(T t) {
		reset();
		_active = true;
		_pair.first = t;
	}

	void reset(T t, D d) {
		reset();
		_active = true;
		_pair = std::make_pair(t, d);
	}

	void reset() {
		if (_active) {
			(*_pair.second)(_pair.first);
			_active = false;
		}
	}

	T release() {
		_active = false;
		return _pair.first;;
	}

	T get() {
		return _pair.first;
	}

	operator bool() const {
		return _active; 
	}

	D& get_deleter() {
		return _pair.second;
	}

	const D& get_deleter() const {
		return _pair.second;
	}


private:
	std::pair<T, D> _pair;
	bool _active = false;
};

#define MAKE_UNIQUE_RESOURCE(T, D) \
	unique_resource<decltype(T), decltype(D) *>(T, D)

template<class T, class D>
unique_resource<T, D> make_unique_resource(T t, D d) {
	return unique_resource<T, D>(t, d);
}

#endif
