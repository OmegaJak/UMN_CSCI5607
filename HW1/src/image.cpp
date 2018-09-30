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

PrecisePixel operator* (double f, PrecisePixel p) {
	return p * f;
}

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

void Image::SeparableConvolve(int radius, double* xFilter, double* yFilter) {
	// First, the horizontal pass (i = 0), then vertical pass (i = 1)
	PrecisePixel result;
	for (int i = 0; i < 2; i++) {
		Image oldImage = Image(*this);
		for (int x = 0; x < Width(); x++) {
			for (int y = 0; y < Height(); y++) {
				result = Pixel(0, 0, 0);
				for (int j = -radius; j <= radius; j++) {
					if (i == 0) {
						int horizontal = reflectValue(x - j, Width() - 1);
						result += PrecisePixel(oldImage.GetPixel(horizontal, y)) * xFilter[j];
					} else if (i == 1) {
						int vertical = reflectValue(y - j, Height() - 1);
						result += PrecisePixel(oldImage.GetPixel(x, vertical)) * yFilter[j];
					}
				}
				SetPixel(x, y, result.ToPixel());
			}
		}
	}
}

void Image::Convolve(int radius, double **filter, Image &image) {
	PrecisePixel result;
	Image oldImage = Image(image);
	for (int x = 0; x < image.Width(); x++) {
		for (int y = 0; y < image.Height(); y++) {
			result = PrecisePixel(0, 0, 0, 1);
			for (int i = 0; i < radius * 2 + 1; i++) {
				for (int j = 0; j < radius * 2 + 1; j++) {
					int reflectedX = reflectValue(x + radius - i, image.Width() - 1);
					int reflectedY = reflectValue(y + radius - j, image.Height() - 1);
					result += PrecisePixel(oldImage.GetPixel(reflectedX, reflectedY)) * filter[i][j];
				}
			}

			//std::cout << result.ToPixel() << std::endl;
			image.SetPixel(x, y, result.ToPixel());
		}
	}
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
	
	SeparableConvolve(n, filter, filter);
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

double edgeDetectX[3][3] = {
	{ -1, 0, 1 },
	{ -2, 0, 2 },
	{ -1, 0, 1 }
};

double edgeDetectY[3][3] = {
	{ -1, -2, -1 },
	{  0,  0,  0 },
	{  1,  2,  1 }
};

void Image::EdgeDetect()
{
	// Method inspired by: https://www.projectrhea.org/rhea/index.php/An_Implementation_of_Sobel_Edge_Detection
	double *f[3];
	
	ChangeSaturation(0.0);
	Blur(1);

	// Detect right edges
	for (int i = 0; i < 3; i++) f[i] = edgeDetectX[i];
	Image xDetected = Image(*this);
	Convolve(1, f, xDetected);

	// Detect bottom edges
	for (int i = 0; i < 3; i++) f[i] = edgeDetectY[i];
	Image yDetected = Image(*this);
	Convolve(1, f, yDetected);

	// Detect left edges
	for (int i = 0; i < 3; i++) f[i] = edgeDetectX[2 - i];
	Image xDetected2 = Image(*this);
	Convolve(1, f, xDetected2);

	// Detect top edges
	for (int i = 0; i < 3; i++) f[i] = edgeDetectY[2 - i];
	Image yDetected2 = Image(*this);
	Convolve(1, f, yDetected2);
	
	for (int x = 0; x < Width(); x++) {
		for (int y = 0; y < Height(); y++) {
			SetPixel(x, y, xDetected.GetPixel(x, y) + yDetected.GetPixel(x, y) + Pixel(0, 0, 0, 255) + xDetected2.GetPixel(x, y) + yDetected2.GetPixel(x, y));
		}
	}
}

Image *Image::Scale(double sx, double sy)
{
	Image *newImage = new Image(floor(sx * Width()), floor(sy * Height()));
	for (int u = 0; u < newImage->Width(); u++) {
		for (int v = 0; v < newImage->Height(); v++) {
			newImage->SetPixel(u, v, Sample(u / sx, v / sy));
		}
	}

	return newImage;
}

Image *Image::Rotate(double angle) // Assumes angle is in degrees, positive is clockwise
{
	double theta = (angle * M_PI) / 180.0; // Rads
	double newWidth = Width() * fabs(cos(theta)) + Height() * fabs(sin(theta));
	double newHeight = Width() * fabs(sin(theta)) + Height() * fabs(cos(theta));
	Image *newImage = new Image(newWidth, newHeight);

	double centerX = Width() / 2.0;
	double centerY = Height() / 2.0;
	for (int u = 0; u < newImage->Width(); u++) {
		for (int v = 0; v < newImage->Height(); v++) {
			int newU = u - (int)(newWidth / 2.0);
			int newV = v - (int)(newHeight / 2.0);
			double x = (newU * cos(-theta) - newV * sin(-theta)) + centerX;
			double y = (newU * sin(-theta) + newV * cos(-theta)) + centerY;

			if (y == Height()) y = y - 1; // Weird special case hack

			if (ValidCoord(x, y) && x >= -0.0001 && y >= -0.0001) {
				newImage->SetPixel(u, v, Sample(x, y));
			} else {
				//printf("Not showing (newU, newV) (u, v) (x, y): (%i, %i) (%i, %i) (%f, %f)\n", newU, newV, u, v, x, y);
				newImage->SetPixel(u, v, Pixel(0, 0, 0, 0));
			}
		}
	}
	
	//printf("CenterX, CenterY: %f, %f\n", centerX, centerY);
	return newImage;
}

void Image::Fun()
{
	// The commented-out code following does some nice recursive-looking scaling stuff
	//Image destImage = Image(*this);
	/* double scalingFactor = 0.9;
	Image *scaledImage = new Image(*this);
	Image *tmp;
	const int max_levels = 30;
	for (int i = 0; i < max_levels; i++) {
		tmp = scaledImage->Scale(scalingFactor, scalingFactor);
		delete scaledImage;
		scaledImage = tmp;
		scaledImage->ChangeSaturation(1 - (i / (double)max_levels));

		int offsetX = (Width() - scaledImage->Width()) / 2.0;
		int offsetY = (Height() - scaledImage->Height()) / 2.0;
		offsetX *= 1.9;
		offsetY *= 0.1;
		printf("OffsetX, OffsetY: %i, %i\n", offsetX, offsetY);
		for (int x = 0; x < scaledImage->Width(); x++) {
			for (int y = 0; y < scaledImage->Height(); y++) {
				SetPixel(x + offsetX, y + offsetY, scaledImage->Sample(x, y));
			}
		}
	} */
	Image oldImage = Image(*this);

	double u, v;
	for (int x = 0; x < Width(); x++) {
		for (int y = 0; y < Height(); y++) {
			u = (32 * sin(M_PI * (x / 128.0)) * cos(M_PI * (x / 128.0)));
			v = (16 * sin(M_PI * (y / 128.0)));

			u = fmin((int)(x + u), Width());
			v = fmin((int)(y + v), Height());

			SetPixel(x, y, oldImage.Sample(u, v));
		}
	}
}

/**
 * Image Sample
 **/
void Image::SetSamplingMethod(int method)
{
	assert((method >= 0) && (method < IMAGE_N_SAMPLING_METHODS));
	printf("Sampling: %i\n", method);
	sampling_method = method;
}

Pixel Image::Sample(double u, double v)
{
	int nearestX = fmax(fmin((int)(u + 0.5), Width() - 1), 0);
	int nearestY = fmax(fmin((int)(v + 0.5), Height() - 1), 0);
	if (sampling_method == IMAGE_SAMPLING_POINT) { // Nearest neighbor
		return GetPixel(nearestX, nearestY);
	} else if (sampling_method == IMAGE_SAMPLING_BILINEAR) {
		// Method derived from here: http://eeweb.poly.edu/~yao/EL5123/lecture8_sampling.pdf
		int left = reflectValue(floor(u), Width() - 1);
		int right = reflectValue(left + 1, Width() - 1);
		int top = reflectValue(floor(v), Height() - 1);
		int bottom = reflectValue(top + 1, Height() - 1);
		double a = u - left;
		double b = v - top;

		PrecisePixel out = PrecisePixel();
		out += (1 - a) * (1 - b) * PrecisePixel(GetPixel(left, top));
		out += a * (1 - b) * PrecisePixel(GetPixel(right, top));
		out += (1 - a) * b * PrecisePixel(GetPixel(left, bottom));
		out +=  a * b *  PrecisePixel(GetPixel(right, bottom));
		
		return out.ToPixel();
	} else if (sampling_method == IMAGE_SAMPLING_GAUSSIAN) {
		double filter[3][3] = {
			{ 1/16.0, 2/16.0, 1/16.0 },
			{ 2/16.0, 4/16.0, 2/16.0 },
			{ 1/16.0, 2/16.0, 1/16.0 }
		};
		PrecisePixel out = PrecisePixel();
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				int x = reflectValue(round(u + (-1 + i)), Width() - 1);
				int y = reflectValue(round(v + (-1 + j)), Height() - 1);
				out += filter[i][j] * PrecisePixel(GetPixel(x, y));
			}
		}

		return out.ToPixel();
	} else {
		printf("Invalid sampling method\n");
		exit(-1);
	}
	return Pixel();
}

void Image::Checkerboard() {
	Pixel dark, light;
	for (int x = 0; x < Width(); x++) {
		for (int y = 0; y < Height(); y++) {
			if (x == 0 || y == 0 || x == Width() - 1 || y == Height() - 1) {
				dark = Pixel(0, 0, 255, 255);
				light = Pixel(255, 255, 0, 255);
			} else {
				dark = Pixel(0, 0, 0, 255);
				light = Pixel(255, 255, 255, 255);
			}

			if ((x + y) % 2 == 0) {
				SetPixel(x, y, dark);
			} else {
				SetPixel(x, y, light);
			}
		}
	}
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