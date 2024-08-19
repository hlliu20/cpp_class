#ifndef CPP_QOI_H
#define CPP_QOI_H

#define QOI_SRGB   0
#define QOI_LINEAR 1

// // 文件头
// struct qoi_header_t {
// 	char     magic[4];   // magic bytes "qoif"
// 	uint32_t width;      // image width in pixels (BE)
// 	uint32_t height;     // image height in pixels (BE)
// 	uint8_t  channels;   // 3 = RGB, 4 = RGBA
// 	uint8_t  colorspace; // 0 = sRGB with linear alpha, 1 = all channels linear
// };

typedef struct {
    // char magic[4];      // magic bytes "qoif"
    uint32_t width;     // image width in pixels (BE)
    uint32_t height;    // image height in pixels (BE)
    uint8_t channels;   // 3 = RGB, 4 = RGBA
    uint8_t colorspace; // 0 = sRGB with linear alpha
                        // 1 = all channels linear
} qoi_desc;

#define QOI_OP_INDEX  0x00 /* 00xxxxxx */
#define QOI_OP_DIFF   0x40 /* 01xxxxxx */
#define QOI_OP_LUMA   0x80 /* 10xxxxxx */
#define QOI_OP_RUN    0xc0 /* 11xxxxxx */
#define QOI_OP_RGB    0xfe /* 11111110 */
#define QOI_OP_RGBA   0xff /* 11111111 */

#define QOI_MASK_2    0xc0 /* 11000000 */
// 定义哈希函数
#define QOI_COLOR_HASH(C) (C.rgba.r*3 + C.rgba.g*5 + C.rgba.b*7 + C.rgba.a*11)
//将“qiof”转为uint32_t 储存
#define QOI_MAGIC \
	(((uint32_t)'q') << 24 | ((uint32_t)'o') << 16 | \
	 ((uint32_t)'i') <<  8 | ((uint32_t)'f'))
// 文件头的大小：14字节
#define QOI_HEADER_SIZE 14
// 文件像素最大值：400000000
#define QOI_PIXELS_MAX ((uint32_t)400000000)
// 储存一个点的像素
typedef union {
	struct { uint8_t r, g, b, a; } rgba;
	uint32_t v;
} qoi_rgba_t;
// 文件尾
static const uint8_t qoi_padding[8] = {0,0,0,0,0,0,0,1};
// 文件尾的大小：8字节
#define QOI_ENDER_SIZE 8

/// @brief 写入4字节
/// @param bytes uint8_t * 指向数据头部的指针
/// @param p int * "bytes[p]" 就是要写入的位置
/// @param v uint32_t 要写入的四字节
static void qoi_write_32(uint8_t *bytes, int *p, uint32_t v) {
	bytes[(*p)++] = (0xff000000 & v) >> 24;
	bytes[(*p)++] = (0x00ff0000 & v) >> 16;
	bytes[(*p)++] = (0x0000ff00 & v) >> 8;
	bytes[(*p)++] = (0x000000ff & v);
}

/// @brief 读取四字节数据存入 uint32_t
/// @param bytes const uint8_t * 指向数据头部的指针
/// @param p int *  "bytes[p]" 就是要读取的位置
/// @return uint32_t 返回int
static uint32_t qoi_read_32(const uint8_t *bytes, int *p) {
	uint32_t a = bytes[(*p)++];
	uint32_t b = bytes[(*p)++];
	uint32_t c = bytes[(*p)++];
	uint32_t d = bytes[(*p)++];
	return a << 24 | b << 16 | c << 8 | d;
}

/// @brief 编码为qoi格式
/// @param pixels const uint8_t * 储存像素数据的指针
/// @param desc const qoi_desc * 储存qoi_desc结构体的指针
/// @param out_len int *  储存编码后的数据字节数，可返回
/// @return uint8_t *  返回指向编码后数据位置的指针
uint8_t * qoi_encode(const uint8_t * pixels, const qoi_desc * desc, int * out_len){
	// 储存要new的数组的大小、当前编码后数据的字节数、相同像素的个数
	int max_size, p=0, run=0;
	// 传入像素数据的字节数，px_len - 通道数，已处理的字节数，通道数
	int px_len, px_end, px_pos, channels;
	// 要返回的，储存编码后的数据
	uint8_t * bytes;
	// 哈希数组
	qoi_rgba_t index[64];
	// 储存当前像素、上一个像素
	qoi_rgba_t px,px_prev;
	// 如果传入的参数不正确，函数停止，返回空指针
	if (
		pixels == nullptr  || desc == nullptr ||
		desc->width == 0 || desc->height == 0 ||
		desc->channels < 3 || desc->channels > 4 ||
		desc->colorspace > 1 ||
		desc->height >= QOI_PIXELS_MAX / desc->width
	) {
		return nullptr;
	}
	
	// max_size =
	// 	desc->width * desc->height * (desc->channels + 1) +
	// 	QOI_HEADER_SIZE + QOI_ENDER_SIZE;
	// ====>
	//应该不用多分配这么多空间吧，如果出错可以试试改回去
	max_size =
		desc->width * desc->height * desc->channels +
		QOI_HEADER_SIZE + QOI_ENDER_SIZE+10;
	// 分配空间，如果分配失败则函数结束，返回空指针
	bytes = new uint8_t[max_size];
	if(bytes == nullptr) {return nullptr;}

	// 写入空间头
	qoi_write_32(bytes, &p, QOI_MAGIC);
	qoi_write_32(bytes, &p, desc->width);
	qoi_write_32(bytes, &p, desc->height);
	bytes[p++] = desc->channels;
	bytes[p++] = desc->colorspace;
	// 初始化哈希数组，全设为0
	for(int j=0;j<64;++j){index[j].v = 0;} // 这行应该可以省略

	// 默认第1个像素前的像素为(0,0,0,255)
	px_prev.rgba.r = 0;
	px_prev.rgba.g = 0;
	px_prev.rgba.b = 0;
	px_prev.rgba.a = 255;
	px = px_prev;

	px_len = desc->width * desc->height * desc->channels;
	px_end = px_len - desc->channels;
	channels = desc->channels;

	// 对像素数据逐像素处理
	for(px_pos = 0; px_pos < px_len; px_pos += channels){
		px.rgba.r = pixels[px_pos + 0];
		px.rgba.g = pixels[px_pos + 1];
		px.rgba.b = pixels[px_pos + 2];
		// 如果有a通道，px存入a通道的数据
		if (channels == 4) {
			px.rgba.a = pixels[px_pos + 3];
		}
		// 如果这个像素和前一个像素一样
		if (px.v == px_prev.v) {
			run++;
			// run加一，然后看run到没到62（0b111110）到了就写入一次且run归0
			if (run == 62 || px_pos == px_end) {
				bytes[p++] = QOI_OP_RUN | (run - 1);
				run = 0;
			}
		} else { // 如果这个像素和前一个不同
			// 先看看run是否为0，如果不为0要先把前面那些相同的像素保存
			if (run > 0) {
				bytes[p++] = QOI_OP_RUN | (run - 1);
				run = 0;
			}
			// 计算哈希值
			int index_pos =  QOI_COLOR_HASH(px) % 64;
			// 如果已经有了，就按INDEX的方式储存否则记录在哈希表中
			if (index[index_pos].v == px.v) {
				bytes[p++] = QOI_OP_INDEX | index_pos;
			} else {
				index[index_pos] = px;
				// 先看a通道是否相同，如果是三通道则a通道一直为255
				// 这里没有分3、4通道，即使是4通道且a通道相同，也按3通道保存
				if(px.rgba.a == px_prev.rgba.a){
					signed char vr = px.rgba.r - px_prev.rgba.r;
					signed char vg = px.rgba.g - px_prev.rgba.g;
					signed char vb = px.rgba.b - px_prev.rgba.b;

					signed char vg_r = vr - vg;
					signed char vg_b = vb - vg;

					// a通道相同就计算一下，然后从好到坏逐个方法看行不行，都不行就把RGB三个像素都存上
					if (vr > -3 && vr < 2 && vg > -3 && vg < 2 && vb > -3 && vb < 2) {
						bytes[p++] = QOI_OP_DIFF | (vr + 2) << 4 | (vg + 2) << 2 | (vb + 2);
					} else if(vg_r >  -9 && vg_r <  8 && vg   > -33 && vg   < 32 && vg_b >  -9 && vg_b <  8) {
						bytes[p++] = QOI_OP_LUMA     | (vg   + 32);
						bytes[p++] = (vg_r + 8) << 4 | (vg_b +  8);
					} else {
						bytes[p++] = QOI_OP_RGB;
						bytes[p++] = px.rgba.r;
						bytes[p++] = px.rgba.g;
						bytes[p++] = px.rgba.b;
					}
				} else {  // a通道不同，按RGBA把各个通道都存上
					bytes[p++] = QOI_OP_RGBA;
					bytes[p++] = px.rgba.r;
					bytes[p++] = px.rgba.g;
					bytes[p++] = px.rgba.b;
					bytes[p++] = px.rgba.a;
				}
			}
		}
		px_prev = px;

	}
	// 最后把文件尾加上
	for (int i = 0; i < QOI_ENDER_SIZE; ++i) {
		bytes[p++] = qoi_padding[i];
	}
	// 将编码数据的字节数传出函数
	*out_len = p;
	return bytes;
}

// 原文是这样的
// void *qoi_decode(const void *data, int size, qoi_desc *desc, int channels) {
// 我把最后一个参数去了，改了返回值


/// @brief 解码qoi数据为像素数据
/// @param bytes const uint8_t * 传入的待解码数据
/// @param size int 待解码数据的字节数；好像也能省略？
/// @param desc qoi_desc *  储存qoi_desc结构体的指针
/// @return uint8_t * 返回解码后像素数据的指针
uint8_t * qoi_decode(const uint8_t *bytes, int size, qoi_desc *desc) {
	uint32_t header_magic;
	uint8_t *pixels;
	qoi_rgba_t index[64];
	qoi_rgba_t px;
	int px_len, chunks_len, px_pos;
	int p = 0, run = 0;

	if (
		bytes == nullptr || desc == nullptr ||
		// 因为参数里去除了channels，这里注释了
		// (channels != 0 && channels != 3 && channels != 4) ||
		size < QOI_HEADER_SIZE + QOI_ENDER_SIZE
	) {
		return nullptr;
	}

	header_magic = qoi_read_32(bytes, &p);
	desc->width = qoi_read_32(bytes, &p);
	desc->height = qoi_read_32(bytes, &p);
	desc->channels = bytes[p++];
	desc->colorspace = bytes[p++];

	if (
		desc->width == 0 || desc->height == 0 ||
		desc->channels < 3 || desc->channels > 4 ||
		desc->colorspace > 1 ||
		header_magic != QOI_MAGIC ||
		desc->height >= QOI_PIXELS_MAX / desc->width
	) {
		return nullptr;
	}

	
	int channels = desc->channels;

	px_len = desc->width * desc->height * channels;

	pixels = new uint8_t[px_len];
	if(pixels == nullptr) {return nullptr;}

	for(int j=0;j<64;++j){index[j].v=0;}

	px.rgba.r = 0;
	px.rgba.g = 0;
	px.rgba.b = 0;
	px.rgba.a = 255;

	chunks_len = size - QOI_ENDER_SIZE;
	for (px_pos = 0; px_pos < px_len; px_pos += channels) {
		if (run > 0) {
			run--;
		} else if (p < chunks_len) {
			int b1 = bytes[p++];

			if (b1 == QOI_OP_RGB) {
				px.rgba.r = bytes[p++];
				px.rgba.g = bytes[p++];
				px.rgba.b = bytes[p++];
			} else if (b1 == QOI_OP_RGBA) {
				px.rgba.r = bytes[p++];
				px.rgba.g = bytes[p++];
				px.rgba.b = bytes[p++];
				px.rgba.a = bytes[p++];
			} else if ((b1 & QOI_MASK_2) == QOI_OP_INDEX) {
				px = index[b1];
			} else if ((b1 & QOI_MASK_2) == QOI_OP_DIFF) {
				px.rgba.r += ((b1 >> 4) & 0x03) - 2;
				px.rgba.g += ((b1 >> 2) & 0x03) - 2;
				px.rgba.b += ( b1       & 0x03) - 2;
			} else if ((b1 & QOI_MASK_2) == QOI_OP_LUMA) {
				int b2 = bytes[p++];
				int vg = (b1 & 0x3f) - 32;
				px.rgba.r += vg - 8 + ((b2 >> 4) & 0x0f);
				px.rgba.g += vg;
				px.rgba.b += vg - 8 +  (b2       & 0x0f);
			}
			else if ((b1 & QOI_MASK_2) == QOI_OP_RUN) {
				run = (b1 & 0x3f);
			}

			index[QOI_COLOR_HASH(px) % 64] = px;
		}

		pixels[px_pos + 0] = px.rgba.r;
		pixels[px_pos + 1] = px.rgba.g;
		pixels[px_pos + 2] = px.rgba.b;
		
		if (channels == 4) {
			pixels[px_pos + 3] = px.rgba.a;
		}
	}

	return pixels;
}

/// @brief 以qoi格式写入文件
/// @param filename const char * 写入的文件名/文件路径
/// @param pixels const uint8_t * 像素数据储存位置的指针
/// @param desc const qoi_desc *  储存qoi_desc结构体的指针
/// @return int 写入失败返回-1，成功返回写入的字节数
int qoi_write(const char *filename, const uint8_t *pixels, const qoi_desc *desc) {
	std::ofstream ofs(filename, std::ios::out|std::ios::binary);
	int size;
	uint8_t *encoded;

	if (ofs.is_open()!=true) {
		return -1;
	}

	encoded = qoi_encode(pixels, desc, &size);
	if (encoded==nullptr) {
		ofs.close();
		return -1;
	}
	ofs.write((char *)encoded, size);
	ofs.close();

	delete[] encoded;
	return size;
}

/// @brief 读取qoi文件的像素
/// @param filename const char * 读取的文件名/文件路径
/// @param desc const qoi_desc *  储存qoi_desc结构体的指针，可返回
/// @return uint8_t * 读取失败返回nullptr，成功返回储存像素数据的指针
uint8_t * qoi_read(const char *filename, qoi_desc *desc) {
	std::ifstream ifs(filename, std::ios::in|std::ios::binary|std::ios::ate);
	int size, bytes_read;
	uint8_t * pixels, *data;

	if (ifs.is_open()!=true) {
		return nullptr;
	}
	size = ifs.tellg();
	if (size <= 0) {
		ifs.close();
		return nullptr;
	}
	ifs.seekg(0, std::ios::beg);

	data = new uint8_t[size];
	if (data==nullptr) {
		ifs.close();
		return nullptr;
	}

	ifs.read((char *)data, size);
	ifs.close();
	pixels = qoi_decode(data, size, desc);
	delete[] data;
	return pixels;
}
#endif