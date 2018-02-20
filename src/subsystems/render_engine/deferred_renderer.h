#pragma once

namespace SAS_3D {
	class SASWindow;
	class DeferredRenderer {
	public:
		DeferredRenderer(const SASWindow* window);
		void BindFBO();
		void UnbindFBO();
		void BindMRTs();
		void BlitDepthBuffer();
	private:
		float _screenwidth;
		float _screenheight;
		unsigned int _buffer;
		unsigned int _position;
		unsigned int _normal;
		unsigned int _albedoSpec;
	};
}
