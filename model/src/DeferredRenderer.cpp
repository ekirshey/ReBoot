#include "DeferredRenderer.h"
#include "Model.h"

DeferredRenderer::DeferredRenderer() : _mrtFBO(3), _deferredShader("deferredShader"){
    
}

DeferredRenderer::~DeferredRenderer() {

}

void DeferredRenderer::deferredLighting(ShadowRenderer* shadowRenderer, std::vector<Light*>& lights, ViewManager* viewManager) {
	_deferredShader.runShader(shadowRenderer, lights, viewManager, _mrtFBO);
}

void DeferredRenderer::bind() {
    //Bind frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, _mrtFBO.getFrameBufferContext());

    //Clear color buffer from frame buffer otherwise framebuffer will contain data from the last draw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    // Specify what to render an start acquiring
    GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, buffers);
}
void DeferredRenderer::unbind() {
    //unbind frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}