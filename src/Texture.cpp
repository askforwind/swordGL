#include"Texture.h"
#include "FreeImageCodec.h"
#include"LogManager.h"
#include<GL/glew.h>
#include"config.h"

using std::max;

SWORD_BEGIN

const uint8_t Texture::default_mipmap_num_ = 4;

Texture::Texture()
    : tex_id_(0)
	, width_(0)
    , height_(0)
    , inte_format_(0)
    , texture_format_(0) {
}

bool Texture::load(const std::string& filename,
				   bool compress,
				   bool auto_create_mipmap,
				   uint8_t mipmap_num) {

	WIND_LOG_TRACE(DEFAULT_WIND_LOGGER, "start load texture:  " + filename);

    unload();
	
	FreeImageCodec codec;
	FreeImageCodec::Result r = codec.decode(filename.c_str());

	if (r.byte == nullptr) return false;

	width_ = r.width;
	height_ = r.height;

    createTexture(r.byte, r.color_type,
                   compress, auto_create_mipmap, mipmap_num);

    return true;
}

void Texture::createTexture(uint8_t* data,
                            uint8_t color_type,
							bool compress,
                            bool create_mipmap,
							uint8_t mipmap_num
                           ) {

    switch(color_type) {
	case FreeImageCodec::RGB:
        texture_format_ = GL_BGR;
#if GL_EXT_texture_compression_s3tc
        inte_format_ = compress ? GL_COMPRESSED_RGB_S3TC_DXT1_EXT : GL_RGB;
#else
        inte_format_ = compress ? GL_COMPRESSED_RGB : GL_RGB;
#endif
        break;
    case FreeImageCodec::RGBA:
        texture_format_ = GL_BGRA;
#if GL_EXT_texture_compression_s3tc
        inte_format_ = compress ? GL_COMPRESSED_RGBA_S3TC_DXT5_EXT : GL_RGBA;
#else
        inte_format_ = compress ? GL_COMPRESSED_RGBA : GL_RGBA;
#endif
        break;
    default:// TODO check it later
        texture_format_ = GL_BGR;
        inte_format_ = GL_LUMINANCE;
		assert(0 && "we need a BGRA or BGR picture now");
        break;
    }

    // default settings
    CHECK_GL_ERROR(glGenTextures(1, &tex_id_));
    CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, tex_id_));
    CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    if(!(width_ & 3)) CHECK_GL_ERROR(glPixelStorei(GL_UNPACK_ALIGNMENT, 4));
	assert(width_&&height_);
    CHECK_GL_ERROR(glTexImage2D(GL_TEXTURE_2D, 0, inte_format_, width_, height_, 0,
                                texture_format_, GL_UNSIGNED_BYTE, data));

    if(create_mipmap) {
        CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0));
        CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipmap_num));
        CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        setMipmapAuto(mipmap_num, 1);
    };

    CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, 0));
}
void Texture::setMipmapAuto(uint8_t max_level, uint8_t start_level) {
    assert(tex_id_);
    assert(max_level >= start_level);
    assert(max_level <= 8);
	assert(start_level > 0);

    CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, tex_id_));
    if(max_level > default_mipmap_num_)
        CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, max_level));

    uint16_t w = width_, h = height_;
    uint8_t mip_lv = 0;
    while(mip_lv < start_level) {
        mip_lv++;
        w = max(1, w / 2);
        h = max(1, h / 2);
    }

    for(; mip_lv <= max_level; ++mip_lv) {
        CHECK_GL_ERROR(glTexImage2D(GL_TEXTURE_2D, mip_lv,
                                    inte_format_, w, h, 0,
                                    texture_format_, GL_UNSIGNED_BYTE, NULL));

        w = max(1, w / 2);
        h = max(1, h / 2);
    }

    CHECK_GL_ERROR(glGenerateMipmap(GL_TEXTURE_2D));
    CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::bindToActiveUnit(uint8_t idx)const {
    assert(tex_id_);
	assert(width_&&height_);
	CHECK_GL_ERROR(glActiveTexture(idx + GL_TEXTURE0));
	CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, tex_id_));
}

void Texture::unload() {
	if (tex_id_)
		CHECK_GL_ERROR(glDeleteTextures(1, &tex_id_));

	tex_id_ = 0;
    inte_format_ = 0;
    texture_format_ = 0;
    width_ = 0;
    height_ = 0;
}
SWORD_END


