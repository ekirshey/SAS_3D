#include <GL/glew.h>
#include "deferred_renderer.h"
#include "core/sas_video.h"

namespace SAS_3D {
	DeferredRenderer::DeferredRenderer(const SASWindow* window) {
		_screenwidth = window->GetScreenWidth();
		_screenheight = window->GetScreenHeight();
		glGenFramebuffers(1, &_buffer);
		glBindFramebuffer(GL_FRAMEBUFFER, _buffer);


		// position color buffer
		glGenTextures(1, &_position);
		glBindTexture(GL_TEXTURE_2D, _position);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _screenwidth, _screenheight, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _position, 0);

		// normal color buffer
		glGenTextures(1, &_normal);
		glBindTexture(GL_TEXTURE_2D, _normal);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _screenwidth, _screenheight, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _normal, 0);

		// color + specular color buffer
		glGenTextures(1, &_albedoSpec);
		glBindTexture(GL_TEXTURE_2D, _albedoSpec);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _screenwidth, _screenheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, _albedoSpec, 0);

		// Tell OpenGL which color attachments are being used (of this framebuffer) for rendering
		unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, attachments);

		unsigned int rboDepth;
		glGenRenderbuffers(1, &rboDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _screenwidth, _screenheight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
		// finally check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void DeferredRenderer::BindFBO() {
		glBindFramebuffer(GL_FRAMEBUFFER, _buffer);
	}

	void DeferredRenderer::UnbindFBO() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void DeferredRenderer::BindMRTs() {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _position);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _normal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, _albedoSpec);
	}

	void DeferredRenderer::BlitDepthBuffer() {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, _buffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
											// blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
											// the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the 		
											// depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
		glBlitFramebuffer(0, 0, _screenwidth, _screenheight, 0, 0, _screenwidth, _screenheight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}