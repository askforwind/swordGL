#include "FreeImageCodec.h"
#include "FreeImage.h"
#include "LogManager.h"
SWORD_BEGIN

FreeImageCodec::FreeImageCodec() 
	:bitmap_(nullptr){
	startUp();
}

FreeImageCodec::~FreeImageCodec() {
	shutDown();
}

void FreeImageCodec::startUp() {
	FreeImage_Initialise();

	FreeImage_SetOutputMessage(
		[](FREE_IMAGE_FORMAT fif, const char* message)->void {
		std::string old_console_format = WIND::LogManager::
			set_default_console_logger_format("%D{%H:%M:%S} %p: %m\n");
		std::string old_file_format = WIND::LogManager::
			set_default_file_logger_format("%D{%H:%M:%S} %p: %m\n");

		std::string error = "Image Error:";
		WIND_LOG_ERROR(DEFAULT_WIND_LOGGER,
					   error
					   + FreeImage_GetFormatFromFIF(fif)
					   + " " + message);

		WIND::LogManager::set_default_console_logger_format(old_console_format);
		WIND::LogManager::set_default_file_logger_format(old_file_format);
	});
}

void FreeImageCodec::shutDown() {
	FreeImage_Unload(bitmap_);
	FreeImage_DeInitialise();
}

FreeImageCodec::Result 
FreeImageCodec::decode(const char* filename) {
	FreeImage_Unload(bitmap_);

	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);

	Result ret;
	ret.byte = nullptr;
	ret.height = 0;
	ret.width = 0;

	std::string msg;
	if (format == FIF_UNKNOWN) {
		msg = "Unknown file Type:";
		WIND_LOG_ERROR(DEFAULT_WIND_LOGGER, msg + filename);
		return ret;
	}

	bitmap_ = FreeImage_Load(format, filename);
	if (!bitmap_) {
		msg = "Can't load file:";
		WIND_LOG_ERROR(DEFAULT_WIND_LOGGER, msg + filename);
		return ret;
	}

	BYTE* bite_data = FreeImage_GetBits(bitmap_);
	unsigned int width = FreeImage_GetWidth(bitmap_);
	unsigned int height = FreeImage_GetHeight(bitmap_);

	if (!bite_data || width == 0 || height == 0) {
		msg = "Can't Load Resource Correct in:";
		WIND_LOG_ERROR(DEFAULT_WIND_LOGGER, msg + filename);
		return ret;
	}

	ret.byte = bite_data;
	ret.width = width;
	ret.height = height;

	uint8_t ct = FreeImage_GetColorType(bitmap_);

	switch (ct) {
	case FIC_RGB:
		ret.color_type = RGB; break;
	case FIC_RGBALPHA:
		ret.color_type = RGBA; break;
	default:
		ret.color_type = OTHER;
		break;
	}

	return ret;
}

SWORD_END;