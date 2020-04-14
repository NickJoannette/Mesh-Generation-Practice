#include "Surface.h"
#include "stb_image.h"

/*
void visualizeImageWrite() {

}

*/

Surface::Surface(unsigned int w, unsigned int l, bool f)
{

	std::string path = "../Textures/earthHeightMap.png";

	int width, height, channels;
	unsigned char *image = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
	unsigned int * heightMap = new unsigned int[129*129];


	size_t img_size = width * height * channels;
	unsigned char *new_image = new unsigned char[img_size];


	int i = 0;
	int k = 0;
	for (unsigned char *p = image, *pn = new_image; p != image + img_size; p += channels, pn += channels) {
		unsigned int height = static_cast<unsigned int>(*p);

		*(heightMap + k++) = height;
		//std::cout << "Height value: " << height << std::endl;
		/**pn = *p;
		*(pn+1) = (uint8_t)(225);
		*(pn + 2) = *(p + 2);
		*(pn + 3) = *(p + 3);
		*/
	}

	stbi_image_free(image);
	
	
	
	
	
	
	
	
	
	
	// MEMBER INITIALIZATIONS
	width = w; length = l; flat = f;
	float normalizer = width >= length ? width : length;
	// Algorithm for generating the plane's vertex data
	int h = 0;
	if (flat)
	{
		for (int j = 0; j <= width; j++)
			for (int i = 0; i <= length; i++) {
				vertices.push_back((float)i / normalizer - 0.5f);
				vertices.push_back(((float) (*(heightMap + h++)))/255.0 - 0.5f);
				vertices.push_back((float)j / normalizer - 0.5f);
				vertices.push_back((((float)i)/normalizer ) - 0.5f);
				vertices.push_back((((float)j)/normalizer) - 0.5f);
				//std::cout << " ------------------------------------ " << std::endl;
				//std::cout << "Vertex coordinate generated: " << " ( " << (((float)i)) << " , " << (((float)j)) << " ) " << std::endl;
				//std::cout << "Texel coordinate generated: " << " ( " << (((float)i) ) << " , " << (((float)j) ) << " ) " << std::endl;
			}
	}
	else
	{
		srand(3);
		for (int j = 0; j <= width; j++)
			for (int i = 0; i <= length; ++i)
			{
				float rnd = rand() % 100 + 1;
				vertices.push_back((float)i / normalizer);
				vertices.push_back((float)j / normalizer);
				vertices.push_back((float)(rand() % 8) / 5 /normalizer);
				vertices.push_back((((float)i / normalizer) + 1.0) / 2.0);
				vertices.push_back((((float)j / normalizer) + 1.0) / 2.0);
			}
	}

	for (int i = 0; i < length; i++)
	{
		indices.push_back(i + length + 1);
		indices.push_back(i + length + 2);
		indices.push_back(i + 1);
		indices.push_back(i + 1);
		indices.push_back(i);
		indices.push_back(i + length + 1);
	}

	for (int k = 1; k < width; k++) // Generate indices (valid for GL_TRIANGLES draw mode) // To review - generates 1 extra set of indices 
		for (int i = k * length + k; i < (k + 1)*length + k; i++)
		{
			indices.push_back(i + length + 1);
			indices.push_back(i + length + 2);
			indices.push_back(i + 1);
			indices.push_back(i + 1);
			indices.push_back(i);
			indices.push_back(i + length + 1);
		}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

}

void Surface::Draw()
{
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}
Surface::~Surface()
{
}
