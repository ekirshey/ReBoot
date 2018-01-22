#include "DeferredShader.h"

DeferredShader::DeferredShader(std::string shaderName) : Shader(shaderName) {

	//Build 2 triangles for screen space quad
	const float length = 1.0f; 
    const float depth = 0.0f; 
    //2 triangles in screen space 
    float triangles[] = { -length, -length, depth,
                         -length, length, depth,
                         length, length, depth,

                         -length, -length, depth,
                         length, length, depth,
                         length, -length, depth };

    glGenBuffers(1, &_quadBufferContext);
    glBindBuffer(GL_ARRAY_BUFFER, _quadBufferContext);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 3, triangles, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //2 texture coordinates in screen space 
    float textures[] = { 0.0, 0.0,
                          0.0, 1.0,
                          1.0, 1.0,

                          0.0, 0.0,
                          1.0, 1.0,
                          1.0, 0.0 };

    glGenBuffers(1, &_textureBufferContext);
    glBindBuffer(GL_ARRAY_BUFFER, _textureBufferContext);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 2, textures, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	
    //Manually find the two texture locations for loaded shader
    _diffuseTextureLocation = glGetUniformLocation(_shaderContext, "diffuseTexture");
    _normalTextureLocation = glGetUniformLocation(_shaderContext, "normalTexture");
    _positionTextureLocation = glGetUniformLocation(_shaderContext, "positionTexture");
    _depthTextureLocation = glGetUniformLocation(_shaderContext, "depthTexture");
    _lightLocation = glGetUniformLocation(_shaderContext, "light");
    _lightViewLocation = glGetUniformLocation(_shaderContext, "lightViewMatrix");
    _viewsLocation = glGetUniformLocation(_shaderContext, "views");

	//glUniform mat4 combined model and world matrix
    _modelLocation = glGetUniformLocation(_shaderContext, "model");

    //glUniform mat4 view matrix
    _viewLocation = glGetUniformLocation(_shaderContext, "view");

    //glUniform mat4 projection matrix
    _projectionLocation = glGetUniformLocation(_shaderContext, "projection");

    //glUniform mat4 normal matrix
    _normalLocation = glGetUniformLocation(_shaderContext, "normal");
}

DeferredShader::~DeferredShader() {

}

void DeferredShader::runShader(ShadowRenderer* shadowRenderer, 
							   std::vector<Light*>& lights, 
							   ViewManager* viewManager,
							   MRTFrameBuffer& mrtFBO) {

    for(Light* light : lights){
        //Take the generated texture data and do deferred shading
        //LOAD IN SHADER
        glUseProgram(_shaderContext); //use context for loaded shader

                                                          //LOAD IN VBO BUFFERS 
                                                          //Bind vertex buff context to current buffer
        glBindBuffer(GL_ARRAY_BUFFER, _quadBufferContext);

        //Say that the vertex data is associated with attribute 0 in the context of a shader program
        //Each vertex contains 3 floats per vertex
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        //Now enable vertex buffer at location 0
        glEnableVertexAttribArray(0);

        //Bind texture coordinate buff context to current buffer
        glBindBuffer(GL_ARRAY_BUFFER, _textureBufferContext);

        //Say that the texture coordinate data is associated with attribute 2 in the context of a shader program
        //Each texture coordinate contains 2 floats per texture
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

        //Now enable texture buffer at location 2
        glEnableVertexAttribArray(1);

        //Get light position
        Vector4 transformedLight = light->getPosition();
        float* buff = transformedLight.getFlatBuffer();
        glUniform3f(_lightLocation, buff[0], buff[1], buff[2]);

        //Change of basis from camera view position back to world position
        MVP lightMVP = light->getMVP();
        Matrix cameraToLightSpace = lightMVP.getProjectionMatrix() * 
            lightMVP.getViewMatrix() * 
            viewManager->getView().inverse();

        //glUniform mat4 view matrix, GL_TRUE is telling GL we are passing in the matrix as row major
        glUniformMatrix4fv(_lightViewLocation, 1, GL_TRUE, cameraToLightSpace.getFlatBuffer());

        //glUniform mat4 view matrix, GL_TRUE is telling GL we are passing in the matrix as row major
        glUniformMatrix4fv(_projectionLocation, 1, GL_TRUE, viewManager->getProjection().getFlatBuffer());

        //glUniform mat4 view matrix, GL_TRUE is telling GL we are passing in the matrix as row major
        glUniformMatrix4fv(_viewLocation, 1, GL_TRUE, viewManager->getView().getFlatBuffer());

        glUniform1i(_viewsLocation, static_cast<GLint>(viewManager->getViewState()));

        auto textures = mrtFBO.getTextureContexts();

        //Diffuse texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        //glUniform texture 
        glUniform1iARB(_diffuseTextureLocation, 0);

        //Normal texture
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[1]);
        //glUniform texture 
        glUniform1iARB(_normalTextureLocation, 1);

        //Position texture
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, textures[2]);
        //glUniform texture 
        glUniform1iARB(_positionTextureLocation, 2);

        //Depth texture
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowRenderer->getDepthTexture());
        //glUniform texture 
        glUniform1iARB(_depthTextureLocation, 3);

        //Draw triangles using the bound buffer vertices at starting index 0 and number of vertices
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)6);

        glDisableVertexAttribArray(0); //Disable vertex attribute
        glDisableVertexAttribArray(1); //Disable texture attribute
        glBindBuffer(GL_ARRAY_BUFFER, 0); //Unbind buffer
        glUseProgram(0);//end using this shader
    }
}