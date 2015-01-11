/*
 * sphere_mapping.cpp
 *
 *  Created on: Sep 20, 2009
 *      Author: arnaud
 */
/*!
 * start the sphere mapping
 */
void sphereMap_init(int textureId, int texture_unit) {
	//Texture use for automatic coordinate generation
	load_texture(textureId, texture_unit);

	//Sphere mapping and enable s & t texture generation
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable( GL_TEXTURE_GEN_S);
	glEnable( GL_TEXTURE_GEN_T);

}

/*!
 * end the sphere mapping
 */
void sphereMap_end(int texture_unit) {
	glDisable( GL_TEXTURE_GEN_S);
	glDisable( GL_TEXTURE_GEN_T);
	unload_texture_unit(texture_unit);
}

/*!
 * apply sphere mapping for a hero
 * @param textureId the index of the texture
 * @param h the hero
 */
void sphereMap_hero(int textureId, int texture_unit, Hero* h) {
	sphereMap_init(textureId, texture_unit);
	drawHero(h, false);
	sphereMap_end(texture_unit);
}

/*!
 * apply sphere mapping for a sphere
 * @param textureId
 * @param x
 * @param y
 * @param z
 * @param r
 */
void sphereMap_sphere(int textureId, int texture_unit, double x, double y, double z, double r) {
	sphereMap_init(textureId, texture_unit);

	glPushMatrix();
	glTranslatef(x, y, z);
	glutSolidSphere(r, 10, 10);
	glPopMatrix();

	sphereMap_end(texture_unit);
}

/*!
 * render a view in a texture
 * @param id the id of the texture
 * @param w the width
 * @param h the height
 * @param eyeX, eyeY, eyeZ the position of the camera
 * @param centerX, centerY, centerZ the position of the looked point
 */
void render_view_in_texture(int id, int w, int h, //
		double eyeX, double eyeY, double eyeZ, //
		double centerX, double centerY, double centerZ) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearDepth(1);
	create_black_texture(id, w, h);

	/*
	 * copy the stencil in the texture
	 */
	glPushMatrix();
	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0, 1, 0);
	drawFloor(true);
	drawHero(devil, true);
	drawHero(dog, true);
	glPopMatrix();
	/*
	 * copy the buffer in the texture
	 */
	// source
	glReadBuffer( GL_COLOR);
	// dest
	load_texture(id, TEX_UNIT_1);
	// copy !
	glCopyTexSubImage2D(GL_TEXTURE_2D, //target   Specifies the target	texture.  Must be GL_TEXTURE_2D
			0, //level	   Specifies the level-of-detail number.  Level	0 is
			//the base image level.  Level	n is the nth mipmap reduction image.
			0, //xoffset  Specifies a texel offset in the x direction within the texture array.
			0, //yoffset  Specifies a texel offset in the y direction within the texture array.
			0, 0, //x, y	   Specify the window coordinates of the lower left
			//corner of the rectangular region of pixels to be copied.
			w, //width	   Specifies the width of the texture subimage.
			h //height   Specifies the height	of the texture subimage.
	);

	/*
	 * erase
	 */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
