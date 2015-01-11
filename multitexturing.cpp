/*
 * multitexturing.cpp
 *
 *  Created on: Sep 21, 2009
 *      Author: arnaud
 */
/*!
 * start the sphere mapping
 */
void multiTex_init(int textureId1, bool mirror1, int textureId2, bool mirror2) {
	if (mirror1)
		sphereMap_init(textureId1, TEX_UNIT_1);
	else
		load_texture(textureId1, TEX_UNIT_1);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);

	if (mirror2)
		sphereMap_init(textureId2, TEX_UNIT_2);
	else
		load_texture(textureId2, TEX_UNIT_2);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_INCR);
}

/*!
 * end the sphere mapping
 */
void multiTex_end() {
	unload_texture_unit( TEX_UNIT_1);
	unload_texture_unit( TEX_UNIT_2);
}

/*!
 * apply sphere mapping for a sphere
 * @param textureId
 * @param x
 * @param y
 * @param z
 * @param r
 */
void multiTex_sphere(int textureId1, bool mirror1, int textureId2,
		bool mirror2, double x, double y, double z) {
	multiTex_init(textureId1, mirror1, textureId2, mirror2);
	glPushMatrix();
	glTranslatef(x, y, z);

	double size = 4;
	glBegin( GL_QUADS);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0, 1.0);
	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);

	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0, 0.0);
	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0, 0.0);
	glVertex3f(0.0, size * 1.0, 0.0);

	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0, 0.0);
	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0, 0.0);
	glVertex3f(size * 1.0, size * 1.0, 0.0);

	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0, 1.0);
	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0, 1.0);
	glVertex3f(size * 1.0, 0.0, 0.0);
	glEnd();

	//	glutSolidSphere(r, 10, 10);

	glPopMatrix();
	multiTex_end();
}

/*!
 * apply sphere mapping for a hero
 * @param textureId the index of the texture
 * @param h the hero
 */
void multiTex_hero(int textureId1, bool mirror1, int textureId2, bool mirror2,
		Hero* h) {
	multiTex_init(textureId1, mirror1, textureId2, mirror2);
	drawHero(h, false);
	multiTex_end();
}
