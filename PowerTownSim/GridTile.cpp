#include "GridTile.h"

GridTile::GridTile(unsigned int w, unsigned int l, float seed)
{
	srand(seed);
	noise = new float[w * l];
	std::cout << "Memory allotted for noise : " << (w*l) << std::endl;
	auto start = std::chrono::steady_clock::now();


	auto end = std::chrono::steady_clock::now();
	/*

	start = std::chrono::steady_clock::now();

	std::string path = "../Textures/testHeightSmall.png";

	int iWidth, iHeight, channels;
	unsigned char *image = stbi_load(path.c_str(), &iWidth, &iHeight, &channels, STBI_rgb_alpha);
	unsigned int * heightMap = new unsigned int[iWidth*iHeight];//[(w+1)*(l+1)];


	size_t img_size = iWidth * iHeight * channels;
	unsigned char *new_image = new unsigned char[img_size];


	int i = 0;
	int k = 0;
	for (unsigned char *p = image, *pn = new_image; p != image + img_size; p += channels, pn += channels) {
		unsigned int height = static_cast<unsigned int> (*(p));
		*(heightMap + k++) = height;

		//std::cout << "Height value: " << height << std::endl;
		/**pn = *p;
		*(pn+1) = (uint8_t)(225);
		*(pn + 2) = *(p + 2);
		*(pn + 3) = *(p + 3);

	}

	stbi_image_free(image);




	 end = std::chrono::steady_clock::now();

	std::cout << "Elapsed time for HEightmap read : "
		<< std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << std::endl;



	 start = std::chrono::steady_clock::now();

	 */

	 // MEMBER INITIALIZATIONS
	if (w == 1 && l == 1) w = l = 2;

	width = w; length = l;

	vertices = new float[w*l * 8];
	HeightMap = new heightMapping[(w*l)];
	PossibleXValues = new float[length];
	PossibleZValues = new float[width];
	float normalizer = width >= length ? width : length;
	// Algorithm for generating the plane's vertex data
	int h = 0;

	// Create the vertices for the grid. The length of the grid is in the z axis, the width in the x axis.
	int c = 0;
	for (int j = 0; j < l; j++) {
		float z = (float)j / (l - 1) - 0.5f;
		for (int i = 0; i < w; i++) {
			float x = (float)i / (w - 1) - 0.5f;

			float y = 0;
			//	Position coordinates
			*(vertices + c++) = x;
			*(vertices + c++) = y;
			*(vertices + c++) = z;
			// Textures coordinates
			*(vertices + c++) = x + 0.5f;
			*(vertices + c++) = z + 0.5f;
			// Normal coordinates
			*(vertices + c++) = -5;
			*(vertices + c++) = -5;
			*(vertices + c++) = -5;

			// Add a height mapping to the height map data structure (SuperMap);
			//heightMapping hm{ x, z, y };
			//*(HeightMap + h - 1) = hm;

		//	SuperMap.insert(std::make_pair(x, zHeightMapping{ z , y }));

		}

	}

	NoiseManager NM(w, l, seed);
	NM.GenerateSimplexTerrainNoiseFor(noise);

	// Place the noise values as the y coordinates for the mesh (hack to simplify normal calc for testing right now)

	for (int i = 0; i < w*l; i++) vertices[8 * i + 1] = noise[i];


	//for (int i = 0; i < w*l; i++) noise[i] = 0;
	// Calculate face normals

		// Create the indices for the drawElements call / GPU

		// Create the first pass of triangles

	indices = new GLuint[6 * (w - 1)*(l - 1)];
	c = 0;
	int ct = 0;
	for (int i = 0; i < (w - 1)*(l - 1); i++, c++) {
		if ((c + 1) % w == 0) ++c;

		// Indices for one face
		*(indices + ct++) = c + l;
		*(indices + ct++) = c + 1;
		*(indices + ct++) = c;

		// Calculate its normal
		unsigned int v1Ind = 8 * (c + l);
		unsigned int v2Ind = 8 * (c + 1);
		unsigned int v3Ind = 8 * c;

		float  v1x, v1y, v1z, v2x, v2y, v2z, v3x, v3y, v3z;
		v1x = vertices[v1Ind], v1y = vertices[v1Ind + 1], v1z = vertices[v1Ind + 2];
		v2x = vertices[v2Ind], v2y = vertices[v2Ind + 1], v2z = vertices[v2Ind + 2];
		v3x = vertices[v3Ind], v3y = vertices[v3Ind + 1], v3z = vertices[v3Ind + 2];

		// Cross product
		float cx, cy, cz;

		cx = (v3y - v1y) * (v2z - v1z) - (v3z - v1z) * (v2y - v1y);
		cy = (v3x - v1x) * (v2z - v1z) - (v3z - v1z) * (v2x - v1x);
		cz = (v3x - v1x) * (v2y - v1y) - (v3y - v1y) * (v2x - v1x);

		float c2x, c2y, c2z;

		c2x = (v1y - v2y) * (v3z - v2z) - (v1z - v2z) * (v3y - v2y);
		c2y = (v1x - v2x) * (v3z - v2z) - (v1z - v2z) * (v3x - v2x);
		c2z = (v1x - v2x) * (v3y - v2y) - (v1y - v2y) * (v3x - v2x);

		glm::vec3 vec1(v3x - v2x, v3y - v2y, v3z - v2z);
		glm::vec3 vec2(v1x - v2x, v1y - v2y, v1z - v2z);

		glm::vec3 faceNormal = glm::cross(vec1, vec2);



		// Set the normalalized normal coordinates in the vertex array

		faceNormal = glm::normalize(faceNormal);

		if (vertices[v3Ind + 6] == -5) {
			vertices[v1Ind + 5] = faceNormal.x;
			vertices[v1Ind + 6] = faceNormal.y;
			vertices[v1Ind + 7] = faceNormal.z;
			vertices[v2Ind + 5] = faceNormal.x;
			vertices[v2Ind + 6] = faceNormal.y;
			vertices[v2Ind + 7] = faceNormal.z;
			vertices[v3Ind + 5] = faceNormal.x;
			vertices[v3Ind + 6] = faceNormal.y;
			vertices[v3Ind + 7] = faceNormal.z;
		}

		*(indices + ct++) = c + 1;
		*(indices + ct++) = c + w;
		*(indices + ct++) = c + w + 1;

		// Calculate its normal
		v1Ind = 8 * (c + 1);
		v2Ind = 8 * (c + w);
		v3Ind = 8 * (c + w + 1);
		v1x = vertices[v1Ind], v1y = vertices[v1Ind + 1], v1z = vertices[v1Ind + 2];
		v2x = vertices[v2Ind], v2y = vertices[v2Ind + 1], v2z = vertices[v2Ind + 2];
		v3x = vertices[v3Ind], v3y = vertices[v3Ind + 1], v3z = vertices[v3Ind + 2];

		// Cross product

		cx = (v3y - v1y) * (v2z - v1z) - (v3z - v1z) * (v2y - v1y);
		cy = (v3x - v1x) * (v2z - v1z) - (v3z - v1z) * (v2x - v1x);
		cz = (v3x - v1x) * (v2y - v1y) - (v3y - v1y) * (v2x - v1x);


		c2x = (v1y - v2y) * (v3z - v2z) - (v1z - v2z) * (v3y - v2y);
		c2y = (v1x - v2x) * (v3z - v2z) - (v1z - v2z) * (v3x - v2x);
		c2z = (v1x - v2x) * (v3y - v2y) - (v1y - v2y) * (v3x - v2x);

		vec1 = glm::vec3(v3x - v1x, v3y - v1y, v3z - v1z);
		vec2 = glm::vec3(v2x - v1x, v2y - v1y, v2z - v1z);

		faceNormal = glm::cross(vec2, vec1);


		// Set the normalalized normal coordinates in the vertex array
		if (vertices[v3Ind + 6] == -5) {
			faceNormal = glm::normalize(faceNormal);
			vertices[v1Ind + 5] = faceNormal.x;
			vertices[v1Ind + 6] = faceNormal.y;
			vertices[v1Ind + 7] = faceNormal.z;
			vertices[v2Ind + 5] = faceNormal.x;
			vertices[v2Ind + 6] = faceNormal.y;
			vertices[v2Ind + 7] = faceNormal.z;
			vertices[v3Ind + 5] = faceNormal.x;
			vertices[v3Ind + 6] = faceNormal.y;
			vertices[v3Ind + 7] = faceNormal.z;
		}
	}


	/*
			// Now set each vertex normal to the average of the surrounding triangles
			for (int i = 0; i < (8*w*l)/width; i++) {
				int ind = i * 8;
				float nX, nY, nZ;
				// If it's the first row of vertices
				if (i < length) {

					if (i == 0) {

						nX = (vertices[ind + 8] + vertices[ind + 8 * width]) / 2.0;
						nY = (vertices[ind + 9] + vertices[ind + 8 * width + 1]) / 2.0;
						nZ = (vertices[ind + 10] + vertices[ind + 8 * width + 2]) / 2.0;

					}
					else {

						nX = (vertices[ind - 8] + vertices[ind + 8] + vertices[ind + 8 * width]) / 3.0;
						nY = (vertices[ind - 7] + vertices[ind + 9] + vertices[ind + 8 * width + 1]) / 3.0;
						nZ = (vertices[ind - 6] + vertices[ind + 10] + vertices[ind + 8 * width + 2]) / 3.0;

					}


				}
				else if (i % width == 0) {
					nX = (vertices[ind - 8*width] + vertices[ind + 8] + vertices[ind + 8 * width]) / 3.0;
					nY = (vertices[ind - 8*width + 1] + vertices[ind + 9] + vertices[ind + 8 * width + 1]) / 3.0;
					nZ = (vertices[ind - 8*width + 2] + vertices[ind + 10] + vertices[ind + 8 * width + 2]) / 3.0;

				}
				else {
					nX = (vertices[ind - 8 * width] + vertices[ind - 8] +  vertices[ind + 8] + vertices[ind + 8 * width]) / 4.0;
					nY = (vertices[ind - 8 * width + 1] + vertices[ind - 7] + vertices[ind + 9] + vertices[ind + 8 * width + 1]) / 4.0;
					nZ = (vertices[ind - 8 * width + 2] + vertices[ind - 6] + vertices[ind + 10] + vertices[ind + 8 * width + 2]) / 4.0;
				}

				vertices[ind + 5] = nX;
				vertices[ind + 6] = nY;
				vertices[ind + 7] = nZ;

			//	std::cout << "Averaged normal : (" << nX << "," << nY << "," << nZ << ")" << std::endl;

			}
		*/

		/*


		for (int j = 0; j <= width; j++) {

			for (int i = 0; i <= length; i++) {
				float x = (float)i / normalizer - 0.5f;
				float height = .0f;// ((float)(*(heightMap + h++))) / 255.0 - 0.5f;
				float z = (float)j / normalizer - 0.5f;

				if (height < lowestLow) lowestLow = height;
				if (height > highestHigh) highestHigh = height;

				vertices.push_back(x);
				vertices.push_back(height);
				vertices.push_back(z);

				// Texture coordinates
				vertices.push_back((((float)i) / normalizer) - 0.5f);
				vertices.push_back((((float)j) / normalizer) - 0.5f);


				// Normal coordinates
				float nX = 0.5 + height;
				float nY = 1.0 - abs(cosf(height));
				float nZ = 0.5  + height;
				vertices.push_back(nX);
				vertices.push_back(nY);
				vertices.push_back(nZ);







				heightMapping hm{ x, z, height };
				*(HeightMap + h - 1) = hm;

				SuperMap.insert(std::make_pair(x, zHeightMapping{ z,height }));

				//s	std::cout << "Added Height Map w/ Height: " << HeightMap[h - 1].height << std::endl;
			}
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

	*/
	/*for (int i = 0; i < length; i++)
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
		*/

		
	
	/*
	int nic = 0;
	std::string writePath = "../Textures/TestMap.png";

	size_t img_size = width * length * 4;
	unsigned char *new_image = new unsigned char[img_size];
	for (unsigned char *pn = new_image; pn != new_image + img_size; pn += 4) {

		*(pn) = (uint8_t)(noise[nic++] * 255);
		*(pn+1) = (uint8_t)(0);
		*(pn+2) = (uint8_t)(0);
		*(pn+3) = (uint8_t)(255);
	}
	stbi_write_png(writePath.c_str(), width, length, 4, new_image, width*4);
	*/

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glGenBuffers(1, &heightBO);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, w*l * 8 * sizeof(float), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, heightBO);
	glBufferData(GL_ARRAY_BUFFER, w*l * sizeof(float), noise, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * (w - 1)*(l - 1) * sizeof(GLuint), indices, GL_STATIC_DRAW);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	// Clear allocated memory now that it's been sent to the GPU


	//delete[8 * w*l] vertices;
	delete[6 * (w - 1)*(l - 1)] indices;

	setPossibleValues();

}

void GridTile::regenHeights(float seed) {

	NoiseManager NM(width, length, seed);
	NM.GenerateSimplexTerrainNoiseFor(noise);

}

void GridTile::setPossibleValues() {
	float normalizer = width >= length ? width : length;

	for (int i = 0; i < length; i++) PossibleXValues[i] = (float)i / normalizer - 0.5f;
	for (int i = 0; i < width; i++) PossibleZValues[i] = (float)i / normalizer - 0.5f;

}

float GridTile::mapToGridX(float xCoord) {
	for (int i = 1; i < length; i++) if (PossibleXValues[i - 1] <= xCoord && xCoord <= PossibleXValues[i]) return PossibleXValues[i];
}

float GridTile::mapToGridZ(float zCoord) {
	for (int i = 1; i < width; i++) if (PossibleZValues[i - 1] <= zCoord && zCoord <= PossibleZValues[i]) return PossibleZValues[i];
}

float * GridTile::findHeight(float xCoord, float zCoord, float xScale, float zScale) {


	float normalizedXCoord = mapToGridX(xCoord / xScale);
	float normalizedZCoord = mapToGridZ(zCoord / zScale);

	auto r = SuperMap.equal_range(normalizedXCoord);

	for (auto begin = r.first; begin != r.second; begin++) if (begin->second.z == normalizedZCoord) {
		if (!(begin->first == normalizedXCoord)) return new float(2);
		return &begin->second.height;
	}
	return new float(2);
}



void GridTile::Draw()
{
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glDrawElements(GL_TRIANGLES, 6 * (width - 1)*(length - 1), GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

void GridTile::DrawInstanced(unsigned int count)
{
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glDrawElementsInstanced(GL_TRIANGLES, 6 * (width - 1)*(length - 1), GL_UNSIGNED_INT, 0, count);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}


GridTile::~GridTile()
{
}
