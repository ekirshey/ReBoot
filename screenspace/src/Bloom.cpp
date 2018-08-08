#include "Bloom.h"
#include "SSCompute.h"
#include "GLIncludes.h"
#include "SimpleContext.h"
#include "Texture.h"

Bloom::Bloom() {

    _luminanceFilter = new SSCompute("highLuminanceFilter", SimpleContext::screenPixelWidth, SimpleContext::screenPixelHeight, TextureFormat::RGBA_UNSIGNED_BYTE);
    _horizontalBlur = new SSCompute("blurHorizontalShaderRGB", SimpleContext::screenPixelWidth / 4, SimpleContext::screenPixelHeight / 4, TextureFormat::RGBA_UNSIGNED_BYTE);
    _verticalBlur = new SSCompute("blurVerticalShaderRGB", SimpleContext::screenPixelWidth / 4, SimpleContext::screenPixelHeight / 4, TextureFormat::RGBA_UNSIGNED_BYTE);
    
    //downsampling shader
    _downSample = new SSCompute("downsampleRGB", SimpleContext::screenPixelWidth / 4, SimpleContext::screenPixelHeight / 4, TextureFormat::RGBA_UNSIGNED_BYTE);

    //upsampling shader
    _upSample = new SSCompute("upsampleRGB", SimpleContext::screenPixelWidth, SimpleContext::screenPixelHeight, TextureFormat::RGBA_UNSIGNED_BYTE);
}

Bloom::~Bloom() {

}

unsigned int Bloom::getTextureContext() {
    return _upSample->getTextureContext();
}

Texture* Bloom::getTexture() {
    return _upSample->getTexture();
}

void Bloom::compute(Texture* deferredFBOTexture) {

    //Luminance threshold test
    _luminanceFilter->compute(deferredFBOTexture);

    _downSample->compute(_luminanceFilter->getTexture());

    //Do a horizontal and then a vertical blur pass!
    _horizontalBlur->compute(_downSample->getTexture());

    //Blur 4 more times!
    for (int i = 0; i < 4; i++) {
        _verticalBlur->compute(_horizontalBlur->getTexture());
        _horizontalBlur->compute(_verticalBlur->getTexture());
    }
    _verticalBlur->compute(_horizontalBlur->getTexture());

    _upSample->compute(_verticalBlur->getTexture());
}