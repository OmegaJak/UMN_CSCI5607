#include "image.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

/**
 * Image
 **/
Image::Image(int width_, int height_)
{
	assert(width_ > 0);
	assert(height_ > 0);

	width = width_;
	height = height_;
	num_pixels = width * height;
	sampling_method = IMAGE_SAMPLING_POINT;

	data.raw = new uint8_t[num_pixels * 4];
	int b = 0; //which byte to write to
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			data.raw[b++] = 0;
			data.raw[b++] = 0;
			data.raw[b++] = 0;
			data.raw[b++] = 0;
		}
	}

	assert(data.raw != NULL);
}

Image::Image(const Image &src)
{
	width = src.width;
	height = src.height;
	num_pixels = width * height;
	sampling_method = IMAGE_SAMPLING_POINT;

	data.raw = new uint8_t[num_pixels * 4];

	memcpy(data.raw, src.data.raw, num_pixels * 4);
	//*data.raw = *src.data.raw;
}

Image::Image(char *fname)
{
	int numComponents; //(e.g., Y, YA, RGB, or RGBA)
	data.raw = stbi_load(fname, &width, &height, &numComponents, 4);

	if (data.raw == NULL)
	{
		printf("Error loading image: %s", fname);
		exit(-1);
	}

	num_pixels = width * height;
	sampling_method = IMAGE_SAMPLING_POINT;
}

Image::~Image()
{
	delete data.raw;
	data.raw = NULL;
}

void Image::Write(const char *fname)
{
	int lastc = strlen(fname);

	switch (fname[lastc - 1])
	{
	case 'g':													   //jpeg (or jpg) or png
		if (fname[lastc - 2] == 'p' || fname[lastc - 2] == 'e')	//jpeg or jpg
			stbi_write_jpg(fname, width, height, 4, data.raw, 95); //95% jpeg quality
		else													   //png
			stbi_write_png(fname, width, height, 4, data.raw, width * 4);
		break;
	case 'a': //tga (targa)
		stbi_write_tga(fname, width, height, 4, data.raw);
		break;
	case 'p': //bmp
	default:
		stbi_write_bmp(fname, width, height, 4, data.raw);
	}
}

void Image::AddNoise(double factor)
{
	int newR, newG, newB;
	Pixel randomPixel;
	for (int i = 0; i < num_pixels; i++)
	{
		randomPixel = PixelRandom();
		newR = data.pixels[i].r + (randomPixel.r - 128) * factor;
		newG = data.pixels[i].g + (randomPixel.g - 128) * factor;
		newB = data.pixels[i].b + (randomPixel.b - 128) * factor;
		data.pixels[i].SetClamp(newR, newG, newB, data.pixels[i].a);
	}
}

void Image::Brighten(double factor)
{
	int x, y;
	for (x = 0; x < Width(); x++)
	{
		for (y = 0; y < Height(); y++)
		{
			Pixel p = GetPixel(x, y);
			Pixel scaled_p = p * factor;
			scaled_p.a = p.a;
			GetPixel(x, y) = scaled_p;
		}
	}
}

void Image::ChangeContrast(double factor)
{
	int total_luminance = 0;
	for (int i = 0; i < num_pixels; i++)
	{
		total_luminance += data.pixels[i].Luminance();
	}
	double average_luminance = total_luminance / (double)num_pixels;

	Pixel luminant_pixel = Pixel(average_luminance, average_luminance, average_luminance, 1.0);
	int alpha;
	for (int i = 0; i < num_pixels; i++)
	{
		alpha = data.pixels[i].a;											// Save the initial alpha
		data.pixels[i] = PixelLerp(luminant_pixel, data.pixels[i], factor); // Interpolate from the average luminant pixel and this one by factor
		data.pixels[i].a = alpha;											// Reset the alpha to leave it unaffected by contrast change
	}
}

void Image::ChangeSaturation(double factor)
{
	int local_luminance;
	Pixel local_grayscale;
	for (int i = 0; i < num_pixels; i++)
	{
		// Interpolate from the greyscale version of this pixel and itself by factor
		local_luminance = data.pixels[i].Luminance();
		local_grayscale = Pixel(local_luminance, local_luminance, local_luminance, data.pixels[i].a);
		data.pixels[i] = PixelLerp(local_grayscale, data.pixels[i], factor);
	}
}

Image *Image::Crop(int x, int y, int w, int h)
{
	if (x >= Width() || y >= Height()) {
		fprintf(stderr, "Invalid crop corner. The corner coordinates must be within the image. "
						"X value must be strictly less than the image width,  Y value must be less than the image height. "
						"The top left corner of the image is (0,0). The bottom right pixel is (%i, %i).\n",
				Width() - 1, Height() - 1);
		return NULL;
	} else if (w > Width() - x || h > Height() - y) {
		fprintf(stderr, "Invalid width or height parameter. "
						"The input image is %ix%i pixels.\n",
				Width(), Height());
		return NULL;
	}

	Image *image = new Image(w, h);
	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			image->GetPixel(i, j).Set(GetPixel(x + i, y + j));
		}
	}

	return image;
}

void Image::ExtractChannel(int channel)
{
	if (channel < 0 || channel > 3)
	{
		fprintf(stderr, "Invalid channel number. It must be a number between 0 and 3 (inclusive). Following are the channel mappings: \n"
						"red: 0\ngreen: 1\nblue: 2\nalpha: 3\n");

		return;
	}

	for (int i = 0; i < num_pixels; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (j != channel)
			{
				data.raw[i * 4 + j] = 0;
			}
		}
	}
}

void Image::Quantize(int nbits)
{
	double fraction, newFraction;
	int quantizedVal, maxValue;
	for (int i = 0; i < num_pixels * 4; i++)
	{
		if (i % 4 == 3)
		{ // Don't mess with alpha
			continue;
		}

		maxValue = pow(2, nbits) - 1;
		fraction = data.raw[i] / 255.0;
		quantizedVal = round(fraction * maxValue);
		newFraction = quantizedVal / (double)maxValue;
		data.raw[i] = round(newFraction * 255.0);
	}
}

// Has a chance from 0 to 1, given by chanceToGetAOne, to return a 1. 0.4 has a 40% to return 1, 60% chance to return 0.
int random(double chanceToGetAOne)
{
	double res = rand() % 1000;
	if (res < chanceToGetAOne * 1000)
		return 1;
	else
		return 0;
}

void Image::RandomDither(int nbits)
{
	double fractional, newFraction, diff;
	int quantizedVal, maxValue;
	for (int i = 0; i < num_pixels * 4; i++)
	{
		if (i % 4 == 3)
		{ // Don't mess with alpha
			//printf("(r, g, b): (%i, %i, %i)\n", data.pixels[i / 4].r, data.pixels[i / 4].g, data.pixels[i / 4].b);
			continue;
		}

		maxValue = pow(2, nbits) - 1;
		fractional = (data.raw[i] / 255.0) * maxValue;
		diff = fractional - floor(fractional);
		quantizedVal = floor(fractional) + random(diff); // Clamp to nearest number that can be stored with nbits

		// Scale back up to 0 to 255
		newFraction = quantizedVal / (double)maxValue;
		data.raw[i] = round(newFraction * 255.0);
	}
}

static int Bayer4[4][4] =
	{
		{15, 7, 13, 5},
		{3, 11, 1, 9},
		{12, 4, 14, 6},
		{0, 8, 2, 10}};

void Image::OrderedDither(int nbits)
{
	/* WORK HERE */
}

/* Error-diffusion parameters */
const double
	ALPHA = 7.0 / 16.0,
	BETA = 3.0 / 16.0,
	GAMMA = 5.0 / 16.0,
	DELTA = 1.0 / 16.0;

struct PrecisePixel {
	double r, g, b, a;

	PrecisePixel(double r_=0, double g_=0, double b_=0, double a_=0) : r(r_), g(g_), b(b_), a(a_) {}
	PrecisePixel(Pixel pixel) { r = pixel.r; g = pixel.g, b = pixel.b, a = pixel.a; }

	Pixel ToPixel() { Pixel pixel = Pixel(); pixel.SetClamp(r, g, b, a); return pixel; }

	void Set(const PrecisePixel& other) {
		r = other.r;
		g = other.g;
		b = other.b;
		a = other.a;
	}

	const PrecisePixel operator* (double f) const {
		return PrecisePixel(r * f, g * f, b * f, a * f);
	}

	const PrecisePixel operator+ (PrecisePixel& other) const {
		return PrecisePixel(r + other.r, g + other.g, b + other.b, a + other.a);
	}

	PrecisePixel &operator+= (const PrecisePixel other) {
		r += other.r;
		g += other.g;
		b += other.b;
		a += other.a;
		return *this;
	}
};


void Image::FloydSteinbergDither(int nbits)
{
	PrecisePixel error[Width()][2];
	for (int x = 0; x < Width(); x++) {
		for (int y = 0; y < 2; y++) {
			error[x][y] = PrecisePixel();
		}
	}

	Pixel oldPixel, newPixel;
	PrecisePixel quantError;
	for (int y = 0; y < Height(); y++) {
		for (int x = 0; x < Width(); x++) {
			oldPixel = GetPixel(x, y);
			PrecisePixel err = error[x][0];
			Pixel old = Pixel(oldPixel);
			old.SetClamp(old.r + err.r, old.g + err.g, old.b + err.b);
			newPixel = SetPixel(x, y, PixelQuant(old, nbits));
			
			quantError = PrecisePixel(oldPixel.r - newPixel.r, oldPixel.g - newPixel.g, oldPixel.b - newPixel.b);
			if (ValidCoord(x + 1, y)) error[x + 1][0] += quantError * ALPHA; // Right
			if (ValidCoord(x + 1, y + 1)) error[x + 1][1] += quantError * DELTA; // Down right
			if (ValidCoord(x, y + 1)) error[x][1] += quantError * GAMMA; // Down
			if (ValidCoord(x - 1, y + 1)) error[x - 1][1] += quantError * BETA; // Down left
		}

		for (int x = 0; x < Width(); x++) {
			error[x][0].Set(error[x][1]);
			error[x][1] = PrecisePixel();
		}
	}
}

static double Gaussian[5] = { 1.0/16.0, 4.0/16.0, 6.0/16.0, 4.0/16.0, 1.0/16.0 };

int reflectValue(int value, int maxValueInclusive) {
	if (value < 0) {
		return -1 * value;
	} else if (value > maxValueInclusive) {
		return maxValueInclusive + (maxValueInclusive - value);
	} else {
		return value;
	}
}

double Gauss(int x, int radius) {
	double sigma = radius < 1 ? 1 : radius;
	return (1 / (sigma * sqrt(2 * M_PI))) * exp(-(x*x) / (2.0 * sigma * sigma));
}

void Image::Blur(int n)
{
	double arr[n * 2 + 1];
	double *filter = arr + n; // This is pretty risky but makes other things nicer. It centers the array to allow negative indexing to match the loops used.
	double total = 0;
	for (int i = -n; i <= n; i++) {
		filter[i] = Gauss(i, n);
		total = total + filter[i];
	}
	for (int i = -n; i <= n; i++) {
		filter[i] = filter[i] / total;
	}
	

	// First, the horizontal pass, then vertical pass
	Pixel result;
	for (int i = 0; i < 2; i++) {
		Image oldImage = Image(*this);
		for (int x = 0; x < Width(); x++) {
			for (int y = 0; y < Height(); y++) {
				result = Pixel(0, 0, 0, 0);
				for (int j = -n; j <= n; j++) {
					if (i == 0) {
						int horizontal = reflectValue(x - j, Width() - 1);
						result = result + filter[j] * oldImage.GetPixel(horizontal, y);
					} else if (i == 1) {
						int vertical = reflectValue(y - j, Height() - 1);
						result = result + filter[j] * oldImage.GetPixel(x, vertical);
					}
					SetPixel(x, y, result);
				}
			}
		}
	}
}

void Image::Sharpen(int n)
{
	Image oldImage = Image(*this);
	Blur(n);
	for (int x = 0; x < Width(); x++) {
		for (int y = 0; y < Height(); y++) {
			SetPixel(x, y, PixelLerp(oldImage.GetPixel(x, y), GetPixel(x, y), -1));
		}
	}
}

void Image::EdgeDetect()
{
	/* WORK HERE */
}

Image *Image::Scale(double sx, double sy)
{
	/* WORK HERE */
	return NULL;
}

Image *Image::Rotate(double angle)
{
	/* WORK HERE */
	return NULL;
}

void Image::Fun()
{
	/* WORK HERE */
}

/**
 * Image Sample
 **/
void Image::SetSamplingMethod(int method)
{
	assert((method >= 0) && (method < IMAGE_N_SAMPLING_METHODS));
	sampling_method = method;
}

Pixel Image::Sample(double u, double v)
{
	/* WORK HERE */
	return Pixel();
}

void Image::Purplify(double factor)
{
	const double red_factor = 76 / 256.0;
	const double green_factor = 150 / 256.0;
	const double blue_factor = 29 / 256.0;

	for (int i = 0; i < num_pixels; i++)
	{
		double luminance_diff = data.pixels[i].Luminance();
		double desired_luminance = factor * luminance_diff;
		double green_red = data.pixels[i].g / (double)data.pixels[i].r;
		double blue_red = data.pixels[i].b / (double)data.pixels[i].b;

		double new_red = desired_luminance / (red_factor + green_factor * green_red + blue_factor * blue_red);
		data.pixels[i].SetClamp(new_red, green_red * new_red, blue_red * new_red);
	}
}