#pragma once
#include <type_traits>

namespace SAS_3D {
	/*
		Fixed size storage class used for message callbacks
	*/
	class Message {
	public:
		template<typename T>
		Message(int id, T x) 
			: _id(id)
		{
			new(&_data) T(x);
		}

		int ID() const{
			return _id;
		}

		// Cast to void then to the desired type
		template<typename T>
		T* GetData() {
			return static_cast<T*>(static_cast<void*>(&_data));
		}

	private:
		int _id;
		typename std::aligned_storage<64>::type _data;
	};

}
