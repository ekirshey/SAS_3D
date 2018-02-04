#include "core/utility.h"

namespace SAS_3D {
	unsigned int convert(size_t what) {
		if (what > UINT_MAX) {
			throw std::overflow_error("Overflow error while converting size_t to unsigned int");
		}
		return static_cast<unsigned int>(what);
	}
}