#include "core/error_codes.h"

namespace SAS_3D {
	namespace Core {
		namespace {
			ErrorCode global_error = ErrorCode::NO_ERROR;	// my equivalent of errno
		}

		void SetError(ErrorCode ec) {
			global_error = ec;
		}

		ErrorCode GetError() {
			return global_error;
		}
	}
}