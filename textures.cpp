/*
 * textures.c
 *
 *  Created on: Sep 20, 2009
 *      Author: arnaud
 */
/*
 * textures and co
 */

class Tex {
public:
	const char* filename;
	int id;
};

vector<Tex> textures;

/*!
 * load a texture
 * @param id the id of the texture
 */
void load_texture(int idTex, int idTexUnit) {
	glActiveTextureARB(idTexUnit);
	glEnable( GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, idTex);
}

void unload_texture_unit(int idTexUnit) {
	glActiveTextureARB(idTexUnit);
	glDisable( GL_TEXTURE_2D);
}

void load_texture(const char* filename, int idTexUnit) {
	for (vector<Tex>::iterator tex = textures.begin(); tex != textures.end(); ++tex) {
		//			cout << (string)tex->filename << " - "<< tex->id << endl;

		if (((string) tex->filename).find(filename) != string::npos) {
			load_texture(tex->id, idTexUnit);
			break;
		}
	}
}

/*!
 * begin a texture
 * @param id the id of the texture
 */
void create_texture_begin(int id) {
	glEnable( GL_TEXTURE_2D);

	//The first thing that must take place in the process of uploading the texture is a call to glBindTexture.
	// What glBindTexture does is it tells OpenGL which texture "id" we will be working with.
	// A texture "id" is just a number that you will use to access your textures.
	glBindTexture(GL_TEXTURE_2D, id);

	// The glTexParameteri sets the various parameters for the current OpenGL texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// The glTexEnvf call sets environment variables for the current texture.
	// What this does is tell OpenGL how the texture will act when it is rendered into a scene.
	//GL_MODULATE, GL_DECAL, GL_BLEND, or GL_REPLACE.
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

/*!
 * load a texture into the texture number id
 * @param filename the name of the bitmap file
 * @param id the index of the texture
 */
void readTexture(const char* filename, int id_tex) {
	cout << endl << endl << "***" << filename << endl;
	create_texture_begin(id_tex);

	Bitmap* img = new Bitmap(filename);

	//This call tells OpenGL that the pixel data which is going to be passed to it is aligned in byte order,
	//this means that the data has one byte for each component, one for red, green and blue.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// The glTexImage2D call is our goal.
	// This call will upload the texture to the video memory where it will be ready for us to use in our programs
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->width, img->height, 0, GL_RGB,
			GL_UNSIGNED_BYTE, img->data);

	Tex texture;
	texture.filename = filename;
	texture.id = id_tex;
	textures.push_back(texture);
}

/*!
 * create a black texture
 * @param id the id
 * @param w the width
 * @param h the height
 */
void create_black_texture(int id, int w, int h) {
	create_texture_begin(id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE,
			NULL);
}

